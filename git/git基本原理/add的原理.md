> 创建代码文件：
>
> ```sh
> echo "hello git" > hello.txt
> ```





# git add后的文件变化

> ==本质上，git add命令，更新了index，objects==。



从==工作区==添加到==缓存区==：

```sh
git add hello.txt
```

- 执行之后：

  ```sh
  ❯ tree -a .
  .
  ├── .git
  │   ├── config
  │   ├── description
  │   ├── HEAD
  │   ├── hooks
  │   │   ├── applypatch-msg.sample
  │   │   ├── commit-msg.sample
  │   │   ├── fsmonitor-watchman.sample
  │   │   ├── post-update.sample
  │   │   ├── pre-applypatch.sample
  │   │   ├── pre-commit.sample
  │   │   ├── pre-merge-commit.sample
  │   │   ├── prepare-commit-msg.sample
  │   │   ├── pre-push.sample
  │   │   ├── pre-rebase.sample
  │   │   ├── pre-receive.sample
  │   │   ├── push-to-checkout.sample
  │   │   └── update.sample
  │   ├── index
  │   ├── info
  │   │   └── exclude
  │   ├── objects
  │   │   ├── 8d
  │   │   │   └── 0e41234f24b6da002d962a26c2495ea16a425f
  │   │   ├── info
  │   │   └── pack
  │   └── refs
  │       ├── heads
  │       └── tags
  └── hello.txt
  ```

  > 多了两个目录：`./git/objects/8d`
  >
  > 多了两个文件：8d文件夹下的0e41....文件和index文件。



## git的object发生变化

查看objects文件：

```sh
git cat-file
```

> ==objects文件的文件名，包含了对象的文件类型、文件内容、文件长度信息==。
>
> 但不包含文件名信息。

- 查看对象类型：

  ```
  ❯ git cat-file -t 8d0e41
  blob
  ```

- 查看对象内容：

  ```
  ❯ git cat-file -p 8d0e41
  hello git
  ```

- 查看对象长度：

  ```
  ❯ git cat-file -s 8d0e41
  10
  ```

  

> blob类型，是用于存储文件内容的。

>```sh
>❯ echo "blob 10\0hello git" | shasum
>8d0e41234f24b6da002d962a26c2495ea16a425f *-
>```



- object文件的内容：

  ![image-20221019202646562](add%E7%9A%84%E5%8E%9F%E7%90%86.assets/image-20221019202646562.png)

  





## git的index发生变化

- 列出索引区文件的文件名：

  ```sh
  ❯ git ls-files
  hello.txt
  ```

- 同时列出文件权限、对应的blob对象（object文件名）、文件名：

  ```sh
  ❯ git ls-files -s
  100644 8d0e41234f24b6da002d962a26c2495ea16a425f 0       hello.txt
  ```

  

