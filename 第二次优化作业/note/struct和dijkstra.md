# 拓展作业

## 使用SIMD优化结构体

### 题目要求

```c
struct A {
  	int a;
    float b;
    double c;
    char d;
};

//尝试使用指令集对结构体数组的加法算法进行向量化
```

### 过程

​	第一下看到这个题人是傻的...毕竟结构体里的内容并不连续，并不好直接加载到向量化指令的寄存器中（虽然提供了set函数，可以对寄存器中的每个部分进行分别设置，但是猜测性能并不高...）。找学长问了后，得知是考察 结构体 的内存对齐...（可是这里的内存对齐不就是32-32-64-64这样的结构...没理解和向量化有什么关系）。最后决定了这样的尝试思路：

-   先写一个最朴素的方式，编译后查看汇编
-   给编译器开优化后，查看编译器是如何对结构体进行向量化的

```c
#include <immintrin.h>
#include <stdio.h>

struct A {
    int a;
    float b;
    double c;
    char d;
} a[100], b[100];

//最朴素的实现...先把要相加的东西整合到连续的空间里，再使用SIMD优化，最后再存回去
void add(struct A* a, struct A* b) {
    // 这里只写了32位整形的计算，其他的同理
    int arrayA[100] = {0};
    int arrayB[100] = {0};
    for (size_t i = 0; i < 100; i++) {
        arrayA[i] = a[i].a;
        arrayB[i] = b[i].b;
    }

    __m128i rega, regb;
    for (size_t i = 0; i < 100; i += 8) {
        rega = _mm_load_epi32(arrayA + i);
        regb = _mm_load_epi32(arrayB + i);
        rega = _mm_add_epi32(rega, regb);
        _mm_store_epi32(arrayA + i, rega);
    }

    for (size_t i = 0; i < 100; i++) {
        a[i].a = arrayA[i];
    }
}
```

​	粗略计算一下，这样强行使用SIMD带来的问题是计算时间很少，而读写内存的时间要远长于计算的时间，这样优化的效果肯定不好（像是为了用SIMD而用SIMD了...）。

（再后来发现了有gather一系列函数...读就没什么问题了）

>   **关于_mm_set系列函数**
>
>   ​	这一系列函数并不是某个指令的简单封装，而是会根据操作数的不同编译成不同的指令序列：
>
>   -   如果是已知值，将被编译到静态区，在set时会简单地被加载到寄存器中（只需要load一次）
>   -   如果是编译器无法预测的内容，会一个一个插入寄存器中



```c
// 改成这样，再使用O3优化，得到汇编
#include <immintrin.h>
#include <stdio.h>

// 按原题的结构，编译器似乎都不会进行向量化，因此改成这样
struct A {
    int a;
    float b;
} a[100], b[100];


int main() {
    for (size_t i = 0; i < 100; i++) {
        a[i].a += b[i].a;
        a[i].b += b[i].b;
    }
    return 0;
}
```

得到的汇编结果为

