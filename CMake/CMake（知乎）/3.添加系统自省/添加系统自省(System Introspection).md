考虑添加一些依赖于平台的代码到我们的项目中。例如，我们将添加一些代码，他取决于目标平台是否有`log`和`exp`函数。当然，几乎所有平台都有这些函数，但在这个教程中假定它们不常见。



如果目标平台有log和exp函数，那么我们将使用它们在mysqrt函数中计算平方根。

首先在MathFunctions目录下的CMakeLists.txt文件中使用CheckSymbolExists 模块来测试这些函数是否可用。在某些平台（例如Linux）需要链接库m。如果第一次没有找到log和exp，我们需要链接库m并重试。




# 方案一

使用`TutorialConfig.h`更好的地方来保存`HAVE_LOG`和`HAVE_EXP`的值



其余文件同之前。

- MathFunctions/CMakeLists.txt:

  ```cmake
  add_library(MathFunctions mysqrt.cpp)
  
  # state that anybody linking to us needs to include the current source dir
  # to find MathFunctions.h, while we don't.
  target_include_directories(MathFunctions
      INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
      PRIVATE ${CMAKE_BINARY_DIR}
  )
  
  # install rules
  install(TARGETS MathFunctions DESTINATION lib)
  install(FILES MathFunctions.h DESTINATION include)
  
  include(CheckSymbolExists)
  check_symbol_exists(log "math.h" HAVE_LOG)
  check_symbol_exists(exp "math.h" HAVE_EXP)
  if(NOT (HAVE_LOG AND HAVE_EXP))
    unset(HAVE_LOG CACHE)
    unset(HAVE_EXP CACHE)
    set(CMAKE_REQUIRED_LIBRARIES "m")
    check_symbol_exists(log "math.h" HAVE_LOG)
    check_symbol_exists(exp "math.h" HAVE_EXP)
    if(HAVE_LOG AND HAVE_EXP)
      target_link_libraries(MathFunctions PRIVATE m)
    endif()
  endif()
  ```

- mysqrt.cpp：
  ```C++
  #include <iostream>
  #include <cmath>
  #include "TutorialConfig.h"
  
  // a hack square root calculation using simple operations
  double mysqrt(double x)
  {
      if (x <= 0)
      {
          return 0;
      }
  
  #if defined(HAVE_LOG) && defined(HAVE_EXP)
      double result = exp(log(x) * 0.5);
      std::cout << "Computing sqrt of " << x << " to be " << result
                << " using log and exp" << std::endl;
  #else
      double result = x;
  
      // do ten iterations
      for (int i = 0; i < 10; ++i)
      {
          if (result <= 0)
          {
              result = 0.1;
          }
          double delta = x - (result * result);
          result = result + 0.5 * delta / result;
          std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
      }
  #endif
  
      return result;
  }
  ```



# 方案二

使用`target_compile_defintions`



```cmake
if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()
```

`mysqrt.cxx`或者`MathFunctions/CMakeLists.txt`中不需要再包含`TutorialConfig.h`。