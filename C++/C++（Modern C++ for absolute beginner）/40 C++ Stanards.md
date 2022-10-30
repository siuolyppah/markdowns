

# C++11

## Thread

- create a thread:

  ```cpp
  #include <iostream>
  #include <string>
  #include <thread>
  
  void func(const std::string& msg){
      std::cout << msg << "\n";
  }
  
  int main(){
      std::thread t1 {func, "here is t1"};
      std::thread t2 {func, "here is t2"};
      
      t1.join();
      t2.join();
  }
  ```

  >==Once defined, the thread starts executing==.



## mutex

- raw use mutex:

  ```cpp
  #include <iostream>
  #include <string>
  #include <thread>
  #include <mutex>
  
  namespace my_test{
      std::mutex m_cout;
  }
  
  void func(const std::string& msg){
      my_test::m_cout.lock();
      std::cout << msg << "\n";
  	my_test::m_cout.unlock();
  }
  
  int main(){
      std::thread t1 {func, "here is t1"};
      std::thread t2 {func, "here is t2"};
      
      t1.join();
      t2.join();
  }
  ```

- ==`std::lock_guard` function:== prevent forgetting to unlock the mutex.

  It locks the mutex, and ==once it goes out of scope, it automatically unlocks the mutex==.

  ```cpp
  #include <iostream>
  #include <string>
  #include <thread>
  #include <mutex>
  
  namespace my_test
  {
  	std::mutex m_cout;
  }
  
  void func(const std::string& msg)
  {
  	using namespace  std;
  	for(int i =0;i<10000;++i)
  	{
  		std::lock_guard<std::mutex> lg(my_test::m_cout);
  		cout << msg << endl;
  	}	// lock_guard obj goes out of scope here and unlock the mutex.
  }
  
  int main()
  {
  	
  	std::thread t1{ func, "here is t1"};
  	std::thread t2{ func, "here is t222222"};
  
  	t1.join();
  	t2.join();
  }
  ```

  



# C++14

## Auto for Functions

We can deduce the function type based on the return statement value:

```cpp
auto myintfn() // integer
{
    return 123;
}

auto mydoublefn() // double
{
    return 3.14;
}
```



## Generic Lambdas

We can use ==auto <u>parameters</u> in lambda== functions now.

```cpp
#include <iostream>
int main()
{
    auto mylambda = [](auto p) {
        std::cout << "Lambda parameter: " << p <<'\n'; 
    };
    
    mylambda(123);
    mylambda(3.14);
}
```



# C++17

## Nested Namespaces的简写

```cpp
namespace MyNameSpace1
{
    namespace MyNameSpace2
    {
        namespace MyNameSpace3
        {
            // some code
        }
    }
}
```

The above example can now be rewritten as:

```cpp
namespace MyNameSpace1::MyNameSpace2::MyNameSpace3
{
    // some code
}
```



## Constexpr Lambdas

Lambdas can now be a constant expression, meaning they can ==be evaluated during <u>compile-time</u>==:

```cpp
int main()
{
    constexpr auto mylambda = [](int x, int y) { return x + y; };
    static_assert(mylambda(10, 20) == 30, "The lambda condition is not true.");
 }
```

等价形式：

```cpp
int main()
{
    auto mylambda = [](int x, int y) constexpr { return x + y; };
    static_assert(mylambda(10, 20) == 30, "The lambda condition is not true.");
}
```



## Structed Bingdings

Structured binding ==<u>binds the variable names</u> <u>to elements of compile-time known expressions</u>==, such as arrays or maps.

```cpp
auto [myvar1, myvar2, myvar3] = some_expression;
```



- bind to array:

  ```cpp
  int main()
  {
      int arr[] = { 1, 2, 3 };
      auto& [myvar1, myvar2, myvar3] = arr;
      
      myvar1 = 10;
      myvar2 = 20;
      myvar3 = 30;
      for (auto el : arr)
      {
          std::cout << el << ' ';
      }
  }
  ```

> more example: [Practical examples of structured bindings (C++17) (walletfox.com)](https://www.walletfox.com/course/structuredbindings_cpp17.php)



## std::filesystem

- The std::filesystem library allows us to work with files, paths, and folders on our system.
- The library is declared through a `<filesystem>` header.



example:

1. check if folder exist:

   ```cpp
   #include <iostream>
   #include <filesystem>
   int main()
   {
       std::filesystem::path folderpath = "C:\\MyFolder\\";
       if (std::filesystem::exists(folderpath))
       {
           std::cout << "The path: " << folderpath << " exists.";
       }
       else
       {
           std::cout << "The path: " << folderpath << " does not exist.";
       }
   }
   ```

2. iterate over folder elements:

   ```cpp
   #include <iostream>
   #include <filesystem>
   int main()
   {
       auto myfolder = "C:\\MyFolder\\";
       for (auto el : std::filesystem::directory_iterator(myfolder))
       {
           std::cout << el.path() << '\n';
       }
   }
   ```



## std::string_view

- The string_view is a non-owning view of a string or a substring. 

- ==It is a reference to something that is already there in the memory. It is implemented as a pointer to some character sequence plus the size of that sequence==.

- The std::string_view is declared inside the `<string_view>` header file.



```cpp
#include <iostream>
#include <string>
#include <string_view>
int main()
{
    std::string s = "Hello World.";
    std::string_view sw(s.c_str() , 5);
    std::cout << sw;
}
```



## std::any class type

The std::any container can hold a single value of any type.

```cpp
#include <any>
int main()
{
    std::any a = 345.678;
	a = true;
    a = 12.3;
}
```



- `std::any_cast` function:

  the std::any_cast will throw an exception if we try to convert.

  ```cpp
  #include <any>
  #include <iostream>
  
  int main()
  {
      std::any a = 345.678;
      try
      {
          auto ia = std::any_cast<int>(a);
      }
      catch (...)
      {
          std::cerr << "err" << std::endl;
      }
  }
  ```

- member function is .has_value() which checks if the std::any object holds a
  value.

  ```cpp
  #include <iostream>
  #include <any>
  int main()
  {
      std::any b{};
      if (b.has_value())
      {
          std::cout << "Object b contains a value." << '\n';
      }
      else
      {
          std::cout << "Object b does not contain a value." << '\n';
      }
  }
  ```

  

## std::variant

用于替代union.

```cpp
#include <iostream>
#include <variant>
int main()
{
	std::variant<char, int, double> myvariant{ 'a' }; // variant now holds a char
	std::cout << std::get<0>(myvariant) << '\n'; // obtain a data member by index
	std::cout << std::get<char>(myvariant) << '\n'; // obtain a data member by type
	myvariant = 1024; // variant now holds an int
	
	try
	{
		std::cout << std::get<0>(myvariant);
	}catch (const std::bad_variant_access& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
```

When we try to access the wrong data member of type double. An exception is raised. And the particular type of that exception is std::bad_variant_access.





# C++20





