# 指数性质

$$
\begin{gathered}
X^{A} X^{B}=X^{A+B} \\
\frac{X^{A}}{X^{B}}=X^{A-B} \\
\left(X^{A}\right)^{B}=X^{A B} \\
X^{N}+X^{N}=2 X^{N} \neq X^{2 N} \\
2^{N}+2^{N}=2^{N+1}
\end{gathered}
$$



# 对数性质

> 除非特别声明，否则底数默认为2。


$$
X^{A}=B \text { 当且仅当 } \log _{X} B=A
$$

$$
\log _{A} B=\frac{\log _{C} B}{\log _{C} A} ; A, B, C>0, A \neq 1
$$

> 令 $X=\log _{C} B, Y=\log _{C} A$, 以及 $Z=\log _{A} B$ 。此时由对数的定义, $C^{X}=B, C^{Y}=A$ 以及 $A^{Z}=B$, 联 合这三个等式则产生 $\left(C^{Y}\right)^{Z}=C^{X}=B$ 。因此, $X=Y Z$, 这意味着 $Z=X / Y$, 定理得证。


$$
\log A B=\log A+\log B ; A, B>0
$$

> 令 $X=\log B, Y=\log A$, 以及 $Z=\log A B$ 。此时由于假设默认的底为 $2,2^{X}=A, 2^{Y}=B$, 及 $2^{Z}=$ $A B$, 联合最后的三个等式则有 $2^{X} 2^{Y}=2^{Z}=A B$ 。因此 $X+Y=Z$, 这就证明了该定理。


$$
\begin{gathered}
\log A / B=\log A-\log B \\
\log \left(A^{B}\right)=B \log A \\
\log X<X 对所有的 X>0 成立 \\
\log 1=0, \log 2=1, \log 1024=10, \log 1048576=20
\end{gathered}
$$


# 级数

几何级数：
$$
\sum_{i=0}^{N} 2^{i}=2^{N+1}-1
$$

$$
\sum_{i=0}^{N} A^{i}=\frac{A^{N+1}-1}{A-1}
$$

$$
如果 0<A<1，则有：\\
\sum_{i=0}^{N} A^{i} \leqslant \frac{1}{1-A}
$$

算术级数：
$$
\sum_{i=1}^{N} i=\frac{N(N+1)}{2} \approx \frac{N^{2}}{2}
$$

$$
\begin{gathered}
\sum_{i=1}^{N} i^{2}=\frac{N(N+1)(2 N+1)}{6} \approx \frac{N^{3}}{3} \\
\sum_{i=1}^{N} i^{k} \approx \frac{N^{k+1}}{|k+1|} k \neq-1
\end{gathered}
$$



调和数$$H_N$$：
$$
H_{N}=\sum_{i=1}^{N} \frac{1}{i} \approx \log _{ e } N
$$

>$$\text { 误差趋向于 } \gamma \approx 0.57721566 \text {, 称为欧拉常数 (Euler' s constant) }$$

$$
\begin{gathered}
\sum_{i=1}^{N} f(N)=N f(N) \\
\sum_{i=n_{0}}^{N} f(i)=\sum_{i=1}^{N} f(i)-\sum_{i=1}^{n_{0}-1} f(i)
\end{gathered}
$$



# 模运算

如果 $N$ 整除 $A-B$, 那么就说 $A$ 与 $B$ 模 $N$ 同余, 记为 $A \equiv B(\bmod N)$ 。直观地看, 这意味着无 论是 $A$ 还是 $B$ 被 $N$ 去除, 所得余数都是相同的。于是, $81 \equiv 61 \equiv 1(\bmod 10)$ 。如同等号的情形 一样, 若 $A \equiv B(\bmod N)$, 则 $A+C \equiv B+C(\bmod N)$ 以及 $A D \equiv B D(\bmod N)$ 。



