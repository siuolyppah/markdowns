# Null合并操作符

- 即`??`

- 如果操作数不是null，那么把他给我；

  否则，给我一个默认值。

- **短路运算**



```cs
string s1 = null;
string s2 = s1 ?? "nothing";	// 短路求值
```



# Null条件操作符

- 即`?`

- 如果操作数不是null，调用`.`后面的成员；

  <font color="red">否则，返回null。而非抛出空指针异常</font>。

- **短路运算**



```cs
string s1 = "hello";
string s2 = s1?.Substring(1);

// 等同于：
string s2 = (s1 == null ? null : s1.Substring(1));
```



> 最终用于接收表达式值的变量，必须可以接收null。



# Nullable\<T>

```cs
string str =n ull;
int? x = str?.length;	// Nullable<T>类型
```



`Nullable<T>`的常用属性和方法：

- `.HasValue`：null返回false
- `.Value`：null时抛出异常
- `.GetValueOrDefault()`：返回value或该类型默认值
- `.GetValueOrDefualt()`



关于`==`判断：
```cs
{
    int? x = 3;
    int? y = 3;
    Console.WriteLine(x == y);  // true
}
{
    int? x = 2;
    int? y = 3;
    Console.WriteLine(x == y);  // false
}
{
    int? x = 3;
    int? y = null;
    Console.WriteLine(x == y);  // false
}
{
    int? x = null;
    int? y = null;
    Console.WriteLine(x == y);  // true
}
```



关于转换：

- `T` -> `Nullable<T>`：隐式转换
- `Nullable<T>` -> `T`：显式转换

```csharp
{
    int i = 3;
    int? j = i;
}
{
    int? i = 3;
    int j = (int)i;
}
{
    int? i = null;
    int j = (int)i; // InvalidOperationException:
}
```

