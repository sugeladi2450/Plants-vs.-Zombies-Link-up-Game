@echo off
echo Building QLink project...

REM 清理旧的构建目录
if exist build rmdir /s /q build

REM 创建构建目录
mkdir build
cd build

REM 配置CMake
C:\Qt\Tools\CMake_64\bin\cmake.exe -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:\Qt\6.9.0\mingw_64 ..

REM 编译项目
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe

echo Build completed!
pause
