# 使用第三方库

```sh
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```



在自己的项目中，在CMakeLists.txt中添加：：

```cmake
find_package(Libevent REQUIRED core)
target_link_libraries(你的可执行目标 libevent::core)
```

> 以使用[Libevent](https://github.com/libevent/libevent)为例



