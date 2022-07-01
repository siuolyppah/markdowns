# 包

代码文件的开头一般为包的声明：

```kotlin
package com.runoob.main
...
```



> kotlin源文件不需要相匹配的目录和包，
>
> **源文件可以放在任何文件目录**。



>如果没有指定包，默认为 **default** 包。



## 默认导入

有多个包会默认导入到每个 Kotlin 文件中：

- kotlin.*
- kotlin.annotation.*
- kotlin.collections.*
- kotlin.comparisons.*
- kotlin.io.*
- kotlin.ranges.*
- kotlin.sequences.*
- kotlin.text.*



# 函数

## 定义

函数定义使用关键字 `fun`，参数格式为：`参数 : 类型`

```kotlin
fun sum(a: Int, b: Int): Int {   // Int 参数，返回值 Int
    return a + b
}
```



## 返回类型自动推断

表达式作为函数体，返回类型自动推断：

```kotlin
fun sum(a: Int, b: Int) = a + b

public fun sum(a: Int, b: Int): Int = a + b   // public 方法则必须明确写出返回类型
```



## Unit返回类型（Java中的void）

```kotlin
fun printSum(a: Int, b: Int): Unit { 
    print(a + b)
}


// 如果是返回 Unit类型，则可以省略(对于public方法也是这样)：
public fun printSum(a: Int, b: Int) { 
    print(a + b)
}
```



## 可变长参数函数

函数的变长参数可以用 **`vararg`** 关键字进行标识：

```kotlin
fun vars(vararg v:Int){
    for(vt in v){
        print(vt)
    }
}

// 测试
fun main(args: Array<String>) {
    vars(1,2,3,4,5)  // 输出12345
}
```



## lambda(匿名函数)

lambda表达式使用实例：

```kotlin
// 测试
fun main(args: Array<String>) {
    val sumLambda: (Int, Int) -> Int = {x,y -> x+y}
    println(sumLambda(1,2))  // 输出 3
}
```



# 变量

## 定义常量与变量

- 可变变量定义：`var `关键字

  ```kotlin
  var <标识符> : <类型> = <初始化值>
  ```

- 不可变变量定义：`val `关键字，只能赋值一次的变量(类似Java中final修饰的变量)

  ```kotlin
  val <标识符> : <类型> = <初始化值>
  ```

  

> 注意：
>
> - 常量与变量都可以没有初始化值,但是**在引用前必须初始化**
> - 编译器支持自动类型判断,即声明时可以不指定类型,由编译器判断。



```kotlin
val a: Int = 1
val b = 1       // 系统自动推断变量类型为Int
val c: Int      // 如果不在声明时初始化则必须提供变量类型
c = 1           // 明确赋值


var x = 5        // 系统自动推断变量类型为Int
x += 1           // 变量可修改
```



# 注释

Kotlin 支持单行和多行注释，实例如下：

```kotlin
// 这是一个单行注释

/* 这是一个多行的
   块注释。 */
```



>与 Java 不同, Kotlin 中的块注释允许嵌套。



# 字符串模板

- `$` 表示一个变量名或者变量值
- `$varName` 表示变量值
- `${varName.fun()}` 表示变量的方法返回值:

```kotlin
fun main() {
    var a: Int = 1
    val s1: String = "a is $a"
    println(s1)     // a is 1

    a = 2
    val s2 = "${s1.replace("is", "was")},but now is $a"
    println(s2)     // a was 1,but now is 2
}
```



# NULL检查机制

