# 🌳🌳🌳 BinHub 🌳🌳🌳
## 📄 一、简介
个人命令行工具集，收集了一些有意思的命令行工具，编写了部分可执行程序和脚本，为部分 Windows 命令和 GNU/Linux 命令编写了别名（单独的文件）

这些可执行性程序大部分是单文件，只添加一次环境变量，在命令行中就可使用相应目录下的所有可执行文件，实现集中式管理

主要是用于管理自己编写的命令，对于需要在 Windows 上使用 Linux 命令的，推荐使用 `Cygwin` 、 `MSYS2-MINGW64` 或 `WSL2`

## 🛠️ 二、安装步骤

### 🐧 GNU/Linux 发行版系统

使用命令 `sudo vim /etc/profile` 编辑配置文件，添加如下内容（自行决定安装路径）:

```bash
# BinHub 
export BINHUB_HOME=/home/your_username/binhub
export PATH=$BINHUB_HOME/bin:$PATH
```
使用命令 `source /etc/profile` 更新配置

### 🪟 Windows 系统
- 方法一

> 新建变量名为 `BINHUB_HOME` 的系统环境变量 ，变量值为 BINHUB 解压缩目录的根目录；
>
> 编辑系统`Path`值，在 `Path`值 尾部追加`;%BINHUB_HOME%\bin`；
>
> 具体步骤：
>> 使用快捷键 `WIN + R` 打开 `运行`，输入 `sysdm.cpl` 回车打开 `系统属性`
>
>> 依次点击 `高级` - `环境变量(N)`，点击 `系统变量(S)` 下方的 `新建(W)`
>
>> `变量名(N)` 填 `BINHUB_HOME`，`变量值(V)` 填安装 BinHub 项目的目录路径，例如 `D:\app\binhub`
>
>> 编辑 `Path` ，新建项：`%BINHUB_HOME%\bin`
（ 对于老版本 Windows ，在 `Path` 值尾部追加 `;%BINHUB_HOME%\bin` ）

- 方法二

> 将压缩包解压在要安装的目录下，运行项目根目录下的 `setenv.exe` （注意：不要修改项目的目录结构，确保 `setenv.exe` 始终在项目的根目录下），输入`i`或`I`继续，结束后即可使用工具集中的命令了。


## 🧹️ 三、卸载步骤

### 🐧 GNU/Linux 发行版系统
> 使用命令 `sudo vim /etc/profile` 编辑配置文件，删除相应配置

> 使用命令 `sudo rm /home/your_username/binhub -rf` 删除目录及文件

### 🪟 Windows 系统
- 方法一
> 编辑 `Path`，删除其中的 `%BINHUB_HOME%\bin` 

> 删除 `BINHUB_HOME`

> 删除项目文件

- 方法二
> 运行项目根目录下的`setenv.exe`，输入`u`或`U`删除环境变量。删除项目文件。

## 📁 四、目录结构
经配置环境变量后，Windows 可通过命令 `echo %BINHUB_HOME%` 或 `Write-Output $env:BINHUB_HOME` 查看项目根目录的路径，前者为 `CMD` 的语法，后者为 `Powershell` 或 `pwsh` 的语法。
Linux 可通过命令 `echo $BINHUB_HOME` 查看项目根目录的路径。

🔔注意：在使用 `bin/subbin/` （或 `bin\subbin\`） 目录下的命令时，需要在这些命令前面加上`use `。`subbin` 相当于隔离环境，在不添加环境变量的情况下，系统不会自动搜索该目录下的命令，可使用 `use` 命令运行这个目录下的程序。

`bin/subbin/` 目录可能包括以下类型的程序:

    (1) 收集到的有趣的 CLI、TUI、GUI 小游戏或工具
    (2) 来源于第三方的 CLI、TUI、GUI 程序或脚本；
    (3) 不容易记住文件名的 CLI、TUI、GUI 程序或脚本（可以使用 `use -l` 列出目录下文件）；
    (4) 可能与环境变量中名称发生冲突的 CLI、TUI、GUI 程序或脚本；
    (5) 老版本复古情怀命令行程序；
    (6) 自己写的但是已经丢失了源代码、且暂时不愿意重写的程序；


`bin/subbin/` 目录下的程序和 `bin/` 目录下的程序没有本质区别。
最初设计这个子目录，但不添加至环境变量中的主要目的是——避免这些程序和系统中一些同名的自建命令和函数冲突。
故使用 `use` （或 `use.exe`）作为中间媒介传递参数或选项，确保使用的是 `bin/subbin/` 目录下的程序，而非系统中其他同名程序或函数。

```
binhub/
├─bin/			executable(ELF, bash scripts, python | PE, cmd batch, powershell ps1, vbs, lnk, python)
│  └─subbin/
├─docs/			documents
├─res/			resources
└─src/			sources
```

## 🔗 五、其他
- 项目地址 https://github.com/ZinTi/binhub
