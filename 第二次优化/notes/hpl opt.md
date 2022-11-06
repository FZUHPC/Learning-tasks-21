## hpl的性能调优

> 1.Giga简称G，是表示数量的前缀，表示10^9，即10亿，比如9G，就是90亿。
>
> 2.表示浮点运算能力的单位是FLOPS（即“每秒浮点运算次数”，“每秒峰值速度”），而非“Float”。是“每秒所执行的浮点运算次数”(floating-point operations per second) 的缩写。它常被用来估算电脑的执行效能，尤其是在使用到大量浮点运算的科学计算领域中。因为 FLOPS 字尾的那个 S，代表秒，而不是复数，所以不能省略掉。
> 在这里所谓的“浮点运算”，实际上包括了所有涉及小数的运算。这类运算在某类应用软件中常常出现，而它们也比整数运算更花时间。现今大部分的处理器中，都有一个专门用来处理浮点运算的“浮点运算器”（FPU）。也因此 FLOPS 所量测的，实际上就是 FPU 的执行速度。而最常用来测量 FLOPS 的基准程式 (benchmark) 之一，就是 Linpack。
> 3.GigaFLOPS即每秒10亿次浮点运算，也是是描述计算机浮点运算能力的单位，现在的主流CPU一般在20-60 GFLOPS之间。

```
HPLinpack benchmark input file
Innovative Computing Laboratory, University of Tennessee    前两行为说明性文字，不用作修改
HPL.out      output file name (if any) 				输出文件名
file         device out (6=stdout,7=stderr,file)	device out"为"6"时，测试结果输出至标准													  输出（stdout）"device out"为"7"时，													  测试结果输出至标准错误输出																（stderr）"device out"为其它值时，测														试结果输出至第三行所指定的文件中
1            # of problems sizes (N)				矩阵的数量，如1即一个矩阵
29 30 34 35  Ns										每个矩阵的规模
1            # of NBs								测试块的个数
1 2 3 4      NBs									每次使用的分块大小
0            PMAP process mapping 					选择处理器阵列是按列的排列方式还是按行的													排列方式
1            # of process grids (P x Q)				二维处理器网格
2 1 4        Ps										P
2 4 1        Qs										Q
16.0         threshold								测试的精度
1            # of panel fact						指明L分解的方式
0 1 2        PFACTs (0=left, 1=Crout, 2=Right)		
1            # of recursive stopping criterium		
2 4          NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
1            # of recursive panel fact.
0 1 2        RFACTs (0=left, 1=Crout, 2=Right)
1            # of broadcast							说明L的横向广播方式，HPL中提供了6种广播													  方式。
0            BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
1            # of lookahead depth					横向通信的通信深度
0            DEPTHs (>=0)
2            SWAP (0=bin-exch,1=long,2=mix)
64           swapping threshold						说明U的广播算法。
0            L1 in (0=transposed,1=no-transposed) form
0            U  in (0=transposed,1=no-transposed) form
1            Equilibration (0=no,1=yes)
8            memory alignment in double (> 0)		用于在内存分配中对齐地址
```

**参数调试**

```
HPLinpack benchmark input file
Innovative Computing Laboratory, University of Tennessee
HPL.out      output file name (if any)
6            device out (6=stdout,7=stderr,file)
1            # of problems sizes (N)
17644       Ns
1             # of NBs
128            NBs
0            PMAP process mapping (0=Row-,1=Column-major)
1           # of process grids (P x Q)
1            Ps
2            Qs
16.0         threshold
1           # of panel fact
2        PFACTs (0=left, 1=Crout, 2=Right)
1            # of recursive stopping criterium
2           NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
1            # of recursive panel fact.
2           RFACTs (0=left, 1=Crout, 2=Right)
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

> 进行了一些更改，让报告更简单。由于P × Q = 系统CPU数 = 进程数。所以PQ只有1×2, 2×1两种选择，经测试后发现P×Q=1×2时性能较优。确定P×Q为1×2；
>
> 物理内存的容量（单位：byte）乘以80%~85%来进行HPL的运算，剩余内存用于保证系统中的其他程序正常运行。由于一个双精度数占8个字节，所以再处以8，将结果开平方，得到的值比较接近最佳N值。根据经验似乎为384倍数更佳。
>
> 系数矩阵被分成NBxNB的循环块被分配到各个进程当中去处理，NB大小作为计算粒度，在很大程度上影响了计算性能的优劣。NB不可能太大或太小，一般在256以下，此处一般选择128。

硬件环境如下

![huanjing1](https://github.com/xylnon/Learning-tasks-21/blob/assignment/%E7%AC%AC%E4%BA%8C%E6%AC%A1%E4%BC%98%E5%8C%96/pictures/huanjing1.png)

![huanjing2](https://github.com/xylnon/Learning-tasks-21/blob/assignment/%E7%AC%AC%E4%BA%8C%E6%AC%A1%E4%BC%98%E5%8C%96/pictures/huanjing2.png)

|   N   |  NB  |  P   |  Q   |   Gflops   |
| :---: | :--: | :--: | :--: | :--------: |
| 19200 | 128  |  1   |  2   | 5.7403e+01 |
| 19968 | 128  |  1   |  2   | 3.5776e+01 |
| 19968 | 256  |  1   |  2   | 3.9655e+01 |
| 19200 | 256  |  1   |  2   | 5.0191e+01 |
| 18432 | 128  |  1   |  2   | 6.5671e+01 |
| 17664 | 128  |  1   |  2   | 6.7805e+01 |
| 15312 | 128  |  1   |  2   | 4.8044e+01 |
| 17664 | 256  |  1   |  2   | 6.7136e+01 |
| 17664 |  64  |  1   |  2   | 6.0648e+01 |

根据表格，可以发现N=17664，系统性能在NB=128时最高。
