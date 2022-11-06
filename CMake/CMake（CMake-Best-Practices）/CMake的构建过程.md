

```sh
cmake -S /path/to/source -B /path/to/build
cmake --build /path/to/build
```

CMake 的构建过程分为==**两个阶段**==:

1. 第一步称为==**配置**==，读取CMakeLists.txt 文件，并为系统的本地构建工具链==生成指令==。
2. 第二步，==执行这些构建指令，并构建可执行文件或库==。

![image-20221106125356662](CMake%E7%9A%84%E6%9E%84%E5%BB%BA%E8%BF%87%E7%A8%8B.assets/image-20221106125356662.png)
