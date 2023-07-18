#bomblab实验报告<br>

##main
main函数汇编代码如下：
<details>
<summary>Code</summary>

    22e9:	53                   	push   rbx
    22ea:	83 ff 01             	cmp    edi,0x1
    22ed:	0f 84 f8 00 00 00    	je     23eb <main+0x102>
    22f3:	48 89 f3             	mov    rbx,rsi
    22f6:	83 ff 02             	cmp    edi,0x2
    22f9:	0f 85 1c 01 00 00    	jne    241b <main+0x132>
    22ff:	48 8b 7e 08          	mov    rdi,QWORD PTR [rsi+0x8]
    2303:	48 8d 35 fa 1c 00 00 	lea    rsi,[rip+0x1cfa]        # 4004 <_IO_stdin_used+0x4>
    230a:	e8 61 fe ff ff       	call   2170 <fopen@plt>
    230f:	48 89 05 da f4 00 00 	mov    QWORD PTR [rip+0xf4da],rax        # 117f0 <infile>
    2316:	48 85 c0             	test   rax,rax
    2319:	0f 84 df 00 00 00    	je     23fe <main+0x115>
    231f:	e8 b6 06 00 00       	call   29da <initialize_bomb>
    2324:	48 8d 3d 55 1d 00 00 	lea    rdi,[rip+0x1d55]        # 4080 <_IO_stdin_used+0x80>
    232b:	e8 40 fd ff ff       	call   2070 <puts@plt>
    2330:	48 8d 3d 89 1d 00 00 	lea    rdi,[rip+0x1d89]        # 40c0 <_IO_stdin_used+0xc0>
    2337:	e8 34 fd ff ff       	call   2070 <puts@plt>
    233c:	e8 1a 0a 00 00       	call   2d5b <read_line>
    2341:	48 89 c7             	mov    rdi,rax
    2344:	e8 f0 00 00 00       	call   2439 <phase_1>
    2349:	e8 47 0b 00 00       	call   2e95 <phase_defused>
    234e:	48 8d 3d 9b 1d 00 00 	lea    rdi,[rip+0x1d9b]        # 40f0 <_IO_stdin_used+0xf0>
    2355:	e8 16 fd ff ff       	call   2070 <puts@plt>
    235a:	e8 fc 09 00 00       	call   2d5b <read_line>
    235f:	48 89 c7             	mov    rdi,rax
    2362:	e8 f2 00 00 00       	call   2459 <phase_2>
    2367:	e8 29 0b 00 00       	call   2e95 <phase_defused>
    236c:	48 8d 3d ca 1c 00 00 	lea    rdi,[rip+0x1cca]        # 403d <_IO_stdin_used+0x3d>
    2373:	e8 f8 fc ff ff       	call   2070 <puts@plt>
    2378:	e8 de 09 00 00       	call   2d5b <read_line>
    237d:	48 89 c7             	mov    rdi,rax
    2380:	e8 43 01 00 00       	call   24c8 <phase_3>
    2385:	e8 0b 0b 00 00       	call   2e95 <phase_defused>
    238a:	48 8d 3d bd 1c 00 00 	lea    rdi,[rip+0x1cbd]        # 404e <_IO_stdin_used+0x4e>
    2391:	e8 da fc ff ff       	call   2070 <puts@plt>
    2396:	e8 c0 09 00 00       	call   2d5b <read_line>
    239b:	48 89 c7             	mov    rdi,rax
    239e:	e8 6d 02 00 00       	call   2610 <phase_4>
    23a3:	e8 ed 0a 00 00       	call   2e95 <phase_defused>
    23a8:	48 8d 3d 71 1d 00 00 	lea    rdi,[rip+0x1d71]        # 4120 <_IO_stdin_used+0x120>
    23af:	e8 bc fc ff ff       	call   2070 <puts@plt>
    23b4:	e8 a2 09 00 00       	call   2d5b <read_line>
    23b9:	48 89 c7             	mov    rdi,rax
    23bc:	e8 cd 02 00 00       	call   268e <phase_5>
    23c1:	e8 cf 0a 00 00       	call   2e95 <phase_defused>
    23c6:	48 8d 3d 90 1c 00 00 	lea    rdi,[rip+0x1c90]        # 405d <_IO_stdin_used+0x5d>
    23cd:	e8 9e fc ff ff       	call   2070 <puts@plt>
    23d2:	e8 84 09 00 00       	call   2d5b <read_line>
    23d7:	48 89 c7             	mov    rdi,rax
    23da:	e8 f7 02 00 00       	call   26d6 <phase_6>
    23df:	e8 b1 0a 00 00       	call   2e95 <phase_defused>
    23e4:	b8 00 00 00 00       	mov    eax,0x0
    23e9:	5b                   	pop    rbx
    23ea:	c3                   	ret
    23eb:	48 8b 05 de f3 00 00 	mov    rax,QWORD PTR [rip+0xf3de]        # 117d0 <stdin@GLIBC_2.2.5>
    23f2:	48 89 05 f7 f3 00 00 	mov    QWORD PTR [rip+0xf3f7],rax        # 117f0 <infile>
    23f9:	e9 21 ff ff ff       	jmp    231f <main+0x36>
    23fe:	48 8b 53 08          	mov    rdx,QWORD PTR [rbx+0x8]
    2402:	48 8b 33             	mov    rsi,QWORD PTR [rbx]
    2405:	48 8d 3d fa 1b 00 00 	lea    rdi,[rip+0x1bfa]        # 4006 <_IO_stdin_used+0x6>
    240c:	e8 9f fc ff ff       	call   20b0 <printf@plt>
    2411:	bf 08 00 00 00       	mov    edi,0x8
    2416:	e8 85 fd ff ff       	call   21a0 <exit@plt>
    241b:	48 8b 36             	mov    rsi,QWORD PTR [rsi]
    241e:	48 8d 3d fe 1b 00 00 	lea    rdi,[rip+0x1bfe]        # 4023 <_IO_stdin_used+0x23>
    2425:	b8 00 00 00 00       	mov    eax,0x0
    242a:	e8 81 fc ff ff       	call   20b0 <printf@plt>
    242f:	bf 08 00 00 00       	mov    edi,0x8
    2434:	e8 67 fd ff ff       	call   21a0 <exit@plt>
