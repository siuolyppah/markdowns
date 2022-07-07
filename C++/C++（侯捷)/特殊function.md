# conversion function

![image-20220707145543579](%E7%89%B9%E6%AE%8Afunction.assets/image-20220707145543579.png)

在该例中，**Fraction对象被转换为 4**



>事实上，对于表达式 4 + f，编译器将：
>
>1. 先查找是否存在运算符重载函数 operator+ 满足
>2. 再查找conversation function





# non-explicit-one-argument ctor

- argument：实参。

  > 形参：parameter



![image-20220707152157325](%E7%89%B9%E6%AE%8Afunction.assets/image-20220707152157325.png)

在该例中，**4 被转换为一个Fraction对象**



## ambiguous的情况

> ambiguous：歧义的



![image-20220707152948575](%E7%89%B9%E6%AE%8Afunction.assets/image-20220707152948575.png)



## explicit-one-argument ctor

> explicit：明确的



若构造函数被此关键字修饰，则**不允许隐式调用构造函数**。

![image-20220707153405384](%E7%89%B9%E6%AE%8Afunction.assets/image-20220707153405384.png)