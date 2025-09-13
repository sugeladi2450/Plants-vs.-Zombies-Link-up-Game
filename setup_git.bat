@echo off
echo 正在设置Git配置...

echo.
echo 请先安装Git for Windows: https://git-scm.com/download/win
echo 安装完成后，请重新运行此脚本。

echo.
echo 设置Git用户信息...
git config --global user.name "QLink Developer"
git config --global user.email "qlink@example.com"

echo.
echo 设置Git换行符处理...
git config --global core.autocrlf true
git config --global core.safecrlf true

echo.
echo 设置Git编码...
git config --global core.quotepath false
git config --global i18n.commitencoding utf-8
git config --global i18n.logoutputencoding utf-8

echo.
echo Git配置完成！
echo 现在可以运行 git_commit.bat 来提交更改。

pause
