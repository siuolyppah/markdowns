# 相关资料

- [视频教程](https://www.bilibili.com/video/BV1hU4y1M7NA)



# HelloWorld Demo

- helloweight.h

  ```C++
  #ifndef HELLOWIDGET_H
  #define HELLOWIDGET_H
  
  #include <QWidget>
  
  class HelloWidget : public QWidget
  {
      Q_OBJECT	// 引入QT信号和槽的宏
  
  public:
      HelloWidget(QWidget *parent = nullptr);
      ~HelloWidget();
  };
  #endif // HELLOWIDGET_H
  ```

- helloweight.cpp

  ```C++
  #include "hellowidget.h"
  
  HelloWidget::HelloWidget(QWidget *parent)
      : QWidget(parent)
  {
  }
  
  HelloWidget::~HelloWidget()
  {
  }
  ```

- main.cpp

  ```C++
  #include "hellowidget.h"
  
  #include <QApplication>
  
  int main(int argc, char *argv[])
  {
      QApplication a(argc, argv);
      HelloWidget w;
      w.show();
      return a.exec();
  }
  ```

  



- QWidget是所有窗口或控件的父类，典型子类：
  - QMainWindow
  - QDialog



# 父子关系

[QT教程【C++桌面应用开发】_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1hU4y1M7NA?p=8&vd_source=be746efb77e979ca275e4f65f2d8cda3)
