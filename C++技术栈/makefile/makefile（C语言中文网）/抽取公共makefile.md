- 公共makefile：

  ```makefile
  all = $(wildcard ./*)
  
  .PHONY:clean
  clean:
  	@echo ${all}
  	@echo ${TARGET}
  	${RM} *.o ${TARGET}
  ```

- 某个具体的makefile：

  ```makefile
  TARGET = main
  
  include ../Makefile
  ```

  

关于变量赋值：

- `=`：

  运行时求值

  ```makefile
  A = 123
  B = ${A}
  A = 1
  
  .PHONY:show
  show:
  	@echo ${B} # 1
  ```

- `:=`：

  常量



