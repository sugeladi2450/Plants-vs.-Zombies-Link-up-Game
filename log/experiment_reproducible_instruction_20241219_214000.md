# 游戏背景音乐功能实现方案

## 实验目标
将`music2.mp3`作为进入游戏后的背景音乐，通过音乐开关控制，实现菜单音乐和游戏音乐的分离。

## 功能需求
1. 菜单页面播放`music.wav`背景音乐
2. 游戏页面播放`music2.mp3`背景音乐
3. 通过音乐开关统一控制两种背景音乐
4. 从菜单进入游戏时自动切换音乐
5. 从游戏返回菜单时自动切换音乐

## 技术实现

### 1. 资源文件更新
在`resources.qrc`中添加`music2.mp3`：
```xml
<file>music2.mp3</file>
```

### 2. 头文件修改
在`gamewindow.h`中添加游戏背景音乐相关成员：
```cpp
// 背景音乐系统
QMediaPlayer* m_backgroundMusic;     ///< 背景音乐播放器（菜单音乐）
QMediaPlayer* m_gameMusic;           ///< 游戏背景音乐播放器
QAudioOutput* m_audioOutput;         ///< 音频输出设备
bool m_backgroundMusicPlaying;       ///< 背景音乐是否正在播放
bool m_gameMusicPlaying;             ///< 游戏背景音乐是否正在播放
```

添加游戏背景音乐控制函数：
```cpp
void playGameMusic();    ///< 播放游戏背景音乐
void stopGameMusic();    ///< 停止游戏背景音乐
```

### 3. 构造函数修改
在构造函数中初始化游戏背景音乐播放器：
```cpp
, m_gameMusic(new QMediaPlayer(this)) // 游戏背景音乐播放器
, m_gameMusicPlaying(false) // 游戏背景音乐未播放
```

### 4. 音乐初始化
在`initializeSounds()`函数中初始化游戏背景音乐：
```cpp
// 初始化游戏背景音乐
m_gameMusic->setAudioOutput(m_audioOutput);
m_gameMusic->setSource(QUrl("qrc:/music2.mp3"));
m_gameMusic->setLoops(QMediaPlayer::Infinite); // 循环播放
```

### 5. 音乐控制函数实现
```cpp
// 播放游戏背景音乐
void GameWindow::playGameMusic()
{
    if (m_gameMusic && !m_gameMusicPlaying && m_musicEnabled) {
        m_gameMusic->play();
        m_gameMusicPlaying = true;
    }
}

// 停止游戏背景音乐
void GameWindow::stopGameMusic()
{
    if (m_gameMusic && m_gameMusicPlaying) {
        m_gameMusic->stop();
        m_gameMusicPlaying = false;
    }
}
```

### 6. 游戏状态切换时的音乐控制

#### 游戏开始时（start函数）
```cpp
// 停止背景音乐
stopBackgroundMusic();

// 播放游戏背景音乐
playGameMusic();
```

#### 游戏结束时（end函数）
```cpp
// 停止游戏背景音乐
stopGameMusic();
```

#### 返回菜单时（showMenu函数）
```cpp
// 停止游戏背景音乐
stopGameMusic();

// 播放背景音乐
playBackgroundMusic();
```

### 7. 音乐开关控制更新
更新音乐开关，同时控制两种背景音乐：
```cpp
connect(musicToggle, &QAction::triggered, this, [this, musicToggle]() {
    m_musicEnabled = musicToggle->isChecked();
    if (m_musicEnabled) {
        musicToggle->setText("音乐: 开启");
        // 根据当前状态播放对应音乐
        if (m_state == MENU_STATE && !m_backgroundMusicPlaying) {
            playBackgroundMusic();
        }
        if (m_state == GAME_STATE && !m_gameMusicPlaying) {
            playGameMusic();
        }
    } else {
        musicToggle->setText("音乐: 关闭");
        // 立即停止所有背景音乐
        stopBackgroundMusic();
        stopGameMusic();
    }
});
```

## 音乐播放逻辑

### 菜单状态
- 播放：`music.wav`（菜单背景音乐）
- 停止：`music2.mp3`（游戏背景音乐）

### 游戏状态
- 播放：`music2.mp3`（游戏背景音乐）
- 停止：`music.wav`（菜单背景音乐）

### 音乐开关
- 开启：根据当前状态播放对应音乐
- 关闭：停止所有背景音乐

## 构建命令
```bash
# 在build目录下
C:\Qt\Tools\CMake_64\bin\cmake.exe -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:\Qt\6.9.0\mingw_64 ..
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe
```

## 测试步骤
1. 运行QLink.exe
2. 在菜单页面，验证播放music.wav
3. 进入游戏，验证播放music2.mp3
4. 返回菜单，验证播放music.wav
5. 测试音乐开关功能

## 文件依赖
- `resources.qrc`: 包含music.wav和music2.mp3
- `gamewindow.h`: 函数声明和成员变量
- `gamewindow.cpp`: 主要实现
- `music.wav`: 菜单背景音乐
- `music2.mp3`: 游戏背景音乐

## 成功标志
- 项目构建成功，生成QLink.exe
- 菜单页面播放music.wav
- 游戏页面播放music2.mp3
- 音乐开关控制两种音乐
- 状态切换时音乐自动切换

## 注意事项
- 确保music2.mp3文件存在于项目根目录
- 确保resources.qrc正确包含music2.mp3
- 音乐播放器使用相同的音频输出设备
- 音乐开关统一控制所有背景音乐
