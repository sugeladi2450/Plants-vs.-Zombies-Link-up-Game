#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QMouseEvent>
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
#include <QWidget>
#include "linkjudger.h"

/**
 * @brief 玩家类，表示游戏中的玩家角色
 * 
 * 该类封装了玩家的所有属性和行为，包括位置、分数、状态效果等。
 * 支持眩晕和闪烁两种特殊状态效果。
 */
class Player {
public:
    /**
     * @brief 构造函数，初始化玩家对象
     * @param id 玩家ID，用于区分不同玩家（通常为1或2）
     * @param startRow 初始行坐标，范围[0, ROWS-1]
     * @param startCol 初始列坐标，范围[0, COLS-1]
     */
    Player(int id, int startRow, int startCol);
    
    // 基本属性获取函数
    /**
     * @brief 获取玩家ID
     * @return 玩家ID（1或2）
     */
    int getId() const { return m_id; }
    
    /**
     * @brief 获取玩家当前行坐标
     * @return 行坐标，范围[0, ROWS-1]
     */
    int getRow() const { return m_row; }
    
    /**
     * @brief 获取玩家当前列坐标
     * @return 列坐标，范围[0, COLS-1]
     */
    int getCol() const { return m_col; }
    
    /**
     * @brief 获取玩家当前分数
     * @return 玩家累计分数
     */
    int getScore() const { return m_score; }
    
    /**
     * @brief 检查玩家是否处于眩晕状态
     * @return true表示眩晕，false表示正常
     */
    bool isDizzy() const { return m_dizzy; }
    
    /**
     * @brief 获取眩晕状态剩余时间
     * @return 眩晕剩余时间（秒），0表示未眩晕
     */
    int getDizzyTime() const { return m_dizzyTime; }
    
    // 状态设置函数
    /**
     * @brief 设置玩家位置
     * @param row 新的行坐标，范围[0, ROWS-1]
     * @param col 新的列坐标，范围[0, COLS-1]
     */
    void setPosition(int row, int col);
    
    /**
     * @brief 增加玩家分数
     * @param points 要增加的分数，必须为正数
     */
    void addScore(int points);
    
    /**
     * @brief 设置眩晕状态
     * @param dizzy 是否眩晕，true表示进入眩晕，false表示解除眩晕
     * @param time 眩晕持续时间（秒），默认10秒
     */
    void setDizzy(bool dizzy, int time = 10);
    
    /**
     * @brief 设置闪烁效果状态
     * @param active 是否激活闪烁效果，true表示激活，false表示解除
     * @param time 闪烁效果持续时间（秒），默认5秒
     */
    void setFlashActive(bool active, int time = 5);
    
    /**
     * @brief 更新所有状态效果的时间
     * 
     * 每调用一次，所有状态效果的剩余时间减1秒。
     * 当时间归零时，自动解除对应状态。
     */
    void updateEffects();
    
    // 移动相关函数

    
    /**
     * @brief 检查玩家是否处于闪烁状态
     * @return true表示闪烁状态激活，false表示正常状态
     */
    bool isFlashActive() const { return m_flashActive; }
    
    /**
     * @brief 移动玩家位置
     * @param deltaRow 行方向移动量，-1表示向上，1表示向下，0表示不移动
     * @param deltaCol 列方向移动量，-1表示向左，1表示向右，0表示不移动
     */
    void move(int deltaRow, int deltaCol);
    
private:
    int m_id;           // 玩家ID (1或2)
    int m_row, m_col;   // 位置
    int m_score;        // 分数
    bool m_dizzy;       // 是否眩晕
    bool m_flashActive; // 是否激活Flash效果
    int m_dizzyTime;    // 眩晕剩余时间
    int m_flashTime;    // Flash效果剩余时间
};

/**
 * @brief 鼠标点击信息结构体
 * 
 * 用于封装鼠标点击事件的位置和有效性信息
 */
struct ClickInfo {
    int row;        ///< 点击位置的行坐标
    int col;        ///< 点击位置的列坐标
    bool isValid;   ///< 点击位置是否有效（在游戏区域内）
    
    /**
     * @brief 构造函数
     * @param r 行坐标
     * @param c 列坐标
     * @param valid 是否有效
     */
    ClickInfo(int r, int c, bool valid) : row(r), col(c), isValid(valid) {}
};

