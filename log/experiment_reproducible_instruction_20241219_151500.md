# 游戏界面美化实验方案

## 实验目标
1. 将save.jpg作为保存和载入游戏时的背景
2. 将brain.jpg作为道具的形状，字母在其上绘制，以分辨不同的道具

## 实验环境
- 操作系统：Windows 10
- Qt版本：Qt 6.9.0 MinGW 64-bit
- 项目类型：Qt Widgets应用程序
- 编程语言：C++

## 实验步骤

### 1. 资源文件配置
**文件：** `resources.qrc`
**操作：** 在资源文件中添加save.jpg和brain.jpg
```xml
<file>save.jpg</file>
<file>brain.jpg</file>
```

### 2. 头文件修改
**文件：** `gamewindow.h`
**操作：** 
- 添加保存/载入背景图片和大脑道具图片成员变量：
```cpp
QPixmap m_saveBackground;               // 保存/载入背景图片
QPixmap m_brainPropImage;               // 大脑道具图片
```
- 添加加载函数声明：
```cpp
void loadSaveBackground();
void loadBrainPropImage();
```
- 添加绘制函数声明：
```cpp
void drawSaveBackground(QPainter& painter, int widgetWidth, int widgetHeight);
```

### 3. 源文件实现
**文件：** `gamewindow.cpp`

#### 3.1 构造函数修改
在构造函数中初始化新的成员变量：
```cpp
, m_saveBackground() // 保存/载入背景图片
, m_brainPropImage() // 大脑道具图片
```

在构造函数中调用加载函数：
```cpp
// 加载保存/载入背景图片
loadSaveBackground();

// 加载大脑道具图片
loadBrainPropImage();
```

#### 3.2 添加loadSaveBackground函数
```cpp
// 加载保存/载入背景图片
void GameWindow::loadSaveBackground()
{
    // 加载保存/载入背景图片
    m_saveBackground = QPixmap(":/save.jpg");
    
    if (m_saveBackground.isNull()) {
        // 如果加载失败，创建一个默认背景
        m_saveBackground = QPixmap(800, 600);
        m_saveBackground.fill(QColor(40, 40, 80)); // 深蓝紫色背景
        qDebug() << "Failed to load save background image, using default background";
    } else {
        // 输出背景图片信息用于调试
        qDebug() << "Save background image loaded successfully:";
        qDebug() << "Size:" << m_saveBackground.size();
        qDebug() << "Aspect ratio:" << (double)m_saveBackground.width() / m_saveBackground.height();
    }
}
```

#### 3.3 添加loadBrainPropImage函数
```cpp
// 加载大脑道具图片
void GameWindow::loadBrainPropImage()
{
    // 加载大脑道具图片
    m_brainPropImage = QPixmap(":/brain.jpg");
    
    if (m_brainPropImage.isNull()) {
        // 如果加载失败，创建一个默认背景
        m_brainPropImage = QPixmap(32, 32);
        m_brainPropImage.fill(QColor(200, 150, 200)); // 浅紫色背景
        qDebug() << "Failed to load brain prop image, using default background";
    } else {
        // 输出背景图片信息用于调试
        qDebug() << "Brain prop image loaded successfully:";
        qDebug() << "Size:" << m_brainPropImage.size();
        qDebug() << "Aspect ratio:" << (double)m_brainPropImage.width() / m_brainPropImage.height();
    }
}
```

#### 3.4 修改paintEvent函数
在保存/载入界面绘制部分添加背景图片：
```cpp
// 检查是否显示存档槽位选择界面
if (m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
    // 绘制保存/载入背景图片
    drawSaveBackground(painter, widgetWidth, widgetHeight - menuHeight);
    
    // 调整绘制区域为游戏区域
    painter.translate(0, menuHeight);
    drawMenu(painter);
    return;
}
```

