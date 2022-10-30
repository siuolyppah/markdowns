# File Streams

Those files streams are defined inside the `<fstream>` header, and they are:

1. `std::ifstream` – read from a file
2. `std::ofstream` – write to a file
3. `std::fstream` – read from and write to a file



```cpp
#include <iostream>
#include <fstream>
#include <string>
int main()
{
    std::fstream fs{ "myfile.txt" };
    std::string s;
    while (fs)
    {
        std::getline(fs, s); // read each line into a string
        std::cout << s << '\n';
    }
}
```



```CPP
#include <iostream>
#include <fstream>
int main()
{
    std::fstream fs{ "myfile.txt" };
    char c;
    while (fs >> c)		// read each character
    {
        std::cout << c;
    }
}
```

> 默认情况下，会跳过空白符。为了读取到空白符，需要`fs>>std::noskipws>>c`.



```cpp
#include <fstream>
int main()
{
    // filename, flag(或 std::ios::app)
    std::fstream fs{ "myoutputfile.txt", std::ios::out };
    fs << "First line of text." << '\n';
    fs << "Second line of text" << '\n';
    fs << "Third line of text" << '\n';
}
```



# String Streams

- `String Streams`allows us to read from and write to a string.
- defined inside the `<sstream>` header:
  1. `std::istringstream` - the stream to read from a string
  2. `std::otringstream` - the stream to write to a string
  3. `std::stringstream` - the stream to both read from and write to a
     string



```cpp
#include <iostream>
#include <sstream>
int main()
{
    char c = 'A';
    int x = 123;
    double d = 456.78;
    std::stringstream ss;
    ss << "The char is: " << c << ", int is: "<< x << " and double is: " << d;
    std::cout << ss.str();
}
```

```cpp
#include <iostream>
#include <sstream>
#include <string>
int main()
{
    std::string s = "A 123 456.78";
    std::stringstream ss{ s };
    char c;
    int x;
    double d;
    ss >> c >> x >> d;
    std::cout << c << ' ' << x << ' ' << d << ' ';
}
```

