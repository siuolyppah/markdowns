- lambda就是匿名函数（即只调用一次）。

- 语法：

  ```haskell
  \params -> expression
  ```

  > 通常将Lambda表达式以`()`包围，否则将会把整条表达式视作它的函数体。



- Lambda与柯里化：如下两段是等价的：

  ```haxe
  addThree :: (Num a) => a -> a -> a -> a
  addThree x y z = x + y + z
  ```

  ```haxe
  addThree :: (Num a) => a -> a -> a -> a
  addThree = \x -> \y -> \z -> x + y + z
  ```

  