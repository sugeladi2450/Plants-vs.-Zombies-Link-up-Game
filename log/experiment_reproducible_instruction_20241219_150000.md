# 游戏地图背景设置实验方案

## 实验目标
将map.jpg作为进入游戏后的地图背景，考虑美观因素，使整个地图看起来不违和。

## 实验环境
- 操作系统：Windows 10
- Qt版本：Qt 6.9.0 MinGW 64-bit
- 项目类型：Qt Widgets应用程序
- 编程语言：C++

## 实验步骤

### 1. 资源文件配置
**文件：** `resources.qrc`
**操作：** 在资源文件中添加map.jpg
```xml
<file>map.jpg</file>
```

### 2. 头文件修改
**文件：** `gamewindow.h`
**操作：** 
- 添加游戏背景图片成员变量：
```cpp
QPixmap m_gameBackground;               // 游戏背景图片
```
- 添加加载游戏背景图片的函数声明：
```cpp
void loadGameBackground();
```
- 添加绘制游戏背景的函数声明：
```cpp
void drawGameBackground(QPainter& painter, int widgetWidth, int widgetHeight);
```

### 3. 源文件实现
**文件：** `gamewindow.cpp`

#### 3.1 构造函数修改
在构造函数中初始化游戏背景图片成员变量：
```cpp
, m_gameBackground() // 游戏背景图片
```

在构造函数中调用加载函数：
```cpp
// 加载游戏背景图片
loadGameBackground();
```

#### 3.2 添加loadGameBackground函数
```cpp
// 加载游戏背景图片
void GameWindow::loadGameBackground()
{
    // 加载游戏背景图片
    m_gameBackground = QPixmap(":/map.jpg");
    
    if (m_gameBackground.isNull()) {
        // 如果加载失败，创建一个默认背景
        m_gameBackground = QPixmap(800, 600);
        m_gameBackground.fill(QColor(30, 30, 60)); // 深蓝灰色背景
        qDebug() << "Failed to load game background image, using default background";
    } else {
        // 输出背景图片信息用于调试
        qDebug() << "Game background image loaded successfully:";
        qDebug() << "Size:" << m_gameBackground.size();
        qDebug() << "Aspect ratio:" << (double)m_gameBackground.width() / m_gameBackground.height();
    }
}
```

#### 3.3 修改drawGameState函数
在绘制游戏元素前先绘制背景：
```cpp
// 绘制游戏背景图片
drawGameBackground(painter, widgetWidth, gameHeight);
```

#### 3.4 添加drawGameBackground函数
```cpp
//绘制游戏背景
void GameWindow::drawGameBackground(QPainter& painter, int widgetWidth, int widgetHeight)
{
    // 绘制游戏背景图片
    if (!m_gameBackground.isNull()) {
        // 计算背景图片的缩放比例，保持宽高比
        QSize imageSize = m_gameBackground.size();
        double imageRatio = (double)imageSize.width() / imageSize.height();
        double widgetRatio = (double)widgetWidth / widgetHeight;
        
        QPixmap scaledBackground;
        QPoint drawPos(0, 0);
        
        if (imageRatio > widgetRatio) {
            // 图片更宽，以高度为准进行缩放
            int scaledHeight = widgetHeight;
            int scaledWidth = static_cast<int>(widgetHeight * imageRatio);
            scaledBackground = m_gameBackground.scaled(scaledWidth, scaledHeight, 
                                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
            
            // 水平居中
            if (scaledBackground.width() > widgetWidth) {
                drawPos.setX((scaledBackground.width() - widgetWidth) / 2);
            }
        } else {
            // 图片更高，以宽度为准进行缩放
            int scaledWidth = widgetWidth;
            int scaledHeight = static_cast<int>(widgetWidth / imageRatio);
            scaledBackground = m_gameBackground.scaled(scaledWidth, scaledHeight, 
                                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
            
            // 垂直居中
            if (scaledBackground.height() > widgetHeight) {
                drawPos.setY((scaledBackground.height() - widgetHeight) / 2);
            }
        }
        
        // 绘制背景图片（只绘制窗口范围内的部分）
        painter.drawPixmap(0, 0, scaledBackground, drawPos.x(), drawPos.y(), widgetWidth, widgetHeight);
        
        // 添加一个半透明的遮罩层，让背景图片稍微暗一些，突出游戏元素
        painter.fillRect(0, 0, widgetWidth, widgetHeight, QColor(0, 0, 0, 30));
    } else {
        // 如果背景图片加载失败，使用默认背景色
        painter.fillRect(0, 0, widgetWidth, widgetHeight, QColor(30, 30, 60));
    }
}
```

#### 3.5 优化方块透明度
修改drawBlockImage函数，让方块有一定的透明度：
```cpp
// 绘制图片，缩放到方块大小，添加透明度让背景透过来
painter.setOpacity(0.85); // 设置85%的不透明度，让背景图片能够透过来
painter.drawPixmap(rect, pixmap, pixmap.rect());
painter.setOpacity(1.0); // 恢复完全不透明
```

## 关键特性

### 1. 智能缩放
- 保持背景图片的宽高比
- 根据窗口尺寸自动选择缩放策略
- 使用平滑变换算法确保图片质量

### 2. 居中显示
- 背景图片在窗口中居中显示
- 超出窗口部分自动裁剪

### 3. 视觉效果优化
- 添加半透明遮罩层，让背景稍微暗一些
- 方块设置85%透明度，让背景透过来
- 保持游戏元素的清晰可见性

### 4. 错误处理
- 背景图片加载失败时使用默认背景色
- 输出调试信息便于问题排查

## 依赖文件
- `map.jpg` - 游戏背景图片
- `resources.qrc` - Qt资源文件
- `gamewindow.h` - 游戏窗口头文件
- `gamewindow.cpp` - 游戏窗口实现文件

## 编译命令
```bash
mkdir build
cd build
cmake ..
make
```

## 预期效果
1. 游戏进入后，地图区域显示map.jpg作为背景
2. 背景图片自动缩放并居中显示
3. 方块有一定的透明度，背景图片能够透过来
4. 整体视觉效果美观且不违和
5. 游戏元素（方块、玩家、道具等）仍然清晰可见

## 注意事项
1. 确保map.jpg文件存在于项目根目录
2. 背景图片建议使用与游戏风格相符的图片
3. 如果背景图片过大，可能会影响游戏性能
4. 透明度设置可以根据实际效果进行调整
