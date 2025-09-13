# 连连看游戏UI界面恢复实验指令文档

## 实验目标
将连连看游戏的界面恢复到原来的蓝色窗口样式，包括菜单高亮选项效果。

## 实验环境
- 操作系统：Windows 10 (10.0.26100)
- 开发环境：Qt 6.9.0 MinGW 64-bit
- 项目路径：D:\qt\linking
- 构建系统：CMake

## 实验步骤

### 1. 恢复蓝色窗口背景
#### 1.1 菜单背景色修改
**文件：** `gamewindow.cpp`
**函数：** `drawMenu()`
**位置：** 第1414行
**操作：** 将以下代码：
```cpp
// 设置背景
painter.fillRect(rect(), QColor(30, 30, 30));
```
**替换为：**
```cpp
// 设置蓝色背景
painter.fillRect(rect(), QColor(100, 150, 255));
```

#### 1.2 游戏界面背景色修改
**文件：** `gamewindow.cpp`
**函数：** `drawGameState()`
**位置：** 第1106-1107行
**操作：** 添加以下代码：
```cpp
// 设置游戏背景为浅蓝色
painter.fillRect(rect(), QColor(200, 220, 255));
```

### 2. 恢复菜单高亮效果
#### 2.1 菜单选项高亮样式
**文件：** `gamewindow.cpp`
**函数：** `drawOption()`
**位置：** 第1463-1479行
**操作：** 将整个函数替换为：
```cpp
void GameWindow::drawOption(QPainter& painter, const QRect& rect, const QString& text, bool selected)
{
    if (selected) {
        // 选中状态 - 高亮背景（更明显的黄色高亮）
        painter.fillRect(rect, QColor(255, 255, 0, 150));
        painter.setPen(QPen(QColor(255, 0, 0), 3)); // 红色边框
        painter.drawRect(rect);
    } else {
        // 未选中状态 - 半透明背景
        painter.fillRect(rect, QColor(255, 255, 255, 50));
        painter.setPen(QPen(QColor(0, 0, 0), 1)); // 黑色边框
        painter.drawRect(rect);
    }
    
    // 绘制文字
    painter.setFont(QFont("Arial", 16, selected ? QFont::Bold : QFont::Normal));
    painter.setPen(selected ? QColor(255, 0, 0) : QColor(0, 0, 0)); // 选中时红色，未选中时黑色
    
    painter.drawText(rect, Qt::AlignCenter, text);
}
```

### 3. 恢复标题颜色
#### 3.1 游戏标题颜色修改
**文件：** `gamewindow.cpp`
**函数：** `drawTitle()`
**位置：** 第1428行
**操作：** 将以下代码：
```cpp
painter.setPen(QPen(QColor(255, 215, 0), 3)); // 金色
```
**替换为：**
```cpp
painter.setPen(QPen(QColor(255, 255, 0), 3)); // 黄色，在蓝色背景上更突出
```

## 界面效果说明

### 菜单界面
- **背景色：** 蓝色 (RGB: 100, 150, 255)
- **标题：** 黄色文字，32号粗体
- **菜单选项：**
  - 未选中：白色半透明背景，黑色文字，黑色边框
  - 选中：黄色高亮背景，红色文字，红色粗边框

### 游戏界面
- **背景色：** 浅蓝色 (RGB: 200, 220, 255)
- **保持所有游戏功能不变**

## 验证方法

### 1. 视觉验证
- 启动游戏，确认菜单背景为蓝色
- 使用方向键或鼠标移动，确认菜单选项有黄色高亮效果
- 进入游戏，确认游戏界面背景为浅蓝色

### 2. 功能验证
- 测试菜单选择功能
- 测试游戏所有功能（方块消除、道具等）
- 确认UI恢复不影响游戏逻辑

## 预期结果
1. 菜单界面恢复蓝色背景
2. 菜单选项有明显的黄色高亮效果
3. 游戏界面保持浅蓝色背景
4. 所有游戏功能正常工作
5. 界面美观，符合原始设计

## 注意事项
1. 保持所有游戏逻辑不变
2. 确保颜色搭配美观且易读
3. 高亮效果要明显但不刺眼
4. 保持代码的中文注释

## 文件修改清单
- `gamewindow.cpp`: 修改4个绘制函数
  - `drawMenu()`: 蓝色背景
  - `drawGameState()`: 浅蓝色背景
  - `drawOption()`: 高亮效果
  - `drawTitle()`: 黄色标题

## 完成时间
2024年12月19日 14:45:00
