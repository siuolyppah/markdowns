# CSS选择题

## 元素选择器

语法：

```html
标签名{
	属性：值
}
```



 例如：

```html
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>CSS选择器</title>

    <style type="text/css">
        div {
            color: blue;
        }
    </style>
</head>

<body>

    <div>div标签1</div>
    <div>div标签2</div>
    <span>span标签1</span>
    <span>span标签2</span>

</body>
</html>
```



## id选择器

语法：

```html
#id属性值{
	属性:值
}
```



例如：

```html
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>CSS选择器</title>

    <style type="text/css">
        #div1{
            color: blue;
        }
    </style>
</head>

<body>

    <div id="div1">div标签1</div>
    <div id="div2">div标签2</div>

</body>
</html>
```



## class选择器(类选择器)

语法：
```html
.class属性值{
	属性：值
}
```



例如：

```html
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>CSS选择器</title>

    <style type="text/css">
        .class01{
            color: blue;
        }
    </style>

</head>

<body>

    <div class="class01">div标签1</div>
    <div >div标签2</div>

</body>
</html>
```



## 组合选择器

语法：

```html
选择器1,选择器2,选择器n{
	属性：值
}
```

