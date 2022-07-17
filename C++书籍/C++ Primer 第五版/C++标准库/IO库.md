# IO类

## IO库类型与头文件

|  头文件  |             类型              |       作用       |
| :------: | :---------------------------: | :--------------: |
| iostream |       istream, wistream       |   从流读取数据   |
|          |       ostream, wostream       |   向流写入数据   |
|          |      iostream, wiostream      |      读写流      |
| fstream  |      ifstream, wifstream      |  从文件读取数据  |
|          |      ofstream, wofstream      |  向文件写入数据  |
|          |       fstream, wfstream       |     读写文件     |
| sstream  | istringstream, wistringstream | 从string读取数据 |
|          | ostringstream, wostringstream | 向string写入数据 |
|          |  stringstream, wstringstream  |    读写stream    |



## IO对象无拷贝或赋值

由于无能拷贝IO对象，因此不能将形参或返回值，设置为流类型。

```C++
ofstream out;
print(out);		// 错误：不能拷贝流对象
```

通常以引用方式传递流对象。

> 由于读写操作，会改变IO对象，因此不能将IO类型定义为const。



## 条件状态

IO库的条件状态：

|       状态        |                             说明                             |
| :---------------: | :----------------------------------------------------------: |
|  *strm*::iostate  | iostate是一种机器相关的**类型**，提供了表达条件状态的完整功能 |
|  *strm*::badbit   |                       用于指出流已崩溃                       |
|  *strm*::failbit  |                    用于指出一个IO操作失败                    |
|  *strm*::eofbit   |                   用于指出流到达了文件结束                   |
|  *strm*::goodbit  |         用于指出流未处于错误状态。<br />此值保证为0          |
|      s.eof()      |                 若流s的eofbit置位，返回true                  |
|     s.fail()      |            若流s的failtbit或badbit置位，返回true             |
|      s.bad()      |                 若流s的badbit置位，返回true                  |
|     s.good()      |                 若流s处于有效状态，返回true                  |
|     s.clear()     |     将流s中所有条件状态复位，将流的状态设置为有效。void      |
|  s.clear(flags)   | 根据给定的flags标志位，将流s中对应条件状态位复位。<br />flags是*strim*::iostate类型。void |
| s.setstate(flags) |    根据给定的flags标志位，将流s中对应条件状态位置位。void    |
|    s.rdstate()    |                    返回流s的当前条件状态                     |

>*strm*是某种IO类型。
>
>s为某个流对象





- badbit，表示系统级错误。如不可恢复的读写错误，**流无法再使用**。
- fallbit，通常表示发生可恢复的错误，如期望读入int却读入char。这种问题通常是可以修正的，**流可以继续使用**。

- 如果到达文件结束位置，eofbit和failtbit都将被置位。



- **如果badbit, failbit, eofbit任一个被置位，则检测流状态的条件会失败**。



## 管理输出缓冲

每个输入流，都管理一个缓冲区，用于保存程序读写的数据。

```C++
os << "enter a value:";
```

文本串可能被立即打印出来，也可能被操作系统保存在缓冲区，随后再打印。

有了缓冲机制，操作系统就可以**将程序的多个输出操作，组合成单一的系统级写操作**。



注意，**如果程序异常终止，输出缓冲区是不会被刷新的**。



### 导致缓冲刷新的情况

- 程序正常结束，作为main函数的return操作的一部分，缓冲刷新被执行。

- 缓冲区满

- 使用操作符显式刷新

  >- `endl`：换行，并刷新
  >- `flush`：刷新
  >- `ends`：插入空字符，刷新

- 在每个输出操作之后，可以用操作符`unitbuf`设置流的内部状态，来情况缓冲区。

  > 默认情况下，对cerr是设置unitbuf的，因此写到cerr的内容是立即刷新的。

- 一个输出流，可能被关联到另一个流。当读写被关联的流时，该流将被刷新。

  > 默认情况下，cin和cerr都关联到cout。
  >
  > 因此读cin或写cerr，都将导致cout的缓冲区刷新。



unitbuf操作符，将告诉流在接下来的每次写操作时都进行一次flush操作。而nounitbuf操作符则重置流，让流恢复使用正常的系统关联的缓冲区刷新操作。

```C++
cout << unitbuf;	// 之后的所有输出操作，都将立即刷新
cout << nounitbuf;	// 回到正常的缓冲方式。
```



