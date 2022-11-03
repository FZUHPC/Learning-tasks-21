# 本机信息

Linux ozline 5.15.0-48-generic #54-Ubuntu SMP Fri Aug 26 13:26:29 UTC 2022 x86_64 x86_64 x86_64 GNU/Linux

Ubuntu 22.04.1 LTS

Apple Inc. MacbookPro11,5

Intel Core 17-4980HQ cpu @ 2.80GHz × 8

# OneAPI

## 下载包

`https://www.intel.cn/content/www/cn/zh/developer/tools/oneapi/toolkits.html`

找到`Intel oneAPI Base Toolkit`和`Intel® oneAPI HPC Toolkit`，进入下载页面

操作系统选择Linux，分发选APT软件包管理器

## 安装

- 按上述说明下载后安装，已经是图形界面了，跟着一步一步来就行啦，只需要注意一下
  - 选推荐安装`Recommended Installation`
  - 不使用已有配置`Skip Eclipse IDE configuration`
- 最后会提示`Installization location: `这里表示我们文件存放的位置，后续路径都和这个有关
- 安装好后，编辑`vim ~/.zshrc`
- 添加命令`source /opt/intel/oneapi/setvars.sh intel64`
- 应用`source ~/.zshrc`，可以重启一下终端
- 会跳出一堆信息，这时候就需要我们手动更改`setvars.sh`里的内容了（谁喜欢每次打开终端的时候跳出一堆东西呢）
- 编辑`sudo vim /opt/intel/oneapi/setvars.sh`
  -  首先，在第一行插入`# USING #| EXPLAIN MY MODIFICATION`，对于自行更改的内容要随时做好注释
  - 搜索关键字`initializing oneAPI environment ...`，把这四行全部注释，注意，要用`#|`，便于后续定位更改
  - 搜索关键字`:: $arg_base -- $arg_verz`，注释
  - 搜索关键字`:: oneAPI environment initialized `，把这两行都注释掉
  - 重新打开终端
- 如果一切都没问题，重新打开终端后不会跳出输出的信息，我们可以随便输入一个例如`ifort -v`测试是否成功

## 成果图

无，因为有输出的都被我注释掉了~

# HPL

## 下载包

校园网一直连不上，不知道为何安装了Clash也提示502，后面直接电脑下载传阿里云OSS再在物理机上从OSS下载，曲线救国了

## 安装

### 流程

- 下载`wget http://www.netlib.org/benchmark/hpl/hpl-2.3.tar.gz`
- 解压并重命名`tar -xzf hpl-2.3.tar.gz && mv hpl-2.3 hpl`
- 复制`cp setup/Make.Linux_Intel64 Make.test`
- 编辑`Make.test`
  - ==TOPdir==修改为hpl文件夹路径
  - ==ARCH==修改为test
  - ==OMP_DEFS==修改为`-qopenmp`
- 编译`make arch=test`
- 测试`cd /bin/test`，如果前面步骤无误这里会出现一个`HPL.dat`和`xhpl`的二进制文件
- 运行`mpirun ./xhpl`

### 重点分析

在编辑`Make.test`文件上花了较多时间

- 第一次，在`Make.test`文件中只修改了`arch=test`，导致编译过程中遇上链接错误，检查后发现是硬连接地址错误，原因分析后，首先是意识到自己的用户并非root，其次是发现还需要修改`Make.test`中的`TOPdir`，遂更改并重新make
- make后发现还是不行，在这一步选择了把文件夹直接删去，重新解压缩再make
- 第二次，make后没有出现硬链接错误，但是提示`-openmp is not supported`，考虑到自己的物理机系统版本是Ubuntu22.04，同时查证发现这一步需要修改参数`-openmp`为`-qopenmp`，在`Make.test`中将`OMP_DEFS`的参数更改为`-qopenmp`，通过编译

## 成果图

![IMG_9736](/Users/ozliinex/Desktop/markdown/images/IMG_9736.jpeg)

# HPCG

## 下载包

过程顺畅，联系到先前HPL一直无法下载，可能是那个域名并不是proxy名单里的，导致没有走代理

## 安装

- 下载`git clone https://github.com/hpcg-benchmark/hpcg.git`
- 进入目录`cd /hpcg/setup/`
- 修改`Make.Linux_MPI`
  - ==TOPdir==考虑到我们是在home目录下clone的仓库，于是我们修改成`$(HOME)/hpcg`
  - ==MPdir==，这里要定位到oneapi的mpi目录，我这里是`/opt/intel/oneapi/mpi/2021.6.0`
  - ==MPinc==，修改为`-I $(MPDir)/include`，利用到了前面我们设定的MPdir
  - ==MPlib==，修改为`$(MPdir)/lib/release/libmpi.a`
- 建目录并进入`mkdir build && cd build`
- 设置环境`/home/ozline/hpcg/configure Linux_MPI`，注意，这里的`configure`就是hpcg文件夹根目录的那个，因此这句命令的configure路径需要你自己确定
- 安装`make`
- 测试`cd bin`，如果成功，会有一个`hpcg.dat`和`xhpcg`文件
- 运行`./xhpcg`，如果没有报出什么error就是跑通了

## 成果图

展示的是文件`HPGC-Benchmark_3.1_{time}.txt`的部分内容

![IMG_9738](/Users/ozliinex/Desktop/markdown/images/IMG_9738.jpeg)