</details>
由分析知，main函数主体逻辑是首先初始化bomb（即通过initialize_bomb函数检测当前运行环境，连接到服务器等），然后依次调用readline函数读取一行输入并进入一个phase进行判断，只要能从phase中成功返回即表示你成功完成了该阶段的实验。

##phase1
phase1函数汇编代码如下：

<details>
<summary>Code</summary>

	2439:	48 83 ec 08          	sub    rsp,0x8
    243d:	48 8d 35 04 1d 00 00 	lea    rsi,[rip+0x1d04]        # 4148 <_IO_stdin_used+0x148>
    2444:	e8 35 05 00 00       	call   297e <strings_not_equal>
    2449:	85 c0                	test   eax,eax
    244b:	75 05                	jne    2452 <phase_1+0x19>
    244d:	48 83 c4 08          	add    rsp,0x8
    2451:	c3                   	ret
    2452:	e8 03 08 00 00       	call   2c5a <explode_bomb>
    2457:	eb f4                	jmp    244d <phase_1+0x14>

</details>
可以看出就是比较你输入的字符串和[rip+0x1d04]处的字符串是否相等，根据objdump的提示找到4148处的字符串，如图所示：
<details>
<summary>Code</summary>

	4140 6e652e00 00000000 54686520 7472656d  ne......The trem
 	4150 626c696e 67206665 61722069 73206d6f  bling fear is mo
 	4160 72652074 68616e20 49206361 6e207461  re than I can ta
 	4170 6b652077 68656e20 49276d20 75702061  ke when I'm up a
 	4180 6761696e 73742074 68652045 43484f20  gainst the ECHO 
 	4190 696e2074 6865206d 6972726f 722e0000  in the mirror...
</details>
因此答案为The trembling fear is more than I can take when I'm up against the ECHO in the mirror.

##phase2
phase函数汇编代码如下：
<details>
<summary>Code</summary>

	2459:	55                   	push   rbp
    245a:	53                   	push   rbx
    245b:	48 83 ec 28          	sub    rsp,0x28
    245f:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    2466:	00 00 
    2468:	48 89 44 24 18       	mov    QWORD PTR [rsp+0x18],rax
    246d:	31 c0                	xor    eax,eax
    246f:	48 89 e6             	mov    rsi,rsp
    2472:	e8 a3 08 00 00       	call   2d1a <read_six_numbers>
    2477:	83 3c 24 0f          	cmp    DWORD PTR [rsp],0xf
    247b:	75 0a                	jne    2487 <phase_2+0x2e>
    247d:	48 89 e3             	mov    rbx,rsp
    2480:	48 8d 6c 24 14       	lea    rbp,[rsp+0x14]
    2485:	eb 10                	jmp    2497 <phase_2+0x3e>
    2487:	e8 ce 07 00 00       	call   2c5a <explode_bomb>
    248c:	eb ef                	jmp    247d <phase_2+0x24>
    248e:	48 83 c3 04          	add    rbx,0x4
    2492:	48 39 eb             	cmp    rbx,rbp
    2495:	74 15                	je     24ac <phase_2+0x53>
    2497:	8b 05 87 4c 00 00    	mov    eax,DWORD PTR [rip+0x4c87]        # 7124 <mul.2>
    249d:	0f af 03             	imul   eax,DWORD PTR [rbx]
    24a0:	39 43 04             	cmp    DWORD PTR [rbx+0x4],eax
    24a3:	74 e9                	je     248e <phase_2+0x35>
    24a5:	e8 b0 07 00 00       	call   2c5a <explode_bomb>
    24aa:	eb e2                	jmp    248e <phase_2+0x35>
    24ac:	48 8b 44 24 18       	mov    rax,QWORD PTR [rsp+0x18]
    24b1:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    24b8:	00 00 
    24ba:	75 07                	jne    24c3 <phase_2+0x6a>
    24bc:	48 83 c4 28          	add    rsp,0x28
    24c0:	5b                   	pop    rbx
    24c1:	5d                   	pop    rbp
    24c2:	c3                   	ret
    24c3:	e8 d8 fb ff ff       	call   20a0 <__stack_chk_fail@plt>
</details>
函数主体逻辑比较清晰，自2472处指令开始，首先读取输入的6个数字，然后比较DWORD PTR [rsp]（输入的第一个数字）是否为15，若为15则将eax中存入7124位置的数（查看发现是7），再乘以当前的值，比较下一个值（247d和24a0处指令可得）和eax中数值是否相等。<br>
由此可以明白该题需要我们输入一个首项为15，公比为7的等比数列，答案即为15 105 735 5145 36015 252105

