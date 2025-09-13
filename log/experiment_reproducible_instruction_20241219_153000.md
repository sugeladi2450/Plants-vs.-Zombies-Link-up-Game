# 游戏界面背景和道具颜色优化实验方案

## 实验目标
1. 修复存档页面背景，应该使用save.jpg而不是map.jpg（只有菜单页面使用map.jpg）
2. 改进道具字母颜色，使用不同颜色便于区分，不要都用白色

## 实验环境
- 操作系统：Windows 10
- Qt版本：Qt 6.9.0 MinGW 64-bit
- 项目类型：Qt Widgets应用程序
- 编程语言：C++

## 实验步骤

### 1. 修复存档页面背景
**文件：** `gamewindow.cpp`
**问题：** 存档页面使用了map.jpg作为背景，应该使用save.jpg
**解决方案：** 修改paintEvent函数中的背景绘制逻辑

**修改位置：** `paintEvent`函数中的存档槽位选择界面部分
**修改前：**
```cpp
// 检查是否显示存档槽位选择界面
if (m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
    // 绘制游戏背景图片（使用map.jpg）
    drawGameBackground(painter, widgetWidth, widgetHeight - menuHeight);
    
    // 调整绘制区域为游戏区域
    painter.translate(0, menuHeight);
    drawMenu(painter);
    return;
}
```

**修改后：**
```cpp
// 检查是否显示存档槽位选择界面
if (m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
    // 绘制保存/载入背景图片（使用save.jpg）
    drawSaveBackground(painter, widgetWidth, widgetHeight - menuHeight);
    
    // 调整绘制区域为游戏区域
    painter.translate(0, menuHeight);
    drawMenu(painter);
    return;
}
```

### 2. 改进道具字母颜色
**文件：** `gamewindow.cpp`
**问题：** 所有道具字母都使用白色，不便于区分
**解决方案：** 为不同类型的道具使用不同的颜色

**修改位置：** `drawItems`函数中的道具字母绘制部分
**修改前：**
```cpp
// 绘制道具字母标识
painter.setPen(QColorConstants::Svg::white); //设置画笔颜色为白色，在深色背景上更清晰
painter.setFont(QFont("Arial", 10, QFont::Bold)); //设置字体，稍微大一些
QString itemText;
switch (item.type) {
case TIME_BONUS: itemText = "T"; break;  // 时间加成
case SHUFFLE: itemText = "S"; break;     // 重新排列
case HINT: itemText = "H"; break;        // 提示
case DIZZY: itemText = "D"; break;       // 眩晕
case FLASH: itemText = "F"; break;       // 闪烁
}

// 在道具中心绘制字母
QRectF textRect = itemRect;
textRect.adjust(2, 2, -2, -2); // 稍微缩小文字区域
painter.drawText(textRect, Qt::AlignCenter, itemText);
```

**修改后：**
```cpp
// 绘制道具字母标识
painter.setFont(QFont("Arial", 10, QFont::Bold)); //设置字体，稍微大一些
QString itemText;
QColor textColor;

switch (item.type) {
case TIME_BONUS: 
    itemText = "T"; 
    textColor = QColor(0, 255, 0); // 绿色 - 时间加成
    break;
case SHUFFLE: 
    itemText = "S"; 
    textColor = QColor(255, 0, 255); // 紫色 - 重新排列
    break;
case HINT: 
    itemText = "H"; 
    textColor = QColor(255, 255, 0); // 黄色 - 提示
    break;
case DIZZY: 
    itemText = "D"; 
    textColor = QColor(255, 128, 0); // 橙色 - 眩晕
    break;
case FLASH: 
    itemText = "F"; 
    textColor = QColor(0, 255, 255); // 青色 - 闪烁
    break;
}

// 设置画笔颜色
painter.setPen(textColor);

// 在道具中心绘制字母
QRectF textRect = itemRect;
textRect.adjust(2, 2, -2, -2); // 稍微缩小文字区域
painter.drawText(textRect, Qt::AlignCenter, itemText);
```

## 技术实现细节

### 1. 背景图片使用规则
- **菜单页面**：使用map.jpg作为背景
- **存档页面**：使用save.jpg作为背景
- **游戏页面**：使用map.jpg作为背景

### 2. 道具字母颜色方案
- **T (时间加成)**：绿色 (0, 255, 0) - 代表生命/时间
- **S (重新排列)**：紫色 (255, 0, 255) - 代表神秘/魔法
- **H (提示)**：黄色 (255, 255, 0) - 代表智慧/提示
- **D (眩晕)**：橙色 (255, 128, 0) - 代表警告/危险
- **F (闪烁)**：青色 (0, 255, 255) - 代表速度/移动

### 3. 颜色选择原则
- **高对比度**：确保在深色背景上清晰可见
- **语义化**：颜色与道具功能相关联
- **易于区分**：不同道具使用明显不同的颜色
- **视觉和谐**：颜色搭配协调，不会产生视觉冲突

## 关键特性

### 1. 背景图片分离
- 菜单和游戏使用map.jpg，保持游戏主题一致
- 存档页面使用save.jpg，提供不同的视觉体验
- 智能缩放和居中显示

### 2. 道具颜色区分
- 每种道具类型有独特的颜色标识
- 颜色与道具功能语义相关
- 提高游戏可玩性和用户体验

### 3. 代码优化
- 使用switch语句清晰管理不同颜色
- 保持代码可读性和可维护性
- 便于后续添加新的道具类型

## 依赖文件
- `map.jpg` - 游戏背景图片（用于菜单和游戏界面）
- `save.jpg` - 存档背景图片（用于存档页面）
- `brain.png` - 大脑道具图片
- `gamewindow.cpp` - 游戏窗口实现文件

## 编译命令
```bash
mkdir build
cd build
C:\Qt\Tools\CMake_64\bin\cmake.exe -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:\Qt\6.9.0\mingw_64 ..
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe
```

## 预期效果
1. **存档页面背景**：显示save.jpg作为背景，与菜单页面区分
2. **道具颜色区分**：不同道具的字母使用不同颜色，便于玩家识别
3. **视觉层次**：通过不同的背景和颜色创建清晰的视觉层次
4. **用户体验**：提高游戏的可玩性和易用性

## 注意事项
1. 确保save.jpg和map.jpg文件存在于项目根目录
2. 确保brain.png文件存在于项目根目录
3. 颜色选择考虑了在深色背景上的可见性
4. 可以根据实际效果调整颜色值

## 测试建议
1. 进入菜单页面，检查背景是否为map.jpg
2. 进入存档页面，检查背景是否为save.jpg
3. 进入游戏，检查道具字母颜色是否正确
4. 测试不同道具的拾取，验证颜色区分效果
5. 检查颜色在深色背景上的可见性
