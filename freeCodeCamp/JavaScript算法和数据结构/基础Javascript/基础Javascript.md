# 相等与全等

- 相等：==，会进行强制类型转换
- 全等：===，不会类型转换，如果类型不匹配，则直接为false



# 对象

## 创建对象

形如：

```js
const cat = {
  "name": "Whiskers",
  "legs": 4,
  "tails": 1,
  "enemies": ["Water", "Dogs"]
};
```



## 通过点号表示法访问对象属性

如果我们==提前知道要访问的属性名==，使用点号表示法是最方便的。



```js
const myObj = {
  prop1: "val1",
  prop2: "val2"
};

const prop1val = myObj.prop1;
const prop2val = myObj.prop2;
```



## 使用方括号表示法访问对象属性

如果你想访问的==属性名中包含空格，就必须使用方括号==表示法来获取它的属性值。

当然，如果属性名不包含空格，也可以使用方括号表示法。



```js
const myObj = {
  "Space Name": "Kirk",
  "More Space": "Spock",
  "NoSpace": "USS Enterprise"
};

myObj["Space Name"];
myObj['More Space'];
myObj["NoSpace"];
```



特别的，[]中可以使用对象：

```js
const dogs = {
  Fido: "Mutt",
  Hunter: "Doberman",
  Snoopie: "Beagle"
};

const myDog = "Hunter";
const myBreed = dogs[myDog];
console.log(myBreed);
```



## 更新对象属性

即通过点号表示法或方括号表示法，重写属性值

```js
const ourDog = {
  "name": "Camper",
  "legs": 4,
  "tails": 1,
  "friends": ["everything!"]
};

ourDog.name = "Happy Camper";
```



## 给 JavaScript 对象添加新属性

即通过点号表示法或方括号表示法，添加属性值

```js
const ourDog = {
  "name": "Camper",
  "legs": 4,
  "tails": 1,
  "friends": ["everything!"]
};

ourDog.bark = "bow-wow";
```



## 删除对象的属性

 使用点操作符或者中括号操作符，并结合`delete`关键字：

```js
const ourDog = {
  "name": "Camper",
  "legs": 4,
  "tails": 1,
  "friends": ["everything!"],
  "bark": "bow-wow"
};

delete ourDog.bark;
```



# 测试对象的属性

有时检查一个对象属性是否存在是非常有用的。 我们可以用对象的 `.hasOwnProperty(propname)` 方法来检查对象是否有指定的属性。 

```js
const myObj = {
  top: "hat",
  bottom: "pants"
};

myObj.hasOwnProperty("top");
myObj.hasOwnProperty("middle");
```



# var与let

https://chinese.freecodecamp.org/learn/javascript-algorithms-and-data-structures/es6/compare-scopes-of-the-var-and-let-keywords