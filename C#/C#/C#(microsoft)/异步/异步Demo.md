# 同步的情况

```cs
namespace Test;

class Program
{

    static void Main(string[] args)
    {

        MakeTea();
    }

    static string MakeTea()
    {
        var water = BoilWater();

        Console.WriteLine("take the cups out");
        
        Console.WriteLine("put tea in cups");

        return $"pour {water} in cups";
    }

    static string BoilWater()
    {
        Console.WriteLine("Start the kettle");
        
        Console.WriteLine("Waiting for the kettle");
        Task.Delay(2000).GetAwaiter().GetResult();
        Console.WriteLine("kettle finished boiling");

        return "water";
    }
}
```



# 异步的情况

```Cs
namespace Test;

class Program
{

    static async Task Main(string[] args)
    {

        await MakeTeaAsync();
    }

    static async Task<string> MakeTeaAsync()
    {
        Task<string> waterTask = BoilWaterAsync();

        Console.WriteLine("take the cups out");
        
        Console.WriteLine("put tea in cups");

        string water = await waterTask;

        return $"pour {water} in cups";
    }

    static async Task<string>BoilWaterAsync()
    {
        Console.WriteLine("Start the kettle");

        Console.WriteLine("Waiting for the kettle");
        await Task.Delay(2000);
        Console.WriteLine("kettle finished boiling");

        return "water";
    }
}
```





