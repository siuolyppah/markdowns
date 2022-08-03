> [4.6 枚举和迭代器_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1At411N74o?p=3&vd_source=be746efb77e979ca275e4f65f2d8cda3)



`foreach` 语句可扩展为使用 `IEnumerable<T>` 和 `IEnumerator<T>` 接口的标准用语，以便循环访问集合中的所有元素。



# 枚举器Enumerator

- 枚举器是只读的，作用于一序列值的、只能向前的游标。

- 枚举器是实现如下任意一个接口的对象：

  - `System.Collections.IEnumerator`
  - `System.Collections.Generic.IEnumerator<T>`

  > 从技术上讲，满足下列要求的任意对象，都可以被作为枚举器：
  >
  > - 含有一个MoveNext()方法
  > - 含有名为Current的属性



- foreach语句，实际上是使用了枚举器对象。

  ```cs
  foreach (var item in collection)
  {
      Console.WriteLine(item?.ToString());
  }
  ```

  对于上述例子，编译器将其编译为：

  ```cs
  IEnumerator<int> enumerator = collection.GetEnumerator();
  while (enumerator.MoveNext())
  {
      var item = enumerator.Current;
      Console.WriteLine(item.ToString());
  }
  ```



# 实现IDisposable接口的枚举器

如果枚举器实现了该接口，则使用foreach时，编译器将隐式的对这个枚举器对象，调用`Dispose()`方法。



# 可枚举的对象Enumable

一个可枚举的对象，可以是（任意满足一个）：

- 实现了`IEnumable`接口的对象
- 实现了`IEnumable<T>`接口的对象
- 有一个名为`GetEnumerator()`，返回Enumerator的方法。

> 总的来说，实现Enumerate接口的对象，可以获得Enumerator，从而进行迭代。





#  迭代器Iterator

> Enumerator用于消费，Iterator用于生产数据。



- 迭代器，是含有一个或多个yield语句的方法、属性、索引器。
- 迭代器的返回类型，必须是下面4个中的一个：
  - `IEnumerable` / `IEnumerable<T>`
  - `IEnumerator` / `IEnumerator<T>`

- yield return，不可以出现在：

  - 含有catch的try块
  - catch块
  - finally块

  > 即，可以出现在try-finally的try块中。





 

# Demo

```cs
class MyEnumerator : IEnumerator, IDisposable
{
    public object Current => throw new NotImplementedException();

    public void Dispose()
    {
        
    }

    public bool MoveNext()
    {
        throw new NotImplementedException();
    }

    public void Reset()
    {
        throw new NotImplementedException();
    }
}

class MyEnumerable
{
    public  MyEnumerator GetEnumerator()
    {
        return new MyEnumerator();
    }
}


class Program
{    
    static IEnumerable<int> foo()
    {
        for(int i = 0; i < 20; i++)
        {
            yield return i;
        }
    }

    static void Main(string[] args) {
        // foreach MyEnumerable对象的等价形式
        MyEnumerable myEnumerable = new MyEnumerable();
        using(MyEnumerator i = myEnumerable.GetEnumerator()) { 
            while (i.MoveNext())
            {
                Console.WriteLine(i.Current);
            }
        }

        // foreach foo()的等价形式
        using(var i = foo().GetEnumerator())
        {
            while (i.MoveNext())
            {
                Console.WriteLine(i.Current);
            }
        }

    }
}
```

