# Implicit Conversion

- ==narrowing conversions:==

  some information is lost, and the compiler will warn us about this. Ex: convert double to int.



1. an implicit conversion of type int[] to type int\*.

   >In such cases, the array <u>loses its dimension</u>, and it is said it decays to a pointer.





# Explicit Conversion

1. `static_cast<>()`

   - This function performs ==a compile-time conversion==.

   ```cpp
   auto myinteger = static_cast<int>(123.456);
   ```

2. `dynamic_cast<>()`

   - The dynamic_cast function ==converts pointers of base class to pointers to derived class==(反之亦然).
   - If the conversion succeeds, the result is a pointer to a base or derived class, depending on our use-case. If the conversion cannot be done, the result is a pointer of value nullptr.

   ```cpp
   #include <iostream>
   class MyBaseClass {
       public:
       virtual ~MyBaseClass() {}
   };
   class MyDerivedClass : public MyBaseClass {};
   int main()
   {
       MyBaseClass* base = new MyDerivedClass;
       MyDerivedClass* derived = new MyDerivedClass;
       // base to derived
       if (dynamic_cast<MyDerivedClass*>(base))
       {
           std::cout << "OK.\n";
       }
       else
       {
           std::cout << "Not convertible.\n";
       }
       // derived to base
       if (dynamic_cast<MyBaseClass*>(derived))
       {
           std::cout << "OK.\n";
       }
       else
       {
           std::cout << "Not convertible.\n";
       }
       delete base;
       delete derived;
   }
   ```

3. `reinterpret_cast<>()`

   