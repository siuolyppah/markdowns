# 相关资料

- [视频教程](https://www.bilibili.com/video/BV1TB4y1s7eW)



# Demo

- src/main.cpp：

  ```C++
  #include <iostream>
  
  int main()
  {
      std::cout << "hello C++," << __cplusplus << std::endl;
  
      return 0;
  }
  ```

- CMakeLists.txt：

  ```txt
  cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
  
  project(studycmake
      VERSION 0.0.1
      DESCRIPTION "cmake study project"
      LANGUAGES CXX
  )
  
  list(APPEND CMAKE_MESSAGE_CONTEXT Top)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  
  message("=====================temp test start=====================")
  
  add_executable(main src/main.cpp)
  
  message("=====================temp test end=====================")
  ```

  

shell:
```sh
 cmake -S . -B build -G "Unix Makefiles"
```

```
cmake --build build
```

