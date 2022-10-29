# Which Header

- Smart pointers are declared in the `<memory>` header.



# Unique Pointer

- A unique pointer called `std::unique_ptr` is a pointer that owns an object it points to. The pointer can not be copied.

- 智能指针，支持多态。



- example:

  ```cpp
  #include <iostream>
  #include <memory>
  int main()
  {
      std::unique_ptr<int> p(new int{ 123 });
      std::cout << *p;
  }
  ```

  

- ==`std::make_unique<some_type>(some_value)` function==:

  ```cpp
  #include <iostream>
  #include <memory>
  int main()
  {
      std::unique_ptr<int> p = std::make_unique<int>(123);
      std::cout << *p;
  }
  ```

  

# Shared Pointer

- only when last of those pointers get destroyed, our pointed to object gets deleted.



- example:

  ```cpp
  #include <iostream>
  #include <memory>
  int main()
  {
      std::shared_ptr<int> p1 = std::make_shared<int>(123);
      std::shared_ptr<int> p2 = p1;
      std::shared_ptr<int> p3 = p1;
  }
  ```

  