main.cpp:
```C++
#include <iostream>

int add_numbers(int a, int b)
{
    return a + b;
}

int main()
{
    int a = 10;
    int b = 5;
    int c;

    std::cout << "Statement1" << std::endl;
    std::cout << "Statement2" << std::endl;
    c = add_numbers(a, b);
    std::cout << "Statement3" << std::endl;
    std::cout << "Statement3" << std::endl;

    return 0;
}
```

![image-20220730172547805](C++%E7%9A%84%E5%86%85%E5%AD%98%E6%A8%A1%E5%9E%8B.assets/image-20220730172547805.png)