# 玩家2独立GIF动画系统实验记录

## 实验目标
为玩家2添加独立的GIF动画系统，使用zombie2.gif和zombie2_eat.gif，与玩家1的zombie.gif和zombie_eat.gif完全相同的逻辑，只是使用不同的图片资源。

## 实验步骤

### 1. 添加玩家2的GIF相关成员变量
在`gamewindow.h`中添加玩家2的GIF相关成员变量：
```cpp
// 玩家2 GIF动图相关
QMovie* m_zombie2Movie;                 // 玩家2僵尸GIF动图
QMovie* m_zombie2EatMovie;              // 玩家2僵尸攻击GIF动图
QPixmap m_currentZombie2Frame;          // 玩家2当前僵尸帧
QPixmap m_currentZombie2EatFrame;       // 玩家2当前僵尸攻击帧
bool m_isZombie2Attacking;              // 玩家2僵尸是否正在攻击
QTimer* m_attack2Timer;                 // 玩家2攻击动画定时器
```

### 2. 更新构造函数初始化
在`GameWindow`构造函数中初始化玩家2的GIF变量：
```cpp
// 玩家2 GIF动图相关
, m_zombie2Movie(nullptr) // 玩家2僵尸GIF动图
, m_zombie2EatMovie(nullptr) // 玩家2僵尸攻击GIF动图
, m_currentZombie2Frame() // 玩家2当前僵尸帧
, m_currentZombie2EatFrame() // 玩家2当前僵尸攻击帧
, m_isZombie2Attacking(false) // 玩家2僵尸未在攻击
, m_attack2Timer(new QTimer(this)) // 玩家2攻击动画定时器
```

### 3. 创建玩家2的GIF初始化函数
实现`initializeZombie2Animation()`函数：
- 加载zombie2.gif作为普通动画
- 加载zombie2_eat.gif作为攻击动画
- 设置帧改变信号连接
- 配置攻击动画定时器

### 4. 创建玩家2的攻击动画触发函数
实现`triggerZombie2AttackAnimation()`函数：
- 停止普通动画
- 启动攻击动画
- 设置攻击状态
- 启动攻击定时器

### 5. 修改drawPlayer函数
更新`drawPlayer`函数，根据玩家ID选择不同的GIF：
```cpp
if (playerId == 1) {
    // 玩家1使用zombie.gif和zombie_eat.gif
    hasValidGif = (m_zombieMovie && m_zombieMovie->isValid() && !m_currentZombieFrame.isNull()) ||
                 (m_zombieEatMovie && m_zombieEatMovie->isValid() && !m_currentZombieEatFrame.isNull());
    isAttacking = m_isZombieAttacking;
    // ... 设置对应的帧
} else if (playerId == 2) {
    // 玩家2使用zombie2.gif和zombie2_eat.gif
    hasValidGif = (m_zombie2Movie && m_zombie2Movie->isValid() && !m_currentZombie2Frame.isNull()) ||
                 (m_zombie2EatMovie && m_zombie2EatMovie->isValid() && !m_currentZombie2EatFrame.isNull());
    isAttacking = m_isZombie2Attacking;
    // ... 设置对应的帧
}
```

### 6. 更新攻击动画触发逻辑
修改攻击动画触发逻辑，根据当前激活的玩家选择不同的攻击动画：
- 道具拾取时：根据玩家ID选择对应的攻击动画
- 方块消除时：根据当前激活玩家选择对应的攻击动画

### 7. 添加资源文件
在`resources.qrc`中添加玩家2的GIF文件：
```xml
<file>zombie2.gif</file>
<file>zombie2_eat.gif</file>
```

## 修改的文件
- `gamewindow.h` - 添加玩家2的GIF相关成员变量和函数声明
- `gamewindow.cpp` - 实现玩家2的GIF系统，修改绘制和攻击逻辑
- `resources.qrc` - 添加玩家2的GIF资源文件

## 技术实现细节

### 1. 独立的动画系统
- 玩家1和玩家2拥有完全独立的GIF动画系统
- 每个玩家都有自己的普通动画和攻击动画
- 每个玩家都有自己的攻击状态和定时器

### 2. 智能的动画选择
- 根据玩家ID自动选择对应的GIF资源
- 根据攻击状态自动切换普通/攻击动画
- 支持两个玩家同时进行不同的动画

### 3. 资源管理
- 使用Qt的资源系统管理GIF文件
- 支持GIF文件的动态加载和缓存
- 自动处理GIF加载失败的情况

## 新增功能
1. **玩家2独立GIF动画**：玩家2使用zombie2.gif和zombie2_eat.gif
2. **智能动画切换**：根据玩家ID和状态自动选择正确的动画
3. **独立攻击系统**：每个玩家都有独立的攻击动画和定时器
4. **资源文件支持**：在资源文件中添加了玩家2的GIF文件

## 动画逻辑
- **普通状态**：玩家1显示zombie.gif，玩家2显示zombie2.gif
- **攻击状态**：玩家1显示zombie_eat.gif，玩家2显示zombie2_eat.gif
- **攻击触发**：拾取道具或消除方块时触发对应玩家的攻击动画
- **动画恢复**：攻击动画结束后自动恢复普通动画

## 实验时间
2024年12月19日 23:00:00

## 实验状态
✅ 完成 - 成功实现玩家2的独立GIF动画系统

## 测试方法
1. 运行程序：`cd build\Desktop_Qt_6_9_0_MinGW_64_bit-Debug && .\QLink.exe`
2. 选择双人模式开始游戏
3. 观察玩家1和玩家2使用不同的GIF动画
4. 拾取道具或消除方块，验证攻击动画是否正确触发
5. 确认两个玩家的动画系统完全独立

## 注意事项
- 确保zombie2.gif和zombie2_eat.gif文件存在于项目根目录
- 两个玩家的动画系统完全独立，互不影响
- 攻击动画持续时间设置为1.5秒，可根据需要调整