/**
 * @brief 游戏主窗口类
 * 
 * 该类是游戏的核心类，继承自QWidget，负责管理整个游戏的运行。
 * 包括游戏状态管理、用户输入处理、游戏逻辑执行、界面绘制等功能。
 * 支持单人模式和双人模式，包含完整的方块消除游戏逻辑。
 */
class GameWindow : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化游戏窗口
     * @param parent 父窗口指针，默认为nullptr
     */
    GameWindow(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数，清理资源
     */
    ~GameWindow();
    
    /**
     * @brief 获取地图数据
     * @return 地图数据的常量引用
     */
    const std::vector<std::vector<int>>& getMapData() const { return m_mapData; }
    
    /**
     * @brief 保存游戏
     * @param filename 保存文件名
     * 
     * 将当前游戏状态保存到指定文件中，包括地图、玩家状态、道具等。
     */
    void saveGame(const QString& filename);
    
    /**
     * @brief 加载游戏
     * @param filename 加载文件名
     * 
     * 从指定文件加载游戏状态，恢复之前的游戏进度。
     */
    void loadGame(const QString& filename);
    
    // 测试接口 - 为单元测试提供公共接口
    /**
     * @brief 获取玩家1的行坐标（测试接口）
     * @return 玩家1的行坐标
     */
    int getPlayer1Row() const { return m_p1.getRow(); }
    
    /**
     * @brief 获取玩家1的列坐标（测试接口）
     * @return 玩家1的列坐标
     */
    int getPlayer1Col() const { return m_p1.getCol(); }
    
    /**
     * @brief 获取玩家2的行坐标（测试接口）
     * @return 玩家2的行坐标
     */
    int getPlayer2Row() const { return m_p2.getRow(); }
    
    /**
     * @brief 获取玩家2的列坐标（测试接口）
     * @return 玩家2的列坐标
     */
    int getPlayer2Col() const { return m_p2.getCol(); }
    
    /**
     * @brief 检查是否为双人模式（测试接口）
     * @return true表示双人模式，false表示单人模式
     */
    bool isTwoPlayerMode() const { return m_twoPlayer; }
    
    /**
     * @brief 重新生成地图（测试接口）
     */
    void regenerateMap() { generateMap(); }
    
    /**
     * @brief 重新初始化玩家位置（测试接口）
     */
    void reinitializePlayerPositions() { 
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, ROWS - 1);
        std::uniform_int_distribution<> colDist(0, COLS - 1);
        
        // 玩家1位置：随机选择空地
        auto player1Pos = findValidPlayerPosition(gen, rowDist, colDist);
        m_p1.setPosition(player1Pos.first, player1Pos.second);
        
        // 玩家2位置：随机选择空地（双人模式，可以重叠）
        if (m_twoPlayer) {
            auto player2Pos = findValidPlayerPosition(gen, rowDist, colDist);
            m_p2.setPosition(player2Pos.first, player2Pos.second);
        }
    }
    
    // 游戏常量
    static constexpr int ROWS = 16;           // 游戏地图行数
    static constexpr int COLS = 24;           // 游戏地图列数
    static constexpr int TYPES = 8;           // 方块类型数量
    static constexpr int TIME = 300;          // 游戏时间（秒）