#### 3.5 添加drawSaveBackground函数
```cpp
//绘制保存/载入背景
void GameWindow::drawSaveBackground(QPainter& painter, int widgetWidth, int widgetHeight)
{
    // 绘制保存/载入背景图片
    if (!m_saveBackground.isNull()) {
        // 计算背景图片的缩放比例，保持宽高比
        QSize imageSize = m_saveBackground.size();
        double imageRatio = (double)imageSize.width() / imageSize.height();
        double widgetRatio = (double)widgetWidth / widgetHeight;
        
        QPixmap scaledBackground;
        QPoint drawPos(0, 0);
        
        if (imageRatio > widgetRatio) {
            // 图片更宽，以高度为准进行缩放
            int scaledHeight = widgetHeight;
            int scaledWidth = static_cast<int>(widgetHeight * imageRatio);
            scaledBackground = m_saveBackground.scaled(scaledWidth, scaledHeight, 
                                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
            
            // 水平居中
            if (scaledBackground.width() > widgetWidth) {
                drawPos.setX((scaledBackground.width() - widgetWidth) / 2);
            }
        } else {
            // 图片更高，以宽度为准进行缩放
            int scaledWidth = widgetWidth;
            int scaledHeight = static_cast<int>(widgetWidth / imageRatio);
            scaledBackground = m_saveBackground.scaled(scaledWidth, scaledHeight, 
                                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
            
            // 垂直居中
            if (scaledBackground.height() > widgetHeight) {
                drawPos.setY((scaledBackground.height() - widgetHeight) / 2);
            }
        }
        
        // 绘制背景图片（只绘制窗口范围内的部分）
        painter.drawPixmap(0, 0, scaledBackground, drawPos.x(), drawPos.y(), widgetWidth, widgetHeight);
        
        // 添加一个半透明的遮罩层，让背景图片稍微暗一些，突出菜单选项
        painter.fillRect(0, 0, widgetWidth, widgetHeight, QColor(0, 0, 0, 40));
    } else {
        // 如果背景图片加载失败，使用默认背景色
        painter.fillRect(0, 0, widgetWidth, widgetHeight, QColor(40, 40, 80));
    }
}
```

#### 3.6 修改drawItems函数
使用大脑形状并绘制字母：
```cpp
//绘制道具函数
void GameWindow::drawItems(QPainter& painter, float cellWidth, float cellHeight)
{
    // 绘制道具 - 使用大脑形状并绘制字母
    for (const auto& item : m_items) {
        QRectF itemRect(item.col * cellWidth, item.row * cellHeight,
                       cellWidth, cellHeight);
        itemRect.adjust(3, 3, -3, -3);
        
        // 绘制大脑形状的道具
        if (!m_brainPropImage.isNull()) {
            // 使用大脑图片作为道具形状
            painter.drawPixmap(itemRect, m_brainPropImage, m_brainPropImage.rect());
        } else {
            // 如果大脑图片加载失败，使用默认的椭圆形状
            QColor itemColor;
            switch (item.type) {
            case TIME_BONUS: itemColor = QColor(0x00ff00); break;  // 绿色
            case SHUFFLE: itemColor = QColor(0xff00ff); break;     // 紫色
            case HINT: itemColor = QColor(0xffff00); break;        // 黄色
            case DIZZY: itemColor = QColor(0xff8800); break;       // 橙色
            case FLASH: itemColor = QColor(0x00ffff); break;       // 青色
            }
            
            painter.setBrush(itemColor);
            painter.setPen(QPen(QColorConstants::Svg::black, 2));
            painter.drawEllipse(itemRect); //绘制道具，形状为椭圆
        }
        
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
    }
}
```

## 关键特性

### 1. 保存/载入背景
- 使用save.jpg作为保存和载入游戏时的背景
- 智能缩放保持宽高比
- 居中显示，超出部分自动裁剪
- 添加半透明遮罩层突出菜单选项

### 2. 大脑形状道具
- 使用brain.jpg作为道具的形状
- 在道具上绘制字母标识（T、S、H、D、F）
- 白色字母在深色背景上更清晰
- 如果大脑图片加载失败，回退到默认椭圆形状

### 3. 道具类型标识
- T - 时间加成道具（Time Bonus）
- S - 重新排列道具（Shuffle）
- H - 提示道具（Hint）
- D - 眩晕道具（Dizzy）
- F - 闪烁道具（Flash）

## 依赖文件
- `save.jpg` - 保存/载入背景图片
- `brain.jpg` - 大脑道具图片
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
1. 保存和载入游戏时，界面显示save.jpg作为背景
2. 游戏中的道具显示为大脑形状，上面有相应的字母标识
3. 背景图片自动缩放并居中显示
4. 道具字母清晰可见，便于玩家识别不同道具类型
5. 整体视觉效果更加美观和统一

## 注意事项
1. 确保save.jpg和brain.jpg文件存在于项目根目录
2. 背景图片建议使用与游戏风格相符的图片
3. 大脑道具图片建议使用透明背景的PNG格式
4. 如果图片加载失败，会使用默认的备用方案
5. 道具字母颜色和大小可以根据实际效果进行调整
