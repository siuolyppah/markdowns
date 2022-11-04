# 在64位模式下

==***:star:在x64模式下编译***==。

- main.cpp:

  ```cpp
  #include <iostream>
  using namespace std;
  #ifdef __cplusplus
  extern "C" {
  #endif // __CPLUSPLUS
  
  int foo();
  
  #ifdef __cplusplus
  }
  #endif // __CPLUSPLUS
  
  int main() {
  
  	cout << "result:" << foo() << endl;
  
  	return 0;
  }
  ```

- asm.asm:

  ```asm
  .code
  
  foo proc
  	mov rax, 123
  
  	ret 
  foo endp
  
  end
  ```

  

>https://youtu.be/rxsBghsrvpI?list=PLKK11Ligqitg9MOX3-0tFT1Rmh3uJp7kA&t=786



# 在32位模式下

> 可以使用inline的汇编语言



```cpp
#include <iostream>
using namespace std;

int main() {
	int i = 0;
	_asm {
		mov i, 233345
	}

	cout << "result:" << i << endl;

	return 0;
}
```



# 只有汇编的情况

- main.asm:

  ```asm
  code
  main proc
      mov rax, 123
      ret
  main endp
  end
  ```

- visual studio的C++项目中必须包含一个CPP文件，因此随意添加一个空文件即可。

> x64模式下。











