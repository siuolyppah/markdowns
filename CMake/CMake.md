# CMake是什么

- CMake是：**高级编译配置工具**。
- 所有操作都是通过编译CMakeLists.txt来完成的



# CMake安装

- Linux：自带
- Windows：http://www.cmake.org/HTML/Download.html



## Cmake一个HelloWorld

1. 编写一个HelloWorld

   ```cpp
   #main.cpp
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

4. 使用`make`命令进行编译