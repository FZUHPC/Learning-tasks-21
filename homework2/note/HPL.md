# HPL

## 参考链接

[](https://github.com/yunzhongOvO/Linpack-HPL)

[](https://www.jianshu.com/p/fc159fc46c55)

[](https://blog.csdn.net/jycjyc/article/details/125295559)

[](https://blog.csdn.net/gyx1549624673/article/details/86551466)

[](https://blog.csdn.net/weixin_42121713/article/details/100125356)

## 简单介绍

- HPL是针对现代并行计算机提出的测试方式。用户在不修改任意测试程序的基础上，可以调节问题规模大小(矩阵大小)、使用CPU数目、使用各种优化方法等等来执行该测试程序，以获取最佳的性能。
- HPL采用高斯消元法求解线性方程组。求解问题规模为N时，浮点运算次数为$\frac{2}{3} N^{3}+\frac{3}{2} N^{2}$。给出问题规模$$N$$，测得系统计算时间$$T$$，$峰值=\frac{\frac{2}{3} N^{3}+\frac{3}{2} N^{2}}T (FLOPS)$。

## HPL.dat

1. 说明

2. 说明

3. 输出文件名

4. 输出形式 6：stdout  7：stderr 其他：输出到文件

5. 测试的矩阵个数

6. 测试的矩阵规模

   $$ \sqrt{mem * 0.8 / 8} \lt Ns \lt \sqrt{mem * 0.85 / 8} $$

7. 矩阵分块的数量

8. 矩阵分块的规模（阶数）

9. 处理器阵列按列排列或按行排列 按 0：行 1：列的方式排列

10. 二维处理器网格，要求11 12行 P*Q=系统CPU数=进程数

    一般情况下，应使 P 略小于 Q，因为列向通信量的通信次数和通信数据量大于横向通信。另外，由于L分解的列向通信采用二元交换法（Binary Exchange），当列向处理器个数P为2的幂时，性能最优

11. P

12. Q

13. 说明测试的精度。在做完线性方程求解之后，检测求解结果是否正确。误差在这个值以内就正确。测试的精度，无需修改

## 理论峰值计算

### 配置

**CPU: 11th Gen Intel(R) Core(TM) i5-11300H**

**CPU主频: 3.1GHz**

**CPU核数: 1**

**每周期浮点运算次数: 8**

**可用物理内存(byte): 866459648**

**理论浮点运行峰值 = CPU主频 × CPU核心数 × 每周期浮点运算次数**

**Gflops = 3.1 * 1 * 8 = 24.8**

## 参数配置

### N(5)

**取1**

### Ns(problems sizes)(9600)(6)

**取Ns=9600**

> $\sqrt{mem * 0.8 / 8} \lt Ns \lt \sqrt{mem * 0.85 / 8}$
>
> $\sqrt{86645964.8} \lt Ns \lt \sqrt{92061337.6}$
>
> $9308.381 \lt Ns \lt 9594.859$
>
> 根据经验似乎为384倍数更佳

### 分块矩阵数量(7)

**取1**

### 矩阵分块的规模NBs(8)

**取2，4，16，48，128五组**

### 处理器阵列按列排列或按行排列PMAP

**取0**

> 节点多按列，节点少按行

### 二维处理器网格P*Q(11, 12)

**取P=1 Q=1**

> $P×Q=系统CPU数=进程数$

### L横向广播算法 BCASTs(23)

**取0，1两组**

> HPL中提供了6种广播方式(0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM) ，其中，前4种适合快速网络；后两种采用将数据切割后传送的方式，主要适合于速度较慢的网络。一般在小规模系统中选择0或1，大规模系统选择3。本次实验中选择3。

## 测量结果

### 初始配置

```text
HPLinpack benchmark input file
Innovative Computing Laboratory, University of Tennessee
HPL.out      output file name (if any)
6            device out (6=stdout,7=stderr,file)
4            # of problems sizes (N)
29 30 34 35  Ns
4            # of NBs
1 2 3 4      NBs
0            PMAP process mapping (0=Row-,1=Column-major)
3            # of process grids (P x Q)
2 1 4        Ps
2 4 1        Qs
16.0         threshold
3            # of panel fact
0 1 2        PFACTs (0=left, 1=Crout, 2=Right)
2            # of recursive stopping criterium
2 4            NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
3            # of recursive panel fact.
0 1 2        RFACTs (0=left, 1=Crout, 2=Right)
1            # of broadcast
0            BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
1            # of lookahead depth
0            DEPTHs (>=0)
2            SWAP (0=bin-exch,1=long,2=mix)
64           swapping threshold
0            L1 in (0=transposed,1=no-transposed) form
0            U  in (0=transposed,1=no-transposed) form
1            Equilibration (0=no,1=yes)
8            memory alignment in double (> 0)
```

```text
WR00L2L2          29     1     2     2               0.12             1.4506e-04
WR00L2L4          29     1     2     2               0.13             1.3278e-04
WR00L2C2          29     1     2     2               0.12             1.4965e-04
WR00L2C4          29     1     2     2               0.13             1.3012e-04
WR00L2R2          29     1     2     2               0.12             1.4272e-04

WR00L2L2          30     1     2     2               0.11             1.7196e-04
WR00L2L4          30     1     2     2               0.10             1.8534e-04
WR00L2C2          30     1     2     2               0.11             1.8054e-04
WR00L2C4          30     1     2     2               0.09             2.1649e-04
WR00L2R2          30     1     2     2               0.11             1.7435e-04

WR00L2L2          34     1     2     2               0.10             2.8730e-04
WR00L2L4          34     1     2     2               0.09             3.2103e-04
WR00L2C2          34     1     2     2               0.11             2.5490e-04
WR00L2C4          34     1     2     2               0.12             2.4116e-04
WR00L2R2          34     1     2     2               0.11             2.5637e-04

WR00C2C4          35     3     2     2               0.03             1.0194e-03
WR00C2R2          35     3     2     2               0.04             7.5034e-04
WR00C2R4          35     3     2     2               0.03             9.2935e-04
WR00R2L2          35     3     2     2               0.02             1.4298e-03
WR00R2L4          35     3     2     2               0.03             9.0707e-04
```

### 参数优化后

#### HPL.dat

```text
HPLinpack benchmark input file
Innovative Computing Laboratory, University of Tennessee
HPL.out      output file name (if any)
6            device out (6=stdout,7=stderr,file)
1            # of problems sizes (N)
9600         Ns
1            # of NBs
2            NBs
0            PMAP process mapping (0=Row-,1=Column-major)
1            # of process grids (P x Q)
1            Ps
1            Qs
16.0         threshold
1            # of panel fact
1            PFACTs (0=left, 1=Crout, 2=Right)
2            # of recursive stopping criterium
4 8            NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
1            # of recursive panel fact.
2            RFACTs (0=left, 1=Crout, 2=Right)
1            # of broadcast
0            BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
1            # of lookahead depth
0            DEPTHs (>=0)
2            SWAP (0=bin-exch,1=long,2=mix)
64           swapping threshold
0            L1 in (0=transposed,1=no-transposed) form
0            U  in (0=transposed,1=no-transposed) form
1            Equilibration (0=no,1=yes)
8            memory alignment in double (> 0)
```

```text
WR00R2C4        9600     2     1     1              65.83             8.9619e+00
WR00R2C8        9600     2     1     1              65.82             8.9631e+00
WR00R2C4        9600     2     1     1              66.00             8.9394e+00
WR00R2C8        9600     2     1     1              65.75             8.9728e+00
WR00R2C4        9600     2     1     1              66.95             8.8121e+00
WR00R2C8        9600     2     1     1              65.91             8.9515e+00
WR00R2C4        9600     2     1     1              65.96             8.9440e+00
WR00R2C8        9600     2     1     1              68.17             8.6538e+00
WR00R2C4        9600     2     1     1              69.01             8.5489e+00
WR00R2C8        9600     2     1     1              69.06             8.5427e+00
WR00R2C4        9600     2     1     1              67.95             8.6829e+00
WR00R2C8        9600     2     1     1              69.25             8.5189e+00
WR00R2C4        9600     2     1     1              70.28             8.3939e+00
WR00R2C8        9600     2     1     1              69.02             8.5474e+00
WR00R2C4        9600     2     1     1              68.89             8.5644e+00
WR00R2C8        9600     2     1     1              68.36             8.6297e+00
WR00R2C4        9600     2     1     1              68.73             8.5833e+00
WR00R2C8        9600     2     1     1              68.35             8.6320e+00
WR00R2C4        9600     2     1     1              68.37             8.6290e+00
WR00R2C8        9600     2     1     1              68.61             8.5994e+00
```

## 结果

初始配置：1.0194e-03 / 24.8 = 0.00411%

参数优化：8.9619  / 24.8 = 36.13%

虽然但是，跟理论值比较，还是很低的样子。。。

## 总结

小插曲：有一次测量是刚开电脑的时候测得，发现打印的Gflops慢慢变大了，就感觉可能要先让cpu跑一跑预热一下的样子，就写了个计数的代码让他先跑着，然后再测试。

```shell
#!/bin/bash
echo "start"
./hot
for i in {1..10}
do
    mpirun -np 1 ./xhpl | grep WR >> res_$i.txt
done
echo "end"
```



1. Ns: 经过很多次测量，发现Ns设置为9600确实是最优解，可能很小幅度改动可能会更好？但是经过$9600 + n*1920(-3<n<3)$的多次尝试，都是不会有更好的结果。
2. P, Q: 根据P < Q，只有将P改为2，但是Gflops直接就变成了3.n，所以在虚拟机上P = Q = 1应该是最优解
3. NBs: 2，48，128的结果没有很大差别，但是2的结果还是最好的。4，16会让我觉得比较奇怪，Gflops直接算是对半砍了。
4. PMAP: 这个取0取1在电脑上测得结果没有什么区别。

