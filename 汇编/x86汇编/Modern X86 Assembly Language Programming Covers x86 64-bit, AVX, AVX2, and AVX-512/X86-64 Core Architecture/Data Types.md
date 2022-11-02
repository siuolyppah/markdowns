# Fundamental Data Types

A fundamental data type is an elementary unit of data that is manipulated by the processor during program execution.



|    Data Type    | Size(Bits) |                Typical Use                |
| :-------------: | :--------: | :---------------------------------------: |
|      Byte       |     8      |        Characters, small integers         |
|      Word       |     16     |            Character, integers            |
|   Doubleword    |     32     | Integers, single-precision floating-point |
|    Quadword     |     64     | Integers, double-precision floating-point |
| Double Quadword |    128     |  Packed integers, packed floating-point   |

> little endian memory byte ordering
>
> Fundamental data types larger than a single byte are stored in consecutive memory locations starting with the least-significant byte at the lowest memory address.

![image-20221101193449966](Data%20Types.assets/image-20221101193449966.png)



# Numerical Data Types

- A numerical data type is an elementary scalar value such as an integer or floating-point number.

- All numerical data types recognized by the CPU are represented using one of the fundamental data types discussed in the previous section.

- x86 numerical data types along with corresponding C/C++ types:

  | Type              | Size(Bits) | C/C++ Type                  | \<cstdint>     |
  | :---------------- | ---------- | --------------------------- | -------------- |
  | Signed integers   | 8          | char                        | int8_t         |
  |                   | 16         | short                       | int16_t        |
  |                   | 32         | int, long                   | int32_t        |
  |                   | 64         | long long                   | int64_t        |
  | Unsigned integers | 8          | unsigned char               | uint8_t        |
  |                   | 16         | unsigned short              | uint16_t       |
  |                   | 32         | unsigned int, unsigned long | uint32_t       |
  |                   | 64         | unsigned long long          | uint64_t       |
  | Floating-point    | 32         | float                       | Not applicable |
  |                   | 64         | double                      | Not applicable |



# SIMD Data Types

>[SIMD简介 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/55327037)

![img](Data%20Types.assets/v2-0c56ad7326f03b91fbf48bd0ce5f03ef_720w.jpeg)