##phase3
phase3汇编代码如下：<br>
<details>
<summary>Code</summary>

    24c8:	48 83 ec 18          	sub    rsp,0x18
    24cc:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    24d3:	00 00 
    24d5:	48 89 44 24 08       	mov    QWORD PTR [rsp+0x8],rax
    24da:	31 c0                	xor    eax,eax
    24dc:	48 8d 4c 24 04       	lea    rcx,[rsp+0x4]
    24e1:	48 89 e2             	mov    rdx,rsp
    24e4:	48 8d 35 36 21 00 00 	lea    rsi,[rip+0x2136]        # 4621 <array.0+0x421>
    24eb:	e8 60 fc ff ff       	call   2150 <__isoc99_sscanf@plt>
    24f0:	83 f8 01             	cmp    eax,0x1
    24f3:	7e 1d                	jle    2512 <phase_3+0x4a>
    24f5:	83 3c 24 07          	cmp    DWORD PTR [rsp],0x7
    24f9:	0f 87 c0 00 00 00    	ja     25bf <phase_3+0xf7>
    24ff:	8b 04 24             	mov    eax,DWORD PTR [rsp]
    2502:	48 8d 15 d7 1c 00 00 	lea    rdx,[rip+0x1cd7]        # 41e0 <_IO_stdin_used+0x1e0>
    2509:	48 63 04 82          	movsxd rax,DWORD PTR [rdx+rax*4]
    250d:	48 01 d0             	add    rax,rdx
    2510:	ff e0                	jmp    rax
    2512:	e8 43 07 00 00       	call   2c5a <explode_bomb>
    2517:	eb dc                	jmp    24f5 <phase_3+0x2d>
    2519:	8b 15 01 4c 00 00    	mov    edx,DWORD PTR [rip+0x4c01]        # 7120 <delta.1>
    251f:	b8 5d 02 00 00       	mov    eax,0x25d
    2524:	29 d0                	sub    eax,edx
    2526:	8b 54 24 04          	mov    edx,DWORD PTR [rsp+0x4]
    252a:	85 d2                	test   edx,edx
    252c:	78 04                	js     2532 <phase_3+0x6a>
    252e:	39 c2                	cmp    edx,eax
    2530:	74 05                	je     2537 <phase_3+0x6f>
    2532:	e8 23 07 00 00       	call   2c5a <explode_bomb>
    2537:	48 8b 44 24 08       	mov    rax,QWORD PTR [rsp+0x8]
    253c:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    2543:	00 00 
    2545:	0f 85 83 00 00 00    	jne    25ce <phase_3+0x106>
    254b:	48 83 c4 18          	add    rsp,0x18
    254f:	c3                   	ret
    2550:	8b 15 ca 4b 00 00    	mov    edx,DWORD PTR [rip+0x4bca]        # 7120 <delta.1>
    2556:	b8 5f 00 00 00       	mov    eax,0x5f
    255b:	29 d0                	sub    eax,edx
    255d:	eb c7                	jmp    2526 <phase_3+0x5e>
    255f:	8b 15 bb 4b 00 00    	mov    edx,DWORD PTR [rip+0x4bbb]        # 7120 <delta.1>
    2565:	b8 13 01 00 00       	mov    eax,0x113
    256a:	29 d0                	sub    eax,edx
    256c:	eb b8                	jmp    2526 <phase_3+0x5e>
    256e:	8b 15 ac 4b 00 00    	mov    edx,DWORD PTR [rip+0x4bac]        # 7120 <delta.1>
    2574:	b8 8d 01 00 00       	mov    eax,0x18d
    2579:	29 d0                	sub    eax,edx
    257b:	eb a9                	jmp    2526 <phase_3+0x5e>
    257d:	8b 15 9d 4b 00 00    	mov    edx,DWORD PTR [rip+0x4b9d]        # 7120 <delta.1>
    2583:	b8 e8 01 00 00       	mov    eax,0x1e8
    2588:	29 d0                	sub    eax,edx
    258a:	eb 9a                	jmp    2526 <phase_3+0x5e>
    258c:	8b 15 8e 4b 00 00    	mov    edx,DWORD PTR [rip+0x4b8e]        # 7120 <delta.1>
    2592:	b8 7c 00 00 00       	mov    eax,0x7c
    2597:	29 d0                	sub    eax,edx
    2599:	eb 8b                	jmp    2526 <phase_3+0x5e>
    259b:	8b 15 7f 4b 00 00    	mov    edx,DWORD PTR [rip+0x4b7f]        # 7120 <delta.1>
    25a1:	b8 f6 00 00 00       	mov    eax,0xf6
    25a6:	29 d0                	sub    eax,edx
    25a8:	e9 79 ff ff ff       	jmp    2526 <phase_3+0x5e>
    25ad:	8b 15 6d 4b 00 00    	mov    edx,DWORD PTR [rip+0x4b6d]        # 7120 <delta.1>
    25b3:	b8 71 03 00 00       	mov    eax,0x371
    25b8:	29 d0                	sub    eax,edx
    25ba:	e9 67 ff ff ff       	jmp    2526 <phase_3+0x5e>
    25bf:	e8 96 06 00 00       	call   2c5a <explode_bomb>
    25c4:	b8 00 00 00 00       	mov    eax,0x0
    25c9:	e9 58 ff ff ff       	jmp    2526 <phase_3+0x5e>
    25ce:	e8 cd fa ff ff       	call   20a0 <__stack_chk_fail@plt>
</details>
观察该函数逻辑，24e4-24f9的代码表明，该函数首先调用sscanf读取2个整数v1和v2（见4621处字符串%d %d），如果读取个数<=1即爆炸，并且若读取的第一个数v1>7也会爆炸；而24ff-2510的代码则是switch语句的特征，分析可知它最终跳转到[41e0+v1*4]+41e0处。<br>
故接下来去找[41e0]附近的值，代码如下：
<details>
<summary>Code</summary>

	41e0 39e3ffff 70e3ffff 7fe3ffff 8ee3ffff  9...p...........
 	41f0 9de3ffff ace3ffff bbe3ffff cde3ffff  ................
</details>
通过计算可知这些地址对应的[41e0+v1*4]+41e0值分别为2519，2550, 255f，256e，257d，258c，259b，25ad（代码中# 7120 <delta.1>也有提示），switch跳转到每一个分值后都进行相应的操作，最后跳回到2526处进行检查，简单分析可知2526-2532的检查部分c代码如下：
<details>
<summary>Code</summary>

	v3=change(v1);				//处理过后的v1结果存在v3内
	if ( v2 < 0 || v2 != v3 )
    explode_bomb();
</details>
试了下0发现v3为196符合条件，故一种答案即为0 196

