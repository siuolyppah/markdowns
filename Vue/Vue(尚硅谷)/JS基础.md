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

  

![image-20220411221715341](C:/Users/G_xy/AppData/Roaming/Typora/typora-user-images/image-20220411221715341.png)



