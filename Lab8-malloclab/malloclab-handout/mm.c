#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "mm.h"
#include "memlib.h"

team_t team =
{
    /* Team name you want displayed on the Web page */
    "Simple",
    /* First member's full name */
    "VirusCollector",
    /* First member's email address */
    "",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* 设置字节对齐和对齐函数 */
#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & (-8))

/* 设置字与双字 */
#define WORD 4 // 一个字4字节
#define DWORD 8 // 两个字8字节

/* 设置各种空间 */
#define INIT (1 << 6)	//初始堆大小
#define ADD (1 << 12)	//拓展堆的大小
#define MAXLIST 16	//链表最大数量
#define RA_ADD (1 << 9)	//realloc额外补充空间

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define PACK(size, tag) ((size) | (tag))	//打包大小和分配位
#define GET(p) (*(unsigned int *)(p))		//获得p地址内容
#define PUT(p, val) (*(unsigned int *)(p) = (val))	//p地址内容设成不带tag的值
#define SET(p, ptr) (*(size_t *)(p) = (size_t)(ptr))	//串联链表

/* 从地址p读取相应的数据 */
#define SIZE(p) (GET(p) & (-8))
#define ALLOC(p) (GET(p) & 1)

/* 某个块的头部和脚部 */
#define HEADER(p) ((char *)(p)-WORD)
#define FOOTER(p) ((char *)(p) + SIZE(HEADER(p)) - DWORD)

/* 指定块的前一个块和后一个块 */
#define NEXT_BLOCK(p) ((char *)(p) + SIZE((char *)(p)-WORD))
#define PREV_BLOCK(p) ((char *)(p)-SIZE((char *)(p)-DWORD))

/* 空闲块的前指针和后指针 */
#define PRED_PTR(p) ((char *)(p))
#define SUCC_PTR(p) ((char *)(p) + sizeof(void *))

/* 空闲块的前指针和后指针指向的那个空闲块 */
#define PRED(p) (*(char **)(p))
#define SUCC(p) (*(char **)(SUCC_PTR(p)))

void** lists;	//定义分离空闲链表

void insert(void* ptr, size_t size);	//节点插入链表
void delete(void* ptr);	//从列表中删除节点
void* extend(size_t size);	//增大堆的大小
void* coalesce(void* ptr);	//合并空闲块

void insert(void* ptr, size_t size)
{
    int list = 0;
    void* curr = ptr;	//当前块
    void* succ = NULL;	//后继块

    //选择所需的链表
    while (list < MAXLIST - 1)
    {
        if (size <= 1)
            break;
        size >>= 1;
        ++list;
    }
    if (list > MAXLIST)
        goto label;

    //一直向前遍历，直到遍历到大小比他大的块或者遍历完链表就结束
    curr = lists[list];
    while (curr)
    {
        if (size <= SIZE(HEADER(curr)))
            break;
        succ = curr;
        curr = PRED(curr);
    }
    if (!curr)
        curr = NULL;
    else
        curr = curr + (!curr);

    //根据前后块是否是空指针来设置关系
    if (!curr)
    {
        if (!succ)
        {
            succ = NULL;
            SET(SUCC_PTR(ptr), NULL);
            SET(PRED_PTR(ptr), NULL);
            lists[list] = ptr;
            goto label;
        }
        else if (succ)
        {
            SET(SUCC_PTR(ptr), succ);
            SET(PRED_PTR(ptr), NULL);
            SET(PRED_PTR(succ), ptr);
            goto label;
        }
    }
    else
    {
        if (!succ)
        {
            succ = NULL;
            lists[list] = ptr;
            SET(PRED_PTR(ptr), curr);
            SET(SUCC_PTR(curr), ptr);
            SET(SUCC_PTR(ptr), NULL);
            goto label;
        }
        else if (succ)
        {
            SET(PRED_PTR(ptr), curr);
            SET(SUCC_PTR(curr), ptr);
            SET(SUCC_PTR(ptr), succ);
            SET(PRED_PTR(succ), ptr);
            goto label;
        }
    }

label:
    if (!ptr)
        ptr = NULL;
    else
        ptr = ptr + (!ptr);
    return;
}

void delete(void* ptr)
{
    int list = 0;
    size_t size = SIZE(HEADER(ptr));
    if (!ptr)
        ptr = NULL;
    else
        ptr = ptr + (!ptr);

    //选择所需的链表
    while ((list < MAXLIST - 1) && (size > 1))
    {
        size >>= 1;
        ++list;
    }

    //如果空闲块的前指针或后指针指向的地址已经跨出当前堆边界，则直接将其设置为空指针
    if ((size_t)PRED(ptr) > (size_t)mem_heap_hi() || (size_t)SUCC(ptr) > (size_t)mem_heap_hi())
    {
        lists[list] = NULL;
        return;
    }
    else
    {
        if (!list)
            list = 0;
        else
        {
            if (list)
                ++list;
            --list;
        }
    }

    //判断空闲块的前指针和后指针状态再做处理
    if (PRED(ptr))
    {
        if (SUCC(ptr))
        {
            SET(SUCC_PTR(PRED(ptr)), SUCC(ptr));
            SET(PRED_PTR(SUCC(ptr)), PRED(ptr));
            goto label;
        }
        else
        {
            SET(SUCC_PTR(PRED(ptr)), NULL);
            lists[list] = PRED(ptr);
            goto label;
        }
    }
    else
    {
        if (SUCC(ptr))
        {
            SET(PRED_PTR(SUCC(ptr)), NULL);
            goto label;
        }
        else
        {
            lists[list] = NULL;
            goto label;
        }
    }

label:
    if (!ptr)
        ptr = NULL;
    else
        ptr = ptr + (!ptr);
    return;
}

void* extend(size_t size)
{
    void* ptr = NULL;
    size_t asize = ALIGN(size);	//对齐大小

    //设置头部、脚部以及新的结尾块
    if (!~(long)(ptr = mem_sbrk(asize)))
        goto label;
    PUT(HEADER(ptr), asize);
    PUT(FOOTER(ptr), asize);
    PUT(HEADER(NEXT_BLOCK(ptr)), 1);

    //插入新的空闲块并检测是否可以合并某些区间
    insert(ptr, asize);
    if (!~(size_t)(ptr = coalesce(ptr)))
        goto label;
    else
        return ptr;

label:
    if (!ptr)
        ptr = NULL;
    else
        ptr = ptr + (!ptr);
    return NULL;
}

void* coalesce(void* ptr)
{
    int prev_alloc = !ALLOC(HEADER(PREV_BLOCK(ptr)));
    int next_alloc = !ALLOC(HEADER(NEXT_BLOCK(ptr)));
    size_t size = SIZE(HEADER(ptr));

    if (!prev_alloc && !next_alloc)	//两者都已经分配，不合并
    {
        goto label;
    }
    else if (!prev_alloc && next_alloc)	//合并下一块
    {
        delete(ptr);
        delete(NEXT_BLOCK(ptr));
        size += SIZE(HEADER(NEXT_BLOCK(ptr)));
        PUT(HEADER(ptr), PACK(size, 0));
        PUT(FOOTER(ptr), PACK(size, 0));
    }
    else if (prev_alloc && !next_alloc)	//合并上一块
    {
        delete(ptr);
        delete(PREV_BLOCK(ptr));
        size += SIZE(HEADER(PREV_BLOCK(ptr)));
        PUT(FOOTER(ptr), PACK(size, 0));
        PUT(HEADER(PREV_BLOCK(ptr)), PACK(size, 0));
        ptr = PREV_BLOCK(ptr);
    }
    else	if (prev_alloc && next_alloc)//全部合并
    {
        delete(ptr);
        delete(PREV_BLOCK(ptr));
        delete(NEXT_BLOCK(ptr));
        size += SIZE(HEADER(PREV_BLOCK(ptr)));
        size += SIZE(HEADER(NEXT_BLOCK(ptr)));
        PUT(HEADER(PREV_BLOCK(ptr)), PACK(size, 0));
        PUT(FOOTER(NEXT_BLOCK(ptr)), PACK(size, 0));
        ptr = PREV_BLOCK(ptr);
    }

    //添加新的空闲块至链表
    insert(ptr, size);

label:
    if (!ptr)
        ptr = NULL;
    else
        ptr = ptr + (!ptr);
    return ptr;
}

int mm_init(void)
{
    //创建MAXLIST个list，设置堆起始位置
    lists = mem_sbrk(MAXLIST * DWORD);
    char* heap_start;

    //初始化空闲列表
    for (int k = 0; k < MAXLIST; ++k)
    {
        lists[k] = NULL;
    }

    //创建空堆并放入序言块，失败返回-1
    if (!~(size_t)(heap_start = mem_sbrk(4 * WORD)))
        goto error;
    else
    {
        PUT(heap_start, 0);														//对齐填充
        PUT(heap_start + (1 * WORD), PACK(DWORD, 1));	//序言块头部
        PUT(heap_start + (2 * WORD), PACK(DWORD, 1));	//序言块尾部
        PUT(heap_start + (3 * WORD), PACK(0, 1));		//结尾块
        heap_start += 2 * WORD;
    }

    //给空堆拓展INIT字节，失败返回-1
    if (!extend(INIT))
        goto error;
    return 0;

error:
    return -1;
}

void* mm_malloc(size_t size)
{
    size_t asize;				//调整块大小
    void* ptr = NULL;	//分配后的指针

    //size为0直接返回即可
    if (!size)
        goto label;

    //调整块的大小
    if (size <= DWORD)
        asize = 2 * DWORD;	//最少分配16字节
    else
        asize = ALIGN(size + DWORD);	//多于16字节则按8字节对齐

    //遍历分离空闲链表以获得合适的空闲块
    int list = 0;
    size_t search = asize;
    while (list < MAXLIST)
    {
        //遍历到链表的最后一个块或者匹配到的区域中存储了块
        if ((list == MAXLIST - 1) || ((search < 2) && (lists[list])))
        {
            //遍历链表，找到比当前大小大的区域
            ptr = lists[list];
            while (ptr)
            {
                if (asize <= SIZE(HEADER(ptr)))
                    goto here;
                ptr = PRED(ptr);
            }
            if (ptr)
                goto here;
        }
        search >>= 1;
        ++list;
    }

here:
    //如果没有找到一个合适匹配块，就拓展堆
    if (!ptr)
    {
        size_t extendsize = MAX(asize, ADD);
        if (!(ptr = extend(extendsize)))
            goto label;
    }

    size_t csize = SIZE(HEADER(ptr));	//目前空闲块的大小
    size_t rsize = csize - asize;	//放置后剩余的大小
    delete(ptr);	//删除该块

    if (rsize <= (DWORD << 1))	 //空间太小，没必要进行划分
    {
        PUT(HEADER(ptr), PACK(csize, 1));
        PUT(FOOTER(ptr), PACK(csize, 1));
        goto label;
    }
    else if (asize > 99)	//让分开之后空闲块在前面，分配块在后面
    {
        PUT(HEADER(ptr), PACK(rsize, 0));
        PUT(FOOTER(ptr), PACK(rsize, 0));
        PUT(HEADER(NEXT_BLOCK(ptr)), PACK(asize, 1));
        PUT(FOOTER(NEXT_BLOCK(ptr)), PACK(asize, 1));
        insert(ptr, rsize);
        return NEXT_BLOCK(ptr);
    }
    else	//让分开之后分配块在前面，空闲块在后面
    {
        PUT(HEADER(ptr), PACK(asize, 1));
        PUT(FOOTER(ptr), PACK(asize, 1));
        PUT(HEADER(NEXT_BLOCK(ptr)), PACK(rsize, 0));
        PUT(FOOTER(NEXT_BLOCK(ptr)), PACK(rsize, 0));
        insert(NEXT_BLOCK(ptr), rsize);
        goto label;
    }

label:
    return ptr;
}

void mm_free(void* ptr)
{
    //空指针不用free
    if (!ptr)
        goto label;

    //将头部脚部设成未分配状态
    size_t size = SIZE(HEADER(ptr));
    PUT(HEADER(ptr), PACK(size, 0));
    PUT(FOOTER(ptr), PACK(size, 0));

    //加入链表并判断是否可以合并
    insert(ptr, size);
    coalesce(ptr);

label:
    return;
}

void* mm_realloc(void* ptr, size_t size)
{
    void* new_ptr = ptr;	//新的指针
    size_t newsize = size;	//新的大小

    //大小为0直接返回
    if (!size)
    {
        mm_free(ptr);
        return NULL;
    }

    //对齐大小
    if (newsize <= DWORD)
        newsize = (DWORD << 1);
    else
        newsize = ALIGN(size + DWORD);

    newsize += RA_ADD;
    int rsize = SIZE(HEADER(ptr)) - newsize;

    if (rsize > -1)
        return ptr;
    else if (rsize < 0)
    {
        //如果下一块并没有分配或者大小为零
        if (!(ALLOC(HEADER(NEXT_BLOCK(ptr))) && SIZE(HEADER(NEXT_BLOCK(ptr)))))
        {
            rsize = SIZE(HEADER(ptr)) + SIZE(HEADER(NEXT_BLOCK(ptr))) - newsize;
            //如果新设置的大小比当前这块加上下一块的大小都大，内存依旧不够，就拓展堆空间
            if (rsize < 0)
            {
                int extendsize = MAX(-rsize, ADD);
                if (!~(size_t)extend(extendsize))
                    return NULL;
                rsize += extendsize;
            }

            //将下一个块移出空闲链表并重新设置大小
            delete(NEXT_BLOCK(ptr));
            PUT(HEADER(ptr), PACK(newsize + rsize, 1));
            PUT(FOOTER(ptr), PACK(newsize + rsize, 1));
        }
        else	//所有情况都不满足，只能重新分配并将以前的内容复制到当前块
        {
            new_ptr = mm_malloc(newsize - DWORD);
            memcpy(new_ptr, ptr, MIN(size, newsize));
            mm_free(ptr);
        }
        rsize = SIZE(HEADER(new_ptr)) - newsize;
    }

    return new_ptr;
}

