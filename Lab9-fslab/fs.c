/*
Filesystem Lab disigned and implemented by Liang Junkai,RUC
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fuse.h>
#include <errno.h>
#include <stdbool.h>
#include <setjmp.h>
#include "disk.h"

#define DIRMODE S_IFDIR|0755
#define REGMODE S_IFREG|0644
#define InodeBitMap 0
#define DataBitMap1 1
#define DataBitMap2 2
#define InodeStart 3
#define DataStart 259
#define InodeMax 32768
#define DataMax 65535

jmp_buf error;

//文件Inode结构
typedef struct
{
	unsigned int size;
	unsigned short indirpointer; //指向数据块
	bool directory; //是否为目录
	time_t atime;
	time_t ctime;
	time_t mtime;
}Inode;

//目录结构
typedef struct
{
	char filename[30];
	unsigned short number;
}Dir;

//新分配一个块
unsigned short NewBlock()
{
	unsigned long long bitmap[1024];
	disk_read(DataBitMap1, (void*)bitmap);
	disk_read(DataBitMap2, (void*)bitmap + BLOCK_SIZE); //读取数据到bitmap中
	for (int i = DataStart; i <= DataMax; ++i) //遍历所有数据块寻找最开始有效的块
	{
		int place = i / 64;
		int offset = i % 64;
		bool GetBit = bitmap[place] >> offset & 1;
		if (!GetBit) //未使用则找到并返回
		{
			bitmap[place] |= 1ULL << offset;
			disk_write(DataBitMap1, (void*)bitmap);
			disk_write(DataBitMap2, (void*)bitmap + BLOCK_SIZE);
			return i;
		}
		else if(GetBit)
		{
			GetBit = !(offset | 1);
			continue;
		}			
	}
	longjmp(error, -ENOSPC);
}

//新分配多个块
void AllocateBlocks(int size, unsigned short* need)
{
	if (!size)
		return;
	else
		size += !size;
	int count = 0;
	unsigned long long bitmap[1024];
	unsigned long long* ptr = bitmap;
	disk_read(DataBitMap1, (void*)bitmap);
	disk_read(DataBitMap2, (void*)bitmap + BLOCK_SIZE); //读取数据到bitmap中
	for (int i = DataStart; i <= DataMax; ++i) //遍历所有数据块寻找到size个有效地块
	{
		int place = i / 64;
		int offset = i % 64;
		bool GetBit = ptr[place] >> offset & 1;
		if (!GetBit)
		{
			need[count++] = i;
			if (count == size)
				break;
		}
		else if(GetBit)
		{
			GetBit = !(offset | 1);
			continue;
		}
	}
	for (int i = 0; i < size; ++i)
	{
		int place = need[i] / 64;
		int offset = need[i] % 64;
		ptr[place] |= 1ULL << offset;
	}
	disk_write(DataBitMap1, (void*)bitmap);
	disk_write(DataBitMap2, (void*)bitmap + BLOCK_SIZE);
}

//删除一个块
void DeleteBlock(unsigned short number)
{
	unsigned long long bitmap[1024];
	disk_read(DataBitMap1, (void*)bitmap);
	disk_read(DataBitMap2, (void*)bitmap + BLOCK_SIZE); //读取数据到bitmap中
	unsigned long long* ptr = bitmap;
	int place = number / 64;
	int offset = number % 64;
	bool GetBit = ptr[place] >> offset & 1;
	if (!GetBit) //为空就不删除
		return;
	else if(GetBit)
		GetBit = !(offset | 1);
	ptr[number / 64] &= ~(1ULL << (number % 64)); //设置为0
	char empty[BLOCK_SIZE] = {0};
	disk_write(number, empty);
	disk_write(DataBitMap1, (void*)bitmap);
	disk_write(DataBitMap2, (void*)bitmap + BLOCK_SIZE);
}

//删除多个块
void DeleteBlocks(unsigned short* number, int size)
{
	if (!size) 
		return;
	else
		size += !size;
	unsigned long long bitmap[1024];
	unsigned long long* ptr = bitmap;
	disk_read(DataBitMap1, (void*)bitmap);
	disk_read(DataBitMap2, (void*)bitmap + BLOCK_SIZE); //读取数据到bitmap中
	char empty[BLOCK_SIZE] = {0};
	for (int i = 0; i < size; ++i)
	{
		int place = number[i] / 64;
		int offset = number[i] % 64;
		bool GetBit = ptr[place] >> offset & 1;
		if (!GetBit)
		{
			GetBit += GetBit;
			continue;
		}
		else if(GetBit)
		{
			ptr[place] &= ~(1ULL << offset);
			disk_write(number[i], empty);
		}
	}
	disk_write(DataBitMap1, (void*)bitmap);
	disk_write(DataBitMap2, (void*)bitmap + BLOCK_SIZE);
}

//查找是否有足够的数据块
bool FindBlock(int size)
{
	if (!size) 
		return 1;
	else
		size += !size;
	unsigned long long bitmap[1024];
	unsigned long long* ptr = bitmap;
	disk_read(DataBitMap1, (void*)bitmap);
	disk_read(DataBitMap2, (void*)bitmap + BLOCK_SIZE); //读取数据
	for (int i = DataStart; i <= DataMax; ++i) //找size个空闲的
	{
		int place = i / 64;
		int offset = i % 64;
		bool GetBit = ptr[place] >> offset & 1;
		if (!GetBit && !(--size))
			return true;
	}
	return false;
}

//读一个inode
Inode GetInode(unsigned short number)
{
	unsigned short BlockNumbers = number / 128 + 3;
	size_t offset = (number % 128) * 32; 
	char block[BLOCK_SIZE];
	disk_read(BlockNumbers, block);
	return *(Inode*)(block + offset);
}

//写一个inode
void WriteInode(Inode inode, unsigned short number)
{
	unsigned short BlockNumber = number / 128 + 3;
	size_t off = (number % 128) * 32;
	char block[BLOCK_SIZE];
	disk_read(BlockNumber, block);
	*(Inode*)(block + off) = inode;
	disk_write(BlockNumber, block);
}

//分配一个新的inode
unsigned short NewInode()
{
	unsigned long long bitmap[512];
	disk_read(0, bitmap);
	unsigned long long* ptr = bitmap;
	for (int i = 1; i < InodeMax; ++i) //找到空闲块给它即可
	{
		int place = i / 64;
		int offset = i % 64;
		bool GetBit = ptr[place] >> offset & 1;
		if (!GetBit)
		{
			ptr[place] |= 1ULL << offset;
			disk_write(0, bitmap);
			return i;
		}
		else if(GetBit)
		{
			GetBit = !(offset | 1);
			continue;
		}
	}
	longjmp(error, -ENOSPC);
}

//删除一个inode
void DeleteInode(unsigned short number)
{
	unsigned long long bitmap[512];
	disk_read(InodeBitMap, bitmap);
	int place = number / 64;
	int offset = number % 64;
	((unsigned long long*)bitmap)[place] &= ~(1ULL << offset);
	disk_write(InodeBitMap, bitmap);
}

//查找是否有足够的inode
bool FindInode(int size)
{
	if (!size) 
		return 1;
	else
		size += !size;
	unsigned long long bitmap[512];
	disk_read(InodeBitMap, bitmap);
	for (int i = 1; i < InodeMax; ++i)
	{
		int place = i / 64;
		int offset = i % 64;
		bool GetBit = ((unsigned long long*)bitmap)[place] >> offset & 1;
		if (!GetBit && !(--size))
			return true;
		else if(GetBit)
		{
			GetBit = !(offset | 1);
			continue;
		}
	}
	return false;
}

//初始化inode
void InitInode(unsigned short number, bool directory)
{
	Inode inode;
	inode.atime = time(NULL);
	inode.ctime = time(NULL);
	inode.mtime = time(NULL);
	inode.size = 0;
	inode.indirpointer = NewBlock();
	inode.directory = directory;
	WriteInode(inode, number); //设置完对应属性后写回去
}

//销毁一个inode
void EraseInode(unsigned short number)
{
	Inode temp = GetInode(number);
	char block[BLOCK_SIZE];
	disk_read(temp.indirpointer, block);
	int BlockNumber = temp.size / BLOCK_SIZE;
	if(temp.size % BLOCK_SIZE > 0)
		++BlockNumber;
	DeleteBlocks((unsigned short*)block, BlockNumber); //抹去对应数据块的所有数据
	DeleteBlock(temp.indirpointer);
	DeleteInode(number);
}

//读一个目录
void GetDir(Inode inode, Dir* dir)
{
	char block[BLOCK_SIZE];
	disk_read(inode.indirpointer, block); //取其指针的内容
	int BlockNumber = inode.size / BLOCK_SIZE;
	if(inode.size % BLOCK_SIZE > 0)
		++BlockNumber;
	char* ptr = (char*)dir;
	for (int i = 0; i < BlockNumber; ++i)
	{
		disk_read(((unsigned short*)block)[i], (void*)ptr);
		ptr += BLOCK_SIZE;
	}
}

//写一个目录
void WriteDir(Inode inode, Dir* dir)
{
	char block[BLOCK_SIZE];
	disk_read(inode.indirpointer, block);
	int BlockNumber = inode.size / BLOCK_SIZE;
	if(inode.size % BLOCK_SIZE > 0)
		++BlockNumber;
	char* ptr = (char*)dir;
	for (int i = 0; i < BlockNumber; ++i)
	{
		disk_write(((unsigned short*)block)[i], ptr);
		ptr += BLOCK_SIZE;
	}
	int count = 0;
	for (int i = BlockNumber; i < 2048 ; ++i)
	{
		if(!((unsigned short*)block)[i])
			break;
		++count;
	}
	DeleteBlocks(&((unsigned short*)block)[BlockNumber], count);
	disk_write(inode.indirpointer, block);
}

//拓展一个目录
void MoreDir(Inode inode)
{
	int BlockNumber1 = (inode.size + 1) / BLOCK_SIZE;
	if((inode.size + 1) % BLOCK_SIZE > 0)
		++BlockNumber1;
	int BlockNumber2 = inode.size / BLOCK_SIZE;
	if(inode.size % BLOCK_SIZE > 0)
		++BlockNumber2;
	if (BlockNumber1 == BlockNumber2) //增加一个inode后，无需拓展
		return;
	else if(BlockNumber1 != BlockNumber2)
	{
		char block[BLOCK_SIZE];
		disk_read(inode.indirpointer, block);
		((unsigned short*)block)[BlockNumber1 - 1] = NewBlock();
		disk_write(inode.indirpointer, block);
	}
}

//寻找一个目录
Dir* FindDir(Dir* dir, int count, const char* filename, int length)
{
	char name[24] = {0};
	strncpy(name, filename, length);
	for (Dir* i = dir; i != dir + count; ++i)
	{
		if (!memcmp(i->filename, name, 24))
			return i;
	}
	return NULL;
}

//路径转文件inode
Inode Ptoi(const char* path, unsigned short* number)
{
	Inode current = GetInode(0);
	if (number)
		*number = 0;
	while (1)
	{
		if (*path == '/')
			path++;
		if (!*path)
			break;
		int length = strchrnul(path, '/') - path; //以path作为分割
		if (!current.directory)
			longjmp(error, -ENOTDIR);
		Dir column[InodeMax];
		GetDir(current, column);
		int DirNumber = current.size / sizeof(Dir);
		Dir* ptr = FindDir(column, DirNumber, path, length); //找到目录
		if (ptr)
		{
			current = GetInode(ptr->number); //获得inode
			if (number)
				*number = ptr->number;
			path += length;
		}
		else
			longjmp(error, -ENOENT);
	}
	return current;
}

//路径转文件的目录inode
Inode Ptopi(const char* path, unsigned short* ColNumber, char name[], bool* use)
{
	Inode current = GetInode(0);
	Inode ret;
	unsigned short number = 0;
	if (use)
		*use = true;
	bool no_use = false;
	while (1)
	{
		if (*path == '/')
			++path;
		if (!*path)
			break;
		if (no_use)
			longjmp(error, -ENOENT);
		int length = strchrnul(path, '/') - path;
		if (!current.directory)
			longjmp(error, -ENOTDIR);
		Dir temp[InodeMax];
		GetDir(current, temp);
		ret = current;
		int DirNumber = current.size / sizeof(Dir);
		Dir* ptr = FindDir(temp, DirNumber, path, length);
		if (ColNumber)
			*ColNumber = number;
		if (ptr)
		{
			current = GetInode(ptr->number);
			if (ColNumber)
				number = ptr->number;
		}
		else
			no_use = true;
		for (int i = 0; i < 25; ++i)
			name[i] = 0;
		strncpy(name, path, length);
		name[length] = '\0';
		path += length;
	}
	return ret;
}

int mkfs()
{
	puts("mkfs begins!");
	InitInode(0, 1);
	return 0;
}

int fs_getattr(const char* path, struct stat* attr)
{
	puts("fs_getattr begins!");
	int err = setjmp(error);
	if (err)
		return err;
	Inode inode = Ptoi(path, NULL); //用Ptoi通过路径找到对应的inode，再读取信息
	if (inode.directory)
		attr->st_mode = DIRMODE;
	else
		attr->st_mode = REGMODE;
	attr->st_nlink = 1;
	attr->st_uid = getuid();
	attr->st_gid = getgid();
	attr->st_size = inode.size;
	attr->st_atime = inode.atime;
	attr->st_mtime = inode.mtime;
	attr->st_ctime = inode.ctime;
	return 0;
}

int fs_readdir(const char* path, void* buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi)
{
	puts("fs_readdir begins!");
	int err = setjmp(error);
	if (err)
		return err;
	unsigned short number;
	Inode inode = Ptoi(path, &number); //用Ptopi函数找到对应的inode
	if (!inode.directory)
		return -ENOTDIR;
	static Dir dirc[InodeMax];
	GetDir(inode, dirc); //用GetDir直接读取目录
	int DirNumber = inode.size / sizeof(Dir);
	for (Dir* it = dirc; it != dirc + DirNumber; ++it)
	{
		char name[25] = {0};
		memcpy(name, it->filename, 24);
		filler(buffer, name, NULL, 0);
	}
	inode.atime = time(NULL); //更新目录文件的atime
	WriteInode(inode, number);
	return 0;
}

int fs_read(const char* path, char* buffer, size_t size, off_t offset, struct fuse_file_info* fi)
{
	puts("fs_read begins!");
	int err = setjmp(error);
	if (err)
		return err;
	unsigned short number;
	Inode inode = Ptoi(path, &number);
	if (inode.directory)
		return -EISDIR;
	char ptr[BLOCK_SIZE];
	disk_read(inode.indirpointer, ptr);
	unsigned short* BlockNumber = (unsigned short*)ptr;
	//根据偏移量和读取长度进行读取
	for (int i = offset / BLOCK_SIZE, blk_offset = (offset & 0xfff), blk_size, remain_size = size; (blk_size = (BLOCK_SIZE - blk_offset) < (remain_size) ? BLOCK_SIZE - blk_offset : remain_size) != 0; ++i, blk_offset = 0, remain_size -= blk_size)
	{
		char block[BLOCK_SIZE];
		disk_read(BlockNumber[i], block);
		memcpy(buffer, &block[blk_offset], blk_size);
		buffer += blk_size;
	}
	inode.atime = time(NULL);
	WriteInode(inode, number);
	return size;
}

int fs_mknod(const char* path, mode_t mode, dev_t dev)
{
	puts("fs_mknod begins!");
	char name[25];
	bool use;
	unsigned short number;
	int err = setjmp(error);
	if (err)
		return err;
	if (!FindInode(1) || !FindBlock(2)) //空间不够用则直接报错
		return -ENOSPC;
	Inode inode = Ptopi(path, &number, name, &use);
	int count = inode.size / sizeof(Dir);
	Dir ptr[InodeMax];
	GetDir(inode, ptr);
	MoreDir(inode); //检查新增一个目录项时父目录是否需要新分配一个数据块
	inode.size += sizeof(Dir);
	ptr[count].number = NewInode();
	memcpy(ptr[count].filename, name, 24);
	InitInode(ptr[count].number, 0); //为这个新目录分配一个inode并调用InitInode函数
	inode.atime = time(NULL);
	inode.mtime = inode.atime;
	inode.ctime = inode.atime;
	WriteDir(inode, ptr);
	WriteInode(inode, number);
	return 0;
}

int fs_mkdir(const char* path, mode_t mode)
{
	char name[25];
	bool use;
	unsigned short number;
	int err = setjmp(error);
	if (err)
		return err;
	if (!FindInode(1) || !FindBlock(2))
		return -ENOSPC;
	Inode inode = Ptopi(path, &number, name, &use);
	int count;
	count = inode.size / sizeof(Dir);
	Dir ptr[InodeMax];
	GetDir(inode, ptr);
	MoreDir(inode);
	inode.size += sizeof(Dir);
	ptr[count].number = NewInode();
	memcpy(ptr[count].filename, name, 24);
	InitInode(ptr[count].number, 1);
	inode.atime = time(NULL);
	inode.mtime = inode.atime;
	inode.ctime = inode.atime;
	WriteDir(inode, ptr);
	WriteInode(inode, number);
	return 0;
}

int fs_rmdir(const char* path)
{
	puts("fs_rmdir begins!");
	int err = setjmp(error);
	if (err)
		return err;
	char name[25];
	unsigned short number;
	Inode inode = Ptopi(path, &number, name, NULL);
	Dir dir[InodeMax];
	GetDir(inode, dir);
	int DirNumber = inode.size / sizeof(Dir);
	Dir* ptr = FindDir(dir, DirNumber, name, strlen(name));
	if (!ptr)
		return -ENOENT;
	EraseInode(ptr->number); //删除要求文件的目录项
	DirNumber = inode.size / sizeof(Dir);
	memcpy(ptr, ptr + 1, sizeof(Dir) * (dir + DirNumber - (ptr + 1)));
	inode.ctime = time(NULL);
	inode.mtime = inode.ctime;
	inode.size -= sizeof(Dir);
	WriteDir(inode, dir);
	WriteInode(inode, number); //更新时间
	return 0;
}

int fs_unlink(const char* path)
{
	puts("fs_unlink begins!");
	char name[25];
	unsigned short number;
	int err = setjmp(error);
	if (err)
		return err;
	Inode inode = Ptopi(path, &number, name, NULL);
	Dir dir[InodeMax];
	GetDir(inode, dir);
	int DirNumber = inode.size / sizeof(Dir);
	Dir* ptr = FindDir(dir, DirNumber, name, strlen(name));
	if (!ptr)
		return -ENOENT;
	EraseInode(ptr->number);
	DirNumber = inode.size / sizeof(Dir);
	memcpy(ptr, ptr + 1, sizeof(Dir) * (dir + DirNumber - (ptr + 1)));
	inode.ctime = time(NULL);
	inode.mtime = inode.ctime;
	inode.size -= sizeof(Dir);
	WriteDir(inode, dir);
	WriteInode(inode, number);
	return 0;
}

int fs_rename(const char* oldpath, const char* newpath)
{
	puts("fs_rename begins!");
	if (!strcmp(oldpath, newpath))
		return 0;
	fs_unlink(newpath);
	int err = setjmp(error);
	if (err)
		return err;
	char from_name[25];
	char to_name[25];
	unsigned short orgname;
	unsigned short nowname;
	Inode ifrom = Ptopi(oldpath, &orgname, from_name, NULL); //文件原位置inode
	Inode ito = Ptopi(newpath, &nowname, to_name, NULL); //文件目标位置inode
	Dir from_dir[InodeMax];
	GetDir(ifrom, from_dir);
	int DirNumber = ifrom.size / sizeof(Dir);
	Dir* orgdir = FindDir(from_dir, DirNumber, from_name, strlen(from_name));
	ifrom.atime = ifrom.mtime = time(NULL);
	if (!orgdir)
		return -ENOENT;
	if (orgname == nowname) //若二者的目录项相同，则我们只需要在原位置的目录项中改名即可
		memcpy(orgdir->filename, to_name, 24);
	else //在原位置的父目录中删去一个目录项，之后在新位置的父目录中新增一个目录项
	{
		Dir to_dir[InodeMax];
		GetDir(ito, to_dir);
		DirNumber = ito.size / sizeof(Dir);
		Dir* nowdir = FindDir(to_dir, DirNumber, to_name, strlen(to_name));
		if (nowdir)
			return -EEXIST;
		MoreDir(ito);
		DirNumber = ito.size / sizeof(Dir);
		int count = DirNumber;
		memcpy(to_dir[count].filename, to_name, 24);
		to_dir[count].number = orgdir->number;
		ito.size += sizeof(Dir);
		ito.atime = ito.mtime = time(NULL);
		WriteDir(ito, to_dir);
		WriteInode(ito, nowname);
		DirNumber = ifrom.size / sizeof(Dir);
		memcpy(orgdir, orgdir + 1, sizeof(Dir) * ((from_dir + DirNumber) - (orgdir + 1)));
		ifrom.size -= sizeof(Dir);
	}
	WriteDir(ifrom, from_dir);
	WriteInode(ifrom, orgname);
	return 0;
}

int fs_write(const char* path, const char* buffer, size_t size, off_t offset, struct fuse_file_info* fi)
{
	puts("fs_write begins!");
	int err = setjmp(error);
	if (err)
		return err;
	unsigned short number;
	Inode inode = Ptoi(path, &number);
	if (inode.directory)
		return -EISDIR;
	char ptr[BLOCK_SIZE];
	disk_read(inode.indirpointer, ptr);
	unsigned short* BlockNumbers = (unsigned short*)ptr;
	int count = 0;
	for (int i = offset / BLOCK_SIZE, blk_offset = (offset & 0xfff), blk_size, remain_size = size; (blk_size = (BLOCK_SIZE - blk_offset < remain_size) ? BLOCK_SIZE - blk_offset : remain_size) != 0; i++, blk_offset = 0, remain_size -= blk_size)
	{
		if (!BlockNumbers[i])
			++count;
	}
	unsigned short NewBlock[count];
	AllocateBlocks(count, NewBlock);
	for (int i = offset / BLOCK_SIZE, blk_offset = (offset & 0xfff), blk_size, remain_size = size; (blk_size = (BLOCK_SIZE - blk_offset < remain_size) ? BLOCK_SIZE - blk_offset : remain_size) != 0; i++, blk_offset = 0, remain_size -= blk_size)
	{
		if (!BlockNumbers[i])
			BlockNumbers[i] = NewBlock[--count];
		char block[BLOCK_SIZE];
		disk_read(BlockNumbers[i], block);
		memcpy(&block[blk_offset], buffer, blk_size);
		buffer += blk_size;
		disk_write(BlockNumbers[i], block);
	}
	disk_write(inode.indirpointer, ptr);
	inode.mtime = inode.ctime = time(NULL);
	if(inode.size < size + offset)
		inode.size = size + offset;
	WriteInode(inode, number);
	return size;
}

int fs_truncate(const char* path, off_t size)
{
	puts("fs_truncate begins!");
	int err = setjmp(error);
	if (err)
		return err;
	unsigned short number;
	Inode inode = Ptoi(path, &number);
	if (inode.directory)
		return -EISDIR;
	char ptr[BLOCK_SIZE];
	disk_read(inode.indirpointer, ptr);
	int num1 = inode.size / BLOCK_SIZE;
	if(inode.size % BLOCK_SIZE > 0)
		++num1;
	int num2 = size / BLOCK_SIZE;
	if(size % BLOCK_SIZE > 0)
		++num2;
	unsigned short* BlockNumber = (unsigned short*)ptr;
	if (size > inode.size)
	{
		int need = num2 - num1;
		unsigned short NewBlock[need];
		AllocateBlocks(need, NewBlock); //如果块数变多，则新分配相应数量的块
		for (int i = num1; i < num2; i++)
			BlockNumber[i] = NewBlock[--need];
	}
	else if(size <= inode.size)
	{
		int need = size % BLOCK_SIZE;
		if (need)
		{
			char last[BLOCK_SIZE];
			disk_read(BlockNumber[need - 1], last);
			memset(last + need, 0, BLOCK_SIZE - need);
			disk_write(BlockNumber[need - 1], last);
		}
		DeleteBlocks(&BlockNumber[num2], num1 - num2);
	}
	inode.size = size;
	inode.ctime = time(NULL);
	disk_write(inode.indirpointer, ptr);
	WriteInode(inode, number);
	return 0;
}

int fs_utime(const char* path, struct utimbuf* buffer)
{
	puts("fs_utime begins!");
	int err = setjmp(error);
	if (err)
		return err;
	unsigned short number;
	Inode inode;
	inode = Ptoi(path, &number);
	inode.atime = buffer->actime;
	inode.mtime = buffer->modtime;
	inode.ctime = time(NULL);
	WriteInode(inode, number);
	return 0;
}

int fs_statfs(const char* path, struct statvfs* stat)
{
	puts("fs_statfs begins!");
	unsigned long long imap[512];
	unsigned long long bmap[1024];
	disk_read(InodeBitMap, imap);
	disk_read(DataBitMap1, (void*)bmap);
	disk_read(DataBitMap2, (void*)bmap + BLOCK_SIZE);
	stat->f_bsize = BLOCK_SIZE;
	stat->f_blocks = 65277;
	int temp = 0;
	for (int i = DataStart; i < DataMax; ++i)
	{
		int place = i / 64;
		int offset = i % 64;
		if (((unsigned long long*)bmap)[place] >> offset & 1)
			++temp;
	}
	stat->f_bavail = stat->f_bfree = DataMax - DataStart + 1 - temp;
	stat->f_files = InodeMax;
	temp = 0;
	for (int i = 0; i < InodeMax; ++i)
	{
		int place = i / 64;
		int offset = i % 64;
		if (((unsigned long long*)imap)[place] >> offset & 1)
			++temp;
	}
	stat->f_ffree = stat->f_favail = InodeMax - 1 - temp;
	stat->f_namemax = 24;
	return 0;
}

int fs_open(const char* path, struct fuse_file_info* fi)
{
	printf("Open is called:%s\n", path);
	return 0;
}

//Functions you don't actually need to modify
int fs_release(const char* path, struct fuse_file_info* fi)
{
	printf("Release is called:%s\n", path);
	return 0;
}

int fs_opendir(const char* path, struct fuse_file_info* fi)
{
	printf("Opendir is called:%s\n", path);
	return 0;
}

int fs_releasedir(const char* path, struct fuse_file_info* fi)
{
	printf("Releasedir is called:%s\n", path);
	return 0;
}

static struct fuse_operations fs_operations = {
	.getattr = fs_getattr,
	.readdir = fs_readdir,
	.read = fs_read,
	.mkdir = fs_mkdir,
	.rmdir = fs_rmdir,
	.unlink = fs_unlink,
	.rename = fs_rename,
	.truncate = fs_truncate,
	.utime = fs_utime,
	.mknod = fs_mknod,
	.write = fs_write,
	.statfs = fs_statfs,
	.open = fs_open,
	.release = fs_release,
	.opendir = fs_opendir,
	.releasedir = fs_releasedir
};

int main(int argc, char* argv[])
{
	if (disk_init())
	{
		printf("Can't open virtual disk!\n");
		return -1;
	}
	if (mkfs())
	{
		printf("Mkfs failed!\n");
		return -2;
	}
	return fuse_main(argc, argv, &fs_operations, NULL);
}
