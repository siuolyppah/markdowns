# 经典的 CMake 构建流程（TM）

- 经典方式：

  ```sh
  ~/package $ mkdir build
  ~/package $ cd build
  ~/package/build $ cmake ..
  ~/package/build $ make # 或者cmake --build .
  ```

- 较新版本的CMake:

  ```sh
  ~/package $ cmake -S . -B build
  ~/package $ cmake --build build
  ```

  

# 执行安装

任何一条命令，都可执行安装：

```sh
# From the build directory (pick one)
~/package/build $ make install
~/package/build $ cmake --build . --target install
~/package/build $ cmake --install . # CMake 3.15+ only

# From the source directory (pick one)
~/package $ make -C build install
~/package $ cmake --build build --target install
~/package $ cmake --install build # CMake 3.15+ only
```





# 指定编译器

- 指定编译器==必须在第一次运行时在空目录中进行==。这种命令并不属于 CMake 语法。

- 例如选择Clang：

  ```sh
  ~/package/build $ CC=clang CXX=clang++ cmake ..
  ```

- 只需要调用一次；之后 CMake 会继续使用从这些变量里推导出来的路径。



# 指定生成器

- 选择的构建工具有很多；通常默认的是 `make`。

- 显示在你的系统上 CMake 可以调用的所有构建工具，运行：

  ```sh
  ~/package/build $ cmake --help
  ```

- 在一个目录中第一次调用 CMake 时就指定构建工具，用 `-G"My Tool"`（仅当构建工具的名字中包含空格时才需要引号）来指定构建工具。





# 标准选项

大部分软件包中都会用到以下的 CMake 选项：

- `-DCMAKE_BUILD_TYPE=` 从 Release， RelWithDebInfo， Debug， 或者可能存在的更多参数中选择。
- `-DCMAKE_INSTALL_PREFIX=` 这是安装位置。UNIX 系统默认的位置是 `/usr/local`，用户目录是 `~/.local`，也可以是你自己指定的文件夹。
- `-DBUILD_SHARED_LIBS=` 你可以把这里设置为 `ON` 或 `OFF` 来控制共享库的默认值（不过，你也可以明确选择其他值而不是默认值）
- `-DBUILD_TESTING=` 这是启用测试的通用名称，当然不会所有软件包都会使用它，有时这样做确实不错。



# 调试

- `--trace` 选项能够打印出运行的 CMake 的每一行。
- CMake 3.7 添加了 `--trace-source="filename"` 选项，这让你可以打印出你想看的特定文件运行时执行的每一行。