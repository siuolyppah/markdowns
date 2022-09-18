# 前缀与中缀函数

- 中缀函数：
  - 对于FP：`a + b`
- 前缀函数：
  - 对于命令式语言：`f(1,2,3)`
  - 对于FP：`(+) 1 2`



- 对于两个参数的函数，总是能使用其中缀形式：

  ```haxe
  Prelude> 7 `div` 3
  2
  ```

  







# 函数的调用具有最高优先级

- 函数调用拥有最高的优先级：

  ```haxe
  Prelude> succ 9 * 10
  100
  ```

  即此时为`(succ 9) * 10`。



# 分支表达式

- IF表达式：

  ```haxe
  mySgn x =
    if x > 0
      then 1
      else if x < 0 then -1 else 1
  ```

  > Haskell的else分支，是不能省略的。



- Guard：

  ```haxe
  mySgn :: (Ord a, Num a, Num p) => a -> p
  mySgn x
    | x > 0 = 1
    | x == 0 = 0
    | otherwise = -1
  
  func :: (Ord a, Num a, Num p) => a -> p
  func x = mySgn x + 1
  ```



# 没有参数的函数

- 没有参数的函数，被称作是“定义”:

  ```haxe
  Prelude> mystr = "hello,world"
  Prelude> mystr 
  "hello,world"
  ```

  mystr与字符串"hello,world"等价。



# List（使用[]）

在 Haskell 中，List 是一种单类型的数据结构，可以用来存储多个**类型相同**的元素。

>在 ghci 下，我们可以使用 `let` 关键字来定义一个常量。在 ghci 下执行 `let a =1` 与在脚本中编写 `a=1` 是等价的。



List：

```haxe
*Main> let lostNumbers = [1,2,3,4,5]
*Main> lostNumbers 
[1,2,3,4,5]
```

>List 中的 List 可以是不同长度，但必须得是相同的类型。



List支持的运算子：

- 使用`++`合并List：

  ```haxe
  *Main> let r = [1,2,3] ++ [4,5,6]
  *Main> r
  [1,2,3,4,5,6]
  ```

  > 在使用++操作时，会遍历左边的List。

- 使用`:`连接一元素与一List：

  ```haxe
  *Main> let r = [1,2,3] ++ 4:5:[6,7]
  *Main> r 
  [1,2,3,4,5,6,7]
  ```

- 使用`!`根据索引获取元素，从0开始：

  ```haxe
  *Main> let r = [0,1..10]
  *Main> r !! 2
  2
  ```



List常用的函数：

- `head`，返回List的首元素
- `tail`，返回除首元素外的其他元素
- `last`，返回尾元素
- `init`，返回除尾元素以外的其他元素
- `length`，返回长度
- `null`，检查List是否为空
- `reverse`，反转
- `take`，返回List的前n个元素
- `drop`，删除List的前n个元素
- `maximum`，返回最大值
- `minimum`，返回最小值
- `sum`，返回所有元素的累和
- `product`，返回所有元素的累积。
- `elem`，判断一个元素是否在List中



# 使用Range

使用Range可以构造List，但其中的值必须是可枚举的：

```haxe
*Main> ['A'..'Z']
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
```



- 如果是从20到1的Range，则需要额外给出一个元素，用于确定步长
- 浮点数也支持Range，但可能不够精确。

- 可以不标明上限，从而得到一个无限长度的List。

  ```haxe
  *Main> 4 `take` [1,2..]
  [1,2,3,4]
  ```

  

有用的函数：

- cycle：接受一个 List 做参数并 返回一个无限 List

  ```haxe
  *Main> 7 `take` cycle [1,2,3]  
  [1,2,3,1,2,3,1]
  ```

- repeat：接受一个值作参数，并返回一个仅包含该值的无限 List

  ```haxe
  *Main> 4 `take` repeat 2
  [2,2,2,2]
  ```

- replicate：获取包含相同元素的列表

  ```haxe
  *Main> replicate 3 10        
  [10,10,10]
  ```



# List Comprehension

$$
S=\{2 \cdot x \mid x \in N , x \leq 10\}
$$

对于此集合：竖线左端的部分是输出函数，x 是变量，N 是输入集合。



使用List Comprehension：

```haxe
*Main> [x*2 | x<-[1..10]]
[2,4,6,8,10,12,14,16,18,20]
```



再比如，只取乘以 2 后大于等于 12 的元素：

```haxe
Prelude> [x*2 | x<-[1..10], x*2>=12]
[12,14,16,18,20]
```

> 从一个 List 中筛选出符合特定限制条件的操作也可以称为过滤 (flitering)。



除了多个限制条件之外，从多个 List 中取元素也是可以的：

```haxe
Prelude> [x*y | x<-[2,5,10], y <-[8,10,11]]
[16,20,22,40,50,55,80,100,110]
```



length的原理：
```haskell
length' lst = sum [1 | _ <- lst]
```

>_ 表示我们并不关心从 List 中取什么值，与其弄个永远不用的变量，不如直接一个 _。



使用嵌套的List Comprehension：

```haxe
*Main> xxs = [[1, 2, 5], [2, 3, 6]]
*Main> [[x | x <- xs, even x] | xs <- xxs]
[[2],[2,6]]
```



# Tuple（使用()）

> review：List要求元素类型相同，但不关心元素数目。

Tuple则要求你对需要组合的数据的数目非常的明确，它的类型取决于其中项的数目与其各自的类型。

> 例如：想要保存点的List：
>
> - 使用List：`[[1,2],[8,11]]`是可行的，但不安全。
>
>   因为它不检查元素数目，因此`[[1,2],[3,4,5]]`也是合法的。



Tuple支持的函数：

- `fst`，返回首元素，==仅对Pair有效==。
- `snd`，返回尾元素，==仅对Pair有效==。

- `zip`，生成Pair的List：

  ```haxe
  *Main> id = [1,2,3]
  *Main> name= ['a','b','c']
  *Main> zip id name
  [(1,'a'),(2,'b'),(3,'c')]
  ```

  > 较长的那个List将断开。

- 