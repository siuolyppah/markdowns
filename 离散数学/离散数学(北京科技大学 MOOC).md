> https://www.icourse163.org/learn/USTB-1206407811?tid=1466983455#/learn/announce

课程重点：

![Untitled(10)](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/Untitled(10)-16474063810361.png)



课程难点：

![Untitled(11)](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/Untitled(11).png)

# 数理逻辑

数理逻辑：用==数学方法==，来研究==推理规律==的数学学科

![image-20220314135000664](离散数学(北京科技大学 MOOC).assets/image-20220314135000664-16472370033551-16472370980482.png)



## 命题逻辑

逻辑主要研究==推理过程==，而推理过程必须依靠==命题==来表述。

在命题逻辑中，“<u>命题</u>”被看作是<u>最小单位</u>。

![image-20220314135330089](离散数学(北京科技大学 MOOC).assets/image-20220314135330089-16472372119131.png)

### 命题

推理是数理逻辑研究的中心问题，推理的前提和结论都是表达判断的陈述句。

因而表达判断的陈述句构成了推理的基本单位，称<u>**具有真假意义的陈述句**</u>为==命题==。



#### 真值

命题总是具有<u>一个确定的真或假的“值”</u>，称为真值。

真值只有两种情况：

- “真”：记为True，用1表示
- “假”：记为False，用0表示



>##### 真值为真的命题称为*真命题*；真值为假的命题，称为*假命题*



#### 判断给定的句子是否为命题

1. 首先应该是<u>陈述句</u>

2. 其次具有<u>唯一的真值</u>

   > 也许此真值的结果目前不知道，但这个真值必然有唯一的结果



例如：

1.	该吃早饭了！ 
  祈使句，不是命题

2.	我正在说谎
  虽然为陈述句，但无法判定其真值。==悖论不是命题==。

3.	x-y>2

   真值的结果不唯一，无法判断，不是命题

4.	不在同一直线上的三点，确定一个平面

   是陈述句，是正确的判定。是真命题

5.	郑州是河南省的省会

   是陈述句，是真命题

6.	下一个星期天会下雪。

   是命题。虽然==目前无法判定真值，但它是有唯一真值的==。

#### 命题的分类

命题可分为两种类型：

- 原子命题（简单命题）：不能再分解为简单的命题

- ==复合命题==：通过<u>联结词</u>、标点符号将<u>原子命题</u>组合而成。

  > eg：玫瑰是红的并且紫罗兰是蓝的

  

复合命题的基本性质：

​		其真值可以由其<u>==原子命题的真值==</u>，以及它们复合成该复合命题的<u>==联结方式==</u>确定。



#### 命题标识符

为了能用数学的方法来研究命题之间的逻辑关系和推理，需要将命题符号化。

命题标识符，即为==<u>表示命题的符号</u>==。

> 通常使用大写字母P,Q,R...，或者带下标的大写字母(P~1~)，或是数字([12])表示命题



命题标识符的分类：

- 命题常元：一个命题标识符，表示是==确定的==简单命题。

- 命题变元：是一个用于表示任意简单命题的==位置标志==。

  >因为命题变元可以表示任意简单命题，所以它不能确定真值。
  >**故命题变元不是命题**

  当命题变元P，用一个特定的简单命题取代时，P此时才能确定真值。此动作称为对P进行==<u>指派</u>==。



### 联结词

- 原子命题和联结词可以组成复合命题
- 联结词确定复合命题的逻辑形式
- 联结词来源于自然语言，但与自然语言中的联结词有一定的差别

> 从本质上讲，此处讨论的联结词只注重“真值”，而不顾及具体内容。
>
> 亦称“真值联结词”，类似于逻辑上的运算符



离散符号：

https://blog.csdn.net/weixin_55399173/article/details/120988532?utm_medium=distribute.pc_aggpage_search_result.none-task-blog-2~aggregatepage~first_rank_ecpm_v1~rank_v31_ecpm-1-120988532.pc_agg_new_rank&utm_term=markdown+%E7%A6%BB%E6%95%A3&spm=1000.2123.3001.4430



#### 否定联结词（$\neg$ 否）

$$
设P为任一命题，复合命题'非P'(或称P的否定)称为P的否定式，记作\neg P,读作'非P'\\
\neg称为否定联结词
$$



命题P的真值，与其否定$\neg p$的关系：

|  P   | $\neg P$ |
| :--: | :------: |
|  0   |    1     |
|  1   |    0     |

