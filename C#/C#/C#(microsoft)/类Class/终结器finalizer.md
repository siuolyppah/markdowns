- Finalizer，是class专有的一种方法
- 在GC回收对象之前运行
- 本质是对object的Finalize()方法的重写



语法要求：

```cs
class Foo{
	~Foo(){
        
    }
}
```

