# 两个里程碑版本

Haskell 98 和 Haskell 2010  

>本书的很多代码是符合 Haskell 98 标准的，但有一部分用到了 Haskell 2010 中增加的内
>容，和一些 GHC 支持的语法扩展。  





# GHC编译器安装与环境配置

## 编译器

- [GHC](https://www.haskell.org/ghc/)（Glasgow Haskell Compiler，哥拉斯哥 Haskell 编译器）是最常
  用的 Haskell 语言的编译器
- 荷兰乌特勒克大学编写的 UHC
- 由 John Meacham 所编写的 JHC 等  



教学用的两个解释器：

- [Hugs](https://www:haskell:org/hugs/pages/downloading:htm)

  >没有丰富的库函数， 2006 年后不再开发，因此不推荐。  

- ==GHCi==（Glasgow Haskell Compiler interpreter）  ：

  >**GHCi 是 GHC 的一部分**  

  下载[Haskell Platform](https://www.haskell.org/platform/  )，其包含：

  - GHC、 Cabal、 Stack 等很多开发工具  
  - 包装了图形界面的 WinGHCi  

  > - 用ghcup工具配置环境

  



## 继承开发环境

Haskell的集成开发环境：

- Leksah(Haskell的逆写）  
- EclipseFP 





# GHCi的使用

## GHCi的命令

- `:load`，简写为`:l` ，用来导入当前路径或者指定路径下的文件。

  >widows下注意转义`\`，例如：
  >
  >```ghci
  >Prelude> :l "C:\\Users\\ User\\Desktop\\HelloWorld\\HelloWorld.hs"\
  >```

- `:reload`，简写为`:r`，用来重新导入当前的源代码文件。

  >通常，在保存了源文件后， GHCi不会自动重新导入修改后的文件，用户可以很方便地使用 :r 来重新导入。   

- `:cd`，改变当前 GHCi 的路径

- `:edit`，用默认的文本编辑器编辑当前导入的文件。

  >如果使用 GHCi，它会读取系统环境变量中的 EDITOR，启动相应的编辑器。

- `:moudle`，简写为`:m`，用于导入一个库。

  >使用:m +\<module1> \<module2> 与:m -\<module1> \<module2>
  >来增加与移除不同的模块，只在 GHCi 中输入:m 会移除所有载入的模块。  

- `:!`，在 GHCi 中执行一些操作系统的命令。

  > 例如Windows下：`:! dir`

- `:quit`，退出GHCi。
- `:?`，输出帮助信息。





# .hs和.lhs文件

- GHCi 和 Hugs 可以解析扩展名为.hs 和.lhs 的文件。两者所写程序在语法上完全相同。

- 差别是：

  - ==.lhs==（Literate Haskell Script）文件：

    1. 是为了能让 Haskell 的代码生成精美的 pdf

    2. 有效程序代码可以用“大于号（>）”和空格开头：

       ```haskell
       > add :: Int -> Int -> Int
       > add x y = x + y
       ```

    3. 注释：不以`> `开头的内容，且与代码间有一行以上的间隔。

  - ==.hs==文件：

    1. 全局函数要起头写，不允许有其他字符
    2. 单行注释以`--`开头；多行注释以`{-`开头，以`-}`结尾。



# HelloWorld.hs

Helloworld.hs:

```haskell
main = putStrLn "Hello,World!"
```

>程序的入门：名为main的函数。
>
>



运行：

```sh
runghc .\Helloworld.hs
```