##phase 4
phase4汇编代码如下：
<details>
<summary>Code</summary>

	2610:	48 83 ec 18          	sub    rsp,0x18
    2614:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    261b:	00 00 
    261d:	48 89 44 24 08       	mov    QWORD PTR [rsp+0x8],rax
    2622:	31 c0                	xor    eax,eax
    2624:	48 8d 4c 24 04       	lea    rcx,[rsp+0x4]
    2629:	48 89 e2             	mov    rdx,rsp
    262c:	48 8d 35 ee 1f 00 00 	lea    rsi,[rip+0x1fee]        # 4621 <array.0+0x421>
    2633:	e8 18 fb ff ff       	call   2150 <__isoc99_sscanf@plt>
    2638:	83 f8 02             	cmp    eax,0x2
    263b:	75 06                	jne    2643 <phase_4+0x33>
    263d:	83 3c 24 0e          	cmp    DWORD PTR [rsp],0xe
    2641:	76 05                	jbe    2648 <phase_4+0x38>
    2643:	e8 12 06 00 00       	call   2c5a <explode_bomb>
    2648:	ba 0e 00 00 00       	mov    edx,0xe
    264d:	be 00 00 00 00       	mov    esi,0x0
    2652:	8b 3c 24             	mov    edi,DWORD PTR [rsp]
    2655:	e8 79 ff ff ff       	call   25d3 <func4>
    265a:	8b 4c 24 04          	mov    ecx,DWORD PTR [rsp+0x4]
    265e:	8d 51 f6             	lea    edx,[rcx-0xa]
    2661:	89 54 24 04          	mov    DWORD PTR [rsp+0x4],edx
    2665:	83 fa 01             	cmp    edx,0x1
    2668:	75 05                	jne    266f <phase_4+0x5f>
    266a:	83 f8 01             	cmp    eax,0x1
    266d:	74 05                	je     2674 <phase_4+0x64>
    266f:	e8 e6 05 00 00       	call   2c5a <explode_bomb>
    2674:	48 8b 44 24 08       	mov    rax,QWORD PTR [rsp+0x8]
    2679:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    2680:	00 00 
    2682:	75 05                	jne    2689 <phase_4+0x79>
    2684:	48 83 c4 18          	add    rsp,0x18
    2688:	c3                   	ret
    2689:	e8 12 fa ff ff       	call   20a0 <__stack_chk_fail@plt>
</details>
设sscanf读取的两个整数分别为a和b，分析其核心部分的c代码：
<details>
<summary>Code</summary>

	if (a > 14 )
    	explode_bomb();
  	int c = func4(a, 0, 14);
  	b -= 10;
  	if ( b != 1 || c != 1 )
    	explode_bomb();
</details>
至此我们可以确定b的值为11，下面来确定a的值，首先分析fuc4函数的汇编代码：
<details>
<summary>Code</summary>
	
	### a in edi, b in esi, c in edx
	25d3:	48 83 ec 08          	sub    rsp,0x8
    25d7:	89 d0                	mov    eax,edx
    25d9:	29 f0                	sub    eax,esi
    25db:	89 c1                	mov    ecx,eax
    25dd:	c1 e9 1f             	shr    ecx,0x1f
    25e0:	01 c1                	add    ecx,eax
    25e2:	d1 f9                	sar    ecx,1
    25e4:	01 f1                	add    ecx,esi
    25e6:	39 f9                	cmp    ecx,edi
    25e8:	7f 0c                	jg     25f6 <func4+0x23>
    25ea:	b8 00 00 00 00       	mov    eax,0x0
    25ef:	7c 11                	jl     2602 <func4+0x2f>
    25f1:	48 83 c4 08          	add    rsp,0x8
    25f5:	c3                   	ret
    25f6:	8d 51 ff             	lea    edx,[rcx-0x1]
    25f9:	e8 d5 ff ff ff       	call   25d3 <func4>
    25fe:	01 c0                	add    eax,eax
    2600:	eb ef                	jmp    25f1 <func4+0x1e>
    2602:	8d 71 01             	lea    esi,[rcx+0x1]
    2605:	e8 c9 ff ff ff       	call   25d3 <func4>
    260a:	8d 44 00 01          	lea    eax,[rax+rax*1+0x1]
    260e:	eb e1                	jmp    25f1 <func4+0x1e>
</details>
分析可知该函数使用的递归操作，其c代码为：
<details>
<summary>Code</summary>

		int fuc4(int a, int b, int c)
	{
    	//int r=b + ((c-b)>>>31+(c-b))>>1;	>>>为逻辑移位
    	int r = b + (c - b) / 2;
    	if (r > a)
        	return 2 * fuc4(a, b, r - 1);
    	else if (r < a)
        	return 2 * fuc4(a, r + 1, c) + 1;
    	return 0;
	}
</details>
直接写代码爆破即可：
<details>
<summary>Code</summary>

	for (int s = 14;; s--)
    {
        if (fuc4(s, 0, 14) == 1)
        {
            printf("%d\n", s);
            break;
        }
    }
</details>
可得结果为11，故a=11，又已得到b=11，因此答案为11 11

##phase5
phase5的汇编代码如下：
<details>
<summary>Code</summary>

	268e:	53                   	push   rbx
    268f:	48 89 fb             	mov    rbx,rdi
    2692:	e8 ca 02 00 00       	call   2961 <string_length>
    2697:	83 f8 06             	cmp    eax,0x6
    269a:	75 2c                	jne    26c8 <phase_5+0x3a>
    269c:	48 89 d8             	mov    rax,rbx
    269f:	48 8d 7b 06          	lea    rdi,[rbx+0x6]
    26a3:	b9 00 00 00 00       	mov    ecx,0x0
    26a8:	48 8d 35 51 1b 00 00 	lea    rsi,[rip+0x1b51]        # 4200 <array.0>
    26af:	0f b6 10             	movzx  edx,BYTE PTR [rax]
    26b2:	83 e2 0f             	and    edx,0xf
    26b5:	03 0c 96             	add    ecx,DWORD PTR [rsi+rdx*4]
    26b8:	48 83 c0 01          	add    rax,0x1
    26bc:	48 39 f8             	cmp    rax,rdi
    26bf:	75 ee                	jne    26af <phase_5+0x21>
    26c1:	83 f9 3b             	cmp    ecx,0x3b
    26c4:	75 09                	jne    26cf <phase_5+0x41>
    26c6:	5b                   	pop    rbx
    26c7:	c3                   	ret
    26c8:	e8 8d 05 00 00       	call   2c5a <explode_bomb>
    26cd:	eb cd                	jmp    269c <phase_5+0xe>
    26cf:	e8 86 05 00 00       	call   2c5a <explode_bomb>
    26d4:	eb f0                	jmp    26c6 <phase_5+0x38>