> $\neg P$为真，当且仅当$P$为假

在自然语言中，常用的表示否定的联结词：“非”，“不”，“没有”，“无”，“并不”



#### 合取联结词（$\wedge$ 同真才真）(wedge)

$$
设P,Q为任意二命题，复合命题'P并且Q'(或P与Q)称为P和Q的合取式，记作P\wedge Q，读作'P与Q'\\
\wedge称为合取联结词
$$



命题PQ，与其合取$P\wedge Q$的关系：

|  P   |  Q   | $P\wedge Q$ |
| :--: | :--: | :---------: |
|  0   |  0   |      0      |
|  0   |  1   |      0      |
|  1   |  0   |      0      |
|  1   |  1   |      1      |

> $P\wedge Q$为真，当且仅当P和Q同时成立

在自然语言中，表示合取的联结词：“并且”，“同时”，“以及”，“既···又···”，“不但···而且···”



#### 析取联结词（$\vee$ 同假才假）(vee)

$$
设P,Q为任意二命题，复合命题'P或Q'称为P和Q的析取式。记作P\vee Q，读作'P或Q'\\
\vee 称为析取联结词
$$

命题PQ，与其析取$P\vee Q$的关系：

|  P   |  Q   | $P \vee Q$ |
| :--: | :--: | :--------: |
|  0   |  0   |     0      |
|  0   |  1   |     1      |
|  1   |  0   |     1      |
|  1   |  1   |     1      |

> $P \vee Q$为真，当且仅当P与Q中至少一个为真





联结词$\vee$是<u>可兼或</u>；

而自然语言的或，既可以是<u>排斥或</u>，也可以是<u>可兼或</u>

> eg：明天要么下雨要么是晴天（排斥或）
>
> eg：小芳爱唱歌或跳舞（可兼或）



#### 蕴涵联结词（$\rightarrow$ 前真后假才为假）(rightarrow)

蕴涵联结词，也称为条件联结词。

>P$\rightarrow$Q的逻辑关系为：**Q是P的必要条件，P是Q的充分条件**。

$$
P,Q为任意二命题，复合命题'若P,则Q'称为P和Q的蕴涵式，记作P\rightarrow Q，读作"如果P则Q"。\\
\rightarrow称为蕴涵连接词。称P为前件，Q为后件
$$



命题PQ，与命题P$\rightarrow$Q的关系：

|  P   |  Q   | P$\rightarrow$Q |
| :--: | :--: | :-------------: |
|  0   |  0   |        1        |
|  0   |  1   |        1        |
|  1   |  0   |        0        |
|  1   |  1   |        1        |

> P$\rightarrow$Q为假，当且仅当P为真Q为假



对于给定的命题$P\rightarrow Q$:

- 逆换式：$Q\rightarrow P$，类似于逆命题

- 反换式：$\neg P\rightarrow \neg Q$，类似于否命题

- 逆反式：$\neg Q\rightarrow \neg P$，类似于逆否命题

  

#### 等价联结词（$\leftrightarrow$ 相同则真）(leftrightarrow)

等价联结词，也称为双条件联结词

> P$\rightarrow$Q的逻辑关系为：**P与Q互为充分必要条件**。

$$
设P,Q为任意二命题，复合命题“P当且仅当Q”称为命题P和Q的等价式，记作P\leftrightarrow Q，读作“P当且仅当Q”\\
\leftrightarrow称为等价联结词
$$



命题PQ，与命题P$\leftrightarrow$Q的关系：

|  P   |  Q   | P$\leftrightarrow$Q |
| :--: | :--: | :-----------------: |
|  0   |  0   |          1          |
|  0   |  1   |          0          |
|  1   |  0   |          0          |
|  1   |  1   |          1          |



命题P$\leftrightarrow$Q与之完全等价：($P\rightarrow Q$)$\wedge$($Q\rightarrow P$)

即“P当且仅当Q”，与“若P则Q，并且若Q则P”等价



#### 联结词的运算顺序

1. ()

2. $\neg$

3. $\wedge$, $\vee$, $\rightarrow$, $\leftrightarrow$  

   同一优先级的，从左往右运算

   

### 命题公式（赋值后才是命题）

#### 命题公式的定义

命题演算的合式公式，又称==命题公式（简称公式）==，是如下递归定义的：

