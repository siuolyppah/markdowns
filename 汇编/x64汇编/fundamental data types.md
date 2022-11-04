![image-20221104104523019](fundamental%20data%20types.assets/image-20221104104523019.png)



# Fundamental Data types

![image-20221104104541350](fundamental%20data%20types.assets/image-20221104104541350.png)



# Integers Signed vs Unsigned

> ==In asmemblly, we don't have type safety==.
>
> Signed and Unsigned, it's up to the programmer.



![image-20221104104928259](fundamental%20data%20types.assets/image-20221104104928259.png)



# Integer Data Type Ranges 

![image-20221104105123432](fundamental%20data%20types.assets/image-20221104105123432.png)



# Floating Point Data Type Ranges

![image-20221104105346019](fundamental%20data%20types.assets/image-20221104105346019.png)

>real4: 对应float
>
>real8: 对应double





# SIMD Data Types

![image-20221104105718029](fundamental%20data%20types.assets/image-20221104105718029.png)



# Summary

![image-20221104110010558](fundamental%20data%20types.assets/image-20221104110010558-1667530810784-1.png)





```asm
.data
myByte db 78

.code
foo proc
	mov al, byte ptr[myByte]

	mov rax,-1
	mov al, myByte	; 与上面形式等价

	ret
foo endp
end
```

