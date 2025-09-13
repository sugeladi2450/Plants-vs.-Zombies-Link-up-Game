# 道具颜色优化和存档背景修复实验方案

## 实验目标
1. 改进道具颜色，提高区分度，使用更鲜明的颜色
2. 修复存档页面背景问题，确保使用save.jpg而不是map.jpg

## 实验环境
- 操作系统：Windows 10
- Qt版本：Qt 6.9.0 MinGW 64-bit
- 项目类型：Qt Widgets应用程序
- 编程语言：C++

## 实验步骤

### 1. 改进道具颜色区分度
**文件：** `gamewindow.cpp`
**问题：** 道具字母颜色区分度不够，需要更鲜明的颜色
**解决方案：** 优化颜色选择并添加描边效果

#### 1.1 优化颜色选择
**修改位置：** `drawItems`函数中的道具字母绘制部分
**修改前：**
```cpp
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
```

**修改后：**
```cpp
switch (item.type) {
case TIME_BONUS: 
    itemText = "T"; 
    textColor = QColor(0, 255, 0); // 亮绿色 - 时间加成
    break;
case SHUFFLE: 
    itemText = "S"; 
    textColor = QColor(255, 0, 255); // 亮紫色 - 重新排列
    break;
case HINT: 
    itemText = "H"; 
    textColor = QColor(255, 255, 0); // 亮黄色 - 提示
    break;
case DIZZY: 
    itemText = "D"; 
    textColor = QColor(255, 100, 0); // 亮橙色 - 眩晕
    break;
case FLASH: 
    itemText = "F"; 
    textColor = QColor(0, 200, 255); // 亮蓝色 - 闪烁
    break;
}
```

#### 1.2 添加描边效果
**修改前：**
```cpp
// 设置画笔颜色
painter.setPen(textColor);

// 在道具中心绘制字母
QRectF textRect = itemRect;
textRect.adjust(2, 2, -2, -2); // 稍微缩小文字区域
painter.drawText(textRect, Qt::AlignCenter, itemText);
```

**修改后：**
```cpp
// 绘制字母描边（黑色描边提高可见性）
painter.setPen(QPen(QColor(0, 0, 0), 3)); // 黑色描边，3像素宽度
QRectF textRect = itemRect;
textRect.adjust(2, 2, -2, -2); // 稍微缩小文字区域

// 先绘制描边
painter.drawText(textRect, Qt::AlignCenter, itemText);

// 再绘制主颜色
painter.setPen(QPen(textColor, 2)); // 主颜色，2像素宽度
painter.drawText(textRect, Qt::AlignCenter, itemText);
```

### 2. 修复存档页面背景问题
**文件：** `gamewindow.cpp`
**问题：** 存档页面可能没有正确使用save.jpg作为背景
**解决方案：** 添加调试信息和明显的默认背景

#### 2.1 改进loadSaveBackground函数
**修改位置：** `loadSaveBackground`函数
**修改前：**
```cpp
if (m_saveBackground.isNull()) {
    // 如果加载失败，创建一个默认背景
    m_saveBackground = QPixmap(800, 600);
    m_saveBackground.fill(QColor(40, 40, 80)); // 深蓝紫色背景
    qDebug() << "Failed to load save background image, using default background";
}
```

**修改后：**
```cpp
if (m_saveBackground.isNull()) {
    // 如果加载失败，创建一个明显的默认背景（红色，便于识别）
    m_saveBackground = QPixmap(800, 600);
    m_saveBackground.fill(QColor(255, 0, 0)); // 红色背景，便于识别问题
    qDebug() << "Failed to load save background image, using RED default background";
    qDebug() << "This indicates a problem with save.jpg loading";
}
```

#### 2.2 改进drawSaveBackground函数
**修改位置：** `drawSaveBackground`函数
**修改前：**
```cpp
} else {
    // 如果背景图片加载失败，使用默认背景色
    painter.fillRect(0, 0, widgetWidth, widgetHeight, QColor(40, 40, 80));
}
```

**修改后：**
```cpp
} else {
    // 如果背景图片加载失败，使用明显的默认背景色（红色，便于识别问题）
    painter.fillRect(0, 0, widgetWidth, widgetHeight, QColor(255, 0, 0));
    qDebug() << "Drawing RED default background for save/load interface";
}
```

## 技术实现细节

### 1. 道具颜色优化
- **高对比度颜色**：使用更鲜明的颜色提高可见性
- **描边效果**：添加黑色描边确保在任何背景下都清晰可见
- **双重绘制**：先绘制描边，再绘制主颜色，确保效果最佳

### 2. 存档背景调试
- **明显默认背景**：使用红色作为默认背景，便于识别问题
- **详细调试信息**：添加更多调试输出，帮助定位问题
- **双重检查**：在加载和绘制两个阶段都进行检查

### 3. 颜色方案
| 道具类型 | 字母 | 颜色 | RGB值 | 描边 | 效果 |
|---------|------|------|-------|------|------|
| 时间加成 | T | 亮绿色 | (0, 255, 0) | 黑色3px | 高对比度 |
| 重新排列 | S | 亮紫色 | (255, 0, 255) | 黑色3px | 高对比度 |
| 提示 | H | 亮黄色 | (255, 255, 0) | 黑色3px | 高对比度 |
| 眩晕 | D | 亮橙色 | (255, 100, 0) | 黑色3px | 高对比度 |
| 闪烁 | F | 亮蓝色 | (0, 200, 255) | 黑色3px | 高对比度 |

## 关键特性

### 1. 道具可见性增强
- **描边效果**：黑色描边确保字母在任何背景下都清晰可见
- **鲜明颜色**：使用高饱和度的颜色提高区分度
- **双重绘制**：描边和主颜色的组合确保最佳视觉效果

### 2. 背景调试功能
- **问题识别**：红色默认背景便于快速识别背景加载问题
- **调试信息**：详细的日志输出帮助定位问题原因
- **双重检查**：在加载和绘制阶段都进行问题检查

### 3. 代码健壮性
- **错误处理**：完善的错误处理和调试信息
- **可维护性**：清晰的代码结构和注释
- **可扩展性**：易于添加新的道具类型和颜色

## 依赖文件
- `save.jpg` - 存档背景图片
- `map.jpg` - 游戏背景图片
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
1. **道具颜色**：不同道具的字母使用鲜明且易区分的颜色
2. **描边效果**：字母有黑色描边，在任何背景下都清晰可见
3. **存档背景**：存档页面正确显示save.jpg背景，或显示红色背景（如果加载失败）
4. **调试信息**：控制台输出详细的背景加载信息

## 测试建议
1. 进入游戏，检查道具字母颜色是否鲜明且易区分
2. 检查道具字母是否有黑色描边效果
3. 进入存档页面，检查背景是否为save.jpg（或红色背景）
4. 查看控制台输出，确认背景加载状态
5. 测试不同道具的拾取，验证颜色区分效果

## 故障排除
1. **如果存档页面显示红色背景**：说明save.jpg加载失败，检查文件路径和资源文件
2. **如果道具字母不清晰**：检查描边效果是否正确应用
3. **如果颜色区分度不够**：可以进一步调整颜色值或描边宽度
