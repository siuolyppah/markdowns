# Demo

- tutorial.cpp：

  ```C++
  #include <stdio.h> 
  #include <stdlib.h> 
  #include <math.h> 
  int main(int argc, char* argv[]){ 
      if(argc<2){ 
          fprintf(stdout, "Uage: %s number\n", argv[0]); 
          return 1; 
      } 
      double inputValue = atof(argv[1]); 
      double outputValue = sqrt(inputValue); 
      fprintf(stdout, "The square root of %g is %g\n",inputValue, outputValue); 
      return 0; 
  } 
  ```

- CMakeLists.txt：

  ```cmake
  cmake_minimum_required(VERSION 3.10) 
  
  project(
      Tutorial
      VERSION 1.0
  )
  
  add_executable(Tutorial tutorial.cpp)
  ```
  



```sh
mkdir build
cd build
cmake ..
cmake --build . # 或 make
```

