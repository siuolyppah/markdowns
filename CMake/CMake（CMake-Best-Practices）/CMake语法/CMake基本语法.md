# 语法基于关键字和以空格分隔的参数

```cmake
target_sources(MyLibrary
	PUBLIC include/api.h
	PRIVATE src/internals.cpp src/foo.cpp)
```



# project()

```cmake
project(
	"chapter1"
	VERSION 1.0
	DESCRIPTION "A simple C++ project to demonstrate basic CMake usage" 	LANGUAGES CXX
)
```



```
.
├── CMakeLists.txt #defines project("CMakeBestPractices"...)
├── chapter_1
│ ├── CMakeLists.txt # defines project("Chapter 1"...)
```

- 正在解析的当前项目存储在PROJECT_NAME 变量中。
- 根项目存储在CMAKE_PROJECT_NAME 中

当解析根目录下的CMakeLists.txt 文件时，PROJECT_NAME 和CMAKE_PROJECT_NAME 都将是CMakeBestPractices。

当解析chapter_1/CMakeLists.txt 时，PROJECT_NAME 变量将变为“chapter_1”，但CMAKE_PROJECT_NAME 还是CMakeBestPractices，并设置在根文件夹中。





下面是关于项目的其他==内置变量==，可以在根项目的值前加上CMAKE_前缀。若没有在project() 指令中定义，则字符串为空:

- PROJECT_DESCRIPTION: 项目的描述字符串
- PROJECT_HOMEPAGE_URL: 项目的URL 字符串
- PROJECT_VERSION: 项目的完整版本信息
- PROJECT_VERSION_MAJOR: 版本字符串的第一个数字
- PROJECT_VERSION_MINOR: 版本字符串的第二个数字
- PROJECT_VERSION_PATCH: 版本字符串的第三个数字
- PROJECT_VERSION_TWEAK: 版本字符串的第四个数字





# 变量

- 一般的代码约定使用全大写命名变量。在内部，变量总是表示为字符串。



- 定义变量：

  ```cmake
  set(MYVAR "1234")
  ```

- 删除变量定义：

  ```cmake
  unset(MYVAR)
  ```

- 访问变量：

  ```cmake
  message(STATUS "The content of MYVAR are ${MYVAR}")
  ```

  

- ==**变量的引用可以嵌套**==，并由内而外求值:

  ```cmake
  ${outer_${inner_variable}_variable}
  ```


变量的作用域：

- 函数作用域: 在函数内部设置的变量只在函数内部可见。

- 目录作用域: 源树中的每个子目录绑定变量，并包括来自父目录的变量。

- 持久缓存: 缓存的变量可以是系统的，也可以是用户定义的。在多次运行中保持它们的值不变。

  > 将PARENT_SCOPE 选项传递给set() 会使变量在父作用域中可见。



# 列表

尽管CMake 在内部将变量存储为字符串，但可以在CMake 中使用分号分隔值来处理列表。



- 定义列表：

  ```cmake
  set(MYLIST abc def ghi)
  set(MYLIST "abc;def;ghi")
  ```

- 使用list 指令可以通过修改列表内容、重新排序或查找内容来操作列表:

  ```cmake
  list(FIND MYLIST abc ABC_INDEX)
  list(GET MYLIST ${ABC_INDEX} ABC)
  ```

  >在MYLIST 中查询abc 值的索引，然后检索该值并将其存储在名为ABC 的变量中。

- 追加：

  ```cmake
  list(APPEND MYLIST "xyz")
  ```



# Option()设置布尔变量

对于简单的布尔型缓存变量，CMake 还提供了option 指令，默认值为OFF。



# 属性

CMake 中的属性是附加到特定对象或CMake 范围的值，如文件、目标、目录或测试用例。



属性可以通过set_property 指令来设置。要读取属性的值，可以使用get_property。默认情况下，set_property 会覆盖已经存储在属性中的值，可以通过传递APPEND 或APPEND_STRING将值添加到当前值中。

```cmake
set_property(<Scope> <EntityName>
[APPEND] [APPEND_STRING]
PROPERTY <propertyName> [<values>])
```

作用域说明符可以有以下值:

![image-20221106135506590](CMake%E5%9F%BA%E6%9C%AC%E8%AF%AD%E6%B3%95.assets/image-20221106135506590.png)

![image-20221106135517440](CMake%E5%9F%BA%E6%9C%AC%E8%AF%AD%E6%B3%95.assets/image-20221106135517440.png)



当修改属性时，可以使用set_target_properties 和set test_properties，而非set_property。使用显式指令可以避免属性名之间的错误和混淆，可读性会更强。



# 循环和条件

条件块位于if()、elseif()、else()和endif() 语句之间。

条件使用各种关键字表示。

- 一元关键字：

  ```cmake
  if(DEFINED MY_VAR)
  ```

  ![image-20221106135814190](CMake%E5%9F%BA%E6%9C%AC%E8%AF%AD%E6%B3%95.assets/image-20221106135814190.png)

- 二元关键字：

  ```cmake
  if(MYVAR STREQUAL "FOO")
  ```

  ![image-20221106135844877](CMake%E5%9F%BA%E6%9C%AC%E8%AF%AD%E6%B3%95.assets/image-20221106135844877.png)

  ![image-20221106135852341](CMake%E5%9F%BA%E6%9C%AC%E8%AF%AD%E6%B3%95.assets/image-20221106135852341.png)

- 布尔运算符OR、AND 和NOT。



循环可以通过while() 和endwhile()，或foreach() 和endforeach() 实现。循环可以使用break() 终止，Continue() 会中止当前的迭代并立即开始下一个迭代。

```cmake
set(MYVAR 0)
while(MYVAR LESS "5")
    message(STATUS "Chapter1: MYVAR is '${MYVAR}'")
    math(EXPR MYVAR "${MYVAR}+1")
endwhile()
```

遍历列表或范围的循环:

```cmake
foreach(ITEM IN LISTS MYLIST)
# do something with ${ITEM}
endforeach()
```

for 循环可以使用RANGE 关键字:

```cmake
foreach(ITEM RANGE 0 10)
# do something with ${ITEM}
endforeach()
```



# 函数

函数由function()/endfunction() 定义。函数为变量创建了一个新的作用域，因此所有
在内部定义的变量都不能从外部访问，除非将PARENT_SCOPE 选项传递给set()。

函数不区分大小写。



```cmake
function(foo ARG1)
# do something
endfunction()
# invoke foo with parameter bar
foo("bar")
```



# 宏

CMake 宏使用macro()/endmacro() 定义，有点像函数。不同的是，函数参数是真变量，而
在宏中是字符串替换。这意味着必须使用大括号访问宏的所有参数。

另一个区别是，通过调用函数，作用区域转移到函数内部。执行宏时，就好像宏的主体粘到
调用位置一样，宏不会创建变量和控制流的作用域。因此，避免在宏中调用return()。



# 目标

CMake 的构建系统会组织一组逻辑目标，这些==逻辑目标对应于可执行文件、库或自定义命令或工件==，如文档或类似的文件。



CMake 中有==三种主要的方法来创建目标==——add_executable，add_library 和
add_custom_target。前两个用于创建可执行文件和静态或动态库，而第三个可以包含要
执行的定制命令。



目标间可以相互依赖，因此一个目标必须在另一个目标之前建立。



# 生成器表达式

生成器表达式是在构建的配置阶段进行的语句。

以$\<OPERATOR:VALUE> 的形式使用，其中OPERATOR 或直接使用，或与VALUE 进行比
较。这里，可以将生成器表达式看作小型的内联if 语句。