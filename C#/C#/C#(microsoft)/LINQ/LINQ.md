[C# 中的语言集成查询 (LINQ) | Microsoft Docs](https://docs.microsoft.com/zh-cn/dotnet/csharp/linq/)

[C# LINQ (Part 1 - Introduction) - Advanced C# Tutorial (Part 7.1) - YouTube](https://www.youtube.com/watch?v=UfZOmSCCbDY)



- [查询表达式基础（C# 中的 LINQ） | Microsoft Docs](https://docs.microsoft.com/zh-cn/dotnet/csharp/linq/query-expression-basics)

​	





# LINQ支持的类型

- 实现`IEnumable`接口或`IEnumable<T>`接口的对象
- SQL
- XML
- 流



# HelloWorld Demo

```cs
// Specify the data source.
int[] scores = { 97, 92, 81, 60 };

// Define the query expression.
IEnumerable<int> scoreQuery =
    from score in scores
    where score > 80
    select score;

// Execute the query.
foreach (int i in scoreQuery)
{
    Console.Write(i + " ");
}

// Output: 97 92 81
```



# Basic

- 应用程序始终将源数据视为 [IEnumerable](https://docs.microsoft.com/zh-cn/dotnet/api/system.collections.generic.ienumerable-1) 或 [IQueryable](https://docs.microsoft.com/zh-cn/dotnet/api/system.linq.iqueryable-1) 集合。

  >例如，在 LINQ to XML 中，源数据显示为 `IEnumerable`\<[XELEMENT](https://docs.microsoft.com/zh-cn/dotnet/api/system.xml.linq.xelement)>

- 对于此源序列，查询可能会执行三种操作之一。



## 获取子序列

```cs
IEnumerable<int> highScoresQuery =
    from score in scores
    where score > 80
    orderby score descending
    select score;
```

>（假定 `scores` 是 `int[]`）



## 获取子序列并转换类型

演示从 `int` 到 `string` 的投影：

```cs
IEnumerable<string> highScoresQuery2 =
    from score in scores
    where score > 80
    orderby score descending
    select $"The score is {score}";
```

>（假定 `scores` 是 `int[]`）



## 获取某个值

例如：

- 与特定条件匹配的元素数。

- 最大或最小元素。

- 与某个条件匹配的第一个元素，或指定元素集中特定值的总和。

  ```cs
  int highScoreCount = (
      from score in scores
      where score > 80
      select score
  ).Count();
  ```

  等同于：

  ```cs
  IEnumerable<int> highScoresQuery3 =
      from score in scores
      where score > 80
      select score;
  
  int scoreCount = highScoresQuery3.Count();
  ```

  



# 查询表达式

查询表达式由一组用类似于 SQL 或 XQuery 的声明性语法所编写的子句组成。 每个子句进而包含一个或多个 C# 表达式，而这些表达式可能本身是查询表达式或包含查询表达式。



查询表达式：

- 必须以 [from](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/from-clause) 子句开头，
- 且必须以 [select](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/select-clause) 或 [group](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/group-clause) 子句结尾。 

- 在第一个 `from` 子句与最后一个 `select` 或 `group` 子句之间，可以包含以下这些可选子句中的一个或多个：[where](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/where-clause)、[orderby](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/orderby-clause)、[join](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/join-clause)、[let](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/let-clause)，甚至是其他 [from](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/from-clause) 子句。 还可以使用 [into](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/into) 关键字，使 `join` 或 `group` 子句的结果可以充当相同查询表达式中的其他查询子句的源。



# 查询变量

查询变量是存储如何查询，而不是查询结果的任何变量。

> 更具体地说，查询变量始终是IEnumerable类型。



```cs
// Data source.
int[] scores = { 90, 71, 82, 93, 75, 82 };

// Query Expression.
IEnumerable<int> scoreQuery = //query variable
    from score in scores //required
    where score > 80 // optional
    orderby score descending // optional
    select score; //must end with select or group

// Execute the query to produce the results
foreach (int testScore in scoreQuery)
{
    Console.WriteLine(testScore);
}

// Output: 93 90 82 82
```

- 在上面的示例中，`scoreQuery` 是查询变量，它有时仅仅称为查询。
- <font color="Red">查询变量不存储在 `foreach` 循环生成中的任何实际结果数据</font>。 

- 查询结果通过迭代变量 `testScore` 返回。

> 这意味着：
>
> 只要既没有修改查询变量，也没有修改数据源，便会生成相同结果。

