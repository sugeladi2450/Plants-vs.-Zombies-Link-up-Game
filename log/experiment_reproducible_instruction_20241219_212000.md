# 存档页面背景修复实验方案

## 实验目标
修复存档页面背景显示问题，确保存档页面使用save.jpg作为背景，而不是map.jpg。

## 问题分析
通过分析代码发现，问题出现在paintEvent函数中：
1. 虽然调用了drawSaveBackground函数绘制save.jpg背景
2. 但是drawMenu函数中的drawOptions函数会覆盖背景，绘制菜单背景图片（map.jpg）

## 解决方案
1. **创建新的绘制函数**：
   - `drawSaveMenu()`: 绘制存档菜单，不绘制背景图片
   - `drawSaveOptions()`: 绘制存档菜单选项，不绘制背景图片

2. **修改paintEvent逻辑**：
   - 存档页面使用drawSaveMenu()而不是drawMenu()
   - 确保存档页面只绘制save.jpg背景

## 关键代码修改

### 1. paintEvent函数修改
```cpp
// 检查是否显示存档槽位选择界面
if (m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
    // 绘制保存/载入背景图片（使用save.jpg）
    drawSaveBackground(painter, widgetWidth, widgetHeight - menuHeight);
    
    // 调整绘制区域为游戏区域
    painter.translate(0, menuHeight);
    drawSaveMenu(painter);  // 使用新的存档菜单绘制函数
    return;
}
```

### 2. 新增函数
- `drawSaveMenu()`: 绘制存档菜单界面，不绘制背景图片
- `drawSaveOptions()`: 绘制存档菜单选项，不绘制背景图片

### 3. 头文件声明
在gamewindow.h中添加新函数声明：
```cpp
void drawSaveMenu(QPainter& painter);
void drawSaveOptions(QPainter& painter);
```

## 构建命令
```bash
# 清理构建目录
rmdir /s build
mkdir build
cd build

# 配置项目
C:\Qt\Tools\CMake_64\bin\cmake.exe -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:\Qt\6.9.0\mingw_64 ..

# 构建项目
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe
```

## 预期结果
- 存档页面显示save.jpg背景
- 菜单页面显示map.jpg背景
- 游戏页面显示map.jpg背景
- 道具使用brain.png形状，字母有不同颜色和描边

## 测试步骤
1. 运行QLink.exe
2. 进入存档页面（保存游戏或载入游戏）
3. 验证背景是否为save.jpg
4. 返回主菜单，验证背景是否为map.jpg
5. 进入游戏，验证背景是否为map.jpg

## 文件依赖
- `resources.qrc`: 包含save.jpg和map.jpg资源
- `gamewindow.h`: 函数声明
- `gamewindow.cpp`: 主要实现
- `save.jpg`: 存档页面背景图片
- `map.jpg`: 游戏和菜单背景图片

## 注意事项
- 确保save.jpg文件存在于项目根目录
- 确保resources.qrc正确包含save.jpg
- 构建前清理build目录
- 使用正确的Qt路径和编译器

## 成功标志
- 项目构建成功，生成QLink.exe
- 存档页面显示正确的背景图片
- 无编译错误和警告
