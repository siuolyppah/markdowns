# 动机 / 场景

- 在软件构建过程中，对于某一项任务，它常常有稳定的整体操作结构，但各个子步骤却有很多改动的需求；

  或者由于固有的原因（比如框架与应用之间的关系）而无法和任务的整体结构同时实现。



如何在确定稳定操作结构的前提下，来灵活应对各个子步骤的变化或晚期实现需求？



# Demo

## 重构之前

```C++
// 程序库开发人员
class Library {
public:
	void step1() {

	}

	void step3() {

	}

	void step5() {

	}
};

class Application {
public:
	bool step2() {
		
	}

	void step4() {

	}
};


int main() {
	Library lib;
	Application app;

	lib.step1();
    
	if (app.step2()) {
		lib.step3();
	}

	for (int i = 0; i < 4; i++) {
		app.step4();
	}
	
	lib.step5();
}
```

程序开发人员，使用到了已经开发的程序库，并以某种流程将它们串起来。

![image-20220710195225627](Template%20Method.assets/image-20220710195225627.png)





## 重构之后

```C++
#include <iostream>


void printStep(int i) {
	std::cout << i << std::endl;
}

// 程序库开发人员
class Library {
public:
    virtual ~Library(){}
    
	void step1() {
		printStep(1);
	}

	void step3() {
		printStep(3);
	}

	void step5() {
		printStep(5);
	}

	virtual bool step2() = 0; // 变化
	virtual void step4() = 0; // 变化

    // 稳定 template method
	void run() {
		this->step1();
		if (this->step2()) {	// 支持变化
			this->step3();
		}

		for (int i = 0; i < 4; i++) {
			this->step4(); // 支持变化
		}

		this->step5();
	}
    
};


class Application : public Library {
public:
	virtual bool step2() {
		// 子类通过重写，实现相关业务逻辑
	}

	virtual void step4() {
		// 子类通过重写，实现相关业务逻辑
	}
};


int main() {
	Library* lib = new Application();

	lib->run();

	delete lib;
}
```

![image-20220710195232712](Template%20Method.assets/image-20220710195232712.png)

 

## 早绑定 与 晚绑定

![image-20220710195601566](Template%20Method.assets/image-20220710195601566.png)

- 早绑定：开发晚的程序，调用开发时间较早的程序
- 晚绑定：开发晚的程序，被通过多态机制调用



# 模式定义

定义一个操作中的**算法的骨架 (稳定)**，而将一些步骤延迟(变化)到子类中。 Template Method使得子类可以不改变(即复用)一个算法的结构即可重定义(override 重写)该算法的某些特定步骤。  

> 在本案例中，若run()方法是不稳定的，则不合适Template Method。



# 结构

![image-20220710201708228](Template%20Method.assets/image-20220710201708228.png)



# 总结

- Template Method模式是一种非常基础性的设计模式，在面向对象系统中有着大量的应用。它用最简洁的机制（虚函数的多态性）为很多应用程序框架提供了灵活的扩展点，是代码复用方面的基本实现结构。  

- 除了可以灵活应对子步骤的变化外，“不要调用我，让我来调用
  你”的反向控制结构是Template Method的典型应用

- 在具体实现方面，被Template Method调用的虚方法可以具有实现，也可以没有任何实现（抽象方法、纯虚方法），但一般推荐将它们设置为protected方法。  

  > 例如单独的某个step()方法，它的意义是不大的，没必要将其设置为pulic。
