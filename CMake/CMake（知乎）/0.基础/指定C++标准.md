- tutorial.cpp：

  ```C++
  // A simple program that computes the square root of a number
  #include <cmath>
  #include <iostream>
  #include <string>
  
  #include "TutorialConfig.h"
  
  // should we include the MathFunctions header?
  #ifdef USE_MYMATH
  #  include "MathFunctions.h"
  #endif
  
  int main(int argc, char* argv[])
  {
    if (argc < 2) {
      // report version
      std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
                << Tutorial_VERSION_MINOR << std::endl;
      std::cout << "Usage: " << argv[0] << " number" << std::endl;
      return 1;
    }
  
    // convert input to double
    const double inputValue = std::stod(argv[1]);
  
    // which square root function should we use?
  #ifdef USE_MYMATH
    const double outputValue = mysqrt(inputValue);
  #else
    const double outputValue = sqrt(inputValue);
  #endif
  
    std::cout << "The square root of " << inputValue << " is " << outputValue
              << std::endl;
    return 0;
  }
  ```

- TutorialConfig.h.in：

  ```C++
  // the configured options and settings for Tutorial
  #define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
  #define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
  ```

- CMakeLists.txt：

  ```cmake
  cmake_minimum_required(VERSION 3.10) 
  
  project(
      Tutorial
      VERSION 1.0
  )
  
  # 指定c++标准
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
  
  configure_file(TutorialConfig.h.in TutorialConfig.h)
  
  add_executable(Tutorial tutorial.cpp)
  
  # add the binary tree to the search path for include files
  # so that we will find TutorialConfig.h
  target_include_directories(
      Tutorial PUBLIC
      "${PROJECT_BINARY_DIR}"
  )
  ```

  

