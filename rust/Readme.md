# 相关资料

- [简介 - Rust 程序设计语言 简体中文版 (kaisery.github.io)](https://kaisery.github.io/trpl-zh-cn/ch00-00-introduction.html)



# 如何阅读本书

- 第 1 章介绍如何安装 Rust，如何编写 “Hello, world!” 程序，以及如何使用 Rust 的包管理器和构建工具 Cargo。
- 第 2 章是 Rust 语言的实战介绍。我们会站在较高的层次介绍一些概念，而将详细的介绍放在稍后的章节中。如果你希望立刻就动手实践一下，第 2 章正好适合你。
- 第 3 章介绍 Rust 中类似其他编程语言的特性，你可以选择跳过，直接阅读第 4 章学习 Rust 的所有权（ownership）系统。不过，如果你注重细节，可以跳过第 2 章直接看第 3 章，之后想要写项目的时候再回来看第 2 章。

- 第 5 章讨论结构体（struct）和方法，第 6 章介绍枚举（enum）、`match` 表达式和 `if let` 控制流结构。在 Rust 中，创建自定义类型需要用到结构体和枚举。

- 第 7 章介绍 Rust 的模块（module）系统，其中的私有性规则用来组织代码和公开的 API（应用程序接口）。第 8 章讨论标准库提供的常见集合数据结构，例如 Vector（向量）、字符串和 Hash Map（散列表）。第 9 章探索 Rust 的错误处理技术和理念。

- 第 10 章深入介绍泛型（generic）、Trait 和生命周期（lifetime），用这些特性可以写出适用多种类型的代码。第 11 章介绍测试，因为就算 Rust 有安全保证，也需要测试确保程序逻辑正确。`grep` 命令可以查找文件中的文本，第 12 章中我们将会构建一个命令行工具实现它的部分功能，为此会用到之前章节讨论的很多概念。

- 第 13 章探索闭包（closure）和迭代器（iterator），这两个特性来自函数式编程语言。
- 第 14 章会深入探讨 Cargo 并介绍分享代码库的最佳实践。
- 第 15 章讨论标准库提供的智能指针以及相关的 Trait。

- 第 16 章介绍几类并发编程模型，并讨论 Rust 如何助你无畏地编写多线程程序。
- 第 17 章着眼于比较 Rust 风格与 OOP（面向对象编程）原则。

- 第 18 章介绍模式和模式匹配，它是在 Rust 程序中表达思想的有效方式。
- 第 19 章是一个高级主题大杂烩，包括不安全 Rust（unsafe Rust）、宏（macro）和更多关于生命周期、Trait、类型、函数和闭包的内容。

- 第 20 章我们将会完成一个项目，实现一个底层的、多线程的网络服务器！



最后的附录包含一些实用信息，格式类似参考文档。

- 附录 A 介绍 Rust 的关键字，
- 附录 B 介绍 Rust 的运算符和符号，
- 附录 C 介绍标准库提供的可派生 Trait，
- 附录 D 涉及一些有用的开发工具，
- 附录 E 介绍 Rust 的不同版本。



# Ferris提示

| Ferris                                                       | 含义                           |
| ------------------------------------------------------------ | ------------------------------ |
| ![Ferris 头上有问号](https://kaisery.github.io/trpl-zh-cn/img/ferris/does_not_compile.svg) | 这段代码无法通过编译！         |
| ![Ferris 举起两只钳子](https://kaisery.github.io/trpl-zh-cn/img/ferris/panics.svg) | 这段代码会 Panic！             |
| ![Ferris 举起一只钳子并耸肩](https://kaisery.github.io/trpl-zh-cn/img/ferris/not_desired_behavior.svg) | 这段代码的运行结果不符合预期。 |



# 查看本地文档

```sh
rustup doc
```

