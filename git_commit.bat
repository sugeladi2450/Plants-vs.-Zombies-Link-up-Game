@echo off
echo 正在初始化Git仓库...
git init

echo.
echo 正在添加所有文件到Git...
git add .

echo.
echo 正在检查Git状态...
git status

echo.
echo 正在提交更改...
git commit -m "添加游戏背景音乐功能和存档页面背景修复

- 添加music2.mp3作为游戏背景音乐
- 修复存档页面背景使用save.jpg
- 优化道具字母颜色和描边效果
- 分离菜单音乐和游戏音乐的音频输出
- 添加音乐开关统一控制功能
- 完善游戏视觉体验"

echo.
echo 提交完成！
pause
