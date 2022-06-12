# 箭头函数与普通函数

- 普通函数：`function(){}`
  - 普通函数，既可以是函数表达式，也可以是函数声明
  - 普通函数有内置arguments对象
- 箭头函数：`()=>{}`
  - 箭头函数是匿名的，只能是函数表达式
  - 普通函数没有内置arguments对象



> 回调函数默认指向window对象

this的指向问题：

- 普通函数：

  this的值是动态的，确定于执行时，指向调用者对象

  ```html
  <script>
      let normalFunction = function(){
          console.log(this);
          //此函数被normalButton调用，因此this指向normalButton
      };
  
      const normalButton = document.querySelector(".normalButton");
      normalButton.addEventListener("click",normalFunction,false);
  </script>
  ```

- 箭头函数：

  this的值确定于定义时，函数属于什么，this就指向什么

  ```html
  <script>
      let arrowFunction = ()->{
          console.log(this);  
      	//arrowFunction属于window对象，因此this指向window对象
      };
  
      const arrowlButton = document.querySelector(".arrowButton");
      arrowlButton.addEventListener("click",arrowFunction,false);
  </script>
  ```

  

# 原型

- 构造函数.prototype：称为显式原型

- 由构造函数生成的对象实例.\__proto__：称为隐式原型

  显式原型和隐式原型指向的是同一个对象



```javascript
let consturct = function(){
    this.a = 12;
    this.b = 13;
}

let obj = new consturct();

console.log(consturct.prototype === obj.__proto__);

consturct.prototype.c = 13;
console.log(obj.__proto__.c);
//先寻找obj对象的c属性，发现没有之后，顺着原型链去原型对象上寻找
console.log(obj.c);
```