1. ==<u>单个命题变元</u>==是合式公式，并简称为原子命题公式
2. 如果$A$是合式公式，那么($\neg A$)也是合式公式	
3. 如果$A,B$都是合式公式，那么($A\wedge B$)，($A\vee B$)，($A\rightarrow B$)，($A\leftrightarrow B$)都是合式公式
4. 当且仅当有限次地应用前三条，所得到的字符串是合式公式

> 条件1称为基础，2,3称为归纳，4称为界限



注意，==`命题公式本身不一定是命题`==，只有对公式中的每一个<u>命题变元，指派了真值</u>之后，它才是一个命题



#### n元命题公式

如果一个命题公式中，总共含有n种不同的命题变元，则称其为==n元命题公式==

> eg：$(P\wedge Q) \rightarrow \neg(Q \wedge R)$是三元命题公式



#### 命题公式的层次

- 若公式A是单个的命题变元，则称公式A为0层公式
- 称A为n+1(n$\geq$0)层公式，当前仅当：
  1. $A=\neg B$，$B$是n层公式
  2. $A=B\wedge C$，其中B,C分别为i层和j层公式，且n=max(i,j)
  3. $A= B\vee C$，其中B,C的层次同(2)
  4. $A=B\rightarrow C$，其中B,C的层次同(2)
  5. $A=B\leftrightarrow C$，其中B,C的层次同(2)

> eg：$(\neg P \wedge Q)\rightarrow R$是3层公式



#### 命题公式的赋值与真值表

设$P_1,P_2,\dots,P_n$是出现在公式A中的所有命题变元，给$P_1,P_2,\dots,P_n$各指派一个真值，称对A的一个==赋值或解释==

- 若指派的一组值，使得A的值为1，则称这组值为A的==成真赋值==

- 若指派的一组值，使得A的值为0，则称这组值为A的==成假赋值==

> eg：对于公式$P\vee Q\rightarrow R$ ，P,Q,R=1 0 0是成假赋值；P,Q,R=1 0 1是成真 赋值



对于含n个命题变元的命题公式，对其赋值的形式做如下规定：

- 设A中含的命题变元为$P_1,P_2,\dots,P_n$，赋值$a_1\ a_2\ \dots\ a_n$($a_i=0/1$)，是指$P_1=a_1,P_2=a_2,\dots P_n=a_n$
- 设A中含的命题变元为$P,Q,R,\dots$，赋值$a_1\ a_2\ \dots\ a_n$($a_i=0/1$)，是指$P=a_1,Q=a_2,\dots$，即==按字典序赋值==



设$P_1,P_2,\dots,P_n$是出现在公式A中的所有命题变元，将<u>公式A</u>在所有$2^n$个赋值下<u>的</u><u>取值情况</u>列为表，即A的==真值表==

构造真值表的步骤：

1. 枚举所有命题变元的可能性
2. 按从低到高的层次顺序，写出各个子公式的值



例如对于公式$(\neg P \wedge Q) \rightarrow \neg R$，其真值表如下：

| $P\ Q\ R$ | $\neg P$ | $\neg R$ | $\neg P \wedge Q$ | $(\neg P \wedge Q) \rightarrow \neg R$ |
| :-------: | :------: | :------: | :---------------: | :------------------------------------: |
|   0 0 0   |    1     |    1     |         0         |                   1                    |
|   0 0 1   |    1     |    0     |         0         |                   1                    |
|   0 1 0   |    1     |    1     |         1         |                   1                    |
|   0 1 1   |    1     |    0     |         1         |                   0                    |
|   1 0 0   |    0     |    1     |         0         |                   1                    |
|   1 0 1   |    0     |    0     |         0         |                   1                    |
|   1 1 0   |    0     |    1     |         0         |                   1                    |
|   1 1 1   |    0     |    0     |         0         |                   1                    |



设A为任一命题公式，特殊的：

- 若A在它的所有赋值情况下，取值均为真，则称A是==重言式或永真式==
- 若A在它的所有赋值情况下，取值均为假，则称A是==矛盾式或永假式==
  - 若A不是矛盾式，则称A是==可满足式==（重言式+非重言的可满足式）



#### 命题的符号化

将一个用文字描述的命题，转换成由命题标识符、联结词、括号表示的命题的过程，称为==命题的符号化(或称命题的翻译)==



命题符号化的基本步骤：

1. 找出命题中所包含的`原子命题`，并将其用命题标识符表示
2. 确定命题中的连词对应的联结词
3. 用正确的语法，将原命题表示称命题标识符、联结词、括号组成的命题公式



例如：对于命题“虽然你这次取得了第一名，但这并不代表你永远是第一名”，其符号化的过程：