</details>
还是先逆向出c代码：
<details>
<summary>Code</summary>

	unsigned char* phase_5(unsigned char* input)
	{
  		unsigned char* a = input; 
  		unsigned char* b = input +6; 
  		int c = 0; 

  		if ( string_length(input) != 6 )
    		explode_bomb();
  		do
		{
			int temp= (*a) & 0xf;
			a++;
    		c += array[temp];
  		while ( a != b );
  		if ( c != 59 )
    		explode_bomb();
  		return a;
	}
</details>
因此我们需要输入一个长度为6的字符串，其每个字符&0xf后的下标在array内的对应值之和为59即可满足条件，接下来找到array的位置4200：
<details>
<summary>Code</summary>

	4200 02000000 0a000000 06000000 01000000  ................
 	4210 0c000000 10000000 09000000 03000000  ................
 	4220 04000000 07000000 0e000000 05000000  ................
 	4230 0b000000 08000000 0f000000 0d000000  ................
</details>
整理得array[]={2, 10, 6, 1, 12, 16, 9, 3, 4, 7, 14, 5, 11, 8, 15, 13}<br>
因此下标为9,11,12,13,14,15满足要求，给出求解代码：
<details>
<summary>Code</summary>
	
	int ss[] = { 9,11,12,13,14,15 };
	for (int a = 0; a <= 5; a++)
    {
        for (int s = 32; s <= 127; s++)
        {
            if ((s & 0xf) == ss[a])
            {
                putchar(s);
                break;
            }
        }
    }
</details>
解得答案为)+,-./

##phase6
phase6汇编代码如下：（好长，，，，，，）
<details>
<summary>Code</summary>

	26d6:	41 57                	push   r15
    26d8:	41 56                	push   r14
    26da:	41 55                	push   r13
    26dc:	41 54                	push   r12
    26de:	55                   	push   rbp
    26df:	53                   	push   rbx
    26e0:	48 83 ec 78          	sub    rsp,0x78
    26e4:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    26eb:	00 00 
    26ed:	48 89 44 24 68       	mov    QWORD PTR [rsp+0x68],rax
    26f2:	31 c0                	xor    eax,eax
    26f4:	4c 8d 74 24 10       	lea    r14,[rsp+0x10]
    26f9:	4c 89 74 24 08       	mov    QWORD PTR [rsp+0x8],r14
    26fe:	4c 89 f6             	mov    rsi,r14
    2701:	e8 14 06 00 00       	call   2d1a <read_six_numbers>
    2706:	4d 89 f4             	mov    r12,r14
    2709:	41 bf 01 00 00 00    	mov    r15d,0x1
    270f:	4d 89 f5             	mov    r13,r14
    2712:	e9 c6 00 00 00       	jmp    27dd <phase_6+0x107>
    2717:	e8 3e 05 00 00       	call   2c5a <explode_bomb>
    271c:	e9 ce 00 00 00       	jmp    27ef <phase_6+0x119>
    2721:	48 83 c3 01          	add    rbx,0x1
    2725:	83 fb 05             	cmp    ebx,0x5
    2728:	0f 8f a7 00 00 00    	jg     27d5 <phase_6+0xff>
    272e:	41 8b 44 9d 00       	mov    eax,DWORD PTR [r13+rbx*4+0x0]
    2733:	39 45 00             	cmp    DWORD PTR [rbp+0x0],eax
    2736:	75 e9                	jne    2721 <phase_6+0x4b>
    2738:	e8 1d 05 00 00       	call   2c5a <explode_bomb>
    273d:	eb e2                	jmp    2721 <phase_6+0x4b>
    273f:	48 8b 54 24 08       	mov    rdx,QWORD PTR [rsp+0x8]
    2744:	48 83 c2 18          	add    rdx,0x18
    2748:	b9 07 00 00 00       	mov    ecx,0x7
    274d:	89 c8                	mov    eax,ecx
    274f:	41 2b 04 24          	sub    eax,DWORD PTR [r12]
    2753:	41 89 04 24          	mov    DWORD PTR [r12],eax
    2757:	49 83 c4 04          	add    r12,0x4
    275b:	4c 39 e2             	cmp    rdx,r12
    275e:	75 ed                	jne    274d <phase_6+0x77>
    2760:	be 00 00 00 00       	mov    esi,0x0
    2765:	8b 4c b4 10          	mov    ecx,DWORD PTR [rsp+rsi*4+0x10]
    2769:	b8 01 00 00 00       	mov    eax,0x1
    276e:	48 8d 15 eb eb 00 00 	lea    rdx,[rip+0xebeb]        # 11360 <node1>
    2775:	83 f9 01             	cmp    ecx,0x1
    2778:	7e 0b                	jle    2785 <phase_6+0xaf>
    277a:	48 8b 52 08          	mov    rdx,QWORD PTR [rdx+0x8]
    277e:	83 c0 01             	add    eax,0x1
    2781:	39 c8                	cmp    eax,ecx
    2783:	75 f5                	jne    277a <phase_6+0xa4>
    2785:	48 89 54 f4 30       	mov    QWORD PTR [rsp+rsi*8+0x30],rdx
    278a:	48 83 c6 01          	add    rsi,0x1
    278e:	48 83 fe 06          	cmp    rsi,0x6
    2792:	75 d1                	jne    2765 <phase_6+0x8f>
    2794:	48 8b 5c 24 30       	mov    rbx,QWORD PTR [rsp+0x30]
    2799:	48 8b 44 24 38       	mov    rax,QWORD PTR [rsp+0x38]
    279e:	48 89 43 08          	mov    QWORD PTR [rbx+0x8],rax
    27a2:	48 8b 54 24 40       	mov    rdx,QWORD PTR [rsp+0x40]
    27a7:	48 89 50 08          	mov    QWORD PTR [rax+0x8],rdx
    27ab:	48 8b 44 24 48       	mov    rax,QWORD PTR [rsp+0x48]
    27b0:	48 89 42 08          	mov    QWORD PTR [rdx+0x8],rax
    27b4:	48 8b 54 24 50       	mov    rdx,QWORD PTR [rsp+0x50]
    27b9:	48 89 50 08          	mov    QWORD PTR [rax+0x8],rdx
    27bd:	48 8b 44 24 58       	mov    rax,QWORD PTR [rsp+0x58]
    27c2:	48 89 42 08          	mov    QWORD PTR [rdx+0x8],rax
    27c6:	48 c7 40 08 00 00 00 	mov    QWORD PTR [rax+0x8],0x0
    27cd:	00 
    27ce:	bd 05 00 00 00       	mov    ebp,0x5
    27d3:	eb 35                	jmp    280a <phase_6+0x134>
    27d5:	49 83 c7 01          	add    r15,0x1
    27d9:	49 83 c6 04          	add    r14,0x4
    27dd:	4c 89 f5             	mov    rbp,r14
    27e0:	41 8b 06             	mov    eax,DWORD PTR [r14]
    27e3:	83 e8 01             	sub    eax,0x1
    27e6:	83 f8 05             	cmp    eax,0x5
    27e9:	0f 87 28 ff ff ff    	ja     2717 <phase_6+0x41>
    27ef:	41 83 ff 05          	cmp    r15d,0x5
    27f3:	0f 8f 46 ff ff ff    	jg     273f <phase_6+0x69>
    27f9:	4c 89 fb             	mov    rbx,r15
    27fc:	e9 2d ff ff ff       	jmp    272e <phase_6+0x58>
    2801:	48 8b 5b 08          	mov    rbx,QWORD PTR [rbx+0x8]
    2805:	83 ed 01             	sub    ebp,0x1
    2808:	74 11                	je     281b <phase_6+0x145>
    280a:	48 8b 43 08          	mov    rax,QWORD PTR [rbx+0x8]
    280e:	8b 00                	mov    eax,DWORD PTR [rax]
    2810:	39 03                	cmp    DWORD PTR [rbx],eax
    2812:	7d ed                	jge    2801 <phase_6+0x12b>
    2814:	e8 41 04 00 00       	call   2c5a <explode_bomb>
    2819:	eb e6                	jmp    2801 <phase_6+0x12b>
    281b:	48 8b 44 24 68       	mov    rax,QWORD PTR [rsp+0x68]
    2820:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    2827:	00 00 
    2829:	75 0f                	jne    283a <phase_6+0x164>
    282b:	48 83 c4 78          	add    rsp,0x78
    282f:	5b                   	pop    rbx
    2830:	5d                   	pop    rbp
    2831:	41 5c                	pop    r12
    2833:	41 5d                	pop    r13
    2835:	41 5e                	pop    r14
    2837:	41 5f                	pop    r15
    2839:	c3                   	ret
    283a:	e8 61 f8 ff ff       	call   20a0 <__stack_chk_fail@plt>
