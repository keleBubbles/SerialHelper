# SerialHelper

一个使用 Qt 6 和 C++ 编写的简易串口调试助手。

本项目是跟随教程完成的 Qt 学习项目，使用代码动态创建界面，适合用于学习 Qt Widgets、信号与槽、定时器以及 `QSerialPort` 的基本使用。

## 功能

- 自动扫描可用串口，每秒刷新一次串口列表
- 连接和断开串口
- 配置串口号、波特率、数据位、停止位和校验位
- 支持文本发送与接收
- 支持 HEX 发送与接收
- 支持清空发送区和接收区
- 根据串口连接状态启用或禁用相关按钮

目前支持的串口参数：

| 参数 | 可选值 |
| --- | --- |
| 波特率 | 4800、9600、19200 |
| 数据位 | 8 |
| 停止位 | 1、1.5、2 |
| 校验位 | 无、奇校验、偶校验 |
| 收发格式 | HEX、文本 |

## 开发环境

- C++
- Qt 6.5 或更高版本
- Qt Widgets
- Qt Serial Port
- CMake 3.19 或更高版本

本项目当前使用 Qt 6.10.3 和 LLVM-MinGW 工具链进行开发。

## 编译运行

### 使用 Qt Creator

1. 安装 Qt，并确保安装以下组件：
   - Qt Widgets
   - Qt Serial Port
   - CMake
   - 一个与 Qt 套件匹配的编译器
2. 使用 Qt Creator 打开项目根目录中的 `CMakeLists.txt`。
3. 选择可用的 Desktop Kit。
4. 点击“构建并运行”。

### 使用命令行

请先确保 Qt 和 CMake 已正确加入当前构建环境，然后运行：

```bash
cmake -S . -B build
cmake --build build
```

## 使用方法

1. 将串口设备连接到电脑。
2. 在“串口号”下拉框中选择设备对应的串口。
3. 根据设备要求设置波特率、数据位、停止位和校验位。
4. 点击“串口连接”。
5. 选择文本或 HEX 收发格式。
6. 在发送区输入内容，然后点击“发送”。
7. 使用完成后点击“串口断开”。

HEX 数据可以使用空格分隔，例如：

```text
48 65 6C 6C 6F
```

## 使用虚拟串口测试（macOS）

没有物理串口设备时，可以使用 `socat` 创建一对互相连接的虚拟串口：程序连接其中一个端口，终端连接另一个端口，从而测试串口数据的双向收发。

### 1. 安装 socat

使用 Homebrew 安装：

```bash
brew install socat
```

### 2. 创建虚拟串口对

打开一个终端并执行：

```bash
socat -d -d \
  pty,rawer,echo=0,link=/tmp/serial-qt \
  pty,rawer,echo=0,link=/tmp/serial-test
```

保持该终端和 `socat` 进程运行。命令会创建两个互相连接的串口路径：

```text
/tmp/serial-qt   <->   /tmp/serial-test
```

本机测试时得到的实际映射结果为：

```text
/tmp/serial-qt   -> /dev/ttys001
/tmp/serial-test -> /dev/ttys003
```

系统每次分配的 `/dev/ttysXXX` 编号可能不同，因此程序中建议使用固定的 `/tmp/serial-qt` 和 `/tmp/serial-test` 路径。

### 3. 串口助手连接虚拟串口

macOS 下，`QSerialPortInfo::availablePorts()` 通常不会自动列出 `socat` 创建的 PTY，因此本项目将串口号下拉框设置成了可编辑模式。

启动串口助手后，在“串口号”输入框中手动输入：

```text
/tmp/serial-qt
```

选择波特率 `9600`，然后点击“串口连接”。如果程序刚启动时输入内容被串口列表的首次刷新清除，请等待约 1 秒后重新输入。

### 4. 从终端收发测试数据

打开另一个终端，使用 macOS 自带的 `screen` 连接测试端：

```bash
screen /tmp/serial-test 9600
```

在 `screen` 中输入文字，串口助手的接收区将显示对应数据；在串口助手中发送数据，也会显示在 `screen` 中，从而验证双向通信。

退出 `screen` 时，依次按下 `Ctrl+A` 和 `Ctrl+\`，然后确认退出。

也可以不进入 `screen`，直接从命令行发送文本或 HEX 数据：

```bash
printf 'hello\r\n' > /tmp/serial-test
printf '\xAA\x01\x02\x55' > /tmp/serial-test
```

测试完成后，在串口助手中断开连接，并在运行 `socat` 的终端按 `Ctrl+C` 停止虚拟串口。

## 项目结构

```text
SerialHelper/
├── CMakeLists.txt       # CMake 项目配置
├── main.cpp             # 程序入口
├── serialhelper.h       # 主窗口类声明
├── serialhelper.cpp     # 界面和串口功能实现
└── README.md
```

## 后续计划

- 增加更多常用波特率和数据位选项
- 完善 HEX 输入合法性检查
- 增加发送和接收计数
- 支持定时发送
- 保存并恢复用户配置
- 优化界面布局和错误提示

## 参考资料

本项目参考以下教程完成：

- [Qt 串口助手教程（哔哩哔哩）](https://www.bilibili.com/video/BV1yT4y1n7yA/)
- UP 主：[折途想要敲代码](https://space.bilibili.com/1282656305/)

## 说明

本项目主要用于个人学习和练习。使用串口前，请确认设备的通信参数设置正确。
