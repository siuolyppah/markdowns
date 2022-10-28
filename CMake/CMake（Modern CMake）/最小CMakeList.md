# 最低版本要求

这是每个 `CMakeLists.txt` 都必须包含的第一行:

```cmake
cmake_minimum_required(VERSION 3.1)
```

- 从 CMake 3.12 开始，版本号可以声明为一个范围，例如 `VERSION 3.1...3.15`



# 设置一个项目

每一个==顶层== CMakelists 文件都应该有下面这一行：

```cmake
project(MyProject VERSION 1.0
                  DESCRIPTION "Very nice project"
                  LANGUAGES CXX)
```





# 生成一个可执行文件

```cmake
add_executable(one two.cpp three.h)
```

- `one` 既是生成的可执行文件的名称，也是创建的 `CMake` 目标(target)的名称。

- 紧接着的是源文件的列表，==根据拓展名只编译源文件==。

  > 在大多数情况下，==头文件==将会被忽略；==列出他们的唯一原因是为了让他们在 IDE 中被展示出来==，目标文件在许多 IDE 中被显示为文件夹。



# 生成一个库

```cmake
add_library(one STATIC two.cpp three.h)
```

- 库的类型，可以是 `STATIC`,`SHARED`, 或者`MODULE`.如果你不选择它，CMake 将会通过 `BUILD_SHARED_LIBS` 的值来选择构建 STATIC 还是 SHARED 类型的库。



- 经常需要生成一个虚构的目标，也就是说，一个不需要编译的目标。

  > 如只有一个头文件的库。这被叫做 ==`INTERFACE` 库==，后面不能有文件名。

- 也可以用一个现有的库做一个 `ALIAS` 库，这只是给已有的目标起一个别名。





# 配置目标

现在我们已经指定了一个目标，那我们如何添加关于它的信息呢？例如，它可能需要包含一个目录：

```cmake
target_include_directories(one PUBLIC include)
```



- [`target_include_directories`](https://cmake.org/cmake/help/latest/command/target_include_directories.html) 为目标添加了一个目录。

  - `PUBLIC` 等，对于一个二进制目标没有什么含义；
  - 但对于库：
    - PUBLIC：任何链接到这个目标的目标也必须包含这个目录
    - PRIVATE：只影响当前目标，不影响依赖
    - INTERFACE：只影响依赖

- [`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html)，指定目标的依赖关系。

  ```cmake
  target_link_libraries(another PUBLIC one)
  ```

  如果不存在名称为 `one` 的目标，那他会添加一个链接到你路径中的`one` 库。





# Demo

```cmake
cmake_minimum_required(VERSION 3.8)

project(Calculator LANGUAGES CXX)

add_library(calclib STATIC src/calclib.cpp include/calc/lib.hpp)
target_include_directories(calclib PUBLIC include)
target_compile_features(calclib PUBLIC cxx_std_11)

add_executable(calc apps/calc.cpp)
target_link_libraries(calc PUBLIC calclib)
```

