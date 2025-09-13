# 游戏界面修复和僵尸动画增强实验方案

## 实验目标
1. 修复保存游戏和载入游戏时的背景，使用map.jpg作为背景
2. 添加僵尸在吃道具时的进食动作，和消除方块时一样的动作

## 实验环境
- 操作系统：Windows 10
- Qt版本：Qt 6.9.0 MinGW 64-bit
- 项目类型：Qt Widgets应用程序
- 编程语言：C++

## 实验步骤

### 1. 修复保存/载入游戏背景
**文件：** `gamewindow.cpp`
**问题：** 保存和载入游戏时使用了save.jpg作为背景，但应该使用map.jpg
**解决方案：** 修改paintEvent函数中的背景绘制逻辑

**修改前：**
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

**修改后：**
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

### 2. 添加僵尸吃道具的进食动作
**文件：** `gamewindow.cpp`
**问题：** 僵尸只在消除方块时有攻击动画，拾取道具时没有动画
**解决方案：** 在道具拾取逻辑中添加僵尸攻击动画触发

**修改位置：** `checkItemCollision`函数
**修改前：**
```cpp
if (it != m_items.end()) {
        activateItem(it->type, player); //激活道具效果
        m_items.erase(it); //删除碰撞的道具
        // 播放道具拾取音效
        playItemSound();
}
```

**修改后：**
```cpp
if (it != m_items.end()) {
        activateItem(it->type, player); //激活道具效果
        m_items.erase(it); //删除碰撞的道具
        // 播放道具拾取音效
        playItemSound();
        
        // 触发僵尸攻击动画（吃道具）
        triggerZombieAttackAnimation();
}
```

## 技术实现细节

### 1. 背景统一
- **统一背景图片**：保存/载入界面现在使用与游戏界面相同的map.jpg背景
- **保持一致性**：确保整个游戏的视觉风格统一
- **复用现有函数**：直接使用`drawGameBackground`函数，避免重复代码

### 2. 僵尸动画增强
- **复用现有动画**：使用已有的`triggerZombieAttackAnimation`函数
- **统一动画效果**：道具拾取和方块消除使用相同的僵尸攻击动画
- **保持音效**：道具拾取时既有音效又有动画，增强游戏体验

### 3. 动画触发时机
- **方块消除时**：当玩家成功消除方块对时触发
- **道具拾取时**：当玩家拾取任何道具时触发
- **动画持续时间**：1.5秒（与现有设置保持一致）

## 关键特性

### 1. 背景一致性
- 保存/载入界面使用map.jpg作为背景
- 与游戏主界面保持视觉统一
- 智能缩放和居中显示

### 2. 僵尸动画增强
- 道具拾取时触发僵尸攻击动画
- 与方块消除动画保持一致
- 增强游戏的视觉反馈

### 3. 代码优化
- 复用现有的背景绘制函数
- 复用现有的动画触发函数
- 保持代码简洁和可维护性

## 依赖文件
- `map.jpg` - 游戏背景图片（用于保存/载入界面）
- `zombie_eat.gif` - 僵尸攻击动画GIF
- `gamewindow.cpp` - 游戏窗口实现文件

## 编译命令
```bash
mkdir build
cd build
C:\Qt\Tools\CMake_64\bin\cmake.exe -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:\Qt\6.9.0\mingw_64 ..
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe
```

## 预期效果
1. **保存/载入界面**：显示map.jpg作为背景，与游戏界面保持一致
2. **道具拾取动画**：拾取道具时僵尸会播放攻击动画，与消除方块时的动画相同
3. **视觉统一性**：整个游戏的背景和动画效果保持统一
4. **增强体验**：更多的视觉反馈让游戏更加生动有趣

## 注意事项
1. 确保map.jpg文件存在于项目根目录
2. 确保zombie_eat.gif文件存在于项目根目录
3. 动画持续时间已设置为1.5秒，可根据需要调整
4. 背景图片会自动缩放以适应窗口大小

## 测试建议
1. 进入保存游戏界面，检查背景是否为map.jpg
2. 进入载入游戏界面，检查背景是否为map.jpg
3. 在游戏中拾取道具，检查是否触发僵尸攻击动画
4. 消除方块，检查僵尸攻击动画是否正常工作
5. 对比道具拾取和方块消除的动画效果，确保一致性
