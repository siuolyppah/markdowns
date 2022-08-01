- [(29条消息) C# async/awit 嵌套异步 执行顺序 分析_明天的光的博客-CSDN博客](https://blog.csdn.net/qq_35735344/article/details/79876480)

- [理解C#中的ConfigureAwait-C#/.net框架-少有人走的路 (skcircle.com)](http://www.skcircle.com/?id=1805#:~:text=当你在C%23中使用,tring>）。)





# My

```cs
namespace Test;

class Program
{

    static async Task Main(string[] args)
    {
        Thread.CurrentThread.Name = "main";

        Console.WriteLine("Main()" + Thread.CurrentThread.Name);
        await doA().ConfigureAwait(true);
        Console.WriteLine("Main()" + Thread.CurrentThread.Name);
    }

    static async Task doA()
    {
        Console.WriteLine("doA()" + Thread.CurrentThread.Name);
        await doB().ConfigureAwait(true);
        Console.WriteLine("doA()" + Thread.CurrentThread.Name);
    }


    static async Task doB()
    {
        Console.WriteLine("doB_RUN()" + Thread.CurrentThread.Name);

        await Task.Run(() =>
        {
            Console.WriteLine("doB()" + Thread.CurrentThread.Name);
            Thread.Sleep(500);
            Console.WriteLine("doB()" + Thread.CurrentThread.Name);
        }).ConfigureAwait(true);

        Console.WriteLine("doB_RUN()" + Thread.CurrentThread.Name);
    }
}
```



