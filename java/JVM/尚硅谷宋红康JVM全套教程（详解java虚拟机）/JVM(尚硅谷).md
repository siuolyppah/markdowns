# 相关资料

https://www.bilibili.com/video/BV1PJ411n7xZ?spm_id_from=333.788.b_636f6d6d656e74.14



Java虚拟机规范：https://docs.oracle.com/javase/specs/index.html

书籍推荐：

- 《自己动手写Java虚拟机》——张秀宏



# 虚拟机简介

## JVM的整体结构

<img src="JVM(%E5%B0%9A%E7%A1%85%E8%B0%B7).assets/image-20220317192859253.png" alt="image-20220317192859253" style="zoom: 50%;" />

## Java代码的执行流程

<img src="JVM(%E5%B0%9A%E7%A1%85%E8%B0%B7).assets/image-20220317192933366.png" alt="image-20220317192933366" style="zoom:50%;" />

## JVM的架构模型

- 基于栈式架构的特点：
    - 设计和实现简单，适用于资源受限的系统
    - 避开了寄存器的分配难题：使用零地址指令方式进行分配
    - 指令流中的指令，大部分是零地址指令，其执行过程依赖于栈。
    - 指令集更小，编译器容易实现
    - 不需要硬件支持，可移植性强
- 基于存储器架构的特点：（典型的有x86，Android的Davlik虚拟机）
    - 完全依赖于硬件，可移植性差
    - 性能优秀，执行更高效
    - 完成一项操作需要的指令数量更少

## 虚拟机的生命周期

1. 虚拟机的启动：

    通过引导类加载器(bootstrap classloader)创建一个初始类(initial class)来完成。

2. 虚拟机的执行：

    执行Java程序，本质上就是在执行Java虚拟机进程

3. 虚拟机的退出：

    当遇到如下情况时：

    - 程序正常执行结束
    - 程序在执行过程中，遇到了异常
    - 由于操作系统出现错误，而导致的Java虚拟机进程终止
    - 某线程调用Runtime类或System类的exit方法(或Runtime类的halt方法)，并且Java安全管理器允许此次exit或halt方法调用







# 内存与垃圾回收篇

[内存与垃圾回收篇](内存与垃圾回收篇\内存与垃圾回收篇.md)



# 字节码与类的加载篇



# 性能监控与调优篇



# 大厂面试篇