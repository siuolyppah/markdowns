```makefile
<target> : <prerequisites> 
[tab]  <commands>
```

- target：

  目标，**通常是文件名**，指明Make命令所要构建的对象。

  可以是Object File、可执行文件、Label。

- dependencies：

  前置条件通常是**一组文件名**，之间用空格分隔。

  只要有一个前置文件不存在，或者有过更新（前置文件的last-modification时间戳比目标的时间戳新），”目标”就需要重新构建。

- command：

  它是构建”目标”的具体指令，它的运行结果通常就是生成目标文件。
