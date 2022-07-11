# 相关资料

- [(29条消息) 【CMake】CMakeLists.txt的超傻瓜手把手教程（附实例源码）_Yngz_Miao的博客-CSDN博客_cmakelists.txt 教程](https://blog.csdn.net/qq_38410730/article/details/102477162)
- [(29条消息) 【CMake】CMake的configure_file指令_Yngz_Miao的博客-CSDN博客_cmake configure_file](https://blog.csdn.net/qq_38410730/article/details/103741579)
  - [(29条消息) 【CMake】cmake中的include指令（.cmake文件/MACRO宏/function函数）_Yngz_Miao的博客-CSDN博客_cmake macro](https://blog.csdn.net/qq_38410730/article/details/102677143)





# 内置变量

- ` ${PROJECT_SOURCE_DIR}`：

  本CMakeLists.txt所在的文件夹路径

- `${PROJECT_NAME}`：

  本CMakeLists.txt的project名称





# 命令

## project

```cmake
# 本CMakeLists.txt的project名称
# 会自动创建两个变量，PROJECT_SOURCE_DIR和PROJECT_NAME
# ${PROJECT_SOURCE_DIR}：本CMakeLists.txt所在的文件夹路径
# ${PROJECT_NAME}：本CMakeLists.txt的project名称
project(xxx)
```





## aux_source_directory

```cmake
# 获取路径下所有的.cpp/.c/.cc文件，并赋值给变量中
aux_source_directory(路径 变量)
```



## set

```cmake
# 给文件名/路径名或其他字符串起别名，用${变量}获取变量内容
set(变量 文件名/路径/...)
```



## add_definitions

```cmake
# 添加编译选项
add_definitions(编译选项)
```



## message

```cmake
# 打印消息
message(消息)
```



## add_subdirectory

```cmake
# 编译子文件夹的CMakeLists.txt
add_subdirectory(子文件夹名称)
```



## add_library

```cmake
# 将.cpp/.c/.cc文件生成.a静态库
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
add_library(库文件名称 STATIC 文件)
```



## add_executable

```cmake
# 将.cpp/.c/.cc文件生成可执行文件
add_executable(可执行文件名称 文件)
```



## include_directories

```cmake
# 规定.h头文件路径
include_directories(路径)
```



## link_directories

```cmake
# 规定.so/.a库文件路径
link_directories(路径)
```



## target_link_libraries

```cmake
# 对add_library或add_executable生成的文件进行链接操作
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
target_link_libraries(库文件名称/可执行文件名称 链接的库文件名称)
```





# CmakeLists.txt一般格式

```cmake
project(xxx)                                          #必须

add_subdirectory(子文件夹名称)                         #父目录必须，子目录不必

add_library(库文件名称 STATIC 文件)                    #通常子目录(二选一)
add_executable(可执行文件名称 文件)                     #通常父目录(二选一)

include_directories(路径)                              #必须
link_directories(路径)                                 #必须

target_link_libraries(库文件名称/可执行文件名称 链接的库文件名称)       #必须
```



除了这些之外，就是些set变量的语句，if判断的语句，或者其他编译选项的语句，但基本结构都是这样的。



# Demo

**GitHub**：https://github.com/yngzMiao/protobuf-parser-tool

实例的功能是生成和解析`proto`文件，分为`C++`和`python`版本。其中，`C++`版本就是采用`CMakeLists.txt`编写的，目录结构如下：

```
|---example_person.cpp
|---proto_pb2
        |--Person.pb.cc
        |--Person.pb.h
|---proto_buf
        |---General_buf_read.h
        |---General_buf_write.h
|---protobuf
        |---bin
                |---...
        |---include
                |---...
        |---lib
                |---...
```

目录结构含义：

-  **protobuf**：`Google`提供的相关解析库和头文件，被`proto_pb2`文件夹内引用；
- **proto_pb2**：封装的`Person`结构和`Person`相关的处理函数，被`proto_buf`文件夹内引用；
- **proto_buf**：封装的`read`和`write`函数，被`example_persom.cpp`文件引用。

即：`example_person.cpp`–>`proto_buf`文件夹–>`proto_pb2`文件夹–>`protobuf`文件夹





## CMakeLists.txt写在哪

在需要进行编译的文件夹内编写`CMakeLists.txt`，即含有`.cpp/.c/.cc`的文件夹内。



通常的解决方案，就是**将下层目录编译成一个静态库文件，让上层目录直接读取和调用，而上层目录就直接生成一个可执行文件**。



即目录结构如下：

```
|---example_person.cpp
|---CMakeLists.txt
|---proto_pb2
        |--Person.pb.cc
        |--Person.pb.h
        |--CMakeLists.txt
|---proto_buf
        |---General_buf_read.h
        |---General_buf_write.h
|---protobuf
        |---bin
                |---...
        |---include
                |---...
        |---lib
                |---...
```



## 编写CMakeLists.txt

- 上层的CMakeLists.txt：

  ```cmake
  cmake_minimum_required(VERSION 3.0)
  project(example_person)
  
  # 如果代码需要支持C++11，就直接加上这句
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  # 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
  add_definitions("-Wall -g")
  
  # 设置变量，下面的代码都可以用到
  set(GOOGLE_PROTOBUF_DIR ${PROJECT_SOURCE_DIR}/protobuf)
  set(PROTO_PB_DIR ${PROJECT_SOURCE_DIR}/proto_pb2)
  set(PROTO_BUF_DIR ${PROJECT_SOURCE_DIR}/proto_buf)
  
  # 编译子文件夹的CMakeLists.txt
  add_subdirectory(proto_pb2)
  
  # 规定.h头文件路径
  include_directories(${PROJECT_SOURCE_DIR}
      ${PROTO_PB_DIR} ${PROTO_BUF_DIR}
  )
  
  # 生成可执行文件
  add_executable(${PROJECT_NAME} example_person.cpp )
  
  # 链接操作
  target_link_libraries(${PROJECT_NAME}
      general_pb2)
  ```

  当运行到`add_subdirectory`这一句时，会先将子文件夹进行编译，而`libgeneral_pb2.a`是在子文件夹中生成出来的库文件。子文件夹运行完后，父文件夹就已经知道了`libgeneral_pb2.a`这个库，因而不需要`link_directories`了。

- 下层的CMakeLists.txt：

  ```cmake
  project(general_pb2)
  
  aux_source_directory(${PROJECT_SOURCE_DIR} PB_FILES)
  
  add_library(${PROJECT_NAME} STATIC ${PB_FILES})
  
  include_directories(${PROJECT_SOURCE_DIR}
      ${GOOGLE_PROTOBUF_DIR}/include
  )
  
  link_directories(${GOOGLE_PROTOBUF_DIR}/lib/)
  
  target_link_libraries(${PROJECT_NAME}
      protobuf
  )
  ```

  在这里，`GOOGLE_PROTOBUF_DIR`是上层`CMakeLists.txt`中定义的，`libprotobuf.a`是在`${GOOGLE_PROTOBUF_DIR}/lib/`目录下的。



## CMakeLists.txt的编译

一般CMakeLists.txt是，**在最顶层创建build文件夹，然后编译**。即：

```sh
mkdir build && cd build
cmake ..
make
```