protected:
    /**
     * @brief 重绘事件处理函数
     * @param event 重绘事件对象
     * 
     * 根据当前游戏状态绘制相应的界面内容。
     * 包括菜单界面和游戏界面的绘制。
     */
    void paintEvent(QPaintEvent *event) override;
    
    /**
     * @brief 键盘按键事件处理函数
     * @param event 键盘事件对象
     * 
     * 处理玩家的键盘输入，包括方向键移动和空格键激活方块。
     */
    void keyPressEvent(QKeyEvent *event) override;
    
    /**
     * @brief 鼠标按下事件处理函数
     * @param event 鼠标事件对象
     * 
     * 处理鼠标点击事件，包括菜单选择和游戏中的方块点击。
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief 鼠标移动事件处理函数
     * @param event 鼠标事件对象
     * 
     * 处理鼠标移动事件，主要用于菜单选项的高亮显示。
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    // 图片资源
    std::vector<QPixmap> m_blockImages;  ///< 方块图片资源数组
    bool m_imagesLoaded;                 ///< 图片是否已加载完成
    
    
    // 图片加载和绘制函数
    /**
     * @brief 加载方块图片资源
     * 
     * 从可执行文件目录加载1.png到8.png的方块图片文件。
     * 如果加载失败，会尝试从源代码目录加载。
     * 加载完成后设置m_imagesLoaded标志。
     */
    void loadBlockImages();
    
    /**
     * @brief 绘制方块图片
     * @param painter 绘制器对象
     * @param rect 绘制区域矩形
     * @param blockType 方块类型，范围[1, 8]
     * @param isActivated 是否处于激活状态
     * 
     * 根据方块类型绘制对应的图片，激活状态会添加红色边框效果。
     */
    void drawBlockImage(QPainter& painter, const QRectF& rect, int blockType, bool isActivated);

    // 菜单状态
    enum GameState {
        MENU_STATE,     // 开始菜单状态
        GAME_STATE      // 游戏状态
    };
    
    // 菜单选项
    enum MenuOption {
        SINGLE_PLAYER = 0,
        TWO_PLAYER = 1,
        EXIT_GAME = 2
    };
    
    // 菜单相关函数
    /**
     * @brief 显示菜单界面
     * 
     * 切换到菜单状态，重置菜单选项为第一个选项。
     */
    void showMenu();
    
    /**
     * @brief 隐藏菜单界面
     * 
     * 从菜单状态切换到游戏状态。
     */
    void hideMenu();
    
    /**
     * @brief 选择菜单选项
     * @param option 要选择的菜单选项
     * 
     * 根据选择的选项执行相应操作：
     * - SINGLE_PLAYER: 开始单人游戏
     * - TWO_PLAYER: 开始双人游戏
     * - EXIT_GAME: 退出游戏
     */
    void selectOption(MenuOption option);
    
    /**
     * @brief 绘制菜单界面
     * @param painter 绘制器对象
     * 
     * 绘制完整的菜单界面，包括标题和选项列表。
     */
    void drawMenu(QPainter& painter);
    
    /**
     * @brief 绘制游戏标题
     * @param painter 绘制器对象
     * 
     * 在菜单顶部绘制游戏标题文字。
     */
    void drawTitle(QPainter& painter);
    
    /**
     * @brief 绘制菜单选项列表
     * @param painter 绘制器对象
     * 
     * 绘制所有菜单选项，高亮显示当前选中的选项。
     */
    void drawOptions(QPainter& painter);
    
    /**
     * @brief 绘制单个菜单选项
     * @param painter 绘制器对象
     * @param rect 选项绘制区域
     * @param text 选项文字
     * @param selected 是否被选中
     * 
     * 绘制单个菜单选项，选中状态会有不同的视觉效果。
     */
    void drawOption(QPainter& painter, const QRect& rect, const QString& text, bool selected);
    
    // 游戏状态绘制函数
    /**
     * @brief 绘制游戏状态界面
     * @param painter 绘制器对象
     * @param widgetWidth 窗口宽度
     * @param widgetHeight 窗口高度
     * 
     * 绘制完整的游戏界面，包括地图、道具、玩家、连接线等所有元素。
     */
    void drawGameState(QPainter& painter, int widgetWidth, int widgetHeight);
    
    /**
     * @brief 绘制地图方块
     * @param painter 绘制器对象
     * @param cellWidth 单个方块的宽度
     * @param cellHeight 单个方块的高度
     * 
     * 绘制游戏地图中的所有方块，包括普通方块和激活状态的方块。
     */
    void drawMapBlocks(QPainter& painter, float cellWidth, float cellHeight);
    
    /**
     * @brief 绘制道具
     * @param painter 绘制器对象
     * @param cellWidth 单个方块的宽度
     * @param cellHeight 单个方块的高度
     * 
     * 绘制地图上的所有道具，包括时间加成、重新排列、提示等道具。
     */
    void drawItems(QPainter& painter, float cellWidth, float cellHeight);
    
    /**
     * @brief 绘制提示高亮
     * @param painter 绘制器对象
     * @param cellWidth 单个方块的宽度
     * @param cellHeight 单个方块的高度
     * 
     * 当提示道具激活时，高亮显示可消除的方块对。
     */
    void drawHintHighlights(QPainter& painter, float cellWidth, float cellHeight);
    
    /**
     * @brief 绘制连接线
     * @param painter 绘制器对象
     * @param cellWidth 单个方块的宽度
     * @param cellHeight 单个方块的高度
     * 
     * 绘制方块之间的连接路径，用于显示消除路径。
     */
    void drawConnectionLines(QPainter& painter, float cellWidth, float cellHeight);
    
    /**
     * @brief 绘制所有玩家
     * @param painter 绘制器对象
     * @param cellWidth 单个方块的宽度
     * @param cellHeight 单个方块的高度
     * 
     * 绘制游戏中的所有玩家，包括玩家1和玩家2。
     */
    void drawPlayers(QPainter& painter, float cellWidth, float cellHeight);
    
    /**
     * @brief 绘制单个玩家
     * @param painter 绘制器对象
     * @param player 玩家对象
     * @param playerId 玩家ID
     * @param cellWidth 单个方块的宽度
     * @param cellHeight 单个方块的高度
     * 
     * 绘制指定玩家的外观，包括位置和状态效果。
     */
    void drawPlayer(QPainter& painter, const Player& player, int playerId, float cellWidth, float cellHeight);
    
    /**
     * @brief 绘制游戏信息
     * @param painter 绘制器对象
     * @param widgetWidth 窗口宽度
     * @param widgetHeight 窗口高度
     * 
     * 绘制游戏界面的信息区域，包括分数、时间等。
     */
    void drawGameInfo(QPainter& painter, int widgetWidth, int widgetHeight);
    
    /**
     * @brief 绘制玩家状态
     * @param painter 绘制器对象
     * 
     * 绘制玩家的状态信息，包括分数、眩晕状态等。
     */
    void drawPlayerStatus(QPainter& painter);
    
    /**
     * @brief 绘制操作提示
     * @param painter 绘制器对象
     * @param widgetWidth 窗口宽度
     * @param widgetHeight 窗口高度
     * 
     * 绘制游戏操作提示信息，指导玩家如何操作。
     */
    void drawOperationHints(QPainter& painter, int widgetWidth, int widgetHeight);
    
    /**
     * @brief 绘制游戏状态信息
     * @param painter 绘制器对象
     * @param widgetWidth 窗口宽度
     * @param widgetHeight 窗口高度
     * 
     * 绘制游戏的整体状态信息，包括时间、模式等。
     */
    void drawGameStatus(QPainter& painter, int widgetWidth, int widgetHeight);
    
    // 事件处理函数
    /**
     * @brief 处理键盘按键事件
     * @param event 键盘事件对象
     * 
     * 根据按键类型执行相应操作：
     * - 方向键：移动玩家
     * - 空格键：激活方块
     * - ESC键：暂停游戏
     */
    void handleKey(QKeyEvent *event);
    

    
    /**
     * @brief 处理菜单鼠标事件（移动高亮和点击选择）
     * @param event 鼠标事件对象
     * @param isClick 是否为点击事件（true=点击，false=移动）
     * 
     * 统一处理菜单状态下的鼠标移动高亮和点击选择功能。
     */
    void handleMenuMouseEvent(QMouseEvent *event, bool isClick);
    
    // 按键处理辅助函数
    /**
     * @brief 处理上方向键
     * 
     * 在菜单状态下向上选择选项，在游戏状态下向上移动玩家。
     */
    void handleUpKey();
    
    /**
     * @brief 处理下方向键
     * 
     * 在菜单状态下向下选择选项，在游戏状态下向下移动玩家。
     */
    void handleDownKey();
    
    /**
     * @brief 处理左方向键
     * 
     * 在游戏状态下向左移动玩家。
     */
    void handleLeftKey();
    
    /**
     * @brief 处理右方向键
     * 
     * 在游戏状态下向右移动玩家。
     */
    void handleRightKey();
    
    /**
     * @brief 处理空格键
     * 
     * 在游戏状态下激活当前玩家面前的方块。
     */
    void handleSpaceKey();
    
    // 鼠标事件处理辅助函数
    /**
     * @brief 处理Flash道具的鼠标点击
     * @param event 鼠标事件对象
     * 
     * 当玩家处于Flash状态时，处理鼠标点击事件。
     * 允许玩家直接点击方块进行激活或移动到空地。
     */
    void handleFlashMouseClick(QMouseEvent *event);
    
    /**
     * @brief 获取鼠标点击位置信息
     * @param event 鼠标事件对象
     * @return ClickInfo结构体，包含点击位置和有效性
     * 
     * 将鼠标坐标转换为游戏地图坐标，并检查是否在有效范围内。
     */
    struct ClickInfo getClickPosition(QMouseEvent *event);
    
    /**
     * @brief 处理方块点击事件
     * @param clickInfo 点击信息结构体
     * 
     * 处理玩家点击方块的事件，包括移动玩家到方块旁边和激活方块。
     */
    void handleBlockClick(const ClickInfo& clickInfo);
    
    /**
     * @brief 将玩家移动到方块旁边
     * @param blockRow 方块行坐标
     * @param blockCol 方块列坐标
     * @return true表示成功移动，false表示无法移动
     * 
     * 尝试将当前玩家移动到指定方块旁边的空白位置。
     */
    bool movePlayerToBlockSide(int blockRow, int blockCol);
    
    /**
     * @brief 处理方块激活逻辑
     * @param clickedRow 点击的行坐标
     * @param clickedCol 点击的列坐标
     * 
     * 处理方块的激活和消除逻辑，包括检查是否可以消除。
     */
    void handleBlockActivation(int clickedRow, int clickedCol);
    
    /**
     * @brief 处理空地点击事件
     * @param clickInfo 点击信息结构体
     * 
     * 当玩家处于Flash状态时，允许直接移动到点击的空地位置。
     */
    void handleEmptySpaceClick(const ClickInfo& clickInfo);
    
    // 游戏状态管理函数
    /**
     * @brief 开始游戏
     * 
     * 初始化游戏状态，生成地图，设置玩家位置，启动游戏计时器。
     */
    void start();
    
    /**
     * @brief 结束游戏
     * 
     * 停止所有计时器，显示游戏结果，清理游戏状态。
     */
    void end();
    
    /**
     * @brief 暂停游戏
     * 
     * 暂停游戏计时器，保持当前游戏状态。
     */
    void pause();
    
    /**
     * @brief 恢复游戏
     * 
     * 从暂停状态恢复游戏，重新启动计时器。
     */
    void resume();
    
    // 地图和道具管理函数
    /**
     * @brief 生成游戏地图
     * 
     * 创建16x24的游戏地图，随机生成8种不同类型的方块，
     * 确保每种方块都有偶数个，以便能够完全消除。
     */
    void generateMap();
    
    /**
     * @brief 生成道具
     * 
     * 在地图的空白位置随机生成道具，包括时间加成、重新排列、提示等。
     * 根据游戏模式（单人/双人）生成不同的道具类型。
     */
    void generateItems();
    
    /**
     * @brief 检查道具碰撞
     * @param player 要检查的玩家对象
     * 
     * 检查指定玩家是否与地图上的道具发生碰撞，
     * 如果发生碰撞则激活对应道具效果。
     */
    void checkItemCollision(Player& player);
    
    /**
     * @brief 激活道具效果
     * @param itemType 道具类型
     * @param player 使用道具的玩家
     * 
     * 根据道具类型执行相应的效果：
     * - TIME_BONUS: 增加游戏时间
     * - SHUFFLE: 重新排列地图
     * - HINT: 显示可消除方块对
     * - FLASH: 激活闪烁效果（单人模式）
     * - DIZZY: 眩晕对手（双人模式）
     */
    void activateItem(int itemType, Player& player);
    
    // 地图生成辅助函数
    /**
     * @brief 初始化地图数据
     * 
     * 创建16x24的地图数组，将所有位置初始化为0（空白）。
     */
    void initializeMapData();
    
    /**
     * @brief 创建方块类型数组
     * @param gen 随机数生成器
     * @return 包含所有方块类型的向量
     * 
     * 生成包含8种方块类型的数组，每种方块都有偶数个，
     * 确保所有方块都能被消除。
     */
    std::vector<int> createBlockTypes(std::mt19937& gen);
    
    /**
     * @brief 填充游戏区域
     * @param blockTypes 方块类型数组
     * 
     * 将生成的方块类型随机填充到游戏地图中，
     * 避开玩家初始位置和边界区域。
     */
    void fillGameArea(const std::vector<int>& blockTypes);
    
    // 道具效果函数
    /**
     * @brief 时间加成道具效果
     * 
     * 增加游戏剩余时间30秒。
     */
    void addTime();
    
    /**
     * @brief 重新排列道具效果
     * 
     * 重新生成游戏地图，保持方块类型不变但位置随机化。
     */
    void shuffle();
    
    /**
     * @brief 提示道具效果
     * 
     * 查找并高亮显示一对可消除的方块，持续5秒。
     */
    void hint();
    
    /**
     * @brief 眩晕道具效果
     * @param target 目标玩家
     * 
     * 使目标玩家进入眩晕状态10秒，期间无法移动。
     */
    void dizzy(Player& target);
    
    /**
     * @brief 闪烁道具效果
     * @param player 使用道具的玩家
     * 
     * 使玩家进入闪烁状态5秒，期间可以无视路径限制移动。
     */
    void flash(Player& player);
    
    /**
     * @brief 查找下一个提示方块对
     * 
     * 在当前地图中查找一对可消除的方块，用于提示功能。
     */
    void findNextHintPair();
    
    /**
     * @brief 结束提示效果
     * 
     * 清除提示高亮显示，恢复正常游戏状态。
     */
    void endHint();
    
    /**
     * @brief 结束眩晕效果
     * @param player 目标玩家
     * 
     * 解除玩家的眩晕状态，恢复正常移动能力。
     */
    void endDizzy(Player& player);
    
    /**
     * @brief 显示无解消息
     * 
     * 当没有可消除的方块对时，显示提示消息。
     */
    void showNoSolutionMessage();
    
    /**
     * @brief 重新排列地图
     * 
     * 保持方块类型不变，随机重新排列所有方块的位置。
     */
    void shuffleMap();
    
    // 移动和消除函数
    /**
     * @brief 检查位置是否可以移动
     * @param row 目标行坐标
     * @param col 目标列坐标
     * @return true表示可以移动，false表示不能移动
     * 
     * 检查指定位置是否在游戏区域内且为空白。
     */
    bool canMove(int row, int col);
    
    /**
     * @brief 移动玩家
     * @param player 要移动的玩家
     * @param deltaRow 行方向移动量
     * @param deltaCol 列方向移动量
     * 
     * 将玩家移动到新位置，并检查道具碰撞。
     */
    void move(Player& player, int deltaRow, int deltaCol);
    

    
    /**
     * @brief 处理方块消除
     * @param targetRow 目标方块行坐标
     * @param targetCol 目标方块列坐标
     * 
     * 处理两个方块的消除逻辑，包括路径计算、分数计算和界面更新。
     */
    void processElimination(int targetRow, int targetCol);
    
    /**
     * @brief 计算连接路径
     * @param r1 起始方块行坐标
     * @param c1 起始方块列坐标
     * @param r2 目标方块行坐标
     * @param c2 目标方块列坐标
     * 
     * 计算两个方块之间的连接路径，用于显示消除路径。
     */
    void calcPath(int r1, int c1, int r2, int c2);
    
    /**
     * @brief 显示游戏结果
     * 
     * 游戏结束时显示最终结果，包括玩家分数和胜负情况。
     */
    void showGameResult();
    
    // 方块激活和消除
    /**
     * @brief 尝试激活方块
     * @param player 操作的玩家
     * @param deltaRow 行方向偏移
     * @param deltaCol 列方向偏移
     * 
     * 尝试激活玩家面前指定方向的方块，如果无法移动则激活方块。
     */
    void tryActivateBlock(Player& player, int deltaRow, int deltaCol);
    
    // 路径查找函数

    
    /**
     * @brief 检查方块是否可达
     * @param row 方块行坐标
     * @param col 方块列坐标
     * @return true表示可达，false表示不可达
     * 
     * 检查指定位置的方块是否可以被玩家到达。
     */
    bool isBlockReachable(int row, int col);
    

    
    /**
     * @brief 查找可消除的方块对
     * @param hintR1 存储第一个方块行坐标的引用
     * @param hintC1 存储第一个方块列坐标的引用
     * @param hintR2 存储第二个方块行坐标的引用
     * @param hintC2 存储第二个方块列坐标的引用
     * @return 如果找到可消除的方块对返回true，否则返回false
     * 
     * 通用的方块对查找函数，可以用于提示道具和可解性检查
     */
    bool findSolvablePair(int& hintR1, int& hintC1, int& hintR2, int& hintC2);
    
        // 玩家位置初始化函数
    /**
     * @brief 初始化玩家位置
     * @param player 要初始化的玩家对象
     * @param gen 随机数生成器
     * @param rowDist 行坐标分布器
     * @param colDist 列坐标分布器
     * 
     * 在游戏地图的边界位置随机选择玩家的初始位置，
     * 只要位置是空白的即可。
     */
    void initializePlayerPosition(Player& player, std::mt19937& gen,
                                 std::uniform_int_distribution<>& rowDist,
                                 std::uniform_int_distribution<>& colDist);
    
    // 玩家位置初始化辅助函数
    /**
     * @brief 获取随机边界位置
     * @param gen 随机数生成器
     * @return 包含行坐标和列坐标的pair
     * 
     * 在地图的边界位置（上下左右边缘）随机选择一个位置。
     */
    std::pair<int, int> getRandomBorderPosition(std::mt19937& gen);
    

    
    /**
     * @brief 查找有效的玩家位置
     * @param gen 随机数生成器
     * @param rowDist 行坐标分布器
     * @param colDist 列坐标分布器
     * @return 包含行坐标和列坐标的pair
     * 
     * 在地图中查找一个有效的空地位置。
     */
    std::pair<int, int> findValidPlayerPosition(std::mt19937& gen,
                                 std::uniform_int_distribution<>& rowDist,
                                 std::uniform_int_distribution<>& colDist);
    
    // 路径计算辅助函数
    /**
     * @brief 尝试一个拐角连接路径
     * @param r1 起始方块行坐标
     * @param c1 起始方块列坐标
     * @param r2 目标方块行坐标
     * @param c2 目标方块列坐标
     * @return true表示找到路径，false表示未找到
     * 
     * 计算一个拐角的连接路径并存储到m_linePath中。
     */
    bool tryOneCornerPath(int r1, int c1, int r2, int c2);
    
    /**
     * @brief 尝试两个拐角连接路径
     * @param r1 起始方块行坐标
     * @param c1 起始方块列坐标
     * @param r2 目标方块行坐标
     * @param c2 目标方块列坐标
     * @return true表示找到路径，false表示未找到
     * 
     * 计算两个拐角的连接路径并存储到m_linePath中。
     */
    bool tryTwoCornerPath(int r1, int c1, int r2, int c2);
    
    // 工具函数
    
    /**
     * @brief 获取玩家颜色
     * @param id 玩家ID
     * @return 对应的颜色对象
     * 
     * 根据玩家ID返回对应的颜色，用于绘制玩家。
     */
    QColor getPlayerColor(int id);
    


    
    // 其他工具函数


    /**
     * @brief 停止所有计时器
     * 
     * 停止游戏中的所有计时器，包括游戏计时器、提示计时器等。
     */
    void stopAllTimers();
    
    /**
     * @brief 重置游戏效果
     * 
     * 清除所有游戏效果状态，包括提示、眩晕、闪烁等。
     */
    void resetGameEffects();
    
    // 游戏保存辅助函数
    /**
     * @brief 保存游戏头部信息
     * @param out 输出流对象
     * 
     * 保存游戏的基本信息，包括版本、时间等。
     */
    void saveGameHeader(QTextStream& out);
    
    /**
     * @brief 保存玩家信息
     * @param out 输出流对象
     * 
     * 保存所有玩家的状态信息，包括位置、分数、状态效果等。
     */
    void saveGamePlayers(QTextStream& out);
    
    /**
     * @brief 保存游戏地图
     * @param out 输出流对象
     * 
     * 保存当前游戏地图的状态。
     */
    void saveGameMap(QTextStream& out);
    
    /**
     * @brief 保存道具信息
     * @param out 输出流对象
     * 
     * 保存地图上所有道具的位置和类型。
     */
    void saveGameItems(QTextStream& out);
    
    /**
     * @brief 保存游戏效果
     * @param out 输出流对象
     * 
     * 保存当前激活的游戏效果状态。
     */
    void saveGameEffects(QTextStream& out);
    
    // 游戏加载辅助函数
    /**
     * @brief 加载游戏头部信息
     * @param in 输入流对象
     * @return true表示加载成功，false表示失败
     * 
     * 加载游戏的基本信息，验证文件格式。
     */
    bool loadGameHeader(QTextStream& in);
    
    /**
     * @brief 加载玩家信息
     * @param in 输入流对象
     * 
     * 从文件中加载所有玩家的状态信息。
     */
    void loadGamePlayers(QTextStream& in);
    
    /**
     * @brief 加载游戏地图
     * @param in 输入流对象
     * 
     * 从文件中加载游戏地图状态。
     */
    void loadGameMap(QTextStream& in);
    
    /**
     * @brief 加载道具信息
     * @param in 输入流对象
     * 
     * 从文件中加载道具的位置和类型信息。
     */
    void loadGameItems(QTextStream& in);
    
    /**
     * @brief 加载游戏效果
     * @param in 输入流对象
     * 
     * 从文件中加载游戏效果状态。
     */
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
    
    // 消除判断器
    LinkJudger m_judger;
    
    // 音效系统 - 每个音效使用独立的QSoundEffect实例以支持同时播放
    QSoundEffect* m_eliminationSound;    ///< 方块消除音效播放器
    QSoundEffect* m_itemSound;           ///< 道具拾取音效播放器
    QSoundEffect* m_selectSound;         ///< 菜单选择音效播放器
    QSoundEffect* m_winSound;            ///< 游戏胜利音效播放器
    
    // 背景音乐系统
    QMediaPlayer* m_backgroundMusic;     ///< 背景音乐播放器
    QAudioOutput* m_audioOutput;         ///< 音频输出设备
    bool m_backgroundMusicPlaying;       ///< 背景音乐是否正在播放
    
    // 紧凑菜单系统
    QWidget* m_menuWidget;               ///< 菜单容器
    QHBoxLayout* m_menuLayout;           ///< 菜单布局
    QPushButton* m_gameButton;           ///< 游戏按钮
    QPushButton* m_helpButton;           ///< 帮助按钮
    QPushButton* m_saveButton;           ///< 存档按钮
    QPushButton* m_settingsButton;       ///< 设置按钮
    QMenu* m_gameMenu;                   ///< 游戏下拉菜单
    QMenu* m_helpMenu;                   ///< 帮助下拉菜单
    QMenu* m_saveMenu;                   ///< 存档下拉菜单
    QMenu* m_settingsMenu;               ///< 设置下拉菜单

