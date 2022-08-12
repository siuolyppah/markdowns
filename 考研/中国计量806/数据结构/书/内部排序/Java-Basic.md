# 实现 “比较” 语义的思路

有如下方法：

- 使用Object，基于继承体系表示泛型

  > JDK5之前

- 使用接口（实现`Comparable<T>`接口的compareTo()方法）

- 传递一个函数对象(即实现`Comparator<T>`接口的匿名内部类对象)

- Java内置泛型

  > JDK5





# JDK5泛型

>- 书：1.5.4节



## 数组的协变性

> - 数组(T[])==是协变的==。
> - 泛型、泛型集合(如ArrayList\<T>)==不是协变的==。



何谓协变性：

存在一类`Person`，及其子类`Employee`和`Student`。

则允许`Employee[]` is A `Person[]`。

```java
public class Person {

    public static void main(String[] args) {

        Person[] people = new Person[10];
        people[0] = new Student();

        people = new Employee[10];
        people[0] = new Student();      // 运行时异常：java.lang.ArrayStoreException: Student
    }
}

class Employee extends Person{}

class Student extends  Person{}
```

> Java中的数组，在运行时保持着该数组元素的类型。
>
> 由于==多态==、==数组的协变性==，虽然编译通过，但可能在运行时抛出异常。



## 通配符

> 正如前面所提到的，泛型集合是不支持协变的。例如：
>
> ```java
> public class Person {
> 
>     public static void foo(Person[] people) {}
> 
>     public static void foo(ArrayList<Person> people) {}
> 
>     public static void main(String[] args) {
>         foo(new Employee[10]);
>         foo(new ArrayList<Employee>()); // 编译错误
>     }
> }
> 
> class Employee extends Person {}
> 
> class Student extends Person {}
> ```
>
> 但事实上，这是一种合理的可能需求。因此引入通配符解决此不足。



```java
public class Test {

    public static void fooExtends(ArrayList<? extends B> arrayList) {}

    public static void fooSuper(ArrayList<? super B> arrayList) {}

    public static void main(String[] args) {
        fooExtends(new ArrayList<A>());     // 编译错误
        fooExtends(new ArrayList<B>());
        fooExtends(new ArrayList<C>());

        fooSuper(new ArrayList<A>());
        fooSuper(new ArrayList<B>());
        fooSuper(new ArrayList<C>());       // 编译错误
    }
}

class A {}
class B extends A {}
class C extends B {}
```



关于泛型方法声明的注意点：

```java
public class Test {

    /**
     * 说明：
     * A类型实现了Comparable<A>,因此 A is a Comparable<A>.
     * 由于B继承自A，因此B也实现了该接口，因此 B is a Comparable<A>.
     * 问题：显然 B is not a Comparable<B>
     */
    public static <T extends Comparable<T>> T findMax(T[] arr){
        int maxIndex = 0;

        for (int i = 1; i < arr.length; i++) {
            if(arr[i].compareTo(arr[maxIndex]) > 0){
                maxIndex = i;
            }
        }

        return arr[maxIndex];
    }

    public static void main(String[] args) {
        A a = findMax(new A[10]);
        a = findMax(new B[10]);
    }
}


class A implements Comparable<A>{

    @Override
    public int compareTo(A o) {
        return 0;
    }
}

class B extends A { }
```



## 类型擦除与带来的限制

>- 书：1.5.7节



- 在编译器检查阶段，编译器对类型检查。(`ArrayList<T>`)
- 当检查成功后，程序代码被编译为字节码指令，此时类型信息将丢失，即==类型擦除==。(`ArrayList`)

```java
public class Test {

    public static void main(String[] args) {
        ArrayList<Integer> integers = new ArrayList<>();
        ArrayList<String> strings = new ArrayList<>();

        boolean b = integers.getClass() == strings.getClass();
        System.out.println(b);  // true
    }
}
```



由于类型擦除，因此Java实现的是==假泛型==。

例如对于类型为`ArrayList<T>`的变量，JVM不会得到泛型的类型参数(T)，只能得到原始类型(ArrayList)。



Java的泛型实现机制，带来如下限制：

- ==instanceof检测，只能对原始类型进行==。

  ```java
  ArrayList<Integer> integers = new ArrayList<>();
  
  boolean b = integers instanceof ArrayList;
  b = integers instanceof ArrayList<Integer>; // 编译错误
  ```

- ==不能创建泛型类型的实例==。

  ```java
  public static <T> void foo(){
      T t = new T();  // 编译错误：类型形参 'T' 不能直接实例化
  }
  ```

  需要获取对应的`Class<T>`对象，通过反射创建实例：

  ```java
  public static <T> void foo(Class<T> tClass) throws InstantiationException, IllegalAccessException {
      T t = tClass.newInstance();
  }
  ```

- 不能创建泛型实例的数组。

  ```java
  public static <T> void foo(){
      T[] ts = new T[10]; // 编译错误：类型形参 'T' 不能直接实例化
  }
  ```

> 事实上，当类型擦除后，泛型方法中类型参数`T`，将被替换为Object或它的限界。
>
> 例如：
>
> ```java
> public static <T> void foo();	
> // 在该方法中，T的出现被替换为：Object
> 
> public static <? extends SomeObject> foo();
> // 在该方法中，T的出现被替换为：SomeObject
> 
> public static <? super SomeObject> foo();
> // 在该方法中，T的出现被替换为：Object
> ```





