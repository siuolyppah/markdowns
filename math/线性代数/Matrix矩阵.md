# Matrix的定义

$$
\text { Matrix } A=\left[\begin{array}{ccc}
2 & 3 & 5 \\
3 & 1 & -1 \\
-2 & 1 & 1
\end{array}\right]
$$



# 矩阵规模的描述方式

先描述 行 ， 再描述 列

![image-20220713194406406](Matrix%E7%9F%A9%E9%98%B5.assets/image-20220713194406406.png)



# 矩阵的性质

- 相同规模的矩阵，可以做加法和减法。
- 矩阵，可被标量乘
- • A, B, C are mxn matrices, and s and t are scalars:
  - A + B = B + A
  - (A + B) + C = A + (B + C)
  - (st)A = s(tA)
  - s(A + B) = sA + sB
  - (s+t)A = sA + tA  



# 特别的矩阵

- Square matrix：行列的规模相同

  - diagonal：主对角线

  - Upper Triangular  Matrix：左下角为0，主对角线不管。
    $$
    \left[\begin{array}{ccc}
    2 & 3 & 5 \\
    0 & 1 & -1 \\
    0 & 0 & 1
    \end{array}\right]
    $$

  - Lower Triangular  Matrix：右上角为0，主对角线不管。
    $$
    \quad\left[\begin{array}{ccc}
    2 & 0 & 0 \\
    3 & 1 & 0 \\
    -2 & 1 & 1
    \end{array}\right]
    $$

  > 只有Square Matrix，有这些说法。

- Diagonal Matrix：只有主对角线非0

- Identity Matrix：主对角线全1，其余全0
  $$
  I_{3}=\left[\begin{array}{lll}
  1 & 0 & 0 \\
  0 & 1 & 0 \\
  0 & 0 & 1
  \end{array}\right]
  $$

  > 可简记为 $I_n$

- Zero Matrix：全部元素为0

  > 简记：
  >
  > - $O$，任意规模
  > - $O_{m\times n}$，m行n列的零矩阵

- Symmetric Matrix：$A^T = A$

  >对称矩阵，一定是方阵。





# 矩阵的转置

If A is an mxn matrix, 𝐴 𝑇 (transpose of A) is an nxm matrix whose (i,j)-entry is the (j-i)-entry of A



## 转置的性质

A and B are mxn matrices, and s is a scalar:

- $\left(A^{T}\right)^{T}=A$

- $(s A)^{T}=s A^{T}$

- $(A+B)^{T}=A^{T}+B^{T}$

  > 因为转置、标量$\times$矩阵、矩阵加法都是Linear System



