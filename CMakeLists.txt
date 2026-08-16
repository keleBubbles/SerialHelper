# 指定本项目要求的最低 CMake 版本。
cmake_minimum_required(VERSION 3.19)

# 定义项目名称，并声明项目使用 C++ 语言。
project(SerialHelper LANGUAGES CXX)

# 查找 Qt 6.5 或更高版本，并要求安装 Core、Widgets 和 SerialPort 模块。
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets SerialPort)

# 应用 Qt 推荐的通用项目设置，例如自动处理 moc 和 uic。
qt_standard_project_setup()

# 创建名为 SerialHelper 的可执行程序。
# WIN32：在 Windows 上构建为 GUI 程序，不显示命令行窗口。
# MACOSX_BUNDLE：在 macOS 上构建为 .app 应用程序包。
# 其余条目是参与编译的源文件和头文件。
qt_add_executable(SerialHelper
    WIN32 MACOSX_BUNDLE
    main.cpp
    serialhelper.h serialhelper.cpp
)

# 链接程序需要的 Qt 模块。
# PRIVATE 表示这些依赖只供 SerialHelper 自身使用。
target_link_libraries(SerialHelper
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::SerialPort
)

# 定义安装位置：应用包放在当前目录，可执行文件和动态库放在标准目录。
install(TARGETS SerialHelper
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 生成部署脚本，用来收集应用运行所需的 Qt 库和插件。
qt_generate_deploy_app_script(
    TARGET SerialHelper
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

# 在执行安装时运行上面生成的部署脚本。
install(SCRIPT ${deploy_script})
