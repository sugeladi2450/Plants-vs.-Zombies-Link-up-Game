# Qt 植物大战僵尸主题连连看（QLink）

> 基于 Qt 6 实现的「植物大战僵尸」主题连连看桌面小游戏：支持单人/双人、计时计分、道具、存档与音效/背景音乐。

---

## 1. 项目定位

- **核心玩法**：经典连连看规则（相同图案、可用直线/1 个拐角/2 个拐角连通即可消除）。
- **主题适配**：方块素材、僵尸 GIF 动画、音效与背景音乐均使用 PVZ 风格资源。
- **面向人群**：Qt/C++ 初学者（学习 QWidget 绘制、信号槽、资源系统），以及想做小型桌面游戏原型的开发者。

---

## 2. 功能特性

- **连连看消除规则**：
  - 直线连接（同行/同列）
  - 单拐角连接（L 形）
  - 双拐角连接（最多 2 次拐弯）
- **计时/计分**：
  - 默认 300 秒倒计时（可暂停/恢复）
  - 消除后累计“阳光”分数（PVZ 主题化积分）
- **道具系统**（随机生成在空地）：
  - 时间加成、重排、提示、闪烁（单人）、眩晕（双人）
- **单人/双人模式**：
  - 双人模式支持对抗计分，并包含“眩晕”负面效果
- **存档系统**：
  - 3 个存档槽位：保存、载入、删除
- **主题动画/音频**：
  - 僵尸移动/攻击 GIF 动画
  - 消除、拾取、胜利音效
  - 菜单/游戏两套背景音乐

---

## 3. 技术栈

- **语言标准**：C++17（见 [`CMakeLists.txt`](CMakeLists.txt:1)）
- **Qt 版本**：Qt 6（项目配置示例指向 Qt 6.9.0，见 [`CMakeLists.txt`](CMakeLists.txt:12) / [`build.bat`](build.bat:1)）
- **Qt 模块**：
  - `Qt6::Widgets`（窗口、事件、绘制）
  - `Qt6::Multimedia`（背景音乐、音效输出）
  - `Qt6::Test`（核心规则单元测试）

---

## 4. 运行环境要求

- **操作系统**：Windows 10/11（仓库提供 `.bat` 构建脚本）
- **推荐开发环境**：
  - Qt Creator（随 Qt 安装）
  - Qt 6.x（推荐 6.5+，项目示例为 6.9.0）
  - MinGW 64-bit（与 Qt 安装匹配；示例为 `mingw1310_64`）
- **构建工具**：CMake 3.16+（见 [`CMakeLists.txt`](CMakeLists.txt:1)）

---

## 5. 快速开始（Qt Creator 新手可复现）

### 方式 A：Qt Creator 直接打开 CMake 工程（推荐）

1. 安装 Qt（建议选择 **Qt 6.x + MinGW 64-bit** 组件）。
2. 打开 **Qt Creator**。
3. 点击 **File → Open File or Project...**
4. 选择项目根目录下的 [`CMakeLists.txt`](CMakeLists.txt:1)
5. 进入 **Configure Project** 页面：
   - Kit 选择与你安装的 Qt/MinGW 对应的 Kit（例如 *Desktop Qt 6.x MinGW 64-bit*）
   - CMake 构建类型可选 **Debug**
   - 点击 **Configure Project**
6. 左下角选择运行目标 **QLink**（不是 QLinkTest）。
7. 点击左下角绿色三角 **Run** 运行。

### 方式 B：使用脚本一键构建（Windows）

- 双击 [`build_simple.bat`](build_simple.bat:1)（会自动清理并生成 `build/`，再执行 CMake + mingw32-make）。
- 或双击 [`build.bat`](build.bat:1)（显式指定 Qt/CMake/MinGW 路径）。

> 注意：脚本里写死了 Qt 安装路径（例如 `C:\Qt\6.9.0\mingw_64`）。如果你的 Qt 不在该目录，请修改脚本或使用方式 A。

