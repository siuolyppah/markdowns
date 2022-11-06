# 文件结构

```
├── CMakeLists.txt
├── build
├── include/project_name
└── src
```

- build: 放置构建文件和二进制文件的文件夹。
- include/project_name: 此文件夹包含==从项目外部公开访问==的所有头文件， 包含
  <project_name/somefile.h> 使它更容易看出头文件来自哪个库。

- src: 此文件夹包含所有==私有的源文件和头文件==。
- CMakeLists.txt: 这是主CMake 文件。



嵌套结构：

```cmake
├── CMakeLists.txt
├── build
├── include/project_name
├── src
└── subproject
    ├── CMakeLists.txt
    ├── include
    │ └── subproject
    └── src
```





# 创建HelloWorld可执行文件

- src/main.cpp：略

- CMakeLists.txt:

  ```cmake
  cmake_minimum_required(VERSION 3.21)
  
  project(
      demo-proj
      VERSION 1.0
      DESCRIPTION "CMake 测试Demo"
      LANGUAGES CXX
  )
  
  add_executable(helloworld)
  target_sources(helloworld PRIVATE src/main.cpp)
  ```



源码可以直接添加到`add_executable`；也可以单独使用`target_sources`，将它们与target_sources 一起添加。

在范围说明符(此例为PRIVATE)之后，有一个相对于当前CMakeLists.txt 文件路径的源文件列表。如果需要，当前处理的CMakeLists txt 文件的位置可以通过`CMAKE_CURRENT_SOURCE_DIR `得到。



# 创建库

==使用库来模块化和分发代码==。



- CMakeLists.txt：

  ```cmake
  cmake_minimum_required(VERSION 3.21)
  project(
  	ch3.hello_lib
  	VERSION 1.0
  	DESCRIPTION
  	"A simple C++ project to demonstrate creating executables and libraries in CMake"
  	LANGUAGES CXX
  )
  
  add_library(hello)
  target_sources(
  	hello
  	PRIVATE src/hello.cpp src/internal.cpp
  )
  
  target_compile_features(hello PUBLIC cxx_std_17)
  target_include_directories(
      hello
      PRIVATE src/hello
      PUBLIC include
  )
  ```

说明：

- 使用add_library 创建库的目标——本例中，库的类型没有确定。==可以传递STATIC或SHARED 来**显式**确定库的类型==，这里可以**==省略==**设置该类型，我们<u>==允许库的使用者选择如何构建和链接==</u>。通常，静态库很容易处理。

  >若省略了库的类型，则BUILD_SHARED_LIBS 将决定库是默认构建为动态库还是静态库。这个变量不应该在项目的CMake 文件中设置，应该由构建者传递。

- 使用target_sources为库添加源文件。

  第一个参数是目标名称，后面跟PRIVATE、PUBLIC或INTERFACE 关键字分隔相应源文件。

  >实践中，源文件使用PRIVATE 添加，PRIVATE 和PUBLIC关键字指定在何处使用源代码进行编译。PRIVATE 指定的源文件将只在目标hello 中使用。若使用PUBLIC，那么源文件也会将附加到hello 和依赖hello 的目标上，这通常不是我们想要的结果。INTERFACE 关键字说明源文件不会添加到hello 目标中，而是会添加到依赖到hello的目标上。

- 包含目录使用target_include_directories 设置。

  :star:==**该指令指定的文件夹内的所有文件都可以使用#include<file.hpp>(带尖括号) 来访问**==。