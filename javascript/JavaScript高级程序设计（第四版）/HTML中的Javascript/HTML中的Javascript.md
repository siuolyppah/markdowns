# \<scirpt\>元素

将 JavaScript 插入 HTML 的主要方法是使用\<script\>元素。





## \<script>元素的属性

有下列 8 个属性：

- **async**：可选。表示应该立即开始下载脚本，但不能阻止其他页面动作，比如下载资源或等待其他脚本加载。只对外部脚本文件有效。
- **charset**：可选。使用 src 属性指定的代码字符集。这个属性很少使用，因为大多数浏览器不 在乎它的值。
- crossorigin：可选。配置相关请求的CORS（跨源资源共享）设置。默认不使用CORS。
    - crossorigin="anonymous"配置文件请求不必设置凭据标志。
    - crossorigin="use-credentials"设置凭据标志，意味着出站请求会包含凭据
- **defer**：可选。表示脚本可以延迟到文档完全被解析和显示之后再执行。只对外部脚本文件有效。在 IE7 及更早的版本中，对行内脚本也可以指定这个属性。
- integrity：可选。允许比对接收到的资源和指定的加密签名以验证子资源完整性（SRI， Subresource Integrity）。如果接收到的资源的签名与这个属性指定的签名不匹配，则页面会报错，脚本不会执行。这个属性可以用于确保内容分发网络（CDN，Content Delivery Network）不会提供恶意内容
- src：可选。表示包含要执行的代码的外部文件。
- language：**废弃**。最初用于表示代码块中的脚本语言（如"JavaScript"、"JavaScript 1.2" 或"VBScript"）。大多数浏览器都会忽略这个属性，不应该再使用它。
- type：可选。代替 language，表示代码块中脚本语言的内容类型（也称 MIME 类型）。按照惯例，这个值始终都是"text/javascript"，尽管"text/javascript"和"text/ecmascript" 都已经废弃了。JavaScript 文件的 MIME 类型通常是"application/x-javascript"，不过给 type 属性这个值有可能导致脚本被忽略。在非 IE 的浏览器中有效的其他值还有 "application/javascript"和"application/ecmascript"。如果这个值是 module，则代 码会被当成 ES6 模块，而且只有这时候代码中才能出现 import 和 export 关键字。



## 引入JS代码的两种方式

使用\<script>的方式有两种：

- 通过它直接在网页中==嵌入==JavaScript 代码：

    ```javascript
    <script> 
    		function sayHi() { 
    				console.log("Hi!"); 
    		} 
    </script>
    ```

    > 在使用行内 JavaScript 代码时，要注意代码中不能出现字符串"\</script>"
    >
    > ```js
    > <script> 
    > 		function sayScript() { 
    > 				<!--报错方式-->
    > 				console.log("</script>"); 
    > 
    > 				<!--ok方式-->
    > 				console.log("<\/script>");
    > 		} 
    > </script>
    > ```

- 通过使用 src 属性，在网页中==引入外部 JavaScript 文件==：

    ```js
    <script src="example.js"></script>
    ```

    > 如果使用了src属性，就不应该在标签内部再包含js代码。
    >
    > 如果两者都提供的话，则浏览器只会下载并执行脚本文件，从而**忽略行内代码**

    >src 属性可以是一个完整的 URL，而且这个 URL 指向的资源可以跟包含它的 HTML 页面不在同一个域中



按照惯例，外部 JavaScript 文件的扩展名是.js。

==但这不是必需的==，因为浏览器不会检查所包含 JavaScript 文件的扩展名。这就为使用服务器端脚本语言动态生成 JavaScript 代码，或者在浏览器中将 JavaScript扩展语言（如TypeScript，或React的 JSX）转译为JavaScript提供了可能性。

服务器经常会根据文件扩展来确定响应的正确 MIME 类型。因此即便不打算使用.js 扩展名，一定要确保服务器能返回正确的 MIME 类型。



在\<script>元素中的代码被计算完成之前，==页面的其余内容不会被加载，也不会被显示==。

对于没有 defer 和 async 属性的\<script>元素，浏览器会按照其在页面中出现的顺序依次解释它们



虽然可以直接在 HTML 文件中嵌入 JavaScript 代码，但通常认为==最佳实践是尽可能将 JavaScript 代 码放在外部文件中==。不过这个最佳实践并不是明确的强制性规则。推荐使用外部文件的理由如下。

- 可维护性。JavaScript 代码如果分散到很多 HTML 页面，会导致维护困难。而用一个目录保存所有 JavaScript 文件，则更容易维护，这样开发者就可以独立于使用它们的 HTML 页面来编辑代码
- 缓存。浏览器会根据特定的设置缓存所有外部链接的 JavaScript 文件，这意味着如果两个页面都用到同一个文件，则该文件只需下载一次。这最终意味着页面加载更快



## 标签位置

### 位于\<head>标签内的情况