private slots:
    /**
     * @brief 游戏计时器超时槽函数
     * 
     * 每秒调用一次，更新游戏时间，检查游戏是否结束。
     */
    void onTimerTimeout();
    
    /**
     * @brief 提示计时器超时槽函数
     * 
     * 提示效果结束时调用，清除提示高亮显示。
     */
    void onHintTimerTimeout();
    
    /**
     * @brief 眩晕计时器超时槽函数
     * @param player 要处理的玩家对象
     * @param timer 对应的眩晕定时器
     * 
     * 眩晕效果结束时调用，恢复正常移动能力。
     */
    void onDizzyTimerTimeout(Player& player, QTimer* timer);
    
    /**
     * @brief 道具生成计时器超时槽函数
     * 
     * 定期生成新道具，增加游戏趣味性。
     */
    void onItemSpawnTimerTimeout();
    
    
    /**
     * @brief 更新窗口标题
     * @param status 状态信息
     * 
     * 更新窗口标题，显示当前游戏状态。
     */
    void updateWindowTitle(const QString& status = "");
    
    // 音效播放函数
    /**
     * @brief 初始化音效系统
     * 
     * 设置所有音效播放器的文件源和音量。
     */
    void initializeAudioSystem();
    
    /**
     * @brief 播放方块消除音效
     * 
     * 当方块被成功消除时播放音效。
     */
    void playEliminationSound();
    
    /**
     * @brief 播放道具拾取音效
     * 
     * 当拾取道具时播放音效。
     */
    void playItemSound();
    
    /**
     * @brief 播放菜单选择音效
     * 
     * 当在菜单中选中选项时播放音效。
     */
    void playSelectSound();
    
    /**
     * @brief 播放游戏胜利音效
     * 
     * 当游戏结束时播放音效。
     */
    void playWinSound();
    
    // 背景音乐函数
    /**
     * @brief 播放背景音乐
     * 
     * 在菜单界面播放背景音乐，循环播放。
     */
    void playBackgroundMusic();
    
    /**
     * @brief 停止背景音乐
     * 
     * 停止背景音乐播放。
     */
    void stopBackgroundMusic();
    
    // 紧凑菜单系统
    /**
     * @brief 初始化紧凑菜单
     * 
     * 创建并设置紧凑的菜单栏。
     */
    void initializeCompactMenu();
    
    /**
     * @brief 创建游戏菜单
     * 
     * 创建游戏相关的下拉菜单。
     */
    void createGameMenu();
    
    /**
     * @brief 创建帮助菜单
     * 
     * 创建帮助相关的下拉菜单。
     */
    void createHelpMenu();
    
    /**
     * @brief 创建存档菜单
     * 
     * 创建存档相关的下拉菜单。
     */
    void createSaveMenu();
    
    /**
     * @brief 创建设置菜单
     * 
     * 创建设置相关的下拉菜单。
     */
    void createSettingsMenu();
    
};

#endif // GAMEWINDOW_H
