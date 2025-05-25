# Srun4KAutoLogin_Openwrt

## 简介

适用于深澜4K认证系统的openwrt设备自动登录程序，基于shell和C语言。北航校园网环境测试通过。

加密逻辑参考：<https://blog.csdn.net/hackermengzhi/article/details/130499424>

## 环境需求

1. **运行环境**
    安装有Openwrt系统的路由器。在以下设备测试通过。

条目 | 数值
-----|-----
 架构 | x86/64                            
 处理器 | Intel N100 
 固件版本 | OpenWrt 24.10.1 r28597-0425664679 
 内核版本 | 6.6.86 

## 程序特点

1. **轻量化**。使用C语言生成请求体，curl命令向服务器发送GET请求。程序与依赖库占用运行设备空间小于1M，适用于路由器等存储空间有限的场合。
2. **掉线自动重连**。每5分钟+(5-25随机秒数)检测一次在线状态，不在线则重新登陆。登陆失败时会间隔(5-25随机秒数)自动重试。

## 部署方式

可以选择Openwrt本地编译方式或交叉编译方式部署。

- **本地编译：**需在Openwrt上安装GCC以编译可执行文件，过程需要~180MB可用空间（编译完成后可以卸载GCC释放）；

- **交叉编译：**须在外部设备上编译能在Openwrt上运行的可执行文件，再将文件拷贝到路由器上，仅要求路由器具有存放程序与依赖的空间即可。

  出于省事，这里只说本地编译的方案。外部设备搭建交叉编译环境可以参考：[[OpenWrt Wiki\] 使用SDK](https://openwrt.org/zh/docs/guide-developer/using_the_sdk)

  |                    | Openwrt本地编译                   | 交叉编译  |
  | ------------------ | --------------------------------- | --------- |
  | 部署难度           | 简单                              | 复杂      |
  | 目标路由器空间需求 | 大（过程需要180MB，完成后可释放） | 小（<1M） |

- **本地编译部署方法**

  1. **登录后台：**使用SSH连接到路由器后台
  2. **安装依赖：**通过opkg；
  3. **克隆项目：**从github克隆项目（或直接下载zip包解压，也是一样的）；
  4. **修改编译配置：**修改makefile文件；
  5. **编译**
  6. **配置登录脚本**
  7. **安装**
  8. **运行**
```shell
# Openwrt本地编译流程
# 1.选择在路由器上编译，所以登录后台操作
ssh root@xxx.xxx.xxx.xxx;  # 登录路由器后台

# 2.安装依赖
opkg update
opkg install git git-http # 克隆项目用，如果直接上传zip包可以忽略
opkg install make gcc  # 本地编译用
opkg install coreutils-date ubus curl getrandom coreutils-nohup # 运行依赖，必装

# 3.克隆项目
cd /tmp  # 随便找个目录放项目文件，这里选择/tmp，重启后直接清空，很方便;内存小的机器可能得找其它地方放
git clone https://github.com/Li-O-Li/Srun4KAutoLogin_Openwrt.git
cd Srun4KAutoLogin_Openwrt/

# 4.修改编译配置
vi makefile
# 主要修改CC、CFLAGS变量
# CC:编译命令名称（我这里使用路由器本地GCC，故CC=gcc；交叉编译可能改为mipsel-openwrt-linux-gcc的名字
# CFLAGS:编译选项（前面的-I不要动；默认开O2优化，可改为-OSize以进行占用空间优化）
# 修改完成后保存退出

# 5.编译
make clean  # 清除现有编译文件
make  # 进行编译
# 结果在bin文件夹下

# 6.配置登录脚本
vi ./shell/autoLogin.sh
# 着重修改username、passwordPlain为校园网账户名与密码
# 按需修改ipSchoolGateway为校园网登录界面的IP，可以使用nslookup [域名]的指令查询
# executablePath和logPath在安装时自动填写，无需手动干预

# 7.安装
vi ./install.sh  # 按需修改安装路径
# SRUN_INSTALL_BIN_FOLDER : URI生成程序安装文件夹
# SRUN_INSTALL_BIN_NAME : URI生成程序文件名
# SRUN_INSTALL_SHELL_FOLDER : 登录脚本安装文件夹
# SRUN_INSTALL_SHELL_NAME : 登录脚本文件名
# SRUN_LOG_PATH : 登录日志文件位置
chmod +x ./install.sh
./install.sh

# 8.运行
/etc/init.d/srun_login start
/etc/init.d/srun_login enable  # 开机启动

# (可选)清理GCC以腾出150+MB储存空间
opkg uninstall gcc make
```
- **手动部署方法**

  适用于交叉编译后，无法使用install.sh脚本安装的情况。

```shell
# 假设现在在本地，当前文件夹内已经有名为srunRequestBodyGeneration的可执行文件以及github /shell文件夹下的两个shell文件
# 目录结构像这样
# - current_directory
#   - srunRequestBodyGeneration
#   - autoLogin.sh
#   - srun_login
scp ./* root@xxx.xxx.xxx.xxx:~/srun/  # 将本地文件上传到路由器~/srun/文件夹内，自行替换IP地址

# 登陆路由器并切换文件夹
ssh root@xxx.xxx.xxx.xxx
cd ~/srun

# 3.修改autoLogin.sh脚本
EXECUTABLE_PATH="/usr/bin/srunRequestBodyGeneration"  # 安装可执行文件的路径
SHELL_PATH="/usr/scripts/autoLogin.sh"				 # 安装登录shell脚本的路径
vi ./autoLogin.sh
# 着重修改username, passwordPlain, ipSchoolGateway, executablePath和logPath
# 前三项含义项同本地部署
# executablePath:同EXECUTABLE_PATH
# logPath:登录日志文件的路径，建议放在/tmp以便重启清除，如/tmp/login.log

# 4.修改srun_login开机启动脚本
vi ./srun_login
# 着重修改SCRIPT：与SHELL_PATH相同

# 5.安装与授权
cp ./srunRequestBodyGeneration $EXECUTABLE_PATH
cp ./autoLogin.sh $SHELL_PATH
cp ./srun_login /etc/init.d/srun_login
chmod 755 $EXECUTABLE_PATH
chmod 755 $SHELL_PATH
chmod 755 /etc/init.d/srun_login

# 6.运行
/etc/init.d/srun_login start
/etc/init.d/srun_login enable  # 开机启动
```

  更新记录

V1.0 功能实现
V1.1 心跳包实现

V1.2 将行尾符从CRLF替换为LF，适应linux环境
V1.3 修复issue #1:将请求体中的部分内容挪入sh脚本中，方便修改

V1.4 添加install.sh脚本

## QA

### 1.为什么我不在BUAA，直接使用会连接不上？

根据issue#1，可能原因为ac_id与学校默认认证不匹配，可通过查看学校的get_challenge网页/portal网页的响应值对应进行修改。

## To Do

1.优化安装方式（打包为apk）
