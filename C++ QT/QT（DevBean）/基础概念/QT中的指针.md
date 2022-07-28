[继续使用 QPointer - DevBean Tech World](https://www.devbean.net/2012/09/continue-using-qpointer/)

Qt 的智能指针类可以归纳如下：

| 分组                  | 类                                                           | 描述                                                         |
| :-------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| **共享数据**          | [`QSharedDataPointer`](http://qt-project.org/doc/qshareddatapointer.html), [`QExplicitlySharedDataPointer`](http://qt-project.org/doc/qexplicitlyshareddatapointer.html) | 隐式或者显式地共享*数据*（不共享*指针*）。也被称为“侵入式指针”。 |
| **共享指针**          | [`QSharedPointer`](http://qt-project.org/doc/qsharedpointer.html), [`QWeakPointer`](http://qt-project.org/doc/qweakpointer.html) | 线程安全的共享*指针*，类似 C++11 的`std::shared_ptr`，只不过具有良好的 Qt API。 |
| **范围指针**          | [`QScopedPointer`](http://qt-project.org/doc/qscopedpointer.html), [`QScopedPointerArray`](http://qt-project.org/doc/qscopedpointerarray.html) | 为了 [RAII](http://en.wikipedia.org/wiki/RAII) 目的：获取指针所有权，保证其能够在超出作用域的时候恰当地销毁。非共享的。 |
| **追踪`QObject`对象** | [`QPointer`](http://qt-project.org/doc/qpointer.html)        | 追踪给定的 QObject 对象生命。                                |