​	设P：这一次你取得了第一名
​	Q:这代表你永远是第一名
​	原命题可表示为$P^\neg Q$		



####  小结

![image-20220314194233454](离散数学(北京科技大学 MOOC).assets/image-20220314194233454.png)



### 命题公式之间的关系

#### 逻辑等价（$\Leftrightarrow$）

$$
设A,B是两个命题公式，若对于出现在A和B中的所有变元的任一组赋值，公式A和公式B的真值都相同\\
则称公式A与B是逻辑等价(或称逻辑相等)，记作A\Leftrightarrow B
$$

> 注意区分“$\leftrightarrow$”和“$\Leftrightarrow$”：
>
> - “$\leftrightarrow$”是一种逻辑联结词，是一种`逻辑运算`，$A\leftrightarrow B$的结果仍然是一个命题公式
> - “$\Leftrightarrow$”则是描述了两个命题公式A与B之间的一种`逻辑关系`，表示A与B是逻辑等价的，$A\Leftrightarrow B$的结果不是命题公式



逻辑等价的命题公式A与B，构成的等价式$A\leftrightarrow B$，是重言式



#### 基本的逻辑等价公式

- 双重否定律：$A\Leftrightarrow \neg\neg A$
-  幂等律：$A\Leftrightarrow A\vee A$，$A\Leftrightarrow A\wedge A $

- 交换律：$A\vee B\Leftrightarrow B\vee A$，$A \wedge B\Leftrightarrow B \wedge A$
- `结合律`：$(A\vee B)\vee C\Leftrightarrow A\vee(B\vee C)$，$(A\wedge B)\wedge C\Leftrightarrow A\wedge(B\wedge C)$
- `分配律`：
  - $\vee $对$\wedge $的分配律：$A\vee (B\wedge C)\Leftrightarrow (A\vee B)\wedge(A\vee C)$
  - $\wedge $对$\vee $的分配律：$A\wedge (B\vee C)\Leftrightarrow (A\wedge B)\vee(A\wedge C)$

- `德摩根律`：
  - $\neg(A\vee B)\Leftrightarrow \neg A\wedge \neg B$
  - $\neg(A\wedge B)\Leftrightarrow \neg A\vee \neg B$
- `吸收律`：
  - $A\vee(A\wedge B)\Leftrightarrow A$
  - $A\wedge(A\vee B)\Leftrightarrow A$
- 零律：
  - $A\vee 1 \Leftrightarrow 1$
  - $A\wedge 0 \Leftrightarrow 0$

- 同一律：
  - $A \wedge 1 \Leftrightarrow A$
  - $A\vee 0 \Leftrightarrow A$

- 排中律：$A\vee\neg A\Leftrightarrow1$
- 矛盾律：$A\wedge\neg A\Leftrightarrow0$
- `蕴涵律`：$A\rightarrow B\Leftrightarrow\neg A\vee B$
- 等价律：$A\leftrightarrow B\Leftrightarrow(A\rightarrow B)\wedge(B\rightarrow A)$
- 假言易位律：$A\rightarrow B\Leftrightarrow \neg B\rightarrow\neg A$

- 等价否定律：$A\leftrightarrow B\Leftrightarrow \neg A\leftrightarrow\neg B$

- 归谬律：$(A\rightarrow B)\wedge(A\rightarrow\neg B)\Leftrightarrow\neg A$



关于吸收律的证明：
$$
P\wedge(P\vee Q)\Leftrightarrow(P\vee 0)\wedge(P\vee Q)\Leftrightarrow P\vee(0\wedge Q)\Leftrightarrow P\vee 0 \Leftrightarrow P
$$

#### 等价置换定理

如果X是命题公式A的连续一部分，且X是一个命题公式，则可称X为A的==子公式==

> eg：公式($\neg B\wedge C$)是公式($\neg A\wedge(\neg B \wedge C)$)的子公式

 

==置换规则==定理：
	设X是公式A的子公式，且$X\Leftrightarrow Y$，
	若B是将A中的一处或多处出现的X以Y替换得到的，
	则$A\Leftrightarrow B$

证明如下：
	欲证明$A\Leftrightarrow B$，即证$A\leftrightarrow B$是重言式
	对于包含在A和B中的一切命题变元的任意一个赋值，A与B差别仅在于X出现的某些地方替换为了Y，由于$X\Leftrightarrow Y$,
	那么对任意赋值X与Y的真值都相同，故A与B的取值也都相同，从而使得$A\leftrightarrow B$的真值为1。
	又由于赋值的任意性，故$A\leftrightarrow B$的真值恒为1，即$A\leftrightarrow B$是重言式，所以$A\Leftrightarrow B$



