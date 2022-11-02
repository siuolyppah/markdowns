From ==the perspective of an executing program==, the internal architecture of an x86-64 processor can be ==logically partitioned== into several distinct units.

- <u>general-purpose registers,</u> 
- <u>status and control flags (RFLAGS register),</u> 
- <u>instruction pointer (RIP register),</u>
- XMM registers,
- floating-point control
- status (MXCSR).

![image-20221101201024241](Internal%20Architecture.assets/image-20221101201024241.png)



# General-Purpose Registers

The x86-64 execution unit contains 16 <u>64-bit</u> general-purpose registers, which are ==used to perform arithmetic, logical, compare, data transfer, and address calculation operations==.

They also ==can be used as temporary storage locations== for <u>constant values</u>, <u>intermediate results</u>, and <u>pointers</u> to data values stored in memory.

![image-20221101201352033](Internal%20Architecture.assets/image-20221101201352033.png)

>Not shown in Figure are the legacy byte registers AH, BH, CH, and DH. These
>registers are aliased to the high-order bytes of registers AX, BX, CX, and DX, respectively. 





# Register

The RFLAGS register contains a series of status bits (or flags) that the processor uses ==to signify the results== of an arithmetic, logical, or compare operation. 

It also contains a number of control bits that are primarily used by operating systems. Table 1-4 shows the organization of the bits in the RFLAGS register.



![image-20221101202544276](Internal%20Architecture.assets/image-20221101202544276.png)

![image-20221101202554330](Internal%20Architecture.assets/image-20221101202554330.png)

For application programs, the most important bits in the RFLAGS register are the following status flags:

- carry flag (CF), 

  The carry flag is set by the processor to signify an overflow condition when performing unsigned integer arithmetic.

- overflow flag (OF), 

  The overflow flag signals that the result of a signed integer operation is too small or too large.

- parity flag (PF) , 

- sign flag (SF) , 

- zero flag (ZF).

>- [FLAGS register - Wikipedia](https://en.wikipedia.org/wiki/FLAGS_register)
>
>- [FLAG（状态标志寄存器）_百度百科 (baidu.com)](https://baike.baidu.com/item/FLAG/6050220)



# Instruction Pointer指令指针

- The instruction pointer register (==RIP==) ==contains <u>the logical address</u> of the next instruction to be executed.==:star:
- The value in register RIP updates automatically during execution of each instruction.  It is also implicitly altered during execution of control-transfer instructions.





# Instruction Operands

- Nearly all instructions require one or more source operands along with a single destination operand. 

- Most instructions also require the programmer to explicitly specify the source and destination operands.

- There are ==three basic types of operands:==:star: immediate, register, and memory.

  - An immediate operand is a constant value that is encoded as part of the instruction.

    Only source operands can specify an immediate value.

  - Register operands are contained in a general purpose or SIMD register.

  - A memory operand specifies a location in memory, which can contain any of the data types described earlier in this chapter.

![image-20221101223243944](Internal%20Architecture.assets/image-20221101223243944.png)

>The multiplicative product’s high order and low-order quadwords are stored in RDX and RAX, respectively.



# Memory Addressing

>[《汇编快速入门》11 七种寻址方式_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1E54y1B739/?spm_id_from=333.337.search-card.all.click&vd_source=be746efb77e979ca275e4f65f2d8cda3)

An x86-64 instruction requires up to four separate components in order to specify the location of an operand in memory. The four components include a constant displacement value, a base register, an index register, and a scale factor.

```
EffectiveAddress = BaseReg + IndexReg * ScaleFactor + Disp
```



The base register (BaseReg) can be any general-purpose register. The index register (IndexReg) can be any general-purpose register except RSP. Valid scale factors (ScaleFactor) include 2, 4, and 8. Finally, the displacement (Disp) is a constant 8-bit, 16-bit, or 32-bit signed offset that's encoded within the instruction.

>- a default value of zero is used for the displacement if an explicit value is not specified.
>- The final size of an effective address calculation is always 64 bits.

![image-20221101224052610](Internal%20Architecture.assets/image-20221101224052610.png)

:star:The memory addressing forms shown in Table 1-6 are used to directly reference program variables and data structures. For example, the simple displacement form is often used to access a simple global or static variable. The base register form is analogous to a C/C++ pointer and is used to indirectly reference a single value. Individual fields within a data structure can be retrieved using a base register and a displacement. The index register forms are useful for accessing individual elements within an array. Scale factors can reduce the amount code needed to access the elements of an array that contains integer or floating-point values. Elements in more elaborate data structures can be referenced by using a base register together with an index register, scale factor, and displacement.



![image-20221101225409818](Internal%20Architecture.assets/image-20221101225409818.png)

![image-20221101225613181](Internal%20Architecture.assets/image-20221101225613181.png)

![image-20221101225709735](Internal%20Architecture.assets/image-20221101225709735.png)



![image-20221101225902068](Internal%20Architecture.assets/image-20221101225902068.png)

![image-20221101230150232](Internal%20Architecture.assets/image-20221101230150232.png)

![image-20221101230242874](Internal%20Architecture.assets/image-20221101230242874.png)



![image-20221101230301035](Internal%20Architecture.assets/image-20221101230301035.png)

![image-20221101230446587](Internal%20Architecture.assets/image-20221101230446587.png)

![image-20221101230509999](Internal%20Architecture.assets/image-20221101230509999.png)



![image-20221101230553942](Internal%20Architecture.assets/image-20221101230553942.png)

![image-20221101230632754](Internal%20Architecture.assets/image-20221101230632754.png)



![image-20221101230737618](Internal%20Architecture.assets/image-20221101230737618.png)

![image-20221101230831055](Internal%20Architecture.assets/image-20221101230831055.png)



![image-20221101230837939](Internal%20Architecture.assets/image-20221101230837939.png)

![image-20221101230930411](Internal%20Architecture.assets/image-20221101230930411.png)