### 关联输入和输出流

- 当一个输入流，被关联到一个输出流时：

  任何试图从输入读取数据的操作，都先会刷新关联的输出流。

  > 标准库将cout和cin关联在一起，因此下面语句将导致cout的缓冲区被刷新。
  >
  > ```C++
  > cin >> i;
  > ```

  > 交互式系统，通过应该关联输入流和输出流。
  >
  > 这意味着，所有输出，都会在读操作之前被打印出来。

- 一个输出流，被关联到另一个输出流

  



tie()的两个重载版本：

- `tie()`：

  返回指向输出流的指针。

  - 如果本对象关联到一个输出流，则返回指向该流的指针；
  - 否则返回空指针。

- `tie(basic_ostream*)`：

  将自己关联到该ostream，返回上一个被关联输出流的指针。

  例如`x.tie(&o)`，将把流x关联到输出流o

> 每个流，只能关联到一个输出流。
>
> 但一个输出流，可以被多个流关联。



# 文件输入输出

## fstream特有的操作

![image-20220717135503173](IO%E5%BA%93.assets/image-20220717135503173.png)



## 创建文件流对象

当希望读写一个文件时，可以**定义一个文件流对象，令其与文件进行关联**。



每个文件流类，都定义了一个名为open的成员函数，它完成一些系统相关的操作，来定义给定的文件，并视情况打开为 读或写 模式。



创建文件流对象时，可以提供文件名（可选）。如果提供了文件名，则open会自动被调用：

```C++
ifstream in(ifile);	 // 构造一个ifstream并打开给定文件
ofstream out;		// 构造一个ofstream，但未关联到任何文件。
```



## open 和 close

- 调用open可能会失败，此时failbit会被置位。

- 调用close，关闭已经被关联的文件。

  > 事实上，对同一个文件流对象，连续调用两次open，将导致failbit。

  > ### 💡 当一个fstream对象被销毁时，close会自动被调用。



## 文件模式

每个流都有一个关联的文件模式，用于指示如何使用文件：

|  模式  |              说明              |
| :----: | :----------------------------: |
|   in   |          以读方式打开          |
|  out   |          以写方式打开          |
|  app   | 每次写操作前，均定位到文件末尾 |
|  ate   | 打开文件后，立即定位到文件末尾 |
| trunc  |            截断文件            |
| binary |         以二进制进行IO         |



指定文件模式时，存在如下限制：

- 只有ofstream或fstream对象，可以设置out模式

- 只有ifstream或fstream对象，可以设置in模式

- 只有当out被设定时，才可以设置trunc模式

- 只要trunc没被设定，就可以设定app模式。

  app模式，同时也是默认out模式的。

- 以out模式打开的文件，默认是trunc的。

  为了保留以out模式打开的文件内容，可以：

  - 显式指定app模式
  - 或同时指定in模式

- ate和binary模式，可用于任何类型的文件流对象，且可以与其他任何文件模式组合使用。



每个文件流类型，都定义了**默认的文件模式**：

- 与ifstream关联的文件，默认以in模式打开
- ofstream，默认out打开
- fstream，默认in和out打开



演示out模式，默认是trunc的：

```C++
// 在这几条语句中，file1都会被截断
ofstream out("file1");
ofstream out2("file1", ofstream::out);
ofstream out3("file1", ofstream::out | ofstream::trunc);

// file2将被保留
ofstream app("file2", ofstream::app);
ofstream app2("file2", ofstream::out | ofstream::app);
```



# string流

`<sstream>`定理了三个类型，来支持内存IO，这些类型可以向string写入数据，从string读取数据，就像string是一个IO流一样。



## stringstream特有的操作

![image-20220717141639231](IO%E5%BA%93.assets/image-20220717141639231.png)



## 使用istringstream

案例：输入文件如下所示，每一行都是一个人名，和多个联系方式。

``` 
morgan 123123 123123
drew 333333
lee 123456 123456 123456
```



```C++
struct PersonInfo {
    string name;
    vector<string> phones;
};

int main() {

    string line, word;
    vector<PersonInfo> people;

    while (getline(cin, line)) {
        PersonInfo info;
        istringstream record(line);

        record >> info.name;
        while (record >> word) {
            info.phones.push_back(word);
        }

        people.push_back(info);
    }
}
```



## 使用ostringstream

先将内容，保存在该ostringstream对象中，再调用其str()方法，获取结果字符串。