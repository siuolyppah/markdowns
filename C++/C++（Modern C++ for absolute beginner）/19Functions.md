# Function Declaration And Definition

Although ==*a function definition* is also *a declaration*==, you should provide both the declaration and a definition.



# Passing Arguments

1. Passing by Value(Copy)

2. Passing by ==Reference==:

   When passing by reference, we need to pass the variable itself; we can’t pass in a literal representing a value.

   ```cpp
   void func(int& i){}
   
   int main()
   {
   	// error: 非常量引用的初始值必须是左值
   	func(2);
   }
   ```

3. Passing by ==Const Reference==



