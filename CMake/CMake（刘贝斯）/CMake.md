# CMake是什么

- CMake是：**高级编译配置工具**。
- 所有操作都是通过编译CMakeLists.txt来完成的



# CMake安装

- Linux：自带
- Windows：http://www.cmake.org/HTML/Download.html



# HelloWorld Demo

1. 编写一个HelloWorld

   ```cpp
   // main.cpp
   #include <iostream>
   int main(){
       std::cout << "hello word" << std::endl;
   }
   ```

2. 写CMakeList.txt

   ```cmake
   #CMakeLists.txt
   PROJECT (HELLO)
   SET(SRC_LIST main.cpp)
   MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
   MESSAGE(STATUS "This is SOURCE dir "${HELLO_SOURCE_DIR})
   ADD_EXECUTABLE(hello ${SRC_LIST})
   ```

3. 使用`cmake`命令，生成makefile文件

   ```sh
   cmake .
   ```

   > 使用参数的例子：`-S . -B build -G "Unix Makefiles"`

4. 构建：

   ```sh
   cmake --build .
   ```

   

# CMakeLists语法

## PROJECT关键字

可以用来指定工程的名字和支持的语言，默认支持所有语言

- PROJECT (HELLO)： 指定了工程的名字，并且支持所有语言—建议

- PROJECT (HELLO CXX)：指定了工程的名字，并且支持语言是C++


- PROJECT (HELLO C CXX)：指定了工程的名字，并且支持语言是C和C++




该指定，隐式定义了两个CMAKE的变量：

- `<projectname>_BINARY_DIR`，本例中是HELLO_BINARY_DIR，指向当前的工作目录

- `<projectname>_SOURCE_DIR`，本例中是 HELLO_SOURCE_DIR，指向当前的工作目录

  >问题：如果改了工程名，这两个变量名也会改变
  >
  >解决：使用CMake预定义的两个变量：`PROJECT_BINARY_DIR`和`PROJECT_SOURCE_DIR`，这两个变量和HELLO_BINARY_DIR，HELLO_SOURCE_DIR是一致的。所以改了工程名也没有关系



## SET关键字

用于显示的指定变量的



例如：

```cmake
SET(SRL_LIST main.cpp)
```

> 若源文件名称中含有空格，需要使用双引号



## MESSAGE关键字

向终端输出用户自定义的信息：

- SEND_ERROR，产生错误，生成过程被跳过。
- SATUS，输出前缀为`—`的信息。
- FATAL_ERROR，立即终止所有 cmake 过程.



例如：

```cmake
MESSAGE(STATUS "hello")
```



## ADD_EXECUTABLE关键字

用于生成可执行文件。



例如：

```cmake
DD_EXECUTABLE(hello ${SRC_LIST})
# 生成的可执行文件名是hello，源文件读取变量SRC_LIST中的内容
# 等价于： DD_EXECUTABLE(hello main.cpp)
```

> 可以省略文件后缀名，如`DD_EXECUTABLE(hello main.cpp)`，可以省略为：
>
> `DD_EXECUTABLE(hello main)`





# 语法的基本规则

- 获取变量值：${}

  >在 IF 控制语句中可直接使用变量名

- 指令的一般格式：`指令(parm1 parm2 ...)`

  参数之间以 ` `或`,`分隔

- 大小写问题：

  - 指令与大小写无关
  - 参数、变量严格大小写



# 内部构建与外部构建

- 内部构建：特点是临时文件多

- 外部构建（推荐）：

  把生成的临时文件放在build目录下，不会对源文件有任何影响。



## 外部构建的例子

```sh
cmake -S . -B build -G "Unix Makefiles"
```

```sh
cmake --build build
```



# 让Hello World看起来更像一个工程

- src目录：放工程源代码

- doc目录：放工程的文档

- 在工程目录添加文本文件 COPYRIGHT, README

- 在工程目录添加一个 [runhello.sh](http://runhello.sh/) 脚本，用来调用 hello 二进制

  

- 将构建后的目标文件放入构建目录的 bin 子目录

  > 每一级目录下，都要有一个CMakeLists.txt

- 将 doc 目录 的内容以及 COPYRIGHT/README 安装到/usr/share/doc/cmake/



```sh
[root@localhost cmake]# tree
.
├── build
├── CMakeLists.txt
└── src
    ├── CMakeLists.txt
    └── main.cpp
```

- 工程目录下的CMakeLists.txt：

  ```cmake
  PROJECT(HELLO)
  ADD_SUBDIRECTORY(src bin)
  ```

  >将 src 子目录加入工程并指定编译输出(包含编译中间结果)路径为bin 目录
  >
  >>如果不进行 bin 目录的指定，那么编译结果(包括中间结果)都将存放在build/src 目录

- src下的CMakeLists.txt：

  ```cmake
  ADD_EXECUTABLE(hello main.cpp)
  ```

  

``ADD_SUBDIRECTORY(source_dir [binary_dir] [EXCLUDE_FROM_ALL])`

- 这个指令用于向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制存放的位置
- EXCLUDE_FROM_ALL函数是将写的目录从编译中排除