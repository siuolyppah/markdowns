# 折叠的概念

> 用于简化递归时的边界条件。



==所有遍历 List 中元素并据此返回一个值的操作都可以交给 fold 实现==。



fold的参数：

- 一个二元函数，其参数将为：
  - 累加值
  - List的首项（或尾项）

- 一个初始值（累加值）
- 一个需要折叠的List



有个理解折叠的思路：假设我们有个二元函数 f，起始值 z，如果从右折叠 [3,4,5,6]，实际上执行的就是 f 3 (f 4 (f 5 (f 6 z)))。f 会被 List 的尾项和累加值调用，所得的结果会作为新的累加值传入下一个调用。

相似，左折叠一个 List，以 g 为二元函数，z 为累加值，它就与 g (g (g (g z 3) 4) 5) 6 等价。





# foldl与foldr

例如左折叠`foldl`：

```haskell
ssum :: (Foldable t, Num a) => t a -> a
ssum lst = foldl (\acc x -> acc + x) 0 lst
```

```haskell
contains e ls = foldl (\acc x -> (e == x) || acc) False ls
```



右折叠`foldr`：

```haskell
mmap f ls = foldr (\x acc -> f x : acc) [] ls
```

>使用 (++) 往 List 后面追加元素的效率要比使用 (:) 低得多。



==右折叠可以处理无限长度的数据结构，而左折叠不可以==。



# foldl1与flodr1

foldl1 与 foldr1 的行为与 foldl 和 foldr 相似，只是你无需明确提供初始值。

他们假定 List 的首个 (或末尾) 元素作为起始值，并从旁边的元素开始折叠。这一来，sum 函数大可这样实现：sum = foldl1 (+)。



# scanl和scanr

scanl 和 scanr 与 foldl 和 foldr 相似，只是它们会记录下累加值的所有状态到一个 List。也有 scanl1 和 scanr1。



```haxe
ghci> scanl (+) 0 [3,5,2,1]
[0,3,8,10,11]
```