```assembly
main:
.LFB6403:
	pushq	%rbp
	leaq	a(%rip), %rax		# A* a in rax
	vmovdqa	.LC0(%rip), %ymm3	# LC0 0 2 4 6 4 6 0 2
	leaq	b(%rip), %rdx		# A* b in rdx
	vmovdqa	.LC1(%rip), %ymm2	# LC1 4 6 0 2 0 2 4 6
	leaq	768(%rax), %rcx		# &a[96] in rcx
	movq	%rsp, %rbp
	andq	$-32, %rsp
.L2:
	# 读取部分编译器做了优化，比起从内存中一个一个读入要快
	# vpermd是按ymm2中的index读出内存并写入ymm1中
	vpermd	32(%rax), %ymm2, %ymm1	
	# ymm1 = {a[6].a, a[7].a, a[4].a, a[5].a, a[4].a, a[5].a, a[6].a, a[7].a }
	vpermd	(%rax), %ymm3, %ymm0
	# ymm0 = {a[0].a, a[1].a, a[2].a, a[3].a, a[2].a, a[3].a, a[0].a, a[1].a}
	addq	$64, %rax
	addq	$64, %rdx
	vpermd	-32(%rdx), %ymm2, %ymm4
	# ymm4 = {a[14].a, a[15].a, a[12].a, a[13].a, a[12].a, a[13].a, a[14].a, a[15].a}
	vpblendd	$240, %ymm1, %ymm0, %ymm0 # 240 == 1[7:4], 0[3:0]
	# ymm0 = {a[0].a, a[1].a, a[2].a, a[3].a, a[4].a, a[5].a, a[6].a, a[7].a}
	vpermd	-64(%rdx), %ymm3, %ymm1
	# ymm1 = {a[8].a, a[9].a, a[10].a, a[11].a, a[10].a, a[11].a, a[8].a, a[9].a}
	vpblendd	$240, %ymm4, %ymm1, %ymm1
	# ymm1 = {a[8].a, a[9].a, a[10].a, a[11].a, a[12].a, a[13].a, a[14].a, a[15].a}
	vpermd	-28(%rdx), %ymm2, %ymm4 # 这里已经在准备读float了
	vpaddd	%ymm1, %ymm0, %ymm0		# 相加的结果放在ymm0中
	
	# vpextrd 将第index个数放入内存中
	vpextrd	$1, %xmm0, -56(%rax) # a[6] + a[14] 放入 a[6] 中...
	vpextrd	$2, %xmm0, -48(%rax) # a[5] + a[13] 放入 a[5] 中...
	# vmovd 只复制前32bit
	vmovd	%xmm0, -64(%rax)
	vpextrd	$3, %xmm0, -40(%rax)
	vextracti128	$0x1, %ymm0, %xmm0
	vpextrd	$1, %xmm0, -24(%rax)
	vpextrd	$2, %xmm0, -16(%rax)
	vmovd	%xmm0, -32(%rax)
	vpextrd	$3, %xmm0, -8(%rax)
	# int部分计算到这里结束,可以看出编译器在读入的时候做了优化，在存回去的时候也是一个一个存的...
	
	# float部分，与int部分大同小异...
	vpermd	-60(%rax), %ymm3, %ymm0
	vpermd	-28(%rax), %ymm2, %ymm1
	vblendps	$240, %ymm1, %ymm0, %ymm0
	vpermd	-60(%rdx), %ymm3, %ymm1
	vblendps	$240, %ymm4, %ymm1, %ymm1
	vaddps	%ymm1, %ymm0, %ymm0
	vmovss	%xmm0, -60(%rax)
	vextractps	$1, %xmm0, -52(%rax)
	vextractps	$2, %xmm0, -44(%rax)
	vextractps	$3, %xmm0, -36(%rax)
	vextractf128	$0x1, %ymm0, %xmm0
	vmovss	%xmm0, -28(%rax)
	vextractps	$1, %xmm0, -20(%rax)
	vextractps	$2, %xmm0, -12(%rax)
	vextractps	$3, %xmm0, -4(%rax)
	cmpq	%rcx, %rax
	jne	.L2
	xorl	%eax, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
```

### 问题

（存回去时候很麻烦，编译器也只是一个一个存，有没有什么方便的指令？（我没找到...））

## Dijkstra算法的并行实现

```c
void dijkstra(size_t graph[9][9], size_t* result_buf, size_t start, size_t n) {
    // 初始化部分
    bool* visited = new bool[n];
    size_t* len = new size_t[n];
    memset(visited, false, n * sizeof(bool));
    memset(len, -1, n * sizeof(size_t));
    len[start] = 0;
    size_t cur = start;
    // 外循环（循环之间有依赖，不好改并行）
    for (size_t j = 0; j < n; j++) {
        visited[cur] = true;
        // 循环更新距离数组(在这里并行化)
        // 论文上那种没看太懂，之后再研究实现
        #pragma omp parallel for num_threads(16)
        for (size_t i = 0; i < n; i++)
            if (graph[cur][i] + len[cur] < len[i] && graph[cur][i] != 0) {
                len[i] = graph[cur][i] + len[cur];
                result_buf[i] = cur;
            }
        // 遍历距离数组，找出最近的、未访问过的节点
        // 这一步依赖于上一步更新距离数组的结果，不好改并行
        size_t min = -1;
        for (size_t i = 0; i < n; i++) {
            if (len[i] < min && !visited[i]) {
                min = len[i];
                cur = i;
            }
        }
    }
}
```

