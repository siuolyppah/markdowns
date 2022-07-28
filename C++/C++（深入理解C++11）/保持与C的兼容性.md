# `#pragma`预处理命令与`_Pragma`操作符

- `pragma`预处理名， 用于向编译器传达标准以外的信息。

- `_Pragma`操作符，支持宏的展开。

  ```C++
  #define PRAGMA(x) _Pragma(#x)
  #define CONCAT(x) PRAGMA(concat #x)
  
  CONCAT("123") // 等同于： _Pragma(concat 123)
  ```

  

# extern "C"

extern "C"可以抑制C++对函数名、变量名等符号（symbol）进行名称重整（name mangling)，因此编译出的C目标文件和C++目标文件中的变量、函数名称等符号都是相同的（否则不相同），链接器可以可靠地对两种类型的目标文件进行链接。

> 用于C与C++代码，共用头文件。



```C++
#ifdef __cplusplus
extern "C"{
#endif
// 一些代码
#ifdef __cplusplus
}
#endif
```



- `extern` 是 `C/C++` 语言中表明函数和全局变量作用范围（可见性）的关键字，该关键字告诉编译器，其声明的函数和变量可以在本模块或其它模块中使用。
- 被`extern "C"` 修饰的变量和函数是按照 C 语言方式编译和连接的。

