# if语句

```cmake
if(variable)
    # If variable is `ON`, `YES`, `TRUE`, `Y`, or non zero number
else()
    # If variable is `0`, `OFF`, `NO`, `FALSE`, `N`, `IGNORE`, `NOTFOUND`, `""`, or ends in `-NOTFOUND`
endif()
# If variable does not expand to one of the above, CMake will expand it then try again
```

这里还有一些关键字可以设置，例如：

- 一元的: `NOT`, `TARGET`, `EXISTS` (文件), `DEFINED`, 等。
- 二元的: `STREQUAL`, `AND`, `OR`, `MATCHES` ( 正则表达式 ), `VERSION_LESS`, `VERSION_LESS_EQUAL` ( CMake 3.7+ ), 等。
- 括号可以用来分组



# 宏与函数

函数和宏只有作用域上存在区别，==宏没有作用域的限制==。

```cmake
function(SIMPLE REQUIRED_ARG)
    message(STATUS "Simple arguments: ${REQUIRED_ARG}, followed by ${ARGN}")
    set(${REQUIRED_ARG} "From SIMPLE" PARENT_SCOPE)
endfunction()

simple(This Foo Bar)
message("Output: ${This}")
```

```
-- Simple arguments: This, followed by Foo;Bar
Output: From SIMPLE
```

- 如果你想要有一个指定的参数，你应该在列表中明确的列出，初此之外的所有参数都会被存储在 `ARGN` 这个变量中（ `ARGV` 中存储了所有的变量，包括你明确列出的 ）。
- CMake的函数没有返回值。