---

## 6. 项目结构

```text
.
├─ CMakeLists.txt            # CMake 构建入口
├─ main.cpp                  # 程序入口
├─ gamewindow.h/.cpp         # 游戏主窗口（UI+逻辑+绘制）
├─ linkjudger.h/.cpp         # 连连看消除判定核心
├─ simpletest.h/.cpp         # LinkJudger 的 QtTest 单元测试
├─ resources.qrc             # Qt 资源清单（图片/音频/GIF）
├─ build.bat                 # Windows 构建脚本（显式路径）
├─ build_simple.bat          # Windows 构建脚本（简化）
├─ *.png/*.jpg/*.gif         # 游戏素材（方块、背景、僵尸动画等）
├─ *.wav/*.mp3               # 音效/背景音乐
└─ log/                      # 过程/实验类文档
```

关键文件说明：

- 入口：[`main.cpp`](main.cpp:1) 创建 `QApplication` 与 [`GameWindow`](gamewindow.h:99)
- 主循环与渲染：[`GameWindow::paintEvent()`](gamewindow.h:143) + 大量 `draw*()` 私有绘制函数
- 消除规则：[`LinkJudger::canEliminate()`](linkjudger.cpp:8)
- 资源系统：[`resources.qrc`](resources.qrc:1)

---

## 7. 核心功能实现概览（读代码导览）

### 7.1 连连看消除逻辑（规则层）

核心类：[`LinkJudger`](linkjudger.h:6)

- 入口：[`LinkJudger::canEliminate()`](linkjudger.cpp:8)
  1. 基本校验：坐标合法、两格非空、类型相同（[`isValidElimination()`](linkjudger.cpp:45)）
  2. 按优先级判断可连通路径：
     - 直线：[`canLineConnection()`](linkjudger.cpp:126)
     - 单拐角：[`canOneCornerConnection()`](linkjudger.cpp:76)
     - 双拐角：[`canTwoCornersConnection()`](linkjudger.cpp:165)
- 判定“是否有解”：[`LinkJudger::hasSolutions()`](linkjudger.cpp:109)

> 说明：该实现是典型“规则与表现分离”的第一步：`LinkJudger` 不依赖 Qt，仅依赖 `std::vector`，便于测试与复用。

### 7.2 Qt 绘制与事件（表现层）

核心类：[`GameWindow`](gamewindow.h:99)

- **绘制**：重写 [`paintEvent()`](gamewindow.h:143)，内部拆成 `drawGameBackground/drawMapBlocks/drawPlayers/drawItems/...` 等函数（见 [`gamewindow.h`](gamewindow.h:237) 起）。
- **输入事件**：
  - 键盘：[`keyPressEvent()`](gamewindow.h:146) → [`handleKey()`](gamewindow.cpp:498)
  - 鼠标：[`mousePressEvent()`](gamewindow.h:149)、[`mouseMoveEvent()`](gamewindow.h:152)
- **定时器**：多个 `QTimer` 驱动倒计时、提示效果、眩晕效果、道具刷新等（见 [`gamewindow.h`](gamewindow.h:531) 与构造函数 connect：[`gamewindow.cpp`](gamewindow.cpp:177)）。

### 7.3 PVZ 主题适配（资源/动画/音频）

- **素材**：方块图片 `1.png~8.png`、背景 `background.jpg/map.jpg/save.jpg`、道具 `brain.png`（见 [`resources.qrc`](resources.qrc:1)）。
- **动画**：僵尸 GIF（`zombie.gif/zombie_eat.gif/zombie2.gif/...`），由 `QMovie` 管理（成员见 [`gamewindow.h`](gamewindow.h:539)）。
- **音频**：
  - 音效：`QSoundEffect`（消除/拾取/胜利，成员见 [`gamewindow.h`](gamewindow.h:564)）
  - 背景音乐：`QMediaPlayer + QAudioOutput`（成员见 [`gamewindow.h`](gamewindow.h:569)）