置换规则的应用：

-   利用逻辑等价演算的方法，验证公式间的逻辑等价
    $$
    求证(P\rightarrow Q)\rightarrow R \Leftrightarrow (P\vee R)\wedge(\neg Q\vee R)\\
    证明(从左侧开始)：\\
    (P\rightarrow Q)\rightarrow R\\
    \Leftrightarrow(\neg P \vee Q)\rightarrow R\ (蕴涵律)\\
    \Leftrightarrow \neg(\neg P \vee Q)\vee R\ (蕴涵律)\\
    \Leftrightarrow (P\wedge\neg Q)\vee R\ (德摩根律)\\
    \Leftrightarrow (P\vee R)\wedge(\neg Q \vee R)\ (分配律)
    $$

-   利用逻辑等价演算，判断命题公式的类型（重言式、矛盾式、非重言可满足式）
    能化简为与1逻辑等价的即为重言式；能化简为与0逻辑等价的即为矛盾式；否则即为非重言可满足式

-   实际问题分析

    ![image-20220314210537893](离散数学(北京科技大学 MOOC).assets/image-20220314210537893.png)

	<img src="离散数学(北京科技大学 MOOC).assets/image-20220314210813436.png" alt="image-20220314210813436" style="zoom: 50%;" />

	![image-20220314210937205](离散数学(北京科技大学 MOOC).assets/image-20220314210937205.png)
	

	![image-20220314211128447](离散数学(北京科技大学 MOOC).assets/image-20220314211128447.png)



#### 逻辑蕴涵（$\Rightarrow$）

设A、B是任意公式，若$A\rightarrow B$是重言式，则称==A蕴涵B，记作$A\Rightarrow B$==

>   注意区分$\rightarrow $和$\Rightarrow$：
>
>   -   $\rightarrow $是一种逻辑联接词。$A\rightarrow B$是命题公式，其中$\rightarrow$是一种`逻辑运算`，其运算结果仍为命题公式
>   -   而$\Rightarrow$则是描述了两个公式A与B之间的`关系`





==定理==：设A、B为任意两个命题公式，$A\Leftrightarrow B$的充分必要条件是：$A\Rightarrow B$且$B\Rightarrow A$

- 证明必要性（左边推出右边）：

    因为$A\Leftrightarrow B$，则$A\leftrightarrow B$是重言式，又因为$A\leftrightarrow B \Leftrightarrow(A\rightarrow B)\wedge(B\rightarrow A)$，所以$(A\rightarrow B)$$与(B\rightarrow A)$均为重言式，因此$A\Rightarrow B$且$B\Rightarrow A$

- 证明充分性：

    因为$A\Rightarrow B$且$B\Rightarrow A$，故$(A\rightarrow B)$$与(B\rightarrow A)$均为重言式，又因为$A\leftrightarrow B \Leftrightarrow(A\rightarrow B)\wedge(B\rightarrow A)$，故$A\leftrightarrow B$是重言式，因此$A\Leftrightarrow B$



#### 基本的逻辑蕴涵公式（称为推理定律）

- 附加律：$A\Rightarrow (A\vee B)$

- 化简律：$(A\wedge B)\Rightarrow A$

- 假言推理：$(A\rightarrow B)\wedge A\Rightarrow B$

- 拒取式：$(A\rightarrow B)\wedge\neg B\Rightarrow \neg A$

- 析取三段论：$(A\vee B)\wedge\neg B\Rightarrow A$

- 假言三段论：$(A\rightarrow B)\wedge(B\rightarrow C)\Rightarrow(A\rightarrow C)$

- 等价三段论：

    ![image-20220314213428920](离散数学(北京科技大学 MOOC).assets/image-20220314213428920.png)

- 构造性二难：

  ![image-20220314213446255](离散数学(北京科技大学 MOOC).assets/image-20220314213446255.png)

- 破坏性二难：

    ![image-20220314213512204](离散数学(北京科技大学 MOOC).assets/image-20220314213512204.png)

> 特别的，前面的24条逻辑等价公式，都可以分别拆分为两条逻辑蕴涵公式



逻辑蕴涵公式的应用：

1. 验证公式间的逻辑等价关系
2. 判断公式类型



#### 命题公式小结

