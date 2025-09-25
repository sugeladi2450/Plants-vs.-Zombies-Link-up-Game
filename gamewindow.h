#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QMouseEvent>
#include <QMovie>
#include <vector>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <random>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>         // 用于QDialog对话框
#include <QVBoxLayout>     // 用于垂直布局管理器
#include <QLabel>          // 用于标签控件
#include <QLineEdit>       // 用于文本输入框
#include <QIntValidator>   // 用于整数输入验证
#include "linkjudger.h"

// 玩家类，表示游戏中的玩家角色
class Player {
public:
    // 构造函数，初始化玩家对象
    Player(int id, int startRow, int startCol);
    
    // 获取玩家ID
    int getId() const { return m_id; }
    
    // 获取玩家当前行坐标
    int getRow() const { return m_row; }
    
    // 获取玩家当前列坐标
    int getCol() const { return m_col; }
    
    // 获取玩家当前分数
    int getScore() const { return m_score; }
    
    // 检查玩家是否处于眩晕状态
    bool isDizzy() const { return m_dizzy; }
    
    // 获取眩晕状态剩余时间
    int getDizzyTime() const { return m_dizzyTime; }
    
    // 设置玩家位置
    void setPosition(int row, int col);
    
    // 增加玩家分数
    void addScore(int points);
    
    // 设置眩晕状态
    void setDizzy(bool dizzy, int time = 10);
    
    // 设置闪烁效果状态
    void setFlashActive(bool active, int time = 5);
    
    // 更新所有状态效果的时间
    void updateEffects();
    
    // 检查玩家是否处于闪烁状态
    bool isFlashActive() const { return m_flashActive; }
    
    // 移动玩家位置
    void move(int deltaRow, int deltaCol);
    
private:
    int m_id; // 玩家ID
    int m_row, m_col; // 位置坐标
    int m_score; // 分数
    bool m_dizzy; // 是否眩晕
    bool m_flashActive; // 是否激活闪烁效果
    int m_dizzyTime; // 眩晕剩余时间
    int m_flashTime; // 闪烁效果剩余时间
};

// 鼠标点击信息结构体
struct ClickInfo {
    int row; // 点击位置的行坐标
    int col; // 点击位置的列坐标
    bool isValid; // 点击位置是否有效
    
    // 构造函数
    ClickInfo(int r, int c, bool valid) : row(r), col(c), isValid(valid) {}
};

// 游戏主窗口类
class GameWindow : public QWidget {
    Q_OBJECT

public:
    // 构造函数，初始化游戏窗口
    GameWindow(QWidget *parent = nullptr);
    
    // 析构函数，清理资源
    ~GameWindow();
    
    
    // 保存游戏
    void saveGame(const QString& filename);
    
    // 加载游戏
    void loadGame(const QString& filename);
    
    
    // 重新初始化玩家位置（测试接口）
    void reinitializePlayerPositions() { 
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, ROWS - 1);
        std::uniform_int_distribution<> colDist(0, COLS - 1);
        
        // 玩家1位置：随机选择空地
        auto player1Pos = findValidPlayerPosition(gen, rowDist, colDist);
        m_p1.setPosition(player1Pos.first, player1Pos.second);
        checkItemCollision(m_p1);
        
        // 玩家2位置：随机选择空地
        if (m_twoPlayer) {
            auto player2Pos = findValidPlayerPosition(gen, rowDist, colDist);
            m_p2.setPosition(player2Pos.first, player2Pos.second);
        }
    }
    
    int ROWS = 16; // 游戏地图行数
    int COLS = 24; // 游戏地图列数
    static constexpr int TYPES = 8; // 方块类型数量
    static constexpr int TIME = 300; // 游戏时间（秒）

