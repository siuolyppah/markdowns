- ==The static specifier says the object will have a static storage duration==.

- If ==<u>a local variable</u> is marked as static==, the space for it is ==allocated== the first time ==the program control encounters its definition== and deallocated when the program exits.

  ```cpp
  void myfunction()
  {
      static int x = 0; // defined only the first time, skipped every other time
      x++;
      std::cout << x << '\n';
  }
  ```

- ==<u>Static class members</u> are not part of the object==. They live independently of an object of a class.

