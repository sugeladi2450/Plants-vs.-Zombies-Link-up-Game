@echo off
echo Building QLink project with Qt 6.9.0...

REM 清理构建目录
if exist build rmdir /s /q build
mkdir build
cd build

REM 设置环境变量
set PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;%PATH%
set CMAKE_PREFIX_PATH=C:/Qt/6.9.0/mingw_64

REM 运行CMake配置
echo Configuring with CMake...
cmake -G "MinGW Makefiles" ..

REM 编译项目
echo Building project...
mingw32-make

echo Build completed!
pause