过去，所有\<script>元素都被放在页面的<head>标签内，如下面的例子所示：

```html
<!DOCTYPE html>
<html>
<head>
    <title>Example HTML Page</title>
    <script src="example1.js"></script>
    <script src="example2.js"></script>
</head>
<body>
    <!-- 这里是页面内容 -->
</body>
</html>
```

这种做法的主要目的是==把外部的 CSS 和 JavaScript 文件都集中放到一起==。

这就意味着必须把所有 JavaScript 代码都下载、解析和解释完成后，才能开始渲染页面（页面在浏览器解析到\<body>的起始标签时开始渲染）。

对于需要很多 JavaScript 的页面，这会==导致页面渲染的明显延迟，在此期间浏览器窗口完全空白==



### 位于\<body>标签末的情况

```html
<!DOCTYPE html>
<html>
<head>
    <title>Example HTML Page</title>
</head>
<body>
    <!-- 这里是页面内容 -->
    <script src="example1.js"></script>
    <script src="example2.js"></script>
</body>
</html>
```

这样一来，页面会在处理 JavaScript 代码之前完全渲染页面。

用户会感觉页面加载更快了，因为浏览器显示空白页面的时间短了。



## 推迟执行脚本(defer属性)

HTML 4.01 为\<script>元素定义了一个叫 defer 的属性。

> HTML5规定 defer 属性只对外部脚本文件才有效。 因此支持 HTML5的浏览器会忽略行内脚本的 defer 属性



这个属性表示脚本在执行的时候不会改变页面的结构。

也就是说，脚本会被延迟到整个页面都解析完毕后再运行。

因此，在\<script>元素上设置 defer 属性，==相当于告诉浏览器立即下载，但延迟执行==。

```html
<!DOCTYPE html>
<html>
<head>
    <title>Example HTML Page</title>
    <script defer src="example1.js"></script>
    <script defer src="example2.js"></script>
</head>
<body>
    <!-- 这里是页面内容 -->
</body>
</html>
```

> 虽然这个例子中的\<script>元素包含在页面的\<head>中，但它们会在浏览器解析到结束的\</html>标签后才会执行。



HTML5 规范要求脚本应该按照它们出现的顺序执行，因此第一个推迟的脚本会在第二个推迟的脚本之前执行，而且两者都会在 DOMContentLoaded 事件之前执行。

>不过在实际当中，推迟执行的脚本不一定总会按顺序执行或者在 DOMContentLoaded事件之前执行，因此最好只包含一个这样的脚本。



## 异步执行脚本(async属性)

与 defer 不同的是，标记为 async 的脚本并不保证能按照它们出现的次序执行，比如：

```html
<!DOCTYPE html>
<html>
<head>
    <title>Example HTML Page</title>
    <script async src="example1.js"></script>
    <script async src="example2.js"></script>
</head>
<body>
    <!-- 这里是页面内容 -->
</body>
</html>
```

在这个例子中，第二个脚本可能先于第一个脚本执行。因此，==重点在于它们之间没有依赖关系==



给脚本添加 async 属性的目的是告诉浏览器：

- 不必等脚本下载和执行完后再加载页面，

- 同样==也不必等到该异步脚本下载和执行后再加载其他脚本==。

> 正因为如此，异步脚本不应该在加载期间修改 DOM。

异步脚本保证会在页面的 load 事件前执行，但可能会在 DOMContentLoaded(参见第 17 章)之前或之后。



# 动态加载脚本(使用DOM)

因为 JavaScript 可以使用 DOM API，所以通过==向 DOM 中动态添加 script 元素同样可以加载指定的脚本==。只要创建一个 script 元素并将其添加到DOM 即可。

```js
<!--当然，在把 HTMLElement 元素添加到 DOM 且执行到这段代码之前不会发送请求。-->
let script = document.createElement('script'); 
script.src = 'gibberish.js'; 
document.head.appendChild(script);
```

默认情况下，以这种方式创建的\<script>元素是以异步方式加载的，相当于添加了 async 属性。



所有浏览器都支持 createElement()方法，==但不是所有浏览器都支持 async 属性==。

因此，如果要统一动态脚本的加载行为，可以明确将其设置为同步加载：

```js
let script = document.createElement('script'); 
script.src = 'gibberish.js'; 
script.async = false; 
document.head.appendChild(script);
```



以这种方式(DOM API方式)获取的资源==对浏览器预加载器是不可见的==。这会严重影响它们在资源获取队列中的优先级。根据应用程序的工作方式以及怎么使用，这种方式==可能会严重影响性能==。

要想让预加载器知道这些动态请求文件的存在，可以在文档头部显式声明它们：

```js
<link rel="preload" href="gibberish.js">
```



# 文档模式

IE5.5 发明了文档模式的概念，即可以使用 doctype 切换文档模式。



最初的文档模式有两种：