</details>
由2701处指令可以推测出需要输入6个数字（记为v1-v6），接着跳入27dd处，27e0处下断点可知[r14]为输入的第一个数字，即r14是输入的6个数字的数组首地址，此时如果eax>5则跳入2717处的爆炸指令；联系2709和加下来的27ef，可以推测r15d是一个循环用来计数的参数（记为i），接下来如果r15d>5则跳至273f（后来发现就是break），<=5则将r15d（r15与r15d在int内等值，即可认为r15是r15d）移入rbx跳入272e，结合270f可知r13=r14=&array[0]，故2733处指令即为比较两者大小，不相等则转到2721继续循环。<br>
综上可知，第一模块代码是有两层循环，外层要求输入的6个数每个数都不大于6，内层要求这些数两两不相等，即要求输入1-6的一组排列数。<br>
由上述分析可得，所有循环检查结束后程序进入273f执行第二模块，由于代码太过麻烦不利于分析，打算先动调找找思路，观察可发现273f-27d3之间有一些小循环，但不会有跳出这块区域的执行，因此可以判断最后会从27d3跳入280a执行第三模块。
第三模块主要代码如下：
<details>
<summary>Code</summary>

	2801:	48 8b 5b 08          	mov    rbx,QWORD PTR [rbx+0x8]
    2805:	83 ed 01             	sub    ebp,0x1
    2808:	74 11                	je     281b <phase_6+0x145>
    280a:	48 8b 43 08          	mov    rax,QWORD PTR [rbx+0x8]
    280e:	8b 00                	mov    eax,DWORD PTR [rax]
    2810:	39 03                	cmp    DWORD PTR [rbx],eax
    2812:	7d ed                	jge    2801 <phase_6+0x12b>
</details>
模块入口点是280a，判断结束后却跳入2801，说明这是个do-while循环，尝试逆向出c代码：
<details>
<summary>Code</summary>
	
	long long* a; //a=rbx
	int r=5;
	do
  	{
		int b= *(int*) a;
		int c= *(int*)(*(a + 8));
    	if ( b < c )
      		explode_bomb();
    	a += 8;
    	r--;
  	}
  	while ( r );
</details>
即每次取出rbx对应地址的int值和rbx+8地址的值作为地址所指向区域的int值进行比较，且需满足前者>后者，结合276e处的node1不难判断出是链表结构，打印出相应区域：
<details>
<summary>Code</summary>

	11360 c9010000 01000000 70130100 00000000  ........p.......
 	11370 61010000 02000000 80130100 00000000  a...............
 	11380 a8020000 03000000 90130100 00000000  ................
 	11390 72000000 04000000 a0130100 00000000  r...............
 	113a0 7c000000 05000000 a08a0000 00000000  |...............
