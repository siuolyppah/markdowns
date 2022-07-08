# Hello World Demo

```C++
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLabel* label = new QLabel("hello,world!!!!!!");
    label->show();

    return a.exec();
}	// 注意： label对象并未被delete，由操作系统在程序结束后进行回收。
```

说明：

- QApplication对象：

  用于管理<u>应用程序级别</u>的资源。

  > 一般用法固定：
  >
  > 以main方法接收的参数创建QApplication对象a，并将a.exec()作为main方法返回值

- QLabel对象：

  用于显示文本内容。

  > 在Qt中，Widget是所有组件的父类，在widget中可以放置其他的widget对象。
  >
  > > 一般Qt程序的顶级组件：
  > >
  > > - QMainWindow：
  > >
  > >   具有主菜单栏、工具栏和状态栏，类似于一般的应用程序的主窗口
  > >
  > > - QDialog：
  > >
  > >   对话框类，可建立一个基于对话框的界面



> 💡**在Qt中，组件创建出来之后通常是不可见的**，需要手动设置可见。
>
> ```C++
> label->show();
> ```