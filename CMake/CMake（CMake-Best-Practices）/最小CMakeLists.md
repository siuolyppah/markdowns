```cmake
cmake_minimum_required(VERSION 3.21)
project(
	"chapter1"
	VERSION 1.0
	DESCRIPTION "A simple project to demonstrate basic CMakeusage" 	
	LANGUAGES CXX)
	
add_executable(Chapter1)
target_sources(Chapter1 PRIVATE src/main.cpp)
```

- project说明要构建的项目的名称、版本和描述，后面是项目中使用的编程语言。
- add_executable 告诉CMake 要构建一个可执行文件(与库或自定义目标不同)。
- target_sources 告诉CMake 在哪里查找名为Chapter1 的可执行文件的源文件，并且源
  文件的可见性仅限于可执行文件。