</details>
可以看出node1-node5节点地址连续，node6偏离较远需要单独查看，最后得到6个节点的int值分别是0x1c9,0x161,0x2a8,0x72,0x7c,0x2ca，所以若要满足大于关系，我们必须将各个节点按从大到小串联起来，而原始列表的连接顺序是1 2 3 4 5 6，并不具备这个特点，说明我们输入的1-6的排列数对链表串联顺序做了改变。<br>
由于不想看第二模块那堆复杂操作，我先输入需要的值排列顺序，即6 3 1 2 5 4，结果动调发现第二模块对输入值进行了处理，使其真正连接顺序变成了1 4 6 5 2 3，观察可发现这个排列的每个数即是7-原输入排列的每个数，故尝试输入1 4 6 5 2 3，即为正确答案。<br>
之后发现是第二模块这段代码改变了输入的排列：
<details>
<summary>Code</summary>

    273f:	48 8b 54 24 08       	mov    rdx,QWORD PTR [rsp+0x8]
    2744:	48 83 c2 18          	add    rdx,0x18
    2748:	b9 07 00 00 00       	mov    ecx,0x7
    274d:	89 c8                	mov    eax,ecx
    274f:	41 2b 04 24          	sub    eax,DWORD PTR [r12]
    2753:	41 89 04 24          	mov    DWORD PTR [r12],eax
    2757:	49 83 c4 04          	add    r12,0x4
    275b:	4c 39 e2             	cmp    rdx,r12
    275e:	75 ed                	jne    274d <phase_6+0x77>
</details>

##secret_phase
由main函数代码可知，程序没有在main函数内主动调用该函数，因此需要在其他函数里面找入口点，观察到phase_defused函数代码内2f59处有入口点：
<details>
<summary>Code</summary>

	2e95:	48 81 ec 98 00 00 00 	sub    rsp,0x98
    2e9c:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    2ea3:	00 00 
    2ea5:	48 89 84 24 88 00 00 	mov    QWORD PTR [rsp+0x88],rax
    2eac:	00 
    2ead:	31 c0                	xor    eax,eax
    2eaf:	bf 01 00 00 00       	mov    edi,0x1
    2eb4:	e8 d4 fc ff ff       	call   2b8d <send_msg>
    2eb9:	83 3d b8 e9 00 00 06 	cmp    DWORD PTR [rip+0xe9b8],0x6        # 11878 <num_input_strings>
    2ec0:	74 1f                	je     2ee1 <phase_defused+0x4c>
    2ec2:	48 8b 84 24 88 00 00 	mov    rax,QWORD PTR [rsp+0x88]
    2ec9:	00 
    2eca:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    2ed1:	00 00 
    2ed3:	0f 85 87 00 00 00    	jne    2f60 <phase_defused+0xcb>
    2ed9:	48 81 c4 98 00 00 00 	add    rsp,0x98
    2ee0:	c3                   	ret
    2ee1:	48 8d 4c 24 0c       	lea    rcx,[rsp+0xc]
    2ee6:	48 8d 54 24 08       	lea    rdx,[rsp+0x8]
    2eeb:	4c 8d 44 24 10       	lea    r8,[rsp+0x10]
    2ef0:	48 8d 35 74 17 00 00 	lea    rsi,[rip+0x1774]        # 466b <array.0+0x46b>
    2ef7:	48 8d 3d ea ea 00 00 	lea    rdi,[rip+0xeaea]        # 119e8 <input_strings+0x168>
    2efe:	b8 00 00 00 00       	mov    eax,0x0
    2f03:	e8 48 f2 ff ff       	call   2150 <__isoc99_sscanf@plt>
    2f08:	83 f8 03             	cmp    eax,0x3
    2f0b:	74 1a                	je     2f27 <phase_defused+0x92>
    2f0d:	48 8d 3d a4 14 00 00 	lea    rdi,[rip+0x14a4]        # 43b8 <array.0+0x1b8>
    2f14:	e8 57 f1 ff ff       	call   2070 <puts@plt>
    2f19:	48 8d 3d c8 14 00 00 	lea    rdi,[rip+0x14c8]        # 43e8 <array.0+0x1e8>
    2f20:	e8 4b f1 ff ff       	call   2070 <puts@plt>
    2f25:	eb 9b                	jmp    2ec2 <phase_defused+0x2d>
    2f27:	48 8d 7c 24 10       	lea    rdi,[rsp+0x10]
    2f2c:	48 8d 35 41 17 00 00 	lea    rsi,[rip+0x1741]        # 4674 <array.0+0x474>
    2f33:	e8 46 fa ff ff       	call   297e <strings_not_equal>
    2f38:	85 c0                	test   eax,eax
    2f3a:	75 d1                	jne    2f0d <phase_defused+0x78>
    2f3c:	48 8d 3d 15 14 00 00 	lea    rdi,[rip+0x1415]        # 4358 <array.0+0x158>
    2f43:	e8 28 f1 ff ff       	call   2070 <puts@plt>
    2f48:	48 8d 3d 31 14 00 00 	lea    rdi,[rip+0x1431]        # 4380 <array.0+0x180>
    2f4f:	e8 1c f1 ff ff       	call   2070 <puts@plt>
    2f54:	b8 00 00 00 00       	mov    eax,0x0
    2f59:	e8 45 f9 ff ff       	call   28a3 <secret_phase>
    2f5e:	eb ad                	jmp    2f0d <phase_defused+0x78>
    2f60:	e8 3b f1 ff ff       	call   20a0 <__stack_chk_fail@plt>
</details>
检查得466b处的字符串为%d %d %s，即sscanf将从11198处读取两个整数和一个字符串，只要这个字符串与4674处的字符串Testify相等即可进入秘密函数。<br>
在这里断点查看11198处地址，发现了两个整数11 11，且前面还有phase1的字符串，说明我们在phase4的输入被存储到了这里，因此我们需要在phase4输入11 11 Testify就能成功进入秘密函数。

secret_phase汇编代码如下：
<details>
<summary>Code</summary>
	
	28a3:	48 83 ec 18          	sub    rsp,0x18
    28a7:	c7 44 24 0c 1d 00 00 	mov    DWORD PTR [rsp+0xc],0x1d
    28ae:	00 
    28af:	e8 a7 04 00 00       	call   2d5b <read_line>
    28b4:	48 89 c6             	mov    rsi,rax
    28b7:	48 8d 3d 22 62 00 00 	lea    rdi,[rip+0x6222]        # 8ae0 <t0>
    28be:	e8 7c ff ff ff       	call   283f <fun7>
    28c3:	8b 54 24 0c          	mov    edx,DWORD PTR [rsp+0xc]
    28c7:	39 c2                	cmp    edx,eax
    28c9:	75 16                	jne    28e1 <secret_phase+0x3e>
    28cb:	48 8d 3d ce 18 00 00 	lea    rdi,[rip+0x18ce]        # 41a0 <_IO_stdin_used+0x1a0>
    28d2:	e8 99 f7 ff ff       	call   2070 <puts@plt>
    28d7:	e8 b9 05 00 00       	call   2e95 <phase_defused>
    28dc:	48 83 c4 18          	add    rsp,0x18
    28e0:	c3                   	ret
    28e1:	e8 74 03 00 00       	call   2c5a <explode_bomb>
    28e6:	eb e3                	jmp    28cb <secret_phase+0x28>
