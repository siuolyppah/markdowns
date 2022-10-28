- 关于NULL的定义：

  ```cpp
  #ifndef NULL
      #ifdef __cplusplus
          #define NULL 0
      #else
          #define NULL ((void *)0)
      #endif
  #endif
  
  ```

- C++中，==不允许`void *`隐式转换为其他类型==。

- ==允许`nullptr`隐式转换为任意指针类型==。