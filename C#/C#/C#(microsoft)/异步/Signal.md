# Demo

```cs
namespace Test;
class Progrm
{
    static void Main(string[] args)
    {
        var signal = new ManualResetEvent(false);

        new Thread(() =>
        {
            Console.WriteLine("waiting for signal");
            
            signal.WaitOne();
            signal.Dispose();

            Console.WriteLine("Got signal");
        }).Start();

        Thread.Sleep(3000);
        signal.Set();
    }
}
```