protected:
    // 事件处理函数
    void paintEvent(QPaintEvent *event) override;
    
    // 键盘按键事件处理函数
    void keyPressEvent(QKeyEvent *event) override;
    
    // 鼠标按下事件处理函数
    void mousePressEvent(QMouseEvent *event) override;
    
    // 鼠标移动事件处理函数
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::vector<QPixmap> m_blockImages; // 方块图片资源数组
    
    
    // 加载方块图片资源
    void loadBlockImages();
    
    // 绘制方块图片
    void drawBlockImage(QPainter& painter, const QRectF& rect, int blockType, bool isActivated);

    // 菜单状态
    enum GameState {
        MENU_STATE,     // 开始菜单状态
        GAME_STATE      // 游戏状态
    };
    
    // 菜单选项
    enum MenuOption {
        START_NEW_GAME = 0,
        LOAD_GAME = 1,
        EXIT_GAME = 2
    };
    
    enum GameModeOption {
        SINGLE_PLAYER = 0,
        TWO_PLAYER = 1
    };
    
    enum SaveSlot {
        SAVE_SLOT_1 = 0,
        SAVE_SLOT_2 = 1,
        SAVE_SLOT_3 = 2
    };
    
    // 显示菜单界面
    void showMenu();
    
    // 隐藏菜单界面
    void hideMenu();
    
    // 选择菜单选项
    void selectOption(MenuOption option);
    
    // 选择游戏模式
    void selectGameMode(GameModeOption mode);
    
    // 选择存档槽位
    void selectSaveSlot(SaveSlot slot);
    
    // 检查存档是否存在
    bool isSaveSlotExists(SaveSlot slot);
    
    // 获取存档文件名
    QString getSaveFileName(SaveSlot slot);
    
    // 获取存档显示名称
    QString getSaveDisplayName(SaveSlot slot);
    
    // 删除存档
    void deleteSaveSlot(SaveSlot slot);
    
    // 绘制菜单界面
    void drawMenu(QPainter& painter);
    
    // 绘制存档菜单
    void drawSaveMenu(QPainter& painter);
    
    // 绘制存档菜单选项
    void drawSaveOptions(QPainter& painter);
    
    
    // 绘制菜单选项列表
    void drawOptions(QPainter& painter);
    
    // 绘制单个菜单选项
    void drawOption(QPainter& painter, const QRect& rect, const QString& text, bool selected);
    
    // 绘制菜单选项的通用辅助函数
    void drawMenuOptionsHelper(QPainter& painter, const std::initializer_list<QString>& options, int optionCount, bool hasReturnButton);
    
    // 绘制游戏状态界面
    void drawGameState(QPainter& painter, int widgetWidth, int widgetHeight);
    
    // 绘制游戏背景
    void drawGameBackground(QPainter& painter, int widgetWidth, int widgetHeight);
    
    // 绘制保存/载入背景
    void drawSaveBackground(QPainter& painter, int widgetWidth, int widgetHeight);
    
    // 绘制地图方块
    void drawMapBlocks(QPainter& painter, float cellWidth, float cellHeight);
    
    // 绘制道具
    void drawItems(QPainter& painter, float cellWidth, float cellHeight);
    
    // 绘制提示高亮
    void drawHintHighlights(QPainter& painter, float cellWidth, float cellHeight);
    
    // 绘制连接线
    void drawConnectionLines(QPainter& painter, float cellWidth, float cellHeight);
    
    // 绘制所有玩家
    void drawPlayers(QPainter& painter, float cellWidth, float cellHeight);
    
    // 绘制单个玩家
    void drawPlayer(QPainter& painter, const Player& player, int playerId, float cellWidth, float cellHeight);
    
    // 绘制游戏信息
    void drawGameInfo(QPainter& painter, int widgetWidth, int widgetHeight);
    
    // 绘制玩家状态
    void drawPlayerStatus(QPainter& painter);
    
    // 绘制操作提示
    void drawOperationHints(QPainter& painter, int widgetWidth, int widgetHeight);
    
    // 绘制游戏状态信息
    void drawGameStatus(QPainter& painter, int widgetWidth, int widgetHeight);
    
    // 处理键盘按键事件
    void handleKey(QKeyEvent *event);
    

    
    // 处理菜单鼠标事件
    void handleMenuMouseEvent(QMouseEvent *event, bool isClick);
    
    // 处理上方向键
    void handleUpKey();
    
    // 处理下方向键
    void handleDownKey();
    
    // 处理左方向键
    void handleLeftKey();
    
    // 处理右方向键
    void handleRightKey();
    
    // 处理空格键
    void handleSpaceKey();
    
    // 处理Flash道具的鼠标点击
    void handleFlashMouseClick(QMouseEvent *event);
    
    // 获取鼠标点击位置信息
    struct ClickInfo getClickPosition(QMouseEvent *event);
    
    // 处理方块点击事件
    void handleBlockClick(const ClickInfo& clickInfo);
    
    // 将玩家移动到方块旁边
    bool movePlayerToBlockSide(int blockRow, int blockCol);
    
    // 处理方块激活逻辑
    void handleBlockActivation(int clickedRow, int clickedCol);
    
    // 处理空地点击事件
    void handleEmptySpaceClick(const ClickInfo& clickInfo);
    
    // 开始游戏
    void start();
    
    // 结束游戏
    void end();
    
    // 暂停游戏
    void pause();
    
    // 恢复游戏
    void resume();
    
    // 生成游戏地图
    void generateMap();
    
    // 生成道具
    void generateItems();
    
    // 检查道具碰撞
    void checkItemCollision(Player& player);
    
    // 激活道具效果
    void activateItem(int itemType, Player& player);
    
    // 初始化地图数据
    void initializeMapData();
    
    // 创建方块类型数组
    std::vector<int> createBlockTypes(std::mt19937& gen);
    
    // 填充游戏区域
    void fillGameArea(const std::vector<int>& blockTypes);
    
    // 时间加成道具效果
    void addTime();
    
    // 重新排列道具效果
    void shuffle();
    
    // 提示道具效果
    void hint();
    
    // 眩晕道具效果
    void dizzy(Player& target);
    
    // 闪烁道具效果
    void flash(Player& player);
    
    // 查找下一个提示方块对
    void findNextHintPair();
    
    // 结束提示效果
    void endHint();
    
    // 结束眩晕效果
    void endDizzy(Player& player);
    
    // 显示无解消息
    void showNoSolutionMessage();
    
    // 重新排列地图
    void shuffleMap();
    
    // 检查位置是否可以移动
    bool canMove(int row, int col);
    
    // 移动玩家
    void move(Player& player, int deltaRow, int deltaCol);
    

    
    // 处理方块消除
    void processElimination();
    
    // 计算连接路径
    void calcPath(int r1, int c1, int r2, int c2);
    
    // 显示游戏结果
    void showGameResult();
    
    // 尝试激活方块
    void tryActivateBlock(Player& player, int deltaRow, int deltaCol);
    
    // 检查方块是否可达
    bool isBlockReachable(int row, int col);
    

    
    // 查找可消除的方块对
    bool findSolvablePair(int& hintR1, int& hintC1, int& hintR2, int& hintC2);
    
    // 初始化玩家位置
    void initializePlayerPosition(Player& player, std::mt19937& gen,
                                 std::uniform_int_distribution<>& rowDist,
                                 std::uniform_int_distribution<>& colDist);
    
    // 获取随机边界位置
    std::pair<int, int> getRandomBorderPosition(std::mt19937& gen);
    

    
    // 查找有效的玩家位置
    std::pair<int, int> findValidPlayerPosition(std::mt19937& gen,
                                 std::uniform_int_distribution<>& rowDist,
                                 std::uniform_int_distribution<>& colDist);
    
    // 尝试一个拐角连接路径
    bool tryOneCornerPath(int r1, int c1, int r2, int c2);
    
    // 尝试两个拐角连接路径
    bool tryTwoCornerPath(int r1, int c1, int r2, int c2);
    
    // 获取方块对应的阳光值
    int getBlockSunlight(int blockType);

    // 停止所有计时器
    void stopAllTimers();
    
    // 重置游戏效果
    void resetGameEffects();
    
    // 保存游戏头部信息
    void saveGameHeader(QTextStream& out);
    
    // 保存玩家信息
    void saveGamePlayers(QTextStream& out);
    
    // 保存游戏地图
    void saveGameMap(QTextStream& out);
    
    // 保存道具信息
    void saveGameItems(QTextStream& out);
    
    // 保存游戏效果
    void saveGameEffects(QTextStream& out);
    
    // 加载游戏头部信息
    bool loadGameHeader(QTextStream& in);
    
    // 加载玩家信息
    void loadGamePlayers(QTextStream& in);
    
    // 加载游戏地图
    void loadGameMap(QTextStream& in);
    
    // 加载道具信息
    void loadGameItems(QTextStream& in);
    
    // 加载游戏效果
    void loadGameEffects(QTextStream& in);
    

    
    // 道具类型
    static constexpr int TIME_BONUS = 1;      // 时间加成道具
    static constexpr int SHUFFLE = 2;         // 重新排列道具
    static constexpr int HINT = 3;            // 提示道具
    static constexpr int FLASH = 4;           // 闪烁道具（单人模式）
    static constexpr int DIZZY = 5;           // 眩晕道具（双人模式）
    
    // 菜单相关成员
    GameState m_state;                      // 当前状态
    MenuOption m_option;                    // 选中选项
    GameModeOption m_gameModeOption;        // 选中的游戏模式选项
    bool m_showGameModeSelection;           // 是否显示游戏模式选择
    bool m_backButtonSelected;              // 返回按钮是否被选中
    bool m_showLoadSlots;                   // 是否显示载入存档槽位选择
    bool m_showSaveSlots;                   // 是否显示保存存档槽位选择
    bool m_showDeleteSlots;                 // 是否显示删除存档槽位选择
    SaveSlot m_selectedSaveSlot;            // 选中的存档槽位
    static const int OPTIONS = 3;
    
    // 游戏数据
    std::vector<std::vector<int>> m_mapData;
    Player m_p1;                            // 玩家1
    Player m_p2;                            // 玩家2
    Player* m_active;                       // 当前激活的玩家
    
    // 游戏状态
    int m_time;                             // 剩余时间
    bool m_running;                         // 游戏运行
    bool m_paused;                          // 暂停状态
    bool m_twoPlayer;                       // 双人模式
    
    // 激活状态
    int m_activeRow;                        // 激活行
    int m_activeCol;                        // 激活列
    int m_activeRow2;                        // 玩家2激活行
    int m_activeCol2;                        // 玩家2激活列
    
    // 道具
    struct Item {
        int row, col, type;
        Item(int r, int c, int t) : row(r), col(c), type(t) {}
    };
    std::vector<Item> m_items;
    
    // 道具效果
    bool m_hintOn;                          // 提示开启
    int m_hintTime;                         // 提示时间
    int m_hintR1, m_hintC1, m_hintR2, m_hintC2;  // 提示坐标
    
    // 连接线绘制
    bool m_showLine;                        // 显示连接线
    int m_lineR1, m_lineC1, m_lineR2, m_lineC2;  // 连接线坐标
    std::vector<std::pair<int, int>> m_linePath;  // 连接线路径
    
    // 定时器
    QTimer* m_timer;                        // 主定时器
    QTimer* m_hintTimer;                    // 提示定时器
    QTimer* m_dizzy1;                       // 眩晕定时器1
    QTimer* m_dizzy2;                       // 眩晕定时器2
    QTimer* m_spawnTimer;                   // 道具生成定时器
    QTimer* m_animationTimer;               // 动画定时器
    
    // GIF动图相关
    QMovie* m_zombieMovie;                  // 玩家1僵尸GIF动图
    QMovie* m_zombieEatMovie;               // 玩家1僵尸攻击GIF动图
    QPixmap m_currentZombieFrame;           // 玩家1当前僵尸帧
    QPixmap m_currentZombieEatFrame;        // 玩家1当前僵尸攻击帧
    bool m_isZombieAttacking;               // 玩家1僵尸是否正在攻击
    QTimer* m_attackTimer;                  // 玩家1攻击动画定时器
    
    // 玩家2 GIF动图相关
    QMovie* m_zombie2Movie;                 // 玩家2僵尸GIF动图
    QMovie* m_zombie2EatMovie;              // 玩家2僵尸攻击GIF动图
    QPixmap m_currentZombie2Frame;          // 玩家2当前僵尸帧
    QPixmap m_currentZombie2EatFrame;       // 玩家2当前僵尸攻击帧
    bool m_isZombie2Attacking;              // 玩家2僵尸是否正在攻击
    QTimer* m_attack2Timer;                 // 玩家2攻击动画定时器
    
    // 背景图片相关
    QPixmap m_menuBackground;               // 菜单背景图片
    QPixmap m_gameBackground;               // 游戏背景图片
    QPixmap m_saveBackground;               // 保存/载入背景图片
    QPixmap m_brainPropImage;               // 大脑道具图片
    
    // 消除判断器
    LinkJudger m_judger;
    
    // 音效系统
    QSoundEffect* m_eliminationSound; // 方块消除音效播放器
    QSoundEffect* m_itemSound; // 道具拾取音效播放器
    QSoundEffect* m_winSound; // 游戏胜利音效播放器
    
    // 背景音乐系统
    QMediaPlayer* m_backgroundMusic; // 背景音乐播放器
    QMediaPlayer* m_gameMusic; // 游戏背景音乐播放器
    QAudioOutput* m_audioOutput; // 音频输出设备
    QAudioOutput* m_gameAudioOutput; // 游戏音频输出设备
    bool m_backgroundMusicPlaying; // 背景音乐是否正在播放
    bool m_gameMusicPlaying; // 游戏背景音乐是否正在播放
    
    // 紧凑菜单系统
    QWidget* m_menuWidget; // 菜单容器
    QHBoxLayout* m_menuLayout; // 菜单布局
    QPushButton* m_gameButton; // 游戏按钮
    QPushButton* m_helpButton; // 帮助按钮
    QPushButton* m_saveButton; // 存档按钮
    QPushButton* m_settingsButton; // 设置按钮
    QMenu* m_gameMenu; // 游戏下拉菜单
    QMenu* m_helpMenu; // 帮助下拉菜单
    QMenu* m_saveMenu; // 存档下拉菜单
    QMenu* m_settingsMenu; // 设置下拉菜单
    bool m_soundEnabled; // 音效是否启用
    bool m_musicEnabled; // 背景音乐是否启用

