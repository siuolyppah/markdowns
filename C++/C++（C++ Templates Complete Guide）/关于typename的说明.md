> [c++ - Why is the keyword "typename" needed before qualified dependent names, and not before qualified independent names? - Stack Overflow](https://stackoverflow.com/questions/8584431/why-is-the-keyword-typename-needed-before-qualified-dependent-names-and-not-b)





- `T::A* x;`, this expression can be true for both cases where T::A is a type and T::A is a value. If A is a type, then it will result in pointer declaration; if A is a value, then it will result in multiplication. Thus a single template will have different meaning for 2 different types, which is not acceptable."

- 对于independent的上下文：

  ```cpp
  struct Foo
  {
      typedef int A;                   // type
      static double B;                 // value
      template <typename T> struct C;  // template
  };
  ```

  编译器能知道`FOO:A`到底是type, value, or template.

- 但对于dependent的上下文：

  编译器并不能知道`T::A`是什么。 if `T = Foo`, then `T::A = int`, which is a type, and all is well. But when `T = struct { static char A; };`, then `T::A` is a *value*, which doesn't make sense.

  ==因此，编译器需要上下文信息。==

  - ==在默认情况下，编译器假设该name为一个value。==
  - ==如果指定为typename，则假设为类型。==
  - ==如果指定为template，则假设为模板。==

  ```cpp
  template <typename T> struct Bar
  {
      typename T::A x;    // ah, good, decreed typename
  
      void foo()
      {
          int a = T::B;   // assumed value, OK
  
          T::template C<int> z;  // decreed template
          z.gobble(a * x);
      }
  };
  ```

  

