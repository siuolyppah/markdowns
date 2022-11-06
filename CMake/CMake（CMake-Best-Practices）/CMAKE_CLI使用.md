

# 配置阶段

## -S 和 -G

- -S 参数用于指定要配置的CMake 项目，而-B 参数用于指定配置输出目录。

  ```sh
  cmake -S . -B ./build
  ```

- -G 指定Generator：

  ```sh
   cmake -G "Unix Makefiles" -S . -B ./build
  ```



## 提供配置参数

- 提供配置参数：

  - 指定构建类型：

    ```sh
    cmake -DCMAKE_BUILD_TYPE:STRING=Release -S . -B ./build
    ```

  - 指定编译器：

    ```sh
    cmake -DCMAKE_CXX_COMPILER=clang++ -S . -B ./build
    ```

    >编译器可以通过CMAKE\_\<LANG>_COMPILER 来指定。

  - 传递编译标志：

    ```sh
    cmake -DCMAKE_CXX_FLAGS:STRING="-Wall -Werror" -S . -B ./build
    ```

    构建标志可以为每个构建类型定制，方法是在它们后面加上大写的构建类型字符串：

    1. CMAKE_<LANG>_FLAGS_DEBUG
    2. CMAKE_<LANG>_FLAGS_RELEASE
    3. CMAKE_<LANG>_FLAGS_RELWITHDEBINFO
    4. CMAKE_<LANG>_FLAGS_MINSIZEREL

    只在Release 构建中将警告视为错误：

    ```sh
    cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS:STRING="-Wall
    -Werror" -DCMAKE_CXX_FLAGS_RELEASE:STRING="-O3" -DCMAKE_BUILD_
    TYPE:STRING=Debug -S . -B ./build
    ```



## 显示缓存列表

- 显示缓存变量列表：

  ```sh
  cmake -L ./build/
  ```



# 构建阶段

- 进行构建：

  ```sh
  cmake --build ./build
  ```

  或

  ```sh
  cd build && make
  ```

  > 并行构建：
  >
  > ```sh
  > cmake --build ./build --parallel 2
  > ```



- 只构建特定目标：

  ```cmake
  cmake --build ./build/ --target "ch2_framework_component1"
  --target "ch2_framework_component2"
  ```



- 删除之前的构建：

  ```sh
  cmake --build ./build/ --clean-first
  ```



- 调试构建过程：

  ```sh
  cmake --build ./build/ --verbose
  ```



- 将命令行参数传递给构建工具:

  ```sh
  cmake --build ./build/ -- --trace
  ```

  --trace 将直接传递给构建工具，例子中就是make。这将使make 为构建每个文件的编译和连接输出跟踪信息。



# 通过CLI安装项目

> CMake 允许在环境中进行安装，CMake 代码必须使用install() 指令来指定当调用cmake
> --install(或构建系统等效) 时安装什么。



- install:

  ```cmake
  cmake --install <project_binary_dir>
  ```

  > cmake --install 需要一个已经配置和生成的项目。

  > 不同环境的默认安装目录不同。对于类Unix 环境，默认为/usr/local，而在Windows 环境
  > 中，默认为C:/Program Files。

- 修改默认安装路径：

  ```cmake
  cmake --install build --prefix /tmp/example
  ```



- 在安装时剥离调试信息：

  ```sh
  cmake --install build --strip
  ```



- 安装特定组件：

  若项目在install() 中使用了CMake 的COMPONENT 特性，可以通过指定组件名称来安
  装特定的组件。

  ```sh
  cmake --install build --component executables
  ```



- 安装特定的配置：

  有些生成器支持同一个构建配置的多个配置(例如，Visual Studio)。对于这种类型的生成器， --install 选项提供了一个--config 参数来指定要安装的二进制文件配置。

  ```sh
  cmake --install build --config Debug
  ```

  

