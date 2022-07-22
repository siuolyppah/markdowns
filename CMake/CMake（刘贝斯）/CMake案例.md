- [【使用CMake组织C++工程】2：CMake 常用命令和变量 (elloop.github.io)](https://elloop.github.io/tools/2016-04-10/learning-cmake-2-commands#:~:text=这里面的,add_executable 就是一个CMake命令，它的作用是添加一个可执行文件构建目标。)



# Demo1

```
│  CMakeLists.txt
│  main.cpp
│  printHello.cpp
│
└─headers
        printHello.h  
```



- headers / printHello.h：

  ```c++
  #ifndef __PRINT_HELLO_H__
  #define __PRINT_HELLO_H__
  
  void hello(const char *name);
  
  #endif
  ```

- printHello.cpp：

  ```C++
  #include "printHello.h"
  #include <iostream>
  
  void hello(const char *name)
  {
      std::cout << name << std::endl;
  }
  ```

- main.cpp：

  ```C++
  #include "printHello.h"
  
  int main(){
  
      hello("xiaoming");
      return 0;
  }
  ```

- CMakelists.txt：

  ```cmake
  cmake_minimum_required(VERSION 3.0)
  
  project(hello)
  
  include_directories(${PROJECT_SOURCE_DIR}/headers)
  
  # 添加动态库 
  # 是执行目标
  add_library(printHello SHARED printHello.cpp)
  
  add_executable(hello main.cpp)
  target_link_libraries(hello printHello)
  ```



构建命令：

```sh
mkdir build && cd build
cmake .. -G 'Unix Makefiles'
cmake --build .
```

