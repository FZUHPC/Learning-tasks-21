# git

## git config

### 查看全局用户信息

```bash
git config --global user.name
git config --global user.email
```

### 设置用户信息

```bash
git config --global user.name "cgy"
git config --global user.email "948628463@qq.com"
```

## git init

> 初始化仓库
> 

## git status

> 显示当前状态
> 

![Untitled](git%20aaea5ce9216c4888baf4a174cb13c4da/Untitled.png)

- 在哪个分支
- 显示文件状态

## git add <file>

> 将文件从工作区加入到缓存区
> 

## git commit

> 将缓存区加入到本地仓库
> 
- 直接git commit会进入编辑
- 使用git commit -m “version n”就不需要进入到编辑

## git log

> 查看版本
> 

![Untitled 1](https://cgy-1310478750.cos.ap-nanjing.myqcloud.com//202211060101156.png)

## git branch

> 创建分支
> 

```bash
git branch branch_name
```

> 查看全部分支
> 

```bash
git branch
```

> 删除分支
> 

```bash
git branch -d branch_name
```

有这个报错是因为没有把这个分支合并起来

![Untitled 2](https://cgy-1310478750.cos.ap-nanjing.myqcloud.com//202211060100027.png)

> 确定删除分支
> 

```bash
git branch -D branch_name
```

## git checkout

> 切换分支
> 

```bash
git checkout branch_name
```

> 创建并且切换分支
> 

```bash
git checkout -b branch_name
```

## git merge <branch>

> 将其他分支合并到当前分支上
> 

![Untitled 3](https://cgy-1310478750.cos.ap-nanjing.myqcloud.com//202211060100749.png)

## .gitignore

创建指令

> touch .gitignore
> 

将要隐藏的文件名写到文件里就行