![image-20220314213824029](离散数学(北京科技大学 MOOC).assets/image-20220314213824029.png)

对应的[复习梗概](https://www.icourse163.org/learn/USTB-1206407811?tid=1466983455#/learn/content?type=detail&id=1247261162&cid=1272931479)



### 对偶式

在给定的仅使用联结词$\neg,\vee,\wedge$的命题公式A中，若把$\vee$和$\wedge$互换，0和1互换，
而得到的命题公式A\*，则称==A\*是A的对偶式==。

> 对于含有$\neg,\wedge,\vee$之外联结词的命题公式，必须利用逻辑等价演算，将其他联结词消除后，才能求其对偶式

> 一般情况下，**公式与其对偶式不是逻辑等价的**

- A也是A\*的对偶式
- (A\*)\*=A



`定理`：设A\*和A互为对偶式，$P_1,P_2,\dots,P_n$是出现在A和A\*中的命题变元，则

1. $\neg A(P_1,P_2,\dots,P_n)\Leftrightarrow A*(\neg P_1,\neg P_2,\dots, \neg P_n)$
2. $A(\neg P_1,\neg P_2,\dots,\neg P_n)\Leftrightarrow A*(P_1,P_2,\dots,P_n)$



`定理`：设A和B是两个命题公式，若$A \Leftrightarrow B$，则$A*\Leftrightarrow B*$



### 范式

#### 析取范式与合取范式

`定义`：

- 命题变元，与命题变元的否定称为==文字==。

- 仅由有限个文字构成的析取式，称为==简单析取式==；(只由析取联结词，联结的文字)
    仅由有限个文字构成的合取式，称为==简单合取式==。(只由合区联结词，联结的文字)

    > 一个文字，既是简单析取式，又是简单合取式

	例如：
	​ $\neg P$：既是简单合取式，又是简单析取式
	​ $\neg\neg P$：都不是
	​ $P\wedge \neg Q$：简单合取式
	$\neg(P\wedge\neg Q)$：都不是

`定理`：

- 一个<u>简单析取式是重言式</u>，当且仅当它同时包含某个命题变元及它的否定式
- 一定<u>简单合取式是矛盾式</u> ，当且仅当它同时包含某个命题变元及它的否定式



`定义`：

- 由有限个简单合取式构成的析取式，称为==析取范式==，
- 由有限个简单析取式构成的合取式，称为==合取范式==。

- 析取范式与合取范式，统称为==范式==

`定理`：

- 一个析取范式是矛盾式，当且仅当它的每个简单合取式都是矛盾式
- 一个合取范式是重言式，当且仅当它的每个简单析取式都是重言式



#### 范式存在定理

`范式存在定理`：任一命题公式，都存在与之逻辑等价的<u>析取范式</u>与<u>合取范式</u>。

证明（也是求公式范式的步骤）：

1. 利用蕴含律和等价律，消去联结词$\rightarrow,\leftrightarrow$
    - 蕴含律：$A\rightarrow B\Leftrightarrow\neg A\vee B$
    - 等价律：$A\leftrightarrow B\Leftrightarrow(A\rightarrow B)\wedge(B\rightarrow A)$
2. 利用消去连续的否定号：
    - 双重否定律：$\neg\neg A\Leftrightarrow A$
3. 利用德摩根律内移否定号：
    - 德摩根率：$\neg(A\vee B)\Leftrightarrow\neg A\wedge\neg B$，$\neg(A\wedge B)\Leftrightarrow\neg A\vee\neg B$
4. 利用$\wedge$对$\vee$的分配律求析取范式；利用$\vee$对$\wedge$的分配律求合取范式：
    - $\wedge$对$\vee$的分配律：$A\wedge(B\vee C)\Leftrightarrow(A\wedge B)\vee(A\wedge C)$
    - $\vee$对$\wedge$的分配律：$A\vee(B\wedge C)\Leftrightarrow(A\vee B)\wedge(A\vee C)$

注记图如下：

![image-20220315145625564](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220315145625564.png)



![image-20220315145735722](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220315145735722.png)

![image-20220315145944605](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220315145944605.png)





### 主范式

#### 极小项与极大项

定义：

在含有n个命题变元的简单合取式（或简单析取式）中，若每个命题变元和它的否定式不同时出现，而二者之一必出现且仅出现一次；且第$i$个命题变元或它的否定式出现在从左算起的第$i$位上（若命题变元无脚标，就按x字典序排列），称这样的简单合取式（或简单析取式）为==极小项（或极大项）==

极小项性质：

1. 由于每个命题变元在极小项中以原形或否定形式出现且仅出现一次，因而n个命题变元共可产生$2^n$个不同的极小项。（即在n个位置上，每个位置要么是原形要么是否定形式）

2. $2^n$个极小项两两互不逻辑等价

3. 任意两个不同极小项的合取式永假

4. $2^n$个极小项的析取式永真

5. 每个极小项都==有且仅由一个成真赋值==；

    若成真赋值所对应的二进制数转换为十进制数$i$，就==将极小项用$m_i$进行表示==

极大项性质：

1. n个命题变元，共可产生$2^n$个极大项
2. $2^n$个极大项两两互不逻辑等价
3. 任意两个不同极大项的析取式永真
4. $2^n$个极大项的合取式永假
5. 每个极大项==有且仅有一个成假赋值==，将其对应的十进制数$i$做极大项脚标，记作==$M_i$==



定理：

设$m_i$和$M_i$是命题变元$P_1,P_2,\dots,P_n$形成的极小项和极大项，则$\neg m_i\Leftrightarrow M_i$，$\neg M_i\Leftrightarrow m_i$



以只有两个命题变元PQ组成的极小项和极大项为例：

|                      |  极小项  |       |                    |  极大项  |       |
| :------------------: | :------: | :---: | :----------------: | :------: | :---: |
|         公式         | 成真赋值 | 名称  |        公式        | 成假赋值 | 名称  |
| $\neg P\wedge\neg Q$ |   0 0    | $m_0$ |     $P\vee Q$      |   0 0    | $M_0$ |
|  $\neg P \wedge Q$   |   0 1    | $m_1$ |   $P\vee\neg Q$    |   0 1    | $M_1$ |
|   $P\wedge\neg Q$    |   1 0    | $m_2$ |  $\neg P \vee Q$   |   1 0    | $M_2$ |
|     $P\wedge Q$      |   1 1    | $m_3$ | $\neg P\vee\neg Q$ |   1 1    | $M_3$ |



#### 主析取范式和主合取范式

定义：

- 由有限个极小项构成的析取式，称为==主析取范式==
- 由有限个极大项构成的合取式，称为==主合取范式==

- 主合取范式和主析取范式，统称为主范式

> 主析取范式是由极小项构成的，极小项对应的是成真赋值；
>
> 主合取范式是由极大项构成的，极大项对应的是成假赋值





==范式定理==（为什么要定义主范式）：

任何命题公式，都存在着与之等价的主析取方式和主合取范式，并且是唯一的。

>[范式存在定理](#范式存在定理)仅说明了范式存在，但并不唯一。



求主范式的步骤如下：

1. 求出析取范式（合取范式）
2. 补充命题变元
3. 消去重复出现的极小(大)项和矛盾式(重言式)

注记图如下：

![image-20220315153302025](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220315153302025.png)





例如：

![image-20220315153456642](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220315153456642.png)

![image-20220315153623717](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220315153623717.png)

> 也可以利用真值表进行求解



主析取范式和主合取范式的关系：

设公式A的主析取范式含有s个极小项，则A有$s$个成真赋值，有$2^n-s$个成假赋值(n为命题变元个数)。

那么写出成假赋值对应的极大项，将它们合取起来就是A的主合取范式

#### 主范式的作用

1. 求公式的对应的成真赋值和成假赋值：

    若公式A中含有n个命题变元，A的主析取范式含s个极小项，则A仅有s个成真赋值，其余$2^n-s$个都是成假赋值

2. 判定公式的类型

    - 公式A为重言式，当且仅当A的主析取范式包含$2^n$个极小项（即A的主合取范式不包含任何极大项）。

        此时，记A的主合取范式为1

    - 公式A为矛盾式，当且仅当A的主析取范式不包含任何极小项（即A的合取范式包含$2^n$个极大项）。

        此时，记A的主析取范式为0

    - 公式A为可满足式，当且仅当A的主析取范式至少包含一个极小项

3. 判定两个命题公式是否逻辑等价



#### 范式与主范式小结

![image-20220315172506918](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220315172506918.png)



### 命题逻辑推理理论

https://www.icourse163.org/learn/USTB-1206407811?tid=1466983455#/learn/content?type=detail&id=1247261165&cid=1272931496&replay=true

> 本节核心概念：命题公式$A_1,A_2,\dots,A_k$推出B的推理是正确的，当且仅当$(A_1\wedge A_2\wedge \cdots\wedge A_k)\rightarrow B$是重言式



#### 有效推理

设$A_1,A_2,\dots,A_k$和B都为命题公式，若对于$A_1,A_2,\dots,A_k$和B中出现的命题变元的任意一组赋值，

当$A_1\wedge A_2\wedge\dots\wedge A_k$为假(或$A_1\wedge A_2\wedge\dots\wedge A_k$为真)，B也为真，

则称由前提$A_1,A_2,\dots,A_k$推出B的推理是有效的，并称B为==有效结论==。

>即：只要不是前提为1，结论为0的情况，都是推理正确的情况



特别的，可以将各个前提组成的集合记作为$\Gamma$，可将$\Gamma$推出B的推理记作$\Gamma\vdash B$。
若此推理是正确的，可记为$\Gamma\models B$；否则可记为$\Gamma\nvDash B$。

可称$\Gamma\vdash B$或$\{A_1,A_2,\dots,A_k\}\vdash B$为==推理的形式结构==。



即命题公式$A_1,A_2,\dots,A_k$推出B的推理是正确的，当且仅当$(A_1\wedge A_2\wedge \cdots\wedge A_k)\rightarrow B$是重言式





==判定推理是否有效的手段==：

1. 真值表
2. 逻辑等价演算
3. 主合取（析取）范式
4. 命题演算推证



##### 命题演算推证

命题演算退证，由三个要素组成：

- 推理根据：为命题演算推证的命题定律和推理定律；即基本逻辑等价公式和逻辑蕴含式

    - 命题定律用`E`表示
    - 推理定律用`I`表示

- 推理规则：

    - 前提引入规则（P规则）：在推证的任何步骤上，都可以引入前提

    - 结论引入规则（T规则）：在推证的任何步骤上所得到的结论，都可以作为后续证明的前提

    - 附加前提规则（CP规则）：若从A和B能有效地推出C，则从A可有效地推出B$\rightarrow$C

        > 此规则通常在结论为蕴含式时使用

- 证明方法：

    - 直接证明
    - 间接证明：附加前提法、归谬法



推证格式中包含：

1. 步骤号
2. 给定前提或得出的结论
3. 推理时所用的规则和定律类型



###### 推证格式的例子

![image-20220316132240277](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220316132240277.png)





###### 反证法

在命题演算推证中，为例证明某个结论是某些前提的有效结论，可以先假设结论的否定成立，然后推出此假设与前提相矛盾。此类间接证法又称为反证法。



定义：

假设公式$A_1,A_2,\cdots,A_m$中的全体命题变元为$P_1,P_2,\cdots,P_n$。若存在某些赋值，使得$A_1\wedge A_2\wedge \cdots\wedge A_m$的真值为1，那么称公式$A_1\wedge A_2\wedge \cdots\wedge A_m$是可==相容==的；若对于任何赋值，其真值都为0，则称公式$A_1\wedge A_2\wedge \cdots\wedge A_m$是==不相容==的。



定理：

设命题公式集合$\{A_1,A_2,\cdots,A_m\}$是相容的，那么从$\{A_1,A_2,\cdots,A_m\}$出发可逻辑地推出结论B的充分必要是：从$\{A_1,A_2,\cdots,A_m,\neg B\}$可逻辑地推出一个矛盾式。



例如：

![image-20220316132920336](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220316132920336.png)





###### 附加前提证法

![image-20220316133204486](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220316133204486.png)

![image-20220316133220335](%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220316133220335.png)





#### 命题逻辑推理小结

- 若推理形式结构的符号化表示：$(A_1 \wedge A_2 \wedge \cdots \wedge A_k)\rightarrow B$是重言式，则推理是有效的，并记为$(A_1 \wedge A_2 \wedge \cdots \wedge A_k)\Rightarrow B$
- 判断推理正确与否的方法有：真值表法、逻辑等价演算法、主析（合）取范式法、==命题演算推证==的证明法
- 命题演算推证的证明中内含的证明方法有：直接证明法、间接证明法；间接证明法又包含归谬证明法和附加前提证明法



<img src="%E7%A6%BB%E6%95%A3%E6%95%B0%E5%AD%A6(%E5%8C%97%E4%BA%AC%E7%A7%91%E6%8A%80%E5%A4%A7%E5%AD%A6%20MOOC).assets/image-20220316133417143.png" alt="image-20220316133417143" style="zoom:80%;" />



## [谓词逻辑]()









# 集合论



# 代数结构



# 图论