</details>
逆向可得其核心c代码为：
<details>
<summary>Code</summary>
	
	char* a = read_line();
  	if ( fun7(&t0, a) != 29 )
    	explode_bomb();
</details>

接着看一下fuc7函数汇编代码：
<details>
<summary>Code</summary>

	283f:	55                   	push   rbp
    2840:	53                   	push   rbx
    2841:	48 83 ec 08          	sub    rsp,0x8
    2845:	48 89 fb             	mov    rbx,rdi
    2848:	48 89 f5             	mov    rbp,rsi
    284b:	48 85 ff             	test   rdi,rdi
    284e:	74 2b                	je     287b <fun7+0x3c>
    2850:	0f b6 55 00          	movzx  edx,BYTE PTR [rbp+0x0]
    2854:	84 d2                	test   dl,dl
    2856:	74 2a                	je     2882 <fun7+0x43>
    2858:	80 fa 61             	cmp    dl,0x61
    285b:	74 29                	je     2886 <fun7+0x47>
    285d:	0f be d2             	movsx  edx,dl
    2860:	83 ea 61             	sub    edx,0x61
    2863:	b8 01 00 00 00       	mov    eax,0x1
    2868:	39 d0                	cmp    eax,edx
    286a:	74 1f                	je     288b <fun7+0x4c>
    286c:	83 c0 01             	add    eax,0x1
    286f:	83 f8 1a             	cmp    eax,0x1a
    2872:	75 f4                	jne    2868 <fun7+0x29>
    2874:	e8 e1 03 00 00       	call   2c5a <explode_bomb>
    2879:	eb 21                	jmp    289c <fun7+0x5d>
    287b:	e8 da 03 00 00       	call   2c5a <explode_bomb>
    2880:	eb ce                	jmp    2850 <fun7+0x11>
    2882:	8b 03                	mov    eax,DWORD PTR [rbx]
    2884:	eb 16                	jmp    289c <fun7+0x5d>
    2886:	ba 00 00 00 00       	mov    edx,0x0
    288b:	48 8d 75 01          	lea    rsi,[rbp+0x1]
    288f:	48 63 d2             	movsxd rdx,edx
    2892:	48 8b 7c d3 08       	mov    rdi,QWORD PTR [rbx+rdx*8+0x8]
    2897:	e8 a3 ff ff ff       	call   283f <fun7>
    289c:	48 83 c4 08          	add    rsp,0x8
    28a0:	5b                   	pop    rbx
    28a1:	5d                   	pop    rbp
    28a2:	c3                   	ret
</details>
注意到rdi为第1参数，rsi为第2参数，逆向出其c代码：
<details>
<summary>Code</summary>

	int fun7(long long* a, char* b)
	{
  		int d;
  		if ( !a )
    		explode_bomb();
  		char c = *b;
  		if ( !c )
    		return *(int *)a;
  		if ( c == 'a' )
    		d = 0;
  		else
  		{
    		d = c - 'a';
    		int k = 1;
    		while ( k != d )
    		{
      			if ( ++k == 26 )
        			explode_bomb();
    		}
  		}
  		return fun7(a + d + 1, b + 1);
	}
</details>
这题的递归调用与第6题类似，不过所输入的必须是字母a-z，且需要自己计算偏移量来达到“传送门”的效果，在secret_phase中题目明确了我们需要返回29，也就是说在fuc7中当字符串读到末尾时，我们的a指针指向的int值为29才能满足要求。<br>
搜索data段可得到两处值为29的位置，分别是：
<details>
<summary>Code</summary>

	07a00 1d000000 b3000000 00000000 00000000  ................
	101e0 00000000 1d000000 00000000 00000000  ................
</details>
由return fun7(a + d + 1, b + 1)可得，a指针每次移动(d+1)个long long长度，即移动(2d+2)个int长度，而这个值附近的传送门只有101e0，故不可能从101e0移动偶数个int到101e4，所求地址即为07a00。<br>
于是开始慢慢从07a00往前推，注意每次往前移动的int范围只能为2-54之间的偶数值，可得地址为：<br>
07a00<-0c0a8<---0c040<-0bff8<---0bf60<-0bec8<---0be80<-0be40<---0bda0<-0bcc8<---0bcc0<-0bc48<--0bbe0<-0bb38<---0bb00<-0b408<---0b400<-0b3b0<---0b320<-08b60<---08ae0<br>
其中<---表示偏移量传递，<-表示地址传递，计算所有<---的偏移量并拼接起来即为答案,例如：
(08b60-08ae0)/8-1+'a'='p' &nbsp; (0b3b0-0b320)/8-1+'a'='r'<br>
最后可得答案为pragmatism

##答案展示界面
	# ./bomb          
	Welcome to my fiendish little bomb. You have 6 phases with
	which to blow yourself up. Have a nice day!
	The trembling fear is more than I can take when I'm up against the ECHO in the mirror.
	Phase 1 defused. (Interesting key, huh?)
	15 105 735 5145 36015 252105
	That's number 2.
	0 196
	Halfway there!
	11 11 Testify
	So you got that one.  Try this one.
	)+,-./
	Good work!  On to the next...
	1 4 6 5 2 3
	Curses, you've found the secret phase!
	But finding it and solving it are quite different...
	pragmatism
	Wow! You've defused the secret stage!
	Congratulations! You've defused the bomb!
	Your instructor has been notified and will verify your solution.
