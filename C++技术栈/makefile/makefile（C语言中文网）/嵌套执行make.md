在一个大的工程文件中，不同的文件按照功能被划分到不同的模块中，也就说很多的源文件被放置在了不同的目录下。每个模块可能都会有自己的编译顺序和规则。

>最外层的Makefile称为：总控 Makefile。



# demo演示

例如：

```
.
├── Makefile
└── subdir
    └── Makefile
```



最外层的Makefile：

```makefile
subsystem:
    cd subdir && $(MAKE)
```

或者：

```makefile
subsystem
    $(MAKE) -C subdir
```

> 变量 "CURDIR"，此变量代表 make 的工作目录。
>
> 选项 "-C" 可用于指定该变量，否则默认为当前工作目录。



# export的使用

使用 make 嵌套执行的时候，变量是否传递也是我们需要注意的。



- 如果需要变量的传递，那么可以这样来使用：

  ```makefile
  export <variable>
  ```

- 如果不需要那么可以这样来写：

  ```makefile
  unexport <variable>
  ```

>- \<variable>是变量的名字，不需要使用 "$" 这个字符。
>- 如果所有的变量都需要传递，那么只需要使用 "export" 就可以，不需要添加变量的名字。



# 案例

```
.
├── Makefile
├── src1
│   └── Makefile
└── src2
    └── Makefile
```



- 总控Makefile：

  ```makefile
  .PHONY:all
  all:
  	make -C ./src1
  	make -C ./src2
  ```

- src1下的Makefile：

  ```makefile
  .PHONY:main
  main:
  	@echo "src1"
  ```

- src2下的Makefile：

  ```makefile
  .PHONY:main
  main:
  	@echo "src2"
  ```



改进总控Makefile：

```makefile
DIR = ./src1 ./src2

all:${DIR}

.PHONY:${DIR}
${DIR}:
	make -C $@
```



