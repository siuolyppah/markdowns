# Unscoped enums

```cpp
enum MyEnum{
    myfirstvalue,
    mysecondvalue,
    mythirdvalue
}
```

```cpp
int main()
{
    MyEnum myenum = myfirstvalue;
    myenum = mysecondvalue; // we can change the value of our enum object
}
```



- These unscoped enums have their enumerators ==leak into an outside scope==, the scope in which the enum type itself is defined.



# scoped enums

```cpp
enum class MyEnum
{
    myfirstvalue,
    mysecondvalue,
    mythirdvalue
};
```

```cpp
int main()
{
    MyEnum myenum = MyEnum::myfirstvalue;
}
```



- Scoped enums ==do not leak== their enumerators into an outer scope , and are ==not implicitly convertible== to other types.

- We can specify the underlying type for scoped enum.

  ```cpp
  enum class MyCharEnum : char {}
  ```

  











