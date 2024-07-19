# Srun4KAutoLogin_Openwrt

[toc]

## 简介

适用于深澜4K认证系统的openwrt设备自动登录程序，基于shell和C语言。北航校园网环境测试通过。

加密逻辑参考：<https://blog.csdn.net/hackermengzhi/article/details/130499424>

## 环境需求

1.**编译环境**
任意可运行**openwrt SDK**的设备（后文简称编译设备）。在WSL Ubuntu 22.04.3 LTS系统测试通过。
2.**运行环境**
运行脚本进行自动上网的设备（后文简称运行设备）。在以下设备测试通过。

条目 | 数值
-----|-----
 型号 | 红米AC2100
 处理器 | MT7621
 固件版本 | OpenWrt 23.05.0-rc3/LuCI openwrt-23.05
 内核版本 | 5.15.127

## 程序特点

1. **轻量化**。使用C语言生成请求体，wget命令向服务器发送GET请求。程序与依赖库占用运行设备空间小于1MB，适用于路由器等存储空间有限的场合。
2. **掉线自动重连**。每5分钟+(5-25随机秒数)检测一次在线状态，不在线则重新登陆。登陆失败时会间隔(5-25随机秒数)自动重试。

## 使用方式

1. 在编译设备编译srunRequestBodyGeneration可执行文件
2. 将其与autoLogin.sh文件一同上传到运行设备
3. 配置必要参数与环境
4. 配置开机自动启动项

### 1.编译可执行文件

1. 根据运行设备型号，安装或自行编译openwrt SDK(网址:<https://downloads.openwrt.org/snapshots/targets/>)
2. clone本项目到编译设备本地目录
3. make(按照SDK环境不同，可能需要修改makefile中的CC变量)
4. 在./bin文件夹下获取srunRequestBodyGeneration可执行文件
**注**：已经存储于./bin文件夹中的可执行文件基于测试设备生成，仅可用于**Redmi AC2100+OpenWrt 23.05.0**，其它设备请自行生成。

### 2.上传

将可执行文件与./shell/autoLogin.sh上传到运行设备。
参考代码：

```Shell
sudo scp ./bin/srunRequestBodyGeneration root@192.168.1.1:$DEST_FOLDER/srunRequestBodyGeneration
sudo scp ./shell/autoLogin.sh root@192.168.1.1:$DEST_FOLDER/autoLogin.sh
```

### 3.配置必要参数与环境

#### 修改./shell/autoLogin.sh文件

`sudo vi $DEST_FOLDER/autoLogin.sh`

```Shell
username=testuser                                       # user name
passwordPlain=testpwd                                   # plain password
executablePath="/srunTest/srunRequestBodyGeneration"    # path to srunRequestBodyGeneration
challengeURL="http://10.200.21.4/cgi-bin/get_challenge" # BUAA Srun Certificate server URLs
portalURL="http://10.200.21.4/cgi-bin/srun_portal"
```

username - 校园网认证用户名
passwordPlain - 明文密码
executablePath - 指向请求体生成可执行文件的路径
challengeURL,portalURL - 不同学校的URL不同，可参考加密逻辑参考网页中的做法设置

#### 安装依赖包

本程序依赖coreutils-date、curl、ubus、getrandom库运行

```Shell
opkg update
opkg install coreutils-date
opkg install ubus
opkg install getrandom
```

### 4.配置开机自动启动项

修改开机自启动条目文件
`vi /etc/rc.local`
添加新行
`/srunTest/autoLogin.sh &`

## 更新记录

V1.0 功能实现

## To Do

1.发送心跳包防止掉线
2.优化安装方式
