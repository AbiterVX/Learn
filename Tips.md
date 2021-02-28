## 1.SSH

### 1.1重置

重置host，再使用ssh连接时，需要删除原有的密钥。

C:/Users/Administrator/.ssh

### 1.2 JSCH

https://blog.csdn.net/StromMaybin/article/details/54354431

## 2.Git

基本操作

```
//初始化
git init
//添加所有文件
git add a.
//提交
git commit -m "init"
//添加远程地址
git remote add origin git@github.com:AbiterVX/DiceTown.git
//生成key
ssh-keygen -t rsa -C "576412173@qq.com"
//添加key
Github -> Setting -> SSH and GPG keys -> 添加key
//push到远程
git push -u origin master


git pull sss

git commit -m "sss"
```

### 大文件

安装git lsf

```
git lfs install


```

### 邀请协作者

https://docs.github.com/cn/github/setting-up-and-managing-your-github-user-account/inviting-collaborators-to-a-personal-repository







timestap

ip



receiveBW

transmitBW

cpuUsage

memoryUsage

diskUsage

temp

inum/onum   io数量

energy



## 3.ECharts

修改时间轴label，时间格式。需要axisLabel.formatter 全部修改

https://echarts.apache.org/zh/option.html#xAxis.axisLabel.formatter

```
xAxis: {
                type: 'time',
                splitLine: {
                    show: false
                },
                axisLabel: {
                    formatter: {
                        year: '{yyyy}',
                        month: '{MM}',
                        day: '{dd}',
                        hour: '{HH}:{mm}',
                        minute: '{HH}:{mm}',
                        second: '{HH}:{mm}:{ss}',
                        millisecond: '{HH}:{mm}:{ss} ',
                        none: '{yyyy}-{MM}-{dd} {HH}:{mm}:{ss}'
                    }
                }
            },
```



Unable to locate:

/etc/apt/sourcelist