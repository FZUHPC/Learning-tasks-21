任务四——-在本机完成hpl的性能调优

HPL（The High-Performance Linpack Benchmark）是测试高性能计算集群系统浮点性能的基准。HPL通过对高性能计算集群采用高斯消元法求解一元N次稠密线性代数方程组的测试，评价高性能计算集群的浮点计算能力。

浮点计算峰值是指计算机每秒可以完成的浮点计算次数，包括理论浮点峰值和实测浮点峰值。理论浮点峰值是该计算机理论上每秒可以完成的浮点计算次数，主要由CPU的主频决定。理论浮点峰值＝CPU主频×CPU核数×CPU每周期执行浮点运算的次数。

（网上写的）

## 理解算例文件HPL.dat

共计31行

![img](file:///C:\Users\chenxin\Documents\Tencent Files\1404175682\Image\C2C\287898C5C7BFB53334CD7B0061A23B9D.png)

### 需要调节的参数

第六行 Ns	矩阵规模

问题规模大小N值,要考虑内存容量的制约关系，有一个达到最佳性能的上限值。我们可以用物理内存的容量（单位：byte）乘以80%~85%来进行HPL的运算，剩余内存用于保证系统中的其他程序正常运行。由于一个双精度数占8个字节，所以再处以8，将结果开平方，得到的值比较接近最佳N值。

第八行  NBs	测试块

试了128、192、256。结果256较好

第十行	

要求P×Q=8（本机）

第十一、十二行

一般p<=q,p=2

试了1×8，2×4.结果为2×4较好

## 相关概念

理论峰值计算理论浮点峰值＝CPU主频×CPU核数×CPU每周期执行浮点运算的次数。

理论峰值=3.190(主频)×8（核数）×8（双精度）=204.16

主频即[CPU](https://baike.baidu.com/item/CPU/120556?fromModule=lemma_inlink)的[时钟频率](https://baike.baidu.com/item/时钟频率/103708?fromModule=lemma_inlink)，[计算机](https://baike.baidu.com/item/计算机/140338?fromModule=lemma_inlink)的操作在时钟信号的控制下分步执行，每个时钟信号周期完成一步操作，[时钟频率](https://baike.baidu.com/item/时钟频率/103708?fromModule=lemma_inlink)的高低在很大程度上反映了CPU速度的快慢。

1.查看cpu个数  8

2.查看每个cpu的核数  1

3.查看cpu主频   3.19

4.cpu每周期执行浮点运算的次数

​	intel一般是16，AMD的是8

![image-20221028192246501](C:%5CUsers%5Cchenxin%5CAppData%5CRoaming%5CTypora%5Ctypora-user-images%5Cimage-20221028192246501.png)

## 调参数然后测试

从测试到的数据里挑出的好一点的结果↓

![image-20221102205621045](C:%5CUsers%5Cchenxin%5CAppData%5CRoaming%5CTypora%5Ctypora-user-images%5Cimage-20221102205621045.png)

看了别的同学的，自己的实在是太粗糙辣

