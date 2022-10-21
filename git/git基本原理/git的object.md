# object对象的背后含义

- object的文件名：为文件内容的sha1结果。

  ```sh
  ❯ echo "blob 10\0hello git" | shasum
  8d0e41234f24b6da002d962a26c2495ea16a425f *-
  ```

- object的内容：为文件内容的压缩结果。

  ![image-20221019202653921](git%E7%9A%84object.assets/image-20221019202653921.png)

>而index文件，则保留了文件名和object名的映射关系。
>
>> 这意味着：
>>
>> - ==两个不同名，但内容相同的文件，可以映射到同一object==。
>>
>>   ```
>>   ❯ git ls-files -s
>>   100644 8d0e41234f24b6da002d962a26c2495ea16a425f 0       hello.txt
>>   100644 8d0e41234f24b6da002d962a26c2495ea16a425f 0       hello1.txt
>>   ```
>>
>> - 文件名和object名的映射中，object名会随着版本变动而变动。
>





# blob object

> 见add.md

- 产生于add时。







# commit object

```sh
❯ git commit -m '1st commit'
[master (root-commit) e832f16] 1st commit
 2 files changed, 4 insertions(+)
 create mode 100644 hello.txt
 create mode 100644 hello1.txt
 
❯ git cat-file -p e832f16
tree 440c32f693b2b1b4b152d492fcd2e38f0f7f6e4c
author gxy <2793392818@qq.com> 1666186343 +0800
committer gxy <2793392818@qq.com> 1666186343 +0800

1st commit
```

> 1. commit object，包含一个tree object
>
> 2. tree object，包含所有的blob object
>
> 3. 每一个blob object，都存储着文件的内容。
>
>    > index文件，包含着文件名与blob object的映射。



# tree object

```sh
❯ git cat-file -p 440c32
100644 blob 3e4c98642b9ccae080ad63f5760ea0d057aa7f18    hello.txt
100644 blob 8d0e41234f24b6da002d962a26c2495ea16a425f    hello1.txt
```



# tag object

通过`tag -a -m`创建



# parent object

指向上一次的commit对象。

> 本身并不存在，只是记录该commit的parent是谁。





# git gc

- 文件的每次add，都会生成一个blob对象，即便它们对应的原文件差异很小。
- 使用`git gc`可以进行压缩，objects对象将被压缩至`.git/objects/pack`文件夹。



> `git verify-pack -v xxx.idx`可以查看详细的压缩信息。

> 解压缩pack文件：
> `git unpack-objects < xxx.pack` 



# 垃圾对象

- 垃圾对象的产生：

  - 在一次commit前，对同一文件的多次add。会导致有的blob对象，并没有被commit对象引用。	

- `git prune`命令，用于将无用的对象修剪掉。

  > `git gc`只会压缩有用对象，垃圾对象将遗留未压缩。

![image-20221020114000572](git%E7%9A%84object.assets/image-20221020114000572.png)