- 混杂模式（quirks mode）：让 IE 像 IE5 一样（支持一些非标准的特性）

    混杂模式在所有浏览器中都==以省略文档开头的 doctype 声明作为开关==

- 标准模式（standards mode）：让 IE 具有兼容标准的行为

    标准模式通过下列几种文档类型声明开启：

    ```html
    <!-- HTML 4.01 Strict --> 
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" 
    "<http://www.w3.org/TR/html4/strict.dtd>"> 
    
    <!-- XHTML 1.0 Strict --> 
    <!DOCTYPE html PUBLIC 
    "-//W3C//DTD XHTML 1.0 Strict//EN" 
    "<http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd>"> 
    
    <!-- HTML5 --> 
    <!DOCTYPE html>
    ```

这两种模式的主要区别只体现在通过 CSS 渲染的内容方面，但对JavaScript 也有一些关联影响，或称为副作用。



IE 初次支持文档模式切换以后，其他浏览器也跟着实现了。随着浏览器的普遍实现，又出现了第三种文档模式：

- 准标准模式（almost standards mode）：这种模式下的浏览器支持很多标准的特性，但是没有标准规定得那么严格。==与前两者主要区别在于如何对待图片元素周围的空白（在表格中使用图片时最明显）==

    准标准模式通过过渡性文档类型（Transitional）和框架集文档类型（Frameset）来触发：

    ```html
    <!-- HTML 4.01 Transitional --> 
    <!DOCTYPE HTML PUBLIC 
    "-//W3C//DTD HTML 4.01 Transitional//EN" 
    "<http://www.w3.org/TR/html4/loose.dtd>"> 
    
    <!-- HTML 4.01 Frameset --> 
    <!DOCTYPE HTML PUBLIC 
    "-//W3C//DTD HTML 4.01 Frameset//EN" 
    "<http://www.w3.org/TR/html4/frameset.dtd>"> 
    
    <!-- XHTML 1.0 Transitional --> 
    <!DOCTYPE html PUBLIC 
    "-//W3C//DTD XHTML 1.0 Transitional//EN" 
    "<http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd>"> 
    
    <!-- XHTML 1.0 Frameset --> 
    <!DOCTYPE html PUBLIC 
    "-//W3C//DTD XHTML 1.0 Frameset//EN" 
    "<http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd>">
    ```

> 准标准模式与标准模式非常接近，很少需要区分。人们在说到“标准模式”时，可能指其中任何一个。
>
> 而对文档模式的检测（本书后面会讨论）也不会区分它们。



本书后面所说的标准模式，指的就是除混杂模式以外的模式。



# \<noscript>元素(不支持JS的降级方案)

针对早期浏览器不支持 JavaScript 的问题，需要一个页面优雅降级的处理方案。

\<noscript>元素，被==用于给不支持 JavaScript 的浏览器提供替代内容==

\<noscript>元素可以包含任何可以出现在\<body>中的 HTML 元素，\<script>除外



在下列两种情况下，浏览器将显示包含在<noscript>中的内容：

- 浏览器不支持脚本；
- 浏览器对脚本的支持被关闭。

任何一个条件被满足，包含在\<noscript>中的内容就会被渲染。否则，浏览器不会渲染\<noscript>中的内容。

```html
<!DOCTYPE html>
<html>
<head>
    <title>Example HTML Page</title>
    <script defer="defer" src="example1.js"></script>
    <script defer="defer" src="example2.js"></script>
</head>
<body>
    <noscript>
        <p>This page requires a JavaScript-enabled browser.</p>
    </noscript>
</body>
</html>
```



# 小结

JavaScript 是通过\<script>元素插入到 HTML 页面中的。这个元素可用于把 JavaScript 代码嵌入到 HTML 页面中，跟其他标记混合在一起，也可用于引入保存在外部文件中的 JavaScript。



本章重点：

- 要包含外部 JavaScript 文件，必须将 src 属性设置为要包含文件的 URL。文件可以跟网页在同 一台服务器上，也可以位于完全不同的域。
- ==所有\<script>元素会依照它们在网页中出现的次序被解释===。在不使用 defer 和 async 属性的情况下，包含在\<script>元素中的代码必须严格按次序解释
- ==对不推迟执行的脚本，浏览器必须解释完位于\<script>元素中的代码，然后才能继续渲染页面的剩余部分。为此，**通常应该把\<script>元素放到页面末尾**，介于主内容之后及\</body>标签之前==。
- 可以使用 defer 属性把脚本推迟到文档渲染完毕后再执行。推迟的脚本原则上按照它们被列出的次序执行。
- 可以使用 async 属性==表示脚本不需要等待其他脚本，同时也不阻塞文档渲染==，即异步加载。异步脚本不能保证按照它们在页面中出现的次序执行
- 通过使用\<noscript>元素，可以指定在浏览器不支持脚本时显示的内容。如果浏览器支持并启用脚本，则\<noscript>元素中的任何内容都不会被渲染。