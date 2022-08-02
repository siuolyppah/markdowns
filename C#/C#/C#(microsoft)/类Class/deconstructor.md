- 作用与构造函数相反，用于将字段赋给一堆变量

- 语法要求：

  方法名必须是`Deconstruct`

- Deconstructor可以被重载





```cs
public class Rectangle
{
    public readonly float width, height;

    public Rectangle(float width, float height)
    {
        this.width = width;
        this.height = height;
    }

    public void Deconstruct(out float width,out float height)
    {
        width = this.width;
        height = this.height;
    }
}


class Program
{
    static void Main(string[] args)
    {
        Rectangle rect = new Rectangle(1, 2);

        // way1
        (float w1, float h1) = rect;

        // way2
        float w2, h2;
        rect.Deconstruct(out w2, out h2);

        // way3
        rect.Deconstruct(out var w3, out var h3);

        // way4
        var (w4, h4) = rect;
    }
}
```

