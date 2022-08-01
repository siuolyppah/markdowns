# 区别

- 同步：

  被调用着完成工作后，才返回

- 异步：

  被调用者返回后，进行剩余大部分工作。



# 异步编程

- 传统做法：

  - 将长时间运行的函数，写成同步的。

    然后从新的线程或Task进行调用，并按需引入并发。

- 异步编程：

  - 将长时间运行的函数，写成异步的。
  - 从长时间运行函数的<font color="red">内部</font>，启动并发。



## 同步执行的Demo

```cs
namespace Test;
class Progrm
{
    static async Task Main(string[] args)
    {
        DisplayPrimeCounts();
    }

    static void DisplayPrimeCounts()
    {
        for(int i = 0; i < 10; i++)
        {
            Console.WriteLine(GetPrimesCount(i*100000+2, 100000)+
                " primes between " + (i* 100000) + " and " + ((i+1)* 100000-1));
        }

        Console.WriteLine("Done!");
    }

    static int GetPrimesCount(int start, int count)
    {
        return ParallelEnumerable.Range(start, count).Count(n =>
            Enumerable.Range(2, (int)Math.Sqrt(n) - 1).All(i => n % i > 0));
    }
}
```



## 乱序执行

- 能完成任务，但乱序执行的情况：

  ```cs
  namespace Test;
  class Progrm
  {
      static async Task Main(string[] args)
      {
          DisplayPrimeCounts();
          Console.ReadKey();
      }
  
      static async void DisplayPrimeCounts()
      {
          for (int i = 0; i < 10; i++)
          {
              var awaiter = GetPrimesCountAsync(i * 1000000 + 2, 1000000).GetAwaiter();
              int temp = i;
              awaiter.OnCompleted(async () =>
                  Console.WriteLine(awaiter.GetResult() + " primes between " + (temp * 1000000) + " and " + ((temp + 1) * 1000000 - 1)));
          }
          Console.WriteLine("Done!");
      }
  
      static Task<int> GetPrimesCountAsync(int start, int count)
      {
          return Task.Run(() => ParallelEnumerable.Range(start, count).Count(n =>
              Enumerable.Range(2, (int)Math.Sqrt(n) - 1).All(i => n % i > 0)));
      }
  }
  ```



## 顺序执行

```cs
namespace Test;
class Progrm
{
    static async Task Main(string[] args)
    {
        DisplayPrimeCountsFrom(0);
        Console.ReadKey();
    }

    static void DisplayPrimeCountsFrom(int i)
    {
        var awaiter = GetPrimesCountAsync(i * 1000000 + 2, 1000000).GetAwaiter();
        awaiter.OnCompleted(() =>
        {
            awaiter.OnCompleted(async () =>
                Console.WriteLine(awaiter.GetResult() + " primes between " + (i * 1000000) + " and " + ((i + 1) * 1000000 - 1)));
            
            if(++i < 10)
            {
                DisplayPrimeCountsFrom(i);
            }
            else
            {
                Console.WriteLine("Done");
            }
        });

    }

    static Task<int> GetPrimesCountAsync(int start, int count)
    {
        return Task.Run(() => ParallelEnumerable.Range(start, count).Count(n =>
            Enumerable.Range(2, (int)Math.Sqrt(n) - 1).All(i => n % i > 0)));
    }
}
```



## 使用async和await简化

```cs
namespace Test;
class Progrm
{
    static async Task Main(string[] args)
    {
        await DisplayPrimeCountsAsync();
        Console.ReadKey();
    }

    async static Task DisplayPrimeCountsAsync()
    {
        for(int i = 0; i < 10; i++)
        {
            Console.WriteLine(await
                GetPrimesCountAsync(i * 1000000 + 2, 1000000) + " primes between " + (i * 1000000) + " and " + ((i + 1) * 1000000 - 1));
        }

        Console.WriteLine("Done!");
    }

    static Task<int> GetPrimesCountAsync(int start, int count)
    {
        return Task.Run(() => ParallelEnumerable.Range(start, count).Count(n =>
            Enumerable.Range(2, (int)Math.Sqrt(n) - 1).All(i => n % i > 0)));
    }
}
```



# async和await关键字

- 用于简化异步代码的书写

  - await关键字，简化了附加continuation的过程

    ```cs
    var result = await exepression；
    statement(s);
    ```

    等价于：

    ```cs
    var awaiter = expression.GetAwaiter();
    awaiter.OnCompleted(()=>{
       var result = awaiter.GetResult();
        statment(s);
    });
    ```

  - Task关键字，用于对编译器进行提示。



# 异步方法如何执行

![image-20220801191708024](%E5%90%8C%E6%AD%A5vs%E5%BC%82%E6%AD%A5.assets/image-20220801191708024.png)



# 可以await的类型

- `Task`

- 满足下列条件的任意对象：

  - 有`GetAwaiter()`方法，它返回一个Awaiter。

    > 实现`INotifyCompletion.OnComputed`接口

  - 有`GetResult()`方法，且其返回值类型适当

  - 有bool类型的`IsCompleted`属性



# await可以捕获本地状态

- error的情况：

  ```cs
  namespace Test;
  class Progrm
  {
      static async Task Main(string[] args)
      {
          await DisplayPrimeCountsAsync();
          Console.ReadKey();
      }
  
      async static Task DisplayPrimeCountsAsync()
      {
          for(int i = 0; i < 10; i++)
          {
              Task<int> task = GetPrimesCountAsync(i * 1000000 + 2, 1000000);
              var awaiter = task.GetAwaiter();
              awaiter.OnCompleted(() =>
              {
                  Console.WriteLine(awaiter.GetResult() + "primes between " + (i * 1000000) + " and " + ((i + 1) * 1000000 - 1));
  
              });
  
          Console.WriteLine("Done!");
      }
  
      static Task<int> GetPrimesCountAsync(int start, int count)
      {
          return Task.Run(() => ParallelEnumerable.Range(start, count).Count(n =>
              Enumerable.Range(2, (int)Math.Sqrt(n) - 1).All(i => n % i > 0)));
      }
  }
  ```

  在上述代码中，当OnComputed回调被执行的时候，变量`i`的值已经改变。

- 使用await解决：

  ```cs
  namespace Test;
  class Progrm
  {
      static async Task Main(string[] args)
      {
          await DisplayPrimeCountsAsync();
          Console.ReadKey();
      }
  
      async static Task DisplayPrimeCountsAsync()
      {
          for(int i = 0; i < 10; i++)
          {
              Console.WriteLine(await
                  GetPrimesCountAsync(i * 1000000 + 2, 1000000) + " primes between " + (i * 1000000) + " and " + ((i + 1) * 1000000 - 1));
          }
  
          Console.WriteLine("Done!");
      }
  
      static Task<int> GetPrimesCountAsync(int start, int count)
      {
          return Task.Run(() => ParallelEnumerable.Range(start, count).Count(n =>
              Enumerable.Range(2, (int)Math.Sqrt(n) - 1).All(i => n % i > 0)));
      }
  }
  ```

  