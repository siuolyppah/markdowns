# MOV

![image-20221104113234476](MOV%E5%92%8CLEA%E6%8C%87%E4%BB%A4.assets/image-20221104113234476.png)



- 对src和dest的要求：
  - 它们的==size相同==；
  - 它们中==至少有一个是寄存器==。





# LEA

> [X86 Assemble指令--LEA - 简书 (jianshu.com)](https://www.jianshu.com/p/01e8d5ef369f)

![image-20221104113949817](MOV%E5%92%8CLEA%E6%8C%87%E4%BB%A4.assets/image-20221104113949817.png)

```asm
.data
myByte db 56

.code
foo proc
	lea rax, myByte
	mov byte ptr [rax], 7

	ret
foo endp
end
```

- ==在64位模式下，为平坦模型，所有指针都是64位的==。





```asm
.data
myByte db 56

.code
foo proc
	mov al, myByte	; 虽然myByte本质上地址，
					; 但mov指令会将他指向的数据存入寄存器

	mov rax, -1
	lea al, myByte	; 将myByte表示的地址，存入寄存器

	ret
foo endp
end
```

- 汇编的变量名，本质上的地址。
- 

