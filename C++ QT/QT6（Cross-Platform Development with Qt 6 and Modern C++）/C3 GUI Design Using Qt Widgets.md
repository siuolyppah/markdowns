# Main Topics

- Introducing Qt widgets
- Creating a UI with Qt Designer
- Managing layouts
- Creating custom widgets
- Creating Qt Style Sheets and custom themes
- Exploring custom styles
- Using widgets, windows, and dialogs



All the code used in this chapter can be downloaded from
the following GitHub link: https://github.com/PacktPublishing/Cross-Platform-Development-with-Qt-6-and-Modern-Cpp/tree/master/Chapter03.



# Introducing Qt widgets

## QObject和QWidget

- :star:All widgets inherit from ==`QObject`==.

- ==`QWidget `==is a basic widget and is the base class of all UI widgets. 

  It contains most of the properties required to describe a widget, along with properties such as geometry, color, mouse, keyboard behavior, tooltips, and so on.



## 继承体系及其优点

- `QWidget` inheritance hierarchy:

  ![image-20221103180126475](C3%20GUI%20Design%20Using%20Qt%20Widgets.assets/image-20221103180126475.png)

  

- Any object that inherits from `QObject` has a parent-child relationship. This relationship makes many things convenient for developers, such as the following:
  1. When a widget is destroyed, all its children are destroyed as well due to the parent-children hierarchy. This avoids memory leaks.
  2. You can find children of a given QWidget class by using findChild() and findChildren().
  3. Child widgets in a Qwidget automatically appear inside the parent widget.





# Enable Qt Widgets Moudle

```qmake
QT += widgets
```



# Layouts

- Qt provides a set of convenient layout management classes to automatically arrange child widgets within another widget to ensure that the UI remains usable.
- ==The` QLayout `class is the base class== of all layout managers. You can also create your own layout manager by reimplementing the `setGeometry()`, `sizeHint()`, `addItem()`,` itemAt()`, `takeAt()`, and `minimumSize()` functions.
- Once the layout manager is deleted, the layout management will also stop.



- The following list provides a brief description of the major layout classes:
  1. `QVBoxLayout` lines up widgets vertically.
  2. `QHBoxLayout `lines up widgets horizontally.
  3. `QGridLayout` lays widgets out in a grid.
  4. `QFormLayout` manages forms of input widgets and their associated labels.
  5. `QStackedLayout` provides a stack of widgets where only one widget is visible at a time.



# Creating custom widgets

- A custom widget may be a combination of one or more Qt widgets placed together or may be written from scratch.

  ![image-20221103200147922](C3%20GUI%20Design%20Using%20Qt%20Widgets.assets/image-20221103200147922.png)

>- https://doc.qt.io/qt-6/designer-creating-custom-widgets.html
>
>- 书的C3-Creating custom widgets节.



# Widgets,  Windows, Dialogs

- ==**<u>*A widget*</u>** is a GUI element that can be displayed on the screen==. 

  This could include labels, push buttons, list views, windows, dialogs, and so on. All widgets display certain information to a user on the screen, and most of them allow user interactions through the keyboard or mouse.

- ==<u>***A window***</u> is a top-level widget that doesn’t have another parent widget==.

  - Generally, windows have a title bar and border unless any window flags are specified. The window style and certain policies are determined by the underlying windowing system.

  - Some of the common window classes in Qt are `QMainWindow`, `QMessageBox`, and `QDialog`.

    - A main window usually follows a predefined layout for desktop applications that comprises a menu bar, a toolbar, a central widget area, and a status bar.

      ![QMainWindow layout](C3%20GUI%20Design%20Using%20Qt%20Widgets.assets/image-20221103204349287.png)

- ==**<u>*Dialogs*</u>** are temporary windows that are used to provide notifications or receive user inputs and usually have OK and Cancel-type buttons==.



>The takeaway is that all of these are widgets, Windows are the top-level widgets, and dialogs are a special kind of window.