private slots:
    // 游戏计时器超时槽函数
    void onTimerTimeout();
    
    // 提示计时器超时槽函数
    void onHintTimerTimeout();
    
    // 眩晕计时器超时槽函数
    void onDizzyTimerTimeout(Player& player, QTimer* timer);
    
    // 道具生成计时器超时槽函数
    void onItemSpawnTimerTimeout();
    
    
    // 更新窗口标题
    void updateWindowTitle(const QString& status = "");
    
    // 初始化音效系统
    void initializeAudioSystem();
    
    // 初始化僵尸GIF动图
    void initializeZombieAnimation();
    
    // 初始化玩家2僵尸GIF动图
    void initializeZombie2Animation();
    
    // 触发僵尸攻击动画
    void triggerZombieAttackAnimation();
    
    // 触发玩家2僵尸攻击动画
    void triggerZombie2AttackAnimation();
    
    // 加载菜单背景图片
    void loadMenuBackground();
    
    // 加载游戏背景图片
    void loadGameBackground();
    
    // 加载保存/载入背景图片
    void loadSaveBackground();
    
    // 加载大脑道具图片
    void loadBrainPropImage();
    
    // 根据背景图片比例调整窗口大小
    void adjustWindowSizeToBackground();
    
    // 播放方块消除音效
    void playEliminationSound();
    
    // 播放道具拾取音效
    void playItemSound();
    
    // 播放游戏胜利音效
    void playWinSound();
    
    // 播放背景音乐
    void playBackgroundMusic();
    
    // 停止背景音乐
    void stopBackgroundMusic();
    
    // 播放游戏背景音乐
    void playGameMusic();
    
    // 停止游戏背景音乐
    void stopGameMusic();
    
    // 初始化紧凑菜单
    void initializeCompactMenu();
    
    // 创建游戏菜单
    void createGameMenu();
    
    // 创建帮助菜单
    void createHelpMenu();
    
    // 创建存档菜单
    void createSaveMenu();
    
    // 创建设置菜单
    void createSettingsMenu();
    
};

#endif // GAMEWINDOW_H
