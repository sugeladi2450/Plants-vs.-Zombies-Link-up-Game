#include "gamewindow.h"
#include <QCoreApplication>
#include <QDir>

// Player类实现
/**
 * @brief Player构造函数实现
 * @param id 玩家ID
 * @param startRow 初始行坐标
 * @param startCol 初始列坐标
 * 
 * 初始化玩家对象的所有成员变量
 */
Player::Player(int id, int startRow, int startCol)
    : m_id(id)
    , m_row(startRow)
    , m_col(startCol)
    , m_score(0)
    , m_dizzy(false)
    , m_flashActive(false)
    , m_dizzyTime(0)
    , m_flashTime(0)
{
}

// 设置玩家位置
void Player::setPosition(int row, int col)
{
    m_row = row;
    m_col = col;
}

// 增加玩家分数
void Player::addScore(int points)
{
    m_score += points;
}

//设置玩家的眩晕状态和持续时间
void Player::setDizzy(bool dizzy, int time)
{
    m_dizzy = dizzy;
    m_dizzyTime = time;
}

//设置玩家的闪烁状态和持续时间
void Player::setFlashActive(bool active, int time)
{
    m_flashActive = active;
    m_flashTime = time;
}

/**
 * @brief 更新状态效果实现
 * 
 * 每调用一次，所有状态效果的剩余时间减1秒。
 * 当时间归零时，自动解除对应状态。
 */
void Player::updateEffects()
{
    // 更新眩晕效果
    if (m_dizzy && m_dizzyTime > 0) {
        if (--m_dizzyTime <= 0) {
            m_dizzy = false;
        }
    }
    
    // 更新闪烁效果
    if (m_flashActive && m_flashTime > 0) {
        if (--m_flashTime <= 0) {
            m_flashActive = false;
        }
    }
}


// 移动玩家
void Player::move(int deltaRow, int deltaCol)
{
        // 眩晕时方向颠倒
    if (m_dizzy) {
        deltaRow = -deltaRow;
        deltaCol = -deltaCol;
    }
    
    m_row += deltaRow;
    m_col += deltaCol;
}

//GameWindow构造函数实现
GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    // 游戏状态
    , m_state(MENU_STATE) // 菜单状态
    , m_option(START_NEW_GAME) // 菜单选项，初始选中开始新游戏
    , m_gameModeOption(SINGLE_PLAYER) // 游戏模式选项，默认单人模式
    , m_showGameModeSelection(false) // 不显示游戏模式选择
    , m_backButtonSelected(false) // 返回按钮未选中
    , m_showLoadSlots(false) // 不显示载入存档槽位选择
    , m_showSaveSlots(false) // 不显示保存存档槽位选择
    , m_showDeleteSlots(false) // 不显示删除存档槽位选择
    , m_selectedSaveSlot(SAVE_SLOT_1) // 默认选中存档槽位1
    , m_running(false) // 游戏运行
    , m_paused(false) // 游戏暂停
    , m_twoPlayer(false)  // 双人模式标志，由菜单选择决定
    , m_time(TIME)  // 游戏时间
    
    // 玩家相关
    , m_p1(1, 0, 0) // 玩家1，初始位置为(0,0)
    , m_p2(2, 0, 0) // 玩家2，初始位置为(0,0)
    , m_active(&m_p1)
    
    // 游戏交互
    , m_activeRow(-1) // 激活行
    , m_activeCol(-1) // 激活列
    , m_hintOn(false) // 提示开启
    , m_hintTime(0) // 提示时间
    , m_hintR1(-1), m_hintC1(-1), m_hintR2(-1), m_hintC2(-1)
    
    // 连接线显示
    , m_showLine(false)
    , m_lineR1(-1), m_lineC1(-1), m_lineR2(-1), m_lineC2(-1)
    
    // 定时器
    , m_timer(new QTimer(this))
    , m_hintTimer(new QTimer(this)) // 提示定时器
    , m_dizzy1(new QTimer(this)) // 玩家1眩晕定时器
    , m_dizzy2(new QTimer(this)) // 玩家2眩晕定时器
    , m_spawnTimer(new QTimer(this)) // 道具生成定时器
    , m_animationTimer(new QTimer(this)) // 动画定时器
    
    // GIF动图相关
    , m_zombieMovie(nullptr) // 僵尸GIF动图
    , m_zombieEatMovie(nullptr) // 僵尸攻击GIF动图
    , m_currentZombieFrame() // 当前僵尸帧
    , m_currentZombieEatFrame() // 当前僵尸攻击帧
    , m_isZombieAttacking(false) // 僵尸未在攻击
    , m_attackTimer(new QTimer(this)) // 攻击动画定时器
    
    // 背景图片相关
    , m_menuBackground() // 菜单背景图片
    
    // 其他组件
    , m_judger(ROWS, COLS) // 连接判断器
    , m_imagesLoaded(false) // 图片加载标志
    
    // 音效组件 - 每个音效使用独立的QSoundEffect实例以支持同时播放
    , m_eliminationSound(new QSoundEffect(this)) // 方块消除音效播放器
    , m_itemSound(new QSoundEffect(this)) // 道具拾取音效播放器
    , m_selectSound(new QSoundEffect(this)) // 菜单选择音效播放器
    , m_winSound(new QSoundEffect(this)) // 游戏胜利音效播放器
    
    // 背景音乐组件
    , m_backgroundMusic(new QMediaPlayer(this)) // 背景音乐播放器
    , m_audioOutput(new QAudioOutput(this)) // 音频输出设备
    , m_backgroundMusicPlaying(false) // 背景音乐未播放
    
    // 紧凑菜单组件
    , m_menuWidget(nullptr) // 菜单容器
    , m_menuLayout(nullptr) // 菜单布局
    , m_gameButton(nullptr) // 游戏按钮
    , m_helpButton(nullptr) // 帮助按钮
    , m_saveButton(nullptr) // 存档按钮
    , m_settingsButton(nullptr) // 设置按钮
    , m_gameMenu(nullptr) // 游戏下拉菜单
    , m_helpMenu(nullptr) // 帮助下拉菜单
    , m_saveMenu(nullptr) // 存档下拉菜单
    , m_settingsMenu(nullptr) // 设置下拉菜单
    , m_soundEnabled(true) // 音效默认启用
    , m_musicEnabled(true) // 背景音乐默认启用
{
    setWindowTitle("QLink");
    
    // 设置窗口大小和最小尺寸
    setMinimumSize(800, 600);
    resize(1000, 700);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColorConstants::Svg::darkgray);
    setAutoFillBackground(true); // 设置自动填充背景
    setPalette(pal);
    
    setFocusPolicy(Qt::StrongFocus); // 设置强焦点策略
    setMouseTracking(true); // 启用鼠标跟踪
    
    // 连接定时器信号
    connect(m_timer, &QTimer::timeout, this, &GameWindow::onTimerTimeout); // 游戏计时器超时槽函数
    connect(m_hintTimer, &QTimer::timeout, this, &GameWindow::onHintTimerTimeout); // 提示定时器超时槽函数
    connect(m_dizzy1, &QTimer::timeout, this, [this]() { onDizzyTimerTimeout(m_p1, m_dizzy1); }); // 玩家1眩晕定时器超时槽函数
    connect(m_dizzy2, &QTimer::timeout, this, [this]() { onDizzyTimerTimeout(m_p2, m_dizzy2); }); // 玩家2眩晕定时器超时槽函数
    connect(m_spawnTimer, &QTimer::timeout, this, &GameWindow::onItemSpawnTimerTimeout); // 道具生成定时器超时槽函数
    
    // 初始化音效系统
    initializeAudioSystem();
    
    // 初始化GIF动图
    initializeZombieAnimation();
    
    // 加载菜单背景图片
    loadMenuBackground();
    
    // 根据背景图片比例调整窗口大小
    adjustWindowSizeToBackground();
    
    // 初始化紧凑菜单
    initializeCompactMenu();
    
    // 显示开始菜单
    showMenu();
}

// 显示菜单
void GameWindow::showMenu()
{
    m_state = MENU_STATE; // 菜单状态
    m_option = START_NEW_GAME; // 默认开始新游戏
    m_showGameModeSelection = false; // 不显示游戏模式选择
    m_backButtonSelected = false; // 返回按钮未选中
    m_showLoadSlots = false; // 不显示载入存档槽位选择
    m_showSaveSlots = false; // 不显示保存存档槽位选择
    m_showDeleteSlots = false; // 不显示删除存档槽位选择
    
    // 加载方块图片
    loadBlockImages();
    
    // 播放背景音乐
    playBackgroundMusic();
    
    update();
}

// 隐藏菜单
void GameWindow::hideMenu()
{
    m_state = GAME_STATE;
    update();
}

// 选择菜单选项
void GameWindow::selectOption(MenuOption option)
{
    switch (option) {
    case START_NEW_GAME: // 开始新游戏
        m_showGameModeSelection = true; // 显示游戏模式选择
        update();
        break;
        
    case LOAD_GAME: // 载入游戏
        m_showLoadSlots = true; // 显示载入存档槽位选择
        m_selectedSaveSlot = SAVE_SLOT_1; // 默认选中第一个槽位
        update();
        break;
        
    case EXIT_GAME: // 退出游戏
        close(); // 关闭窗口
        break;
    }
}

// 选择游戏模式
void GameWindow::selectGameMode(GameModeOption mode)
{
    switch (mode) {
    case SINGLE_PLAYER: // 单人模式
        m_twoPlayer = false;
        hideMenu();
        start(); 
        break;
        
    case TWO_PLAYER: // 双人模式
        m_twoPlayer = true;
        hideMenu();
        start();
        break;
    }
}

// 选择存档槽位
void GameWindow::selectSaveSlot(SaveSlot slot)
{
    if (m_showLoadSlots) {
        // 载入存档
        QString fileName = getSaveFileName(slot);
        if (isSaveSlotExists(slot)) {
            loadGame(fileName);
        }
        m_showLoadSlots = false;
        update();
    } else if (m_showSaveSlots) {
        // 保存存档
        QString fileName = getSaveFileName(slot);
        saveGame(fileName);
        m_showSaveSlots = false;
        update();
    } else if (m_showDeleteSlots) {
        // 删除存档
        deleteSaveSlot(slot);
    }
}

// 检查存档是否存在
bool GameWindow::isSaveSlotExists(SaveSlot slot)
{
    QString fileName = getSaveFileName(slot);
    return QFile::exists(fileName);
}

// 获取存档文件名
QString GameWindow::getSaveFileName(SaveSlot slot)
{
    return QString("savegame%1.txt").arg(slot + 1);
}

// 获取存档显示名称
QString GameWindow::getSaveDisplayName(SaveSlot slot)
{
    QString baseName = QString("存档%1").arg(slot + 1);
    if (isSaveSlotExists(slot)) {
        return baseName;
    } else {
        return QString("%1 (空)").arg(baseName);
    }
}

// 删除存档
void GameWindow::deleteSaveSlot(SaveSlot slot)
{
    QString fileName = getSaveFileName(slot);
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }
    m_showDeleteSlots = false;
    update();
}

// 绘制菜单
void GameWindow::drawMenu(QPainter& painter)
{
    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true); // 图形抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing, true); // 文本抗锯齿
    
    // 绘制背景
    painter.fillRect(rect(), QColorConstants::Svg::darkblue);
    
    // 绘制标题
    drawTitle(painter);
    
    // 绘制菜单选项
    drawOptions(painter);
    
    // 绘制操作提示
    painter.setPen(QColorConstants::Svg::white); // 设置白色画笔
    painter.setFont(QFont("Arial", 12)); // 设置字体
    QRect hintRect = rect(); // 设置提示矩形
    hintRect.setTop(height() - 100); // 设置提示矩形顶部位置
    painter.drawText(hintRect, Qt::AlignCenter, // 设置提示文字居中
                    "使用 ↑↓ 键选择，Enter 键确认，或使用鼠标点击选项"); // 设置提示文字
}

// 绘制标题
void GameWindow::drawTitle(QPainter& painter)
{
    painter.setPen(QColorConstants::Svg::white); // 设置白色画笔
    painter.setFont(QFont("Arial", 24, QFont::Bold)); // 设置字体
    
    QRect titleRect = rect(); // 设置标题矩形
    titleRect.setHeight(100); // 设置标题矩形高度
    painter.drawText(titleRect, Qt::AlignCenter, "QLink 连连看游戏"); // 设置标题文字
}

// 绘制菜单选项
void GameWindow::drawOptions(QPainter& painter)
{
    // 绘制菜单背景图片
    if (!m_menuBackground.isNull()) {
        // 获取窗口尺寸
        int windowWidth = width();
        int windowHeight = height();
        
        // 使用Qt::KeepAspectRatioByExpanding确保图片完全填满窗口，边框完全贴合
        QPixmap scaledBackground = m_menuBackground.scaled(
            QSize(windowWidth, windowHeight), 
            Qt::KeepAspectRatioByExpanding, // 保持宽高比，完全填满窗口
            Qt::SmoothTransformation
        );
        
        // 计算绘制位置，使图片居中（可能会超出窗口边界）
        QPoint drawPos(0, 0);
        if (scaledBackground.width() > windowWidth) {
            drawPos.setX((scaledBackground.width() - windowWidth) / 2);
        }
        if (scaledBackground.height() > windowHeight) {
            drawPos.setY((scaledBackground.height() - windowHeight) / 2);
        }
        
        // 绘制背景图片（只绘制窗口范围内的部分）
        painter.drawPixmap(0, 0, scaledBackground, drawPos.x(), drawPos.y(), windowWidth, windowHeight);
    }
    
    if (m_showLoadSlots) {
        // 显示载入存档槽位选择
        static const int startY = 150; // 设置菜单选项起始位置
        static const int optionHeight = 50; // 设置菜单选项高度
        static const int spacing = 20; // 设置菜单选项间距
        static const int cornerRadius = 10; // 设置菜单选项圆角
        static const int selectionOffset = 5; // 设置菜单选项选中偏移
        
        // 绘制返回按钮和存档槽位
        QString options[] = {"返回", getSaveDisplayName(SAVE_SLOT_1), getSaveDisplayName(SAVE_SLOT_2), getSaveDisplayName(SAVE_SLOT_3)};
        
        for (int i = 0; i < 4; ++i) {
        QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            // 设置菜单选项选中状态
            bool isSelected = false;
            if (i == 0) {
                // 返回按钮
                isSelected = m_backButtonSelected;
            } else {
                // 存档槽位 - 只有在返回按钮未选中时才可能被选中
                isSelected = !m_backButtonSelected && (i - 1 == static_cast<int>(m_selectedSaveSlot));
            }
            
            // 选中项放大矩形
        if (isSelected) {
                optionRect.adjust(-selectionOffset, -selectionOffset, 
                                 selectionOffset, selectionOffset); //左边，上边，右边，下边的偏移量
            }
            
            drawOption(painter, optionRect, options[i], isSelected);
        }
    } else if (m_showSaveSlots) {
        // 显示保存存档槽位选择
        static const int startY = 150; // 设置菜单选项起始位置
        static const int optionHeight = 50; // 设置菜单选项高度
        static const int spacing = 20; // 设置菜单选项间距
        static const int cornerRadius = 10; // 设置菜单选项圆角
        static const int selectionOffset = 5; // 设置菜单选项选中偏移
        
        // 绘制返回按钮和存档槽位
        QString options[] = {"返回", getSaveDisplayName(SAVE_SLOT_1), getSaveDisplayName(SAVE_SLOT_2), getSaveDisplayName(SAVE_SLOT_3)};
        
        for (int i = 0; i < 4; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            // 设置菜单选项选中状态
            bool isSelected = false;
            if (i == 0) {
                // 返回按钮
                isSelected = m_backButtonSelected;
            } else {
                // 存档槽位 - 只有在返回按钮未选中时才可能被选中
                isSelected = !m_backButtonSelected && (i - 1 == static_cast<int>(m_selectedSaveSlot));
            }
            
            // 选中项放大矩形
            if (isSelected) {
                optionRect.adjust(-selectionOffset, -selectionOffset, 
                                 selectionOffset, selectionOffset); //左边，上边，右边，下边的偏移量
            }
            
            drawOption(painter, optionRect, options[i], isSelected);
        }
    } else if (m_showDeleteSlots) {
        // 显示删除存档槽位选择
        static const int startY = 150; // 设置菜单选项起始位置
        static const int optionHeight = 50; // 设置菜单选项高度
        static const int spacing = 20; // 设置菜单选项间距
        static const int cornerRadius = 10; // 设置菜单选项圆角
        static const int selectionOffset = 5; // 设置菜单选项选中偏移
        
        // 绘制返回按钮和存档槽位
        QString options[] = {"返回", getSaveDisplayName(SAVE_SLOT_1), getSaveDisplayName(SAVE_SLOT_2), getSaveDisplayName(SAVE_SLOT_3)};
        
        for (int i = 0; i < 4; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            // 设置菜单选项选中状态
            bool isSelected = false;
            if (i == 0) {
                // 返回按钮
                isSelected = m_backButtonSelected;
            } else {
                // 存档槽位 - 只有在返回按钮未选中时才可能被选中
                isSelected = !m_backButtonSelected && (i - 1 == static_cast<int>(m_selectedSaveSlot));
            }
            
            // 选中项放大矩形
            if (isSelected) {
                optionRect.adjust(-selectionOffset, -selectionOffset, 
                                 selectionOffset, selectionOffset); //左边，上边，右边，下边的偏移量
            }
            
            drawOption(painter, optionRect, options[i], isSelected);
        }
    } else if (m_showGameModeSelection) {
        // 显示游戏模式选择
        static const QString gameModeOptions[] = {"返回", "单人模式", "双人模式"}; // 设置选项（包含返回按钮）
        static const int startY = 150; // 设置菜单选项起始位置
        static const int optionHeight = 50; // 设置菜单选项高度
        static const int spacing = 20; // 设置菜单选项间距
        static const int cornerRadius = 10; // 设置菜单选项圆角
        static const int selectionOffset = 5; // 设置菜单选项选中偏移
        
        // 绘制所有选项（包括返回按钮）
        for (int i = 0; i < 3; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            // 设置菜单选项选中状态 - 只有一个选项被选中
            bool isSelected = false;
            if (i == 0) {
                // 返回按钮
                isSelected = m_backButtonSelected;
            } else {
                // 游戏模式选项 - 只有在返回按钮未选中时才可能被选中
                isSelected = !m_backButtonSelected && (i - 1 == static_cast<int>(m_gameModeOption));
            }
            
            // 选中项放大矩形
            if (isSelected) {
                optionRect.adjust(-selectionOffset, -selectionOffset, 
                                 selectionOffset, selectionOffset); //左边，上边，右边，下边的偏移量
            }
            
            drawOption(painter, optionRect, gameModeOptions[i], isSelected);
        }
    } else {
        // 显示主菜单选项
        static const QString options[] = {"开始新游戏", "载入游戏", "退出游戏"}; // 设置菜单选项
        static const int startY = 150; // 设置菜单选项起始位置
        static const int optionHeight = 50; // 设置菜单选项高度
        static const int spacing = 20; // 设置菜单选项间距
        static const int cornerRadius = 10; // 设置菜单选项圆角
        static const int selectionOffset = 5; // 设置菜单选项选中偏移
        
        // 绘制菜单选项
        for (int i = 0; i < OPTIONS; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            // 设置菜单选项选中状态
            bool isSelected = (i == static_cast<int>(m_option)); 
            
            // 选中项放大矩形
            if (isSelected) {
                optionRect.adjust(-selectionOffset, -selectionOffset, 
                                 selectionOffset, selectionOffset); //左边，上边，右边，下边的偏移量
            }
            
            drawOption(painter, optionRect, options[i], isSelected);
        }
    }
}

// 绘制单个菜单选项
void GameWindow::drawOption(QPainter& painter, const QRect& rect, 
                           const QString& text, bool isSelected)
{
        // 绘制圆角矩形背景
        QPainterPath path;
    path.addRoundedRect(rect, 10, 10); // 设置圆角矩形
        
        if (isSelected) {
            // 绘制发光效果
            QPainterPath glowPath; // 设置发光路径
        glowPath.addRoundedRect(rect.adjusted(-2, -2, 2, 2), 12, 12); //创建比原矩形大4像素的发光区域
            painter.setBrush(QColor(255, 165, 0, 50)); // 半透明橙色
            painter.setPen(Qt::NoPen); // 不画边框
            painter.drawPath(glowPath); // 绘制发光效果
            
        // 选中项样式
            painter.setBrush(QColorConstants::Svg::orange); // 设置橙色画刷
            painter.setPen(QPen(QColorConstants::Svg::yellow, 3)); // 设置黄色画笔
        } else {
        // 未选中项样式
            painter.setBrush(QColorConstants::Svg::lightgray); // 设置浅灰色画刷
            painter.setPen(QPen(QColorConstants::Svg::white, 1)); // 设置白色画笔
        }
        
        painter.drawPath(path); // 绘制圆角矩形背景
        
    // 绘制文字
        painter.setFont(QFont("Arial", isSelected ? 18 : 16, QFont::Bold)); // 设置字体
        painter.setPen(isSelected ? QColorConstants::Svg::black : QColorConstants::Svg::white); // 设置画笔颜色
    painter.drawText(rect, Qt::AlignCenter, text); // 绘制文字
}

// 处理按键事件
void GameWindow::handleKey(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up: // 上键
        if (m_showLoadSlots) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到最后一个存档槽位
                m_backButtonSelected = false;
                m_selectedSaveSlot = SAVE_SLOT_3; // 存档槽位3
            } else if (m_selectedSaveSlot > 0) {
                m_selectedSaveSlot = static_cast<SaveSlot>(m_selectedSaveSlot - 1);
            } else {
                // 从第一个存档槽位移动到返回按钮
                m_backButtonSelected = true;
                m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
            }
        } else if (m_showSaveSlots) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到最后一个存档槽位
                m_backButtonSelected = false;
                m_selectedSaveSlot = SAVE_SLOT_3; // 存档槽位3
            } else if (m_selectedSaveSlot > 0) {
                m_selectedSaveSlot = static_cast<SaveSlot>(m_selectedSaveSlot - 1);
            } else {
                // 从第一个存档槽位移动到返回按钮
                m_backButtonSelected = true;
                m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
            }
        } else if (m_showDeleteSlots) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到最后一个存档槽位
                m_backButtonSelected = false;
                m_selectedSaveSlot = SAVE_SLOT_3; // 存档槽位3
            } else if (m_selectedSaveSlot > 0) {
                m_selectedSaveSlot = static_cast<SaveSlot>(m_selectedSaveSlot - 1);
            } else {
                // 从第一个存档槽位移动到返回按钮
                m_backButtonSelected = true;
                m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
            }
        } else if (m_showGameModeSelection) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到最后一个游戏模式选项
                m_backButtonSelected = false;
                m_gameModeOption = static_cast<GameModeOption>(1); // 双人模式
            } else if (m_gameModeOption > 0) {
                m_gameModeOption = static_cast<GameModeOption>(m_gameModeOption - 1);
            } else {
                // 从第一个游戏模式选项移动到返回按钮
                m_backButtonSelected = true;
                m_gameModeOption = SINGLE_PLAYER; // 重置游戏模式选项
            }
        } else {
            if (m_option > 0) { //不是最上面的选项
                m_option = static_cast<MenuOption>(m_option - 1); // 选择上一个选项
        } else {
            // 循环到最后一个选项
                m_option = static_cast<MenuOption>(OPTIONS - 1); // 选择最后一个选项
        }
        }
        // 播放菜单选择音效
        playSelectSound();
        update();
        break;
    case Qt::Key_Down:
        if (m_showLoadSlots) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到第一个存档槽位
                m_backButtonSelected = false;
                m_selectedSaveSlot = SAVE_SLOT_1; // 存档槽位1
            } else if (m_selectedSaveSlot < 2) {
                m_selectedSaveSlot = static_cast<SaveSlot>(m_selectedSaveSlot + 1);
            } else {
                // 从最后一个存档槽位移动到返回按钮
                m_backButtonSelected = true;
                m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
            }
        } else if (m_showSaveSlots) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到第一个存档槽位
                m_backButtonSelected = false;
                m_selectedSaveSlot = SAVE_SLOT_1; // 存档槽位1
            } else if (m_selectedSaveSlot < 2) {
                m_selectedSaveSlot = static_cast<SaveSlot>(m_selectedSaveSlot + 1);
            } else {
                // 从最后一个存档槽位移动到返回按钮
                m_backButtonSelected = true;
                m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
            }
        } else if (m_showDeleteSlots) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到第一个存档槽位
                m_backButtonSelected = false;
                m_selectedSaveSlot = SAVE_SLOT_1; // 存档槽位1
            } else if (m_selectedSaveSlot < 2) {
                m_selectedSaveSlot = static_cast<SaveSlot>(m_selectedSaveSlot + 1);
            } else {
                // 从最后一个存档槽位移动到返回按钮
                m_backButtonSelected = true;
                m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
            }
        } else if (m_showGameModeSelection) {
            if (m_backButtonSelected) {
                // 从返回按钮移动到第一个游戏模式选项
                m_backButtonSelected = false;
                m_gameModeOption = static_cast<GameModeOption>(0); // 单人模式
            } else if (m_gameModeOption < 1) {
                m_gameModeOption = static_cast<GameModeOption>(m_gameModeOption + 1);
            } else {
                // 从最后一个游戏模式选项移动到返回按钮
                m_backButtonSelected = true;
                m_gameModeOption = SINGLE_PLAYER; // 重置游戏模式选项
            }
        } else {
            if (m_option < OPTIONS - 1) { //不是最下面的选项
                m_option = static_cast<MenuOption>(m_option + 1); // 选择下一个选项
        } else {
            // 循环到第一个选项
                m_option = static_cast<MenuOption>(0); // 选择第一个选项
        }
        }
        // 播放菜单选择音效
        playSelectSound();
        update();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (m_showLoadSlots) {
            if (m_backButtonSelected) {
                // 返回主菜单
                m_showLoadSlots = false;
                m_backButtonSelected = false;
                update();
            } else {
                selectSaveSlot(m_selectedSaveSlot); // 选择当前存档槽位
            }
        } else if (m_showSaveSlots) {
            if (m_backButtonSelected) {
                // 返回主菜单
                m_showSaveSlots = false;
                m_backButtonSelected = false;
                update();
            } else {
                selectSaveSlot(m_selectedSaveSlot); // 选择当前存档槽位
            }
        } else if (m_showDeleteSlots) {
            if (m_backButtonSelected) {
                // 返回主菜单
                m_showDeleteSlots = false;
                m_backButtonSelected = false;
                update();
            } else {
                selectSaveSlot(m_selectedSaveSlot); // 选择当前存档槽位
            }
        } else if (m_showGameModeSelection) {
            if (m_backButtonSelected) {
                // 返回主菜单
                m_showGameModeSelection = false;
                m_backButtonSelected = false;
                update();
            } else {
                selectGameMode(m_gameModeOption); // 选择当前游戏模式
            }
        } else {
            selectOption(m_option); // 选择当前选项
        }
        break;
    case Qt::Key_Escape:
        if (m_showLoadSlots) {
            // 从载入存档槽位选择返回主菜单
            m_showLoadSlots = false;
            m_backButtonSelected = false;
            update();
        } else if (m_showSaveSlots) {
            // 从保存存档槽位选择返回主菜单
            m_showSaveSlots = false;
            m_backButtonSelected = false;
            update();
        } else if (m_showDeleteSlots) {
            // 从删除存档槽位选择返回主菜单
            m_showDeleteSlots = false;
            m_backButtonSelected = false;
            update();
        } else if (m_showGameModeSelection) {
            // 从游戏模式选择返回主菜单
            m_showGameModeSelection = false;
            update();
        } else if (m_running) { // 如果游戏正在运行
            //返回菜单
            end();
            showMenu();
        } else {
            close();
        }
        break;
    }
}

// 处理菜单鼠标事件（移动高亮和点击选择）
void GameWindow::handleMenuMouseEvent(QMouseEvent *event, bool isClick)
{
    if (m_state != MENU_STATE && !m_showSaveSlots && !m_showLoadSlots && !m_showDeleteSlots) return; // 如果不是菜单状态且不显示存档槽位选择，则返回
    
    // 计算游戏区域（排除菜单栏）
    int menuHeight = m_menuWidget ? m_menuWidget->height() : 0;
    QPoint adjustedPos = event->pos();
    adjustedPos.setY(adjustedPos.y() - menuHeight); // 调整鼠标位置
    
    if (m_showLoadSlots) {
        // 处理载入存档槽位选择
        int startY = 150; // 设置菜单选项起始位置
        int optionHeight = 50; // 设置菜单选项高度
        int spacing = 20; // 设置菜单选项间距
        
        // 检查所有选项（包括返回按钮和存档槽位）
        for (int i = 0; i < 4; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            if (optionRect.contains(adjustedPos)) {
                if (i == 0) {
                    // 返回按钮
                    bool oldSelected = m_backButtonSelected;
                    m_backButtonSelected = true;
                    m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
                    
                    if (isClick && event->button() == Qt::LeftButton) {
                        m_showLoadSlots = false;
                        m_backButtonSelected = false;
                        update();
                    } else if (!isClick && oldSelected != m_backButtonSelected) {
                        // 播放菜单选择音效
                        playSelectSound();
                        update();
                    }
                } else {
                    // 存档槽位
                    SaveSlot oldSlot = m_selectedSaveSlot; // 保存旧的选项
                    m_selectedSaveSlot = static_cast<SaveSlot>(i - 1); // 设置当前选中的存档槽位
                    m_backButtonSelected = false; // 取消返回按钮选中

                    if (isClick && event->button() == Qt::LeftButton) {
                        // 鼠标点击：执行选择
                        selectSaveSlot(m_selectedSaveSlot);
                    } else { // 不点击左键
                        // 鼠标移动：只更新高亮显示
                        if (oldSlot != m_selectedSaveSlot) { // 如果选项改变了
                            // 播放菜单选择音效
                            playSelectSound();
                        }
                        update();
                    }
                }
                break;
            }
        }
    } else if (m_showSaveSlots) {
        // 处理保存存档槽位选择
        int startY = 150; // 设置菜单选项起始位置
        int optionHeight = 50; // 设置菜单选项高度
        int spacing = 20; // 设置菜单选项间距
        
        // 检查所有选项（包括返回按钮和存档槽位）
        for (int i = 0; i < 4; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            if (optionRect.contains(adjustedPos)) {
                if (i == 0) {
                    // 返回按钮
                    bool oldSelected = m_backButtonSelected;
                    m_backButtonSelected = true;
                    m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
                    
                    if (isClick && event->button() == Qt::LeftButton) {
                        m_showSaveSlots = false;
                        m_backButtonSelected = false;
                        update();
                    } else if (!isClick && oldSelected != m_backButtonSelected) {
                        // 播放菜单选择音效
                        playSelectSound();
                        update();
                    }
                } else {
                    // 存档槽位
                    SaveSlot oldSlot = m_selectedSaveSlot; // 保存旧的选项
                    m_selectedSaveSlot = static_cast<SaveSlot>(i - 1); // 设置当前选中的存档槽位
                    m_backButtonSelected = false; // 取消返回按钮选中

                    if (isClick && event->button() == Qt::LeftButton) {
                        // 鼠标点击：执行选择
                        selectSaveSlot(m_selectedSaveSlot);
                    } else { // 不点击左键
                        // 鼠标移动：只更新高亮显示
                        if (oldSlot != m_selectedSaveSlot) { // 如果选项改变了
                            // 播放菜单选择音效
                            playSelectSound();
                        }
                        update();
                    }
                }
                break;
            }
        }
    } else if (m_showDeleteSlots) {
        // 处理删除存档槽位选择
        int startY = 150; // 设置菜单选项起始位置
        int optionHeight = 50; // 设置菜单选项高度
        int spacing = 20; // 设置菜单选项间距
        
        // 检查所有选项（包括返回按钮和存档槽位）
        for (int i = 0; i < 4; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            if (optionRect.contains(adjustedPos)) {
                if (i == 0) {
                    // 返回按钮
                    bool oldSelected = m_backButtonSelected;
                    m_backButtonSelected = true;
                    m_selectedSaveSlot = SAVE_SLOT_1; // 重置存档槽位选择
                    
                    if (isClick && event->button() == Qt::LeftButton) {
                        m_showDeleteSlots = false;
                        m_backButtonSelected = false;
                        update();
                    } else if (!isClick && oldSelected != m_backButtonSelected) {
                        // 播放菜单选择音效
                        playSelectSound();
                        update();
                    }
                } else {
                    // 存档槽位
                    SaveSlot oldSlot = m_selectedSaveSlot; // 保存旧的选项
                    m_selectedSaveSlot = static_cast<SaveSlot>(i - 1); // 设置当前选中的存档槽位
                    m_backButtonSelected = false; // 取消返回按钮选中

                    if (isClick && event->button() == Qt::LeftButton) {
                        // 鼠标点击：执行选择
                        selectSaveSlot(m_selectedSaveSlot);
                    } else { // 不点击左键
                        // 鼠标移动：只更新高亮显示
                        if (oldSlot != m_selectedSaveSlot) { // 如果选项改变了
                            // 播放菜单选择音效
                            playSelectSound();
                        }
                        update();
                    }
                }
                break;
            }
        }
    } else if (m_showGameModeSelection) {
        // 处理游戏模式选择
        int startY = 150; // 设置菜单选项起始位置
        int optionHeight = 50; // 设置菜单选项高度
        int spacing = 20; // 设置菜单选项间距
        
        // 检查所有选项（包括返回按钮）
        for (int i = 0; i < 3; ++i) {
            QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            if (optionRect.contains(adjustedPos)) {
                if (i == 0) {
                    // 返回按钮
                    bool oldSelected = m_backButtonSelected;
                    m_backButtonSelected = true;
                    m_gameModeOption = SINGLE_PLAYER; // 重置游戏模式选项
                    
                    if (isClick && event->button() == Qt::LeftButton) {
                        m_showGameModeSelection = false;
                        m_backButtonSelected = false;
                        update();
                    } else if (!isClick && oldSelected != m_backButtonSelected) {
                        // 播放菜单选择音效
                        playSelectSound();
                        update();
                    }
                } else {
                    // 游戏模式选项
                    GameModeOption oldOption = m_gameModeOption; // 保存旧的选项
                    m_gameModeOption = static_cast<GameModeOption>(i - 1); // 设置当前选中的游戏模式选项
                    m_backButtonSelected = false; // 取消返回按钮选中

                    if (isClick && event->button() == Qt::LeftButton) {
                        // 鼠标点击：执行选择
                        selectGameMode(m_gameModeOption);
                    } else { // 不点击左键
                        // 鼠标移动：只更新高亮显示
                        if (oldOption != m_gameModeOption) { // 如果选项改变了
                            // 播放菜单选择音效
                            playSelectSound();
                        }
                        update();
                    }
                }
                break;
            }
        }
    } else {
        // 处理主菜单选项
        int startY = 150; // 设置菜单选项起始位置
        int optionHeight = 50; // 设置菜单选项高度
        int spacing = 20; // 设置菜单选项间距
        
        for (int i = 0; i < OPTIONS; ++i) { // 遍历菜单选项
        QRect optionRect(50, startY + i * (optionHeight + spacing), 
                            width() - 100, optionHeight); // 设置菜单选项矩形
            
            if (optionRect.contains(adjustedPos)) {
                MenuOption oldOption = m_option; // 保存旧的选项
                m_option = static_cast<MenuOption>(i); // 设置当前选中的菜单选项

                if (isClick && event->button() == Qt::LeftButton) {
                    // 鼠标点击：执行选择
                    selectOption(m_option);
                } else { // 不点击左键
                    // 鼠标移动：只更新高亮显示
                    if (oldOption != m_option) { // 如果选项改变了
                        // 播放菜单选择音效
                        playSelectSound();
                    }
            update();
                }
            break;
            }
        }
    }
}

// 析构函数
GameWindow::~GameWindow()
{
}

// 开始游戏
void GameWindow::start()
{
    // 停止所有定时器并重置状态
    stopAllTimers();
    resetGameEffects(); // 重置游戏效果
    
    // 停止背景音乐
    stopBackgroundMusic();
    
    m_hintTime = 0;
    m_hintR1 = m_hintC1 = m_hintR2 = m_hintC2 = -1; // 重置提示位置
    m_activeRow = m_activeCol = -1; // 重置激活位置
    m_paused = false;
    m_running = true;
    
    generateMap(); // 生成地图
    
    // 初始化玩家位置
    std::random_device rd; // 随机数生成器
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, ROWS - 1); // 行随机分布
    std::uniform_int_distribution<> colDist(0, COLS - 1); // 列随机分布
    
    // 初始化玩家1位置
    initializePlayerPosition(m_p1, gen, rowDist, colDist);
    
    // 玩家2位置：随机选择空地（双人模式）
    if (m_twoPlayer) {
        initializePlayerPosition(m_p2, gen, rowDist, colDist);
    }
    
    m_active = &m_p1;
    m_p1.addScore(-m_p1.getScore()); // 重置分数
    m_p2.addScore(-m_p2.getScore()); // 重置分数
    
    generateItems(); //在地图的空白位置随机生成道具，包括时间加成、重新排列、提示等。 根据游戏模式（单人/双人）生成不同的道具类型。
    m_time = TIME;
    m_running = true;
    m_timer->start(1000);
    
    // 启动道具生成定时器，每10-20秒随机生成一个道具
    m_spawnTimer->start(std::uniform_int_distribution<>(10000, 20000)(gen));
    
    
    // 更新窗口标题
    updateWindowTitle();
    
    update();
}

// 结束游戏
void GameWindow::end()
{
    m_running = false;
    stopAllTimers(); // 停止所有计时器
    resetGameEffects(); // 重置游戏效果
    
    // 播放游戏胜利音效
    playWinSound();
    
    showGameResult(); //显示游戏结果
}

//暂停游戏
void GameWindow::pause()
{
    if (m_running && !m_paused) { // 如果游戏正在运行且没有暂停
        m_paused = true;
        stopAllTimers(); // 停止所有计时器
        updateWindowTitle("已暂停");
        update();
    }
}

//恢复游戏
void GameWindow::resume()
{
    if (m_running && m_paused) { // 如果游戏正在运行且暂停
        m_paused = false;
        
        // 重新启动定时器
        if (m_running) {
            m_timer->start(1000);
            if (m_hintOn && m_hintTime > 0) { // 如果提示道具还在生效中，重新启动提示定时器
                m_hintTimer->start(1000);
            }

            if (m_p1.isDizzy() && m_p1.getDizzyTime() > 0) { // 如果玩家1眩晕还在生效中，重新启动玩家1眩晕定时器
                m_dizzy1->start(1000);
            }
            if (m_p2.isDizzy() && m_p2.getDizzyTime() > 0) { // 如果玩家2眩晕还在生效中，重新启动玩家2眩晕定时器
                m_dizzy2->start(1000);
            }
            // 重新启动道具生成定时器,每隔10-20秒随机生成一个道具
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(10000, 20000); 
            m_spawnTimer->start(dist(gen)); 
            
        }
        
        updateWindowTitle();
        update();
    }
}

// 显示游戏结果
void GameWindow::showGameResult()
{
    QString result;
    if (m_twoPlayer) { // 双人模式
        int score1 = m_p1.getScore();
        int score2 = m_p2.getScore();
        
        if (score1 > score2) {
            result = QString("游戏结束！\n玩家1获胜！\n玩家1分数：%1\n玩家2分数：%2")
                    .arg(score1).arg(score2);
        } else if (score2 > score1) {
            result = QString("游戏结束！\n玩家2获胜！\n玩家1分数：%1\n玩家2分数：%2")
                    .arg(score1).arg(score2);
        } else {
            result = QString("游戏结束！\n平局！\n玩家1分数：%1\n玩家2分数：%2")
                    .arg(score1).arg(score2);
        }
    } else { //单人模式
        result = QString("游戏结束！\n最终分数：%1\n剩余时间：%2秒")
                .arg(m_p1.getScore()).arg(m_time);
    }
    
    QMessageBox::information(static_cast<QWidget*>(this), "游戏结束", result); 
    showMenu();  // 返回开始菜单
}

// 生成地图
void GameWindow::generateMap()
{
    std::random_device rd; // 随机数生成器
    std::mt19937 gen(rd());
    
    initializeMapData();
    auto blockTypes = createBlockTypes(gen); //创建各种方块类型
    fillGameArea(blockTypes); //填充游戏区域
}

// 初始化地图数据
void GameWindow::initializeMapData()
{
    m_mapData.resize(ROWS); // 设置地图行数
    for (int i = 0; i < ROWS; ++i) {
        m_mapData[i].resize(COLS); // 设置地图列数
    }

    // 初始化地图为空白（值为0）
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            m_mapData[r][c] = 0;
        }
        }
    }

// 创建方块类型
std::vector<int> GameWindow::createBlockTypes(std::mt19937& gen)
{
    // 计算中间部分游戏区域，留出空白）
    int startRow = 2;  // 从第2行开始
    int endRow = ROWS - 3;  // 到倒数第3行结束
    int startCol = 2;  // 从第2列开始
    int endCol = COLS - 3;  // 到倒数第3列结束
    
    int gameAreaRows = endRow - startRow + 1; // 游戏区域行数
    int gameAreaCols = endCol - startCol + 1; // 游戏区域列数
    int totalGameCells = gameAreaRows * gameAreaCols; // 游戏区域总方块数
    
    // 确保游戏区域有偶数个方块，以便成对消除
    if (totalGameCells % 2 != 0) { // 如果是奇数
        totalGameCells--;  // 如果是奇数，减1变成偶数
    }
    
    // 计算每种类型需要多少对方块
    int pairsPerType = totalGameCells / (TYPES * 2); // 每种类型需要多少对方块
    int remainingCells = totalGameCells % (TYPES * 2); // 剩余方块数
    
    // 创建方块类型数组，确保每种类型成对出现
    std::vector<int> blockTypes;
    for (int type = 1; type <= TYPES; ++type) {
        for (int pair = 0; pair < pairsPerType; ++pair) {
            blockTypes.push_back(type);
            blockTypes.push_back(type);
        }
    }
    
    // 如果还有剩余位置，随机选择类型填充（确保成对）
    for (int i = 0; i < remainingCells; i += 2) {
        if (i + 1 < remainingCells) { // 如果剩余方块数大于2
            int randomType = std::uniform_int_distribution<>(1, TYPES)(gen); // 随机选择类型
            blockTypes.push_back(randomType);
            blockTypes.push_back(randomType);
        }
    }
    
    // 随机打乱方块类型
    std::shuffle(blockTypes.begin(), blockTypes.end(), gen);
    return blockTypes;
}

// 填充游戏区域
void GameWindow::fillGameArea(const std::vector<int>& blockTypes)
{
    // 计算游戏区域（中间部分，留出外围空白）
    int startRow = 2;  // 从第2行开始
    int endRow = ROWS - 3;  // 到倒数第3行结束
    int startCol = 2;  // 从第2列开始
    int endCol = COLS - 3;  // 到倒数第3列结束
    
    // 在游戏区域填充方块
    int index = 0;
    for (int r = startRow; r <= endRow && index < blockTypes.size(); ++r) {
        for (int c = startCol; c <= endCol && index < blockTypes.size(); ++c) {
            m_mapData[r][c] = blockTypes[index++];
        }
    }
}

//生成道具函数
void GameWindow::generateItems()
{
    m_items.clear(); //清空道具
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // 根据游戏模式选择道具类型,双人模式下生成时间加成、重新排列、提示、眩晕,单人模式下生成时间加成、重新排列、提示、闪烁
    std::vector<int> availableItems;
    if (m_twoPlayer) { //双人模式下生成时间加成、重新排列、提示、眩晕
        availableItems = {TIME_BONUS, SHUFFLE, HINT, DIZZY};

    } else { //单人模式下生成时间加成、重新排列、提示、闪烁
        availableItems = {TIME_BONUS, SHUFFLE, HINT, FLASH};

    }
    
    // 收集所有空白位置作为道具生成点
    std::vector<std::pair<int, int>> emptyPositions;
    
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (m_mapData[r][c] == 0) { //空白位置
                emptyPositions.emplace_back(r, c); //收集所有空白位置作为道具生成点
            }
        }
    }
    
    // 生成3-5个道具
    int numItems = std::uniform_int_distribution<>(3, 5)(gen);
    
    // 随机打乱空白位置，以确保道具生成位置随机
    std::shuffle(emptyPositions.begin(), emptyPositions.end(), gen);
    
    for (int i = 0; i < numItems; ++i) {
        int row = emptyPositions[i].first;
        int col = emptyPositions[i].second;
        
        // 从可用道具中随机选择，确保道具类型随机
        std::uniform_int_distribution<> itemIndexDist(0, availableItems.size() - 1);
        int itemType = availableItems[itemIndexDist(gen)];
        
        m_items.emplace_back(row, col, itemType);

    }
}

//检查道具碰撞函数
void GameWindow::checkItemCollision(Player& player)
{
    const int playerRow = player.getRow(); //玩家行坐标
    const int playerCol = player.getCol(); //玩家列坐标

    //遍历道具，检查是否与玩家碰撞
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [playerRow, playerCol](const Item& item) {
            return item.row == playerRow && item.col == playerCol;
        });
    
    if (it != m_items.end()) {
            activateItem(it->type, player); //激活道具效果
            m_items.erase(it); //删除碰撞的道具
            // 播放道具拾取音效
            playItemSound();
    }
}

//激活道具效果函数
void GameWindow::activateItem(int itemType, Player& player)
{
    switch (itemType) {
    case TIME_BONUS:
        addTime(); //增加时间
        break;
    case SHUFFLE:
        shuffle(); //重新排列
        break;
    case HINT:
        hint(); //提示
        break;
    case DIZZY:
        // Dizzy道具只在双人模式下有效
        if (m_twoPlayer) {
        // 对对手使用Dizzy
        if (player.getId() == 1) { //玩家1使用Dizzy道具
                dizzy(m_p2); //眩晕玩家2
        } else {
                dizzy(m_p1); //眩晕玩家1
        }
        } // 单人模式下忽略Dizzy道具

        break;
    case FLASH:
        flash(player); //闪烁
        break;
    }
}

//增加时间函数
void GameWindow::addTime()
{
    m_time += 30; //增加30秒
    update();
}

//重新排列道具效果函数
//重新生成游戏地图，保持方块类型不变但位置随机化。
void GameWindow::shuffle()
{
    // 收集所有非空方块
    std::vector<int> blocks;
    blocks.reserve(ROWS * COLS / 2); // 预分配空间
    
    for (const auto& row : m_mapData) {
        std::copy_if(row.begin(), row.end(), std::back_inserter(blocks),
                    [](int blockType) { return blockType > 0; });
    }
    
    // 随机打乱
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(blocks.begin(), blocks.end(), gen);
    
    // 重新填充地图,保持方块类型不变但位置随机化。
    auto blockIt = blocks.begin();
    for (auto& row : m_mapData) {
        for (int& cell : row) {
            if (cell > 0 && blockIt != blocks.end()) {
                cell = *blockIt++;
            }
        }
    }
    
    // 重新生成道具并重置状态
    generateItems();
    m_activeRow = m_activeCol = -1; //重置激活位置
    
    // 如果高亮提示道具还在生效中，重新查找可消除的方块对
    if (m_hintOn) {
        findNextHintPair();
    }
    
    update();
}

//提示道具效果函数
void GameWindow::hint()
{
    m_hintOn = true;
    m_hintTime = 10;
    findNextHintPair();
    m_hintTimer->start(1000); //每秒更新一次
}

//眩晕道具效果函数
void GameWindow::dizzy(Player& targetPlayer)
{
    constexpr int DIZZY_DURATION = 10;  // 眩晕持续时间（秒）
    constexpr int TIMER_INTERVAL = 1000; // 定时器间隔（毫秒）
    
    targetPlayer.setDizzy(true, DIZZY_DURATION);
    
    // 根据玩家ID启动相应的定时器
    QTimer* timer = (targetPlayer.getId() == 1) ? m_dizzy1 : m_dizzy2;
    timer->start(TIMER_INTERVAL); //每秒更新一次
    
    update();
}

//闪烁道具函数。使玩家进入闪烁状态5秒，期间可以用鼠标点击移动。
void GameWindow::flash(Player& player)
{
    constexpr int FLASH_DURATION = 5;  // 闪烁持续时间（秒）
    
    player.setFlashActive(true, FLASH_DURATION); //设置闪烁状态
    
    // 清除激活状态
    m_activeRow = m_activeCol = -1;
    
    update();
}

//计算路径函数 - 计算实际的连接路径用于显示连接线
void GameWindow::calcPath(int r1, int c1, int r2, int c2)
{
    m_linePath.clear(); //清除路径
    m_lineR1 = r1;
    m_lineC1 = c1;
    m_lineR2 = r2;
    m_lineC2 = c2;
    
    // 尝试直线连接
    if (m_judger.canLineConnection(r1, c1, r2, c2, m_mapData)) {
        m_linePath.emplace_back(r1, c1);
        m_linePath.emplace_back(r2, c2);
        return;
    }
    
    // 尝试一个拐角连接
    if (tryOneCornerPath(r1, c1, r2, c2)) {
        return;
    }
    
    // 尝试两个拐角连接
    if (tryTwoCornerPath(r1, c1, r2, c2)) {
        return;
    }
    
    // 如果都失败，至少显示起点和终点
        m_linePath.emplace_back(r1, c1);
        m_linePath.emplace_back(r2, c2);
}

//尝试一个拐角连接路径
bool GameWindow::tryOneCornerPath(int r1, int c1, int r2, int c2)
{
    // 检查水平-垂直路径（一个拐角）
    if (m_judger.canLineConnection(r1, c1, r1, c2, m_mapData) && 
        m_judger.canLineConnection(r1, c2, r2, c2, m_mapData) && 
        m_mapData[r1][c2] == 0) {
        m_linePath.emplace_back(r1, c1);
        m_linePath.emplace_back(r1, c2);
        m_linePath.emplace_back(r2, c2);
        return true;
    }
    
    // 检查垂直-水平路径（一个拐角）
    if (m_judger.canLineConnection(r1, c1, r2, c1, m_mapData) && 
        m_judger.canLineConnection(r2, c1, r2, c2, m_mapData) && 
        m_mapData[r2][c1] == 0) {
        m_linePath.emplace_back(r1, c1);
        m_linePath.emplace_back(r2, c1);
        m_linePath.emplace_back(r2, c2);
        return true;
    }
    
    return false;
}

//尝试两个拐角连接路径
bool GameWindow::tryTwoCornerPath(int r1, int c1, int r2, int c2)
{
    // 情况1：水平-垂直-水平连接
    // 路径：(r1,c1) -> (r1,c) -> (r2,c) -> (r2,c2)
    for (int c = 0; c < COLS; ++c) {
        if (m_mapData[r1][c] == 0 && m_mapData[r2][c] == 0) {
            if (m_judger.canLineConnection(r1, c1, r1, c, m_mapData) &&
                m_judger.canLineConnection(r1, c, r2, c, m_mapData) &&
                m_judger.canLineConnection(r2, c, r2, c2, m_mapData)) {
                m_linePath.emplace_back(r1, c1);
                m_linePath.emplace_back(r1, c);
                m_linePath.emplace_back(r2, c);
                m_linePath.emplace_back(r2, c2);
                return true;
            }
        }
    }
    
    // 情况2：垂直-水平-垂直连接
    // 路径：(r1,c1) -> (r,c1) -> (r,c2) -> (r2,c2)
    for (int r = 0; r < ROWS; ++r) {
        if (m_mapData[r][c1] == 0 && m_mapData[r][c2] == 0) {
            if (m_judger.canLineConnection(r1, c1, r, c1, m_mapData) &&
                m_judger.canLineConnection(r, c1, r, c2, m_mapData) &&
                m_judger.canLineConnection(r, c2, r2, c2, m_mapData)) {
                m_linePath.emplace_back(r1, c1);
                m_linePath.emplace_back(r, c1);
                m_linePath.emplace_back(r, c2);
                m_linePath.emplace_back(r2, c2);
                return true;
            }
        }
    }
    
    return false;
}



//提示道具效果函数
void GameWindow::findNextHintPair()
{
    m_hintR1 = m_hintC1 = m_hintR2 = m_hintC2 = -1; //重置提示坐标
    
    // 查找可消除的方块对
    if (findSolvablePair(m_hintR1, m_hintC1, m_hintR2, m_hintC2)) {
        update(); // 更新显示
    }
}

//结束提示效果函数
void GameWindow::endHint()
{
    m_hintOn = false;
    m_hintR1 = m_hintC1 = m_hintR2 = m_hintC2 = -1;
    update();
}

//结束眩晕效果函数
void GameWindow::endDizzy(Player& player)
{
    player.setDizzy(false);
    update();
}

//定时器超时函数
void GameWindow::onTimerTimeout()
{
    if (m_running && !m_paused) { //如果游戏正在运行且没有暂停
        m_time--; //时间减一
        update();

        // 更新玩家效果
        m_p1.updateEffects();
        m_p2.updateEffects();
        
        
        if (m_time <= 0) { //如果时间结束
            end(); //结束游戏
        }
    }
}

//提示定时器超时函数
void GameWindow::onHintTimerTimeout()
{
    m_hintTime--;
    if (m_hintTime <= 0) {
        endHint();
    } else {
        update();
    }
}

//眩晕定时器超时函数
void GameWindow::onDizzyTimerTimeout(Player& player, QTimer* timer)
{
    player.updateEffects(); //更新玩家效果
    if (!player.isDizzy()) {
        timer->stop(); //停止眩晕定时器
    }
    update();
}

//道具生成定时器超时函数
void GameWindow::onItemSpawnTimerTimeout()
{
    if (!m_running || m_paused) return;
    
    std::random_device rd;
    std::mt19937 gen(rd()); //随机数生成器
    
    // 根据游戏模式选择道具类型
    int itemType;
    if (m_twoPlayer) {
        // 双人模式下不生成Flash道具，从其他道具中随机选择
        std::vector<int> availableItems = {TIME_BONUS, SHUFFLE, HINT, DIZZY};
        std::uniform_int_distribution<> itemIndexDist(0, availableItems.size() - 1);
        itemType = availableItems[itemIndexDist(gen)];

    } else {
        // 单人模式下只生成单人模式的道具，不包括Dizzy
        std::vector<int> availableItems = {TIME_BONUS, SHUFFLE, HINT, FLASH};
        std::uniform_int_distribution<> itemIndexDist(0, availableItems.size() - 1);
        itemType = availableItems[itemIndexDist(gen)];

    }
    
    // 随机选择空白位置
    std::vector<std::pair<int, int>> emptyPositions;
    
    for (int r = 0; r < ROWS; ++r) { //遍历地图
        for (int c = 0; c < COLS; ++c) {
            if (m_mapData[r][c] == 0) { //空白位置
                emptyPositions.emplace_back(r, c); //收集所有空白位置作为道具生成点
            }
        }
    }
    
    if (!emptyPositions.empty()) {
        // 随机选择一个空白位置
        std::uniform_int_distribution<> posDist(0, emptyPositions.size() - 1);
        int posIndex = posDist(gen);
        int row = emptyPositions[posIndex].first;
        int col = emptyPositions[posIndex].second;
        
        // 检查该位置是否已有道具
        bool positionOccupied = false;
        for (const auto& item : m_items) {
            if (item.row == row && item.col == col) {
                positionOccupied = true;
                break;
            }
        }
        
        // 如果位置空闲，生成道具
        if (!positionOccupied) {
            m_items.emplace_back(row, col, itemType);

            update();  // 重绘界面
        }
    }
    
    // 设置下次道具生成时间（10-20秒随机）
    m_spawnTimer->start(std::uniform_int_distribution<>(10000, 20000)(gen));
}

//检查玩家是否可以移动函数
bool GameWindow::canMove(int row, int col)
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return false; //超出地图范围
    }
    
    // 空白区域（值为0）可以移动
    if (m_mapData[row][col] == 0) {
        return true;
    }
    
    // 有方块的位置不能移动
    return false;
}

//移动玩家函数
void GameWindow::move(Player& player, int deltaRow, int deltaCol)
{
    
    // 处理眩晕效果
    if (player.isDizzy()) {
        deltaRow = -deltaRow;
        deltaCol = -deltaCol;
    }
    
    int newRow = player.getRow() + deltaRow;
    int newCol = player.getCol() + deltaCol;

    if (canMove(newRow, newCol)) { //可以移动
        player.setPosition(newRow, newCol);

        checkItemCollision(player); //检查道具碰撞
        update();
    } else {
        tryActivateBlock(player, deltaRow, deltaCol); //尝试激活方块
    }
}

//尝试激活方块函数
void GameWindow::tryActivateBlock(Player& player, int deltaRow, int deltaCol)
{
    int targetRow = player.getRow() + deltaRow;
    int targetCol = player.getCol() + deltaCol;

    if (targetRow >= 0 && targetRow < ROWS && //超出地图范围
        targetCol >= 0 && targetCol < COLS &&
        m_mapData[targetRow][targetCol] > 0) { //有方块

        // 设置当前激活的玩家
        m_active = &player;

        // 如果已经激活了方块
        if (m_activeRow != -1 && m_activeCol != -1) {
            // 如果点击的是相同类型的方块，尝试消除
            if (m_mapData[m_activeRow][m_activeCol] == m_mapData[targetRow][targetCol] &&
                (m_activeRow != targetRow || m_activeCol != targetCol)) { //相同类型的方块
                
                // 尝试消除
                processElimination(targetRow, targetCol); //处理两个方块的消除逻辑，包括路径计算、分数计算和界面更新。
            } else {
                // 不同类型或相同位置，取消之前的激活，激活新的
                m_activeRow = targetRow;
                m_activeCol = targetCol;
            }
        } else {
            // 第一次激活
            m_activeRow = targetRow;
            m_activeCol = targetCol;
        }
        update();
    }
}

//处理两个方块的消除逻辑函数
void GameWindow::processElimination(int targetRow, int targetCol)
{
    if (m_activeRow == -1 || m_activeCol == -1) return; //没有激活方块，返回

    int r1 = m_activeRow, c1 = m_activeCol;
    int r2 = targetRow, c2 = targetCol;

    //判断两个方块是否可以消除
    if (m_judger.canEliminate(r1, c1, r2, c2, m_mapData)) {

        int blockType = m_mapData[r1][c1]; //方块类型
        int points = blockType * 10; //分数
        
        // 给当前玩家加分
        m_active->addScore(points);

        // 计算连接路径并显示连接线
        calcPath(r1, c1, r2, c2); //计算连接路径
        m_showLine = true; //显示连接线
        
        // 播放消除音效
        playEliminationSound();
        
        // 触发僵尸攻击动画
        triggerZombieAttackAnimation();
        
        // 延迟消除，让玩家看到连接线
        QTimer::singleShot(500, this, [this, r1, c1, r2, c2]() { //延迟500毫秒后消除方块
            m_mapData[r1][c1] = 0;
            m_mapData[r2][c2] = 0; //消除方块
            m_activeRow = m_activeCol = -1; //重置激活位置
            m_showLine = false;
            m_linePath.clear(); //清除连接路径
            
            // 检查是否所有方块都被消除了（游戏胜利）
            bool allBlocksEliminated = true;
            for (int r = 0; r < ROWS; ++r) {
                for (int c = 0; c < COLS; ++c) {
                    if (m_mapData[r][c] > 0) {
                        allBlocksEliminated = false;
                        break;
                    }
                }
                if (!allBlocksEliminated) break;
            }
            
            if (allBlocksEliminated) {
                // 所有方块都被消除，游戏胜利
                end();
                return;
            }
            
            if (!m_judger.hasSolutions(m_mapData)) { //判断地图是否有解
                // 显示无解信息并重排地图
                showNoSolutionMessage();
                shuffleMap();
            } else {
                if (m_hintOn) { //提示道具还在生效中
                    findNextHintPair(); //查找下一个提示方块对
                }
            }
            update();
        });
        
        update();
    } else {

        m_activeRow = m_activeCol = -1;
        update();
    }
}

//检查方块是否可达函数
bool GameWindow::isBlockReachable(int row, int col)
{
    // 检查方块周围是否有空地，九宫格里有空地即可到达
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue; //跳过方块本身
            
            int newRow = row + dr;
            int newCol = col + dc;
            
            if (canMove(newRow, newCol)) {
                return true; //找到可到达的空地
            }
        }
    }
    return false;
}

//初始化玩家位置函数
void GameWindow::initializePlayerPosition(Player& player, std::mt19937& gen,
                                         std::uniform_int_distribution<>& rowDist,
                                         std::uniform_int_distribution<>& colDist)
{
    int row, col;
    
    while(true){
        // 随机选择外围空白位置
        auto position = getRandomBorderPosition(gen); // 获取随机边界位置
        row = position.first; // 行坐标
        col = position.second; // 列坐标
        
        // 确保选择的位置是空白区域
        if (m_mapData[row][col] != 0) {
            continue;
        }
            break;
        }
    
    player.setPosition(row, col);
}

//获取随机边界位置函数
std::pair<int, int> GameWindow::getRandomBorderPosition(std::mt19937& gen)
{
        int side = std::uniform_int_distribution<>(0, 3)(gen); //随机选择边界位置
    int row, col;
    
        switch (side) {
    case 0: // 上边
        row = std::uniform_int_distribution<>(0, 1)(gen);
        col = std::uniform_int_distribution<>(0, COLS - 1)(gen);
            break;
    case 1: // 下边
        row = std::uniform_int_distribution<>(ROWS - 2, ROWS - 1)(gen);
        col = std::uniform_int_distribution<>(0, COLS - 1)(gen);
            break;
    case 2: // 左边
        row = std::uniform_int_distribution<>(0, ROWS - 1)(gen);
        col = std::uniform_int_distribution<>(0, 1)(gen);
            break;
    case 3: // 右边
        row = std::uniform_int_distribution<>(0, ROWS - 1)(gen);
        col = std::uniform_int_distribution<>(COLS - 2, COLS - 1)(gen);
            break;
        }
        
    return std::make_pair(row, col);
}

//检查玩家位置周围是否有可消除的方块对
bool GameWindow::findSolvablePair(int& hintR1, int& hintC1, int& hintR2, int& hintC2)
{
    // 遍历所有方块对，找到可消除且可到达的
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int c1 = 0; c1 < COLS; ++c1) {
            if (m_mapData[r1][c1] == 0) continue;
            
            // 检查第一个方块是否可到达
            if (!isBlockReachable(r1, c1)) continue;
            
            for (int r2 = r1; r2 < ROWS; ++r2) {
                int cStart = (r2 == r1) ? c1 + 1 : 0;
                for (int c2 = cStart; c2 < COLS; ++c2) {
                    if (m_mapData[r2][c2] == m_mapData[r1][c1] && 
                        m_judger.canEliminate(r1, c1, r2, c2, m_mapData)) {
                        
                        // 检查第二个方块是否可到达
                        if (isBlockReachable(r2, c2)) {
                            hintR1 = r1; hintC1 = c1;
                            hintR2 = r2; hintC2 = c2;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

//显示无解信息对话框
void GameWindow::showNoSolutionMessage()
{
    // 在屏幕中央显示无解信息
    QMessageBox::information(static_cast<QWidget*>(this), "无解", "当前地图无解，正在重新排列...");
}

//重新排列地图。保持方块类型不变，随机重新排列所有方块的位置。
void GameWindow::shuffleMap()
{
    // 重新生成地图
    generateMap();
    
    // 重新初始化玩家位置
    reinitializePlayerPositions();
    
    // 重新生成道具
    generateItems();
    
    // 如果高亮提示道具还在生效中，重新查找可消除的方块对
    if (m_hintOn) {
        findNextHintPair();
    }
    
    update();
}

//重新初始化玩家位置函数



std::pair<int, int> GameWindow::findValidPlayerPosition(std::mt19937& gen,
                                                       std::uniform_int_distribution<>& rowDist,
                                                       std::uniform_int_distribution<>& colDist)
{
    int row, col;
    
    // 随机选择一个空地位置
    do {
        row = rowDist(gen);
        col = colDist(gen);
    } while (m_mapData[row][col] != 0);
    
    return std::make_pair(row, col);
}

//按键事件处理函数
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    // 根据当前状态处理按键事件
    if (m_state == MENU_STATE || m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
        handleKey(event); //处理菜单按键事件
        return;
    }
    
    if (!m_running) {
        QWidget::keyPressEvent(event);
        return;
    }
    
    switch (event->key()) {
    // 玩家控制
    case Qt::Key_W:
        if (!m_paused) move(m_p1, -1, 0);
        break;
    case Qt::Key_S:
        if (!m_paused) move(m_p1, 1, 0);
        break;
    case Qt::Key_A:
        if (!m_paused) move(m_p1, 0, -1);
        break;
    case Qt::Key_D:
        if (!m_paused) move(m_p1, 0, 1);
        break;
    
    // 方向键控制
    case Qt::Key_Up:
        handleUpKey();
        break;
    case Qt::Key_Down:
        handleDownKey();
        break;
    case Qt::Key_Left:
        handleLeftKey();
        break;
    case Qt::Key_Right:
        handleRightKey();
        break;
    
    // 游戏控制
    case Qt::Key_R:  // 重新开始游戏
        start();
        break;
    case Qt::Key_Space:  // 空格键暂停/恢复
        handleSpaceKey();
        break;
    case Qt::Key_T:  // T键切换双人模式
        m_twoPlayer = !m_twoPlayer;
        start();
        break;
    case Qt::Key_Escape:  // ESC键返回菜单
        end();
        showMenu();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

//向上移动玩家
void GameWindow::handleUpKey()
{
    if (!m_paused) {
        if (m_twoPlayer) {
            move(m_p2, -1, 0);
        } else {
            move(m_p1, -1, 0);
        }
    }
}

//向下移动玩家
void GameWindow::handleDownKey()
{
    if (!m_paused) {
        if (m_twoPlayer) {
            move(m_p2, 1, 0);
        } else {
            move(m_p1, 1, 0);
        }
    }
}

//向左移动玩家
void GameWindow::handleLeftKey()
{
    if (!m_paused) {
        if (m_twoPlayer) {
            move(m_p2, 0, -1);
        } else {
            move(m_p1, 0, -1);
        }
    }
}

//向右移动玩家
void GameWindow::handleRightKey()
{
    if (!m_paused) {
        if (m_twoPlayer) {
            move(m_p2, 0, 1);
        } else {
            move(m_p1, 0, 1);
        }
    }
}

//空格键暂停/恢复
void GameWindow::handleSpaceKey()
{
    if (m_paused) {
        resume();
    } else {
        pause();
    }
}

//鼠标按下事件处理函数
void GameWindow::mousePressEvent(QMouseEvent *event)
{
    // 根据当前状态处理鼠标事件
    if (m_state == MENU_STATE || m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
        handleMenuMouseEvent(event, true); // true表示是点击事件
        return;
    }
    
    // 游戏状态下的鼠标事件处理（Flash道具等）
    if (!m_running || m_paused) {
        QWidget::mousePressEvent(event); //如果游戏还没开始或者暂停了，就把鼠标事件交给父类处理，然后结束。
        return;
    }
    
    // Flash道具的鼠标点击处理
    if (m_active->isFlashActive() && event->button() == Qt::LeftButton) {
        handleFlashMouseClick(event);
    }
    
    QWidget::mousePressEvent(event); //把鼠标事件交给父类处理，然后结束。
}

//Flash道具的鼠标点击处理
void GameWindow::handleFlashMouseClick(QMouseEvent *event)
{

    auto clickInfo = getClickPosition(event); //获取鼠标点击位置
    if (!clickInfo.isValid) return; //如果鼠标点击位置无效，则结束。
    
    if (m_mapData[clickInfo.row][clickInfo.col] > 0) {
        handleBlockClick(clickInfo); //如果点击的是方块，则处理方块点击事件
    } else if (m_mapData[clickInfo.row][clickInfo.col] == 0) {
        handleEmptySpaceClick(clickInfo); //如果点击的是空地，则处理空地点击事件
    }
}

//获取鼠标点击位置
ClickInfo GameWindow::getClickPosition(QMouseEvent *event)
{
        int widgetWidth = width();
    int widgetHeight = height(); //获取窗口宽度高度
    
    // 计算游戏区域（排除菜单栏）
    int menuHeight = m_menuWidget ? m_menuWidget->height() : 0;
    int gameHeight = widgetHeight - menuHeight;
    
    float cellWidth = static_cast<float>(widgetWidth) / COLS;
    float cellHeight = static_cast<float>(gameHeight) / ROWS; //获取每个方块的宽度高度
    
    // 调整鼠标位置（减去菜单栏高度）
    QPoint adjustedPos = event->pos();
    adjustedPos.setY(adjustedPos.y() - menuHeight);
        
    int clickedRow = static_cast<int>(adjustedPos.y() / cellHeight); //获取鼠标点击位置的行
    int clickedCol = static_cast<int>(adjustedPos.x() / cellWidth); //获取鼠标点击位置的列
        
    //如果鼠标点击位置在游戏区域内，则返回鼠标点击位置
    if (clickedRow >= 0 && clickedRow < ROWS && 
        clickedCol >= 0 && clickedCol < COLS) {
        return ClickInfo(clickedRow, clickedCol, true); //返回鼠标点击位置
    }
    
    return ClickInfo(0, 0, false); //返回鼠标点击位置无效
}

//处理方块点击事件
void GameWindow::handleBlockClick(const ClickInfo& clickInfo)
{
    // 首先尝试将玩家移动到方块旁边
    if (!movePlayerToBlockSide(clickInfo.row, clickInfo.col)) {
        return; //如果无法移动到方块旁边，则结束
    }
    
    // 处理方块激活逻辑
    handleBlockActivation(clickInfo.row, clickInfo.col); //处理方块激活逻辑
}

//将玩家移动到方块旁边
bool GameWindow::movePlayerToBlockSide(int blockRow, int blockCol)
{
    for (int dr = -1; dr <= 1; ++dr) { //遍历方块周围的8个方向
                    for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue; // 跳过方块本身
                        
            int sideRow = blockRow + dr; //计算方块周围的8个方向的行
            int sideCol = blockCol + dc; //计算方块周围的8个方向的列
                        
            if (sideRow >= 0 && sideRow < ROWS && 
                sideCol >= 0 && sideCol < COLS && 
                m_mapData[sideRow][sideCol] == 0) { //如果方块周围的8个方向是空地，则移动玩家到方块旁边
                
                m_active->setPosition(sideRow, sideCol);
                return true;
            }
        }
    }
    return false;
}

//处理方块激活逻辑
void GameWindow::handleBlockActivation(int clickedRow, int clickedCol)
{
    // 如果已经激活了方块，尝试消除
    if (m_activeRow != -1 && m_activeCol != -1) {
        if (m_mapData[m_activeRow][m_activeCol] == m_mapData[clickedRow][clickedCol] &&
            (m_activeRow != clickedRow || m_activeCol != clickedCol)) {

            // 尝试消除
            processElimination(clickedRow, clickedCol); //处理两个方块的消除逻辑，包括路径计算、分数计算和界面更新。
                    return;
        } else {
            
        // 不同类型，取消之前的激活，激活新的
        m_activeRow = clickedRow;
        m_activeCol = clickedCol;
        update();
        return; //如果不同类型，则结束
        }
    }
                
    // 激活新方块
    m_activeRow = clickedRow;
    m_activeCol = clickedCol;
    update();
}

//处理空地点击事件
void GameWindow::handleEmptySpaceClick(const ClickInfo& clickInfo)
{

    // 点击空地，直接移动当前玩家（Flash模式下可以无视路径）
    m_active->setPosition(clickInfo.row, clickInfo.col);
    // 清除激活状态，因为移动了位置
    m_activeRow = m_activeCol = -1;
    update();
}

//鼠标移动事件处理函数
void GameWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 根据当前状态处理鼠标移动事件
    if (m_state == MENU_STATE || m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
        handleMenuMouseEvent(event, false); // false表示是移动事件
        return;
    }
    
    QWidget::mouseMoveEvent(event);
}

//重绘事件处理函数
void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(static_cast<QWidget*>(this)); //创建绘制器
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿

    int widgetWidth = width(); //获取窗口宽度
    int widgetHeight = height(); //获取窗口高度
    if (widgetWidth <= 0 || widgetHeight <= 0) return;

    // 计算游戏区域（排除菜单栏）
    int menuHeight = m_menuWidget ? m_menuWidget->height() : 0;
    QRect gameRect(0, menuHeight, widgetWidth, widgetHeight - menuHeight);

    // 根据当前状态绘制不同的内容
    if (m_state == MENU_STATE) {
        // 绘制菜单背景
        painter.fillRect(gameRect, QColorConstants::Svg::darkblue);
        
        // 调整绘制区域为游戏区域
        painter.translate(0, menuHeight);
        drawMenu(painter);
        return; //如果当前状态是菜单状态，则绘制菜单
    }

    // 检查是否显示存档槽位选择界面
    if (m_showSaveSlots || m_showLoadSlots || m_showDeleteSlots) {
        // 绘制存档槽位选择背景
        painter.fillRect(gameRect, QColorConstants::Svg::darkblue);
        
        // 调整绘制区域为游戏区域
        painter.translate(0, menuHeight);
        drawMenu(painter);
        return;
    }

    // 绘制游戏状态
    drawGameState(painter, widgetWidth, widgetHeight); 
}

//绘制游戏状态
void GameWindow::drawGameState(QPainter& painter, int widgetWidth, int widgetHeight)
{
    // 计算游戏区域（排除菜单栏）
    int menuHeight = m_menuWidget ? m_menuWidget->height() : 0;
    int gameHeight = widgetHeight - menuHeight;
    
    float cellWidth = static_cast<float>(widgetWidth) / COLS; 
    float cellHeight = static_cast<float>(gameHeight) / ROWS;

    // 调整绘制位置到游戏区域
    painter.translate(0, menuHeight);

    // 绘制各个游戏元素
    drawMapBlocks(painter, cellWidth, cellHeight); //绘制地图方块
    drawItems(painter, cellWidth, cellHeight); //绘制道具
    drawHintHighlights(painter, cellWidth, cellHeight); //绘制Hint高亮
    drawConnectionLines(painter, cellWidth, cellHeight); //绘制连接线
    drawPlayers(painter, cellWidth, cellHeight); //绘制玩家
    drawGameInfo(painter, widgetWidth, gameHeight); //绘制游戏信息
    drawGameStatus(painter, widgetWidth, gameHeight); //绘制游戏状态
}

//绘制地图方块
void GameWindow::drawMapBlocks(QPainter& painter, float cellWidth, float cellHeight)
{
    // 绘制地图方块
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            int blockType = m_mapData[r][c];
            if (blockType == 0) continue;



            QRectF blockRect(c * cellWidth, r * cellHeight, cellWidth, cellHeight); //创建方块矩形
            blockRect.adjust(2, 2, -2, -2); //调整方块矩形大小，让方块之间有间隙，显得美观
            
            //绘制方块 - 根据状态画不同的样式：
            //激活状态：红色边框（在drawBlockImage中绘制）
            //普通状态：黑色边框，透明背景
            if (r == m_activeRow && c == m_activeCol) {
                // 绘制激活状态的图片（包含红色边框）
                drawBlockImage(painter, blockRect, blockType, true);
            } else {
                // 绘制普通状态的图片（包含边框）
                drawBlockImage(painter, blockRect, blockType, false);
            }
        }
    }
}

//绘制道具函数
void GameWindow::drawItems(QPainter& painter, float cellWidth, float cellHeight)
{
    // 绘制道具 - 根据道具类型画不同的颜色和图标
    for (const auto& item : m_items) {
        QRectF itemRect(item.col * cellWidth, item.row * cellHeight,
                       cellWidth, cellHeight);
        itemRect.adjust(3, 3, -3, -3);
        
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
        
        // 绘制道具图标
        painter.setPen(QColorConstants::Svg::black); //设置画笔颜色为黑色
        painter.setFont(QFont("Arial", 8, QFont::Bold)); //设置字体
        QString itemText;
        switch (item.type) {
        case TIME_BONUS: itemText = "T"; break;
        case SHUFFLE: itemText = "S"; break;
        case HINT: itemText = "H"; break;
        case DIZZY: itemText = "D"; break;
        case FLASH: itemText = "F"; break;
        }
        painter.drawText(itemRect, Qt::AlignCenter, itemText); //绘制道具图标，文字为T、S、H、D、F
    }
    }

//绘制Hint高亮
void GameWindow::drawHintHighlights(QPainter& painter, float cellWidth, float cellHeight)
{
    // 绘制Hint高亮
    if (m_hintOn && m_hintR1 != -1) { //如果Hint效果激活且Hint高亮的第一个方块不为-1，则绘制Hint高亮
        QRectF hintRect1(m_hintC1 * cellWidth, m_hintR1 * cellHeight,
                        cellWidth, cellHeight);
        hintRect1.adjust(1, 1, -1, -1); //调整Hint高亮矩形大小
        painter.setBrush(Qt::transparent); //设置Hint高亮矩形背景为透明
        painter.setPen(QPen(QColorConstants::Svg::yellow, 4)); //设置Hint高亮矩形边框为黄色
        painter.drawRect(hintRect1); //绘制Hint高亮矩形
        
        QRectF hintRect2(m_hintC2 * cellWidth, m_hintR2 * cellHeight,
                        cellWidth, cellHeight);
        hintRect2.adjust(1, 1, -1, -1); //调整Hint高亮矩形大小
        painter.drawRect(hintRect2);
    }
    }
    
//绘制连接线
void GameWindow::drawConnectionLines(QPainter& painter, float cellWidth, float cellHeight)
{
    if (m_showLine && !m_linePath.empty()) { //如果显示连接线且连接路径不为空，则绘制连接线
        painter.setPen(QPen(QColorConstants::Svg::orange, 3));
        painter.setBrush(Qt::NoBrush);
        
        //遍历连接路径，绘制连接线
        for (size_t i = 0; i < m_linePath.size() - 1; ++i) {
            int r1 = m_linePath[i].first; //获取连接线起点行
            int c1 = m_linePath[i].second; //获取连接线起点列   
            int r2 = m_linePath[i + 1].first; //获取连接线终点行
            int c2 = m_linePath[i + 1].second; //获取连接线终点列
            
            QPointF start(c1 * cellWidth + cellWidth / 2, r1 * cellHeight + cellHeight / 2); //计算连接线起点
            QPointF end(c2 * cellWidth + cellWidth / 2, r2 * cellHeight + cellHeight / 2); //计算连接线终点
            
            painter.drawLine(start, end); //绘制连接线
        }
        }
    }

//绘制玩家
void GameWindow::drawPlayers(QPainter& painter, float cellWidth, float cellHeight)
{
    // 绘制玩家1
    drawPlayer(painter, m_p1, 1, cellWidth, cellHeight);
    
    // 绘制玩家2（双人模式）
    if (m_twoPlayer) {
        drawPlayer(painter, m_p2, 2, cellWidth, cellHeight);
    }
}

//绘制玩家
void GameWindow::drawPlayer(QPainter& painter, const Player& player, int playerId, float cellWidth, float cellHeight)
{
    int playerRow = player.getRow();
    int playerCol = player.getCol();
        
        // 确保位置在有效范围内
    if (playerRow >= 0 && playerRow < ROWS && playerCol >= 0 && playerCol < COLS) {
        QRectF playerRect(playerCol * cellWidth, playerRow * cellHeight, //计算玩家矩形
                              cellWidth, cellHeight);
        
        // 绘制僵尸GIF动图
        if ((m_zombieMovie && m_zombieMovie->isValid() && !m_currentZombieFrame.isNull()) ||
            (m_zombieEatMovie && m_zombieEatMovie->isValid() && !m_currentZombieEatFrame.isNull())) {
            
            // 增大僵尸尺寸，减少边距让僵尸更大
            QRectF zombieRect = playerRect.adjusted(2, 2, -2, -2); // 减少边距，增大僵尸
            
            QPixmap currentFrame;
            if (m_isZombieAttacking && !m_currentZombieEatFrame.isNull()) {
                // 攻击状态：使用攻击动画帧
                currentFrame = m_currentZombieEatFrame;
            } else if (!m_currentZombieFrame.isNull()) {
                // 普通状态：使用普通动画帧
                currentFrame = m_currentZombieFrame;
            }
            
            if (!currentFrame.isNull()) {
                // 缩放GIF动图以适应玩家矩形
                QPixmap scaledZombie = currentFrame.scaled(
                    zombieRect.size().toSize(), 
                    Qt::KeepAspectRatio, 
                    Qt::SmoothTransformation
                );
                
                // 计算居中位置
                QPointF drawPos = zombieRect.topLeft();
                drawPos.setX(drawPos.x() + (zombieRect.width() - scaledZombie.width()) / 2);
                drawPos.setY(drawPos.y() + (zombieRect.height() - scaledZombie.height()) / 2);
                
                // 绘制僵尸GIF动图（无边框，直接融入地图）
                painter.drawPixmap(drawPos, scaledZombie);
            }
            
            // 玩家状态指示（只在眩晕时显示，且使用透明边框）
            if (player.isDizzy()) {
                painter.setPen(QPen(QColor(255, 165, 0, 150), 2)); // 半透明橙色边框
                painter.setBrush(Qt::NoBrush); // 无填充
                painter.drawEllipse(zombieRect); // 绘制眩晕效果边框
            }
        } else {
            // 如果GIF加载失败，绘制原来的圆形玩家
            playerRect.adjust(5, 5, -5, -5); //调整玩家矩形大小
            
            // 绘制玩家阴影效果
            QRectF shadowRect = playerRect.adjusted(2, 2, 2, 2);
            painter.setBrush(QColor(0, 0, 0, 100)); //设置玩家阴影效果颜色
            painter.setPen(Qt::NoPen); //设置玩家阴影效果边框为透明
            painter.drawEllipse(shadowRect); //绘制玩家阴影效果
            
            // 绘制玩家主体
            painter.setBrush(getPlayerColor(playerId)); //设置玩家主体颜色
            painter.setPen(QPen(QColorConstants::Svg::black, 2)); //设置玩家主体边框为黑色
            painter.drawEllipse(playerRect); //绘制玩家主体
            
            // 玩家状态指示
            if (player.isDizzy()) {
                painter.setPen(QPen(QColorConstants::Svg::orange, 3)); //设置玩家眩晕效果边框为橙色
                painter.drawEllipse(playerRect); //绘制玩家眩晕效果
            }
        }
    
        // 添加玩家标签
        if (m_zombieMovie && m_zombieMovie->isValid() && !m_currentZombieFrame.isNull()) {
            // 僵尸模式下，标签显示在僵尸下方
            QRectF zombieRect = playerRect.adjusted(2, 2, -2, -2);
            QPointF labelPos = zombieRect.bottomLeft();
            labelPos.setY(labelPos.y() + 15); // 在僵尸下方显示标签
            
            painter.setPen(QColorConstants::Svg::white);
            painter.setFont(QFont("Arial", 8, QFont::Bold));
            if (playerId == 1) {
                painter.drawText(labelPos, m_twoPlayer ? "P1" : "玩家");
            } else {
                painter.drawText(labelPos, "P2");
            }
        } else {
            // 圆形玩家模式下，标签显示在中心
            painter.setPen(QColorConstants::Svg::white);
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            if (playerId == 1) {
                painter.drawText(playerRect.center(), m_twoPlayer ? "P1" : "玩家");
            } else {
                painter.drawText(playerRect.center(), "P2");
            }
        }
    }
}

//绘制游戏信息
void GameWindow::drawGameInfo(QPainter& painter, int widgetWidth, int widgetHeight)
{
    // 绘制时间和分数信息
    painter.setPen(QColorConstants::Svg::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    
    // 显示剩余时间
    int minutes = m_time / 60;
    int seconds = m_time % 60;
    QString timeText = QString("时间: %1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    painter.drawText(10, 25, timeText);
    
    // 显示分数
    if (m_twoPlayer) {
        painter.drawText(10, 45, QString("玩家1分数: %1").arg(m_p1.getScore()));
        painter.drawText(10, 65, QString("玩家2分数: %1").arg(m_p2.getScore()));
    } else {
        painter.drawText(10, 45, QString("分数: %1").arg(m_p1.getScore()));
    }
    
    // 显示道具状态
    if (m_hintOn) {
        painter.drawText(10, 85, QString("Hint效果激活中 (%1秒)").arg(m_hintTime));
    }
    
    // 显示玩家状态
    drawPlayerStatus(painter);
}

//绘制玩家状态
void GameWindow::drawPlayerStatus(QPainter& painter)
{
    if (m_twoPlayer) {
        if (m_p1.isDizzy()) {
            painter.drawText(10, 105, QString("玩家1眩晕 (%1秒)").arg(m_p1.getDizzyTime()));
        }
        if (m_p2.isDizzy()) {
            painter.drawText(10, 125, QString("玩家2眩晕 (%1秒)").arg(m_p2.getDizzyTime()));
        }
    }
}

//绘制游戏状态
void GameWindow::drawGameStatus(QPainter& painter, int widgetWidth, int widgetHeight)
{
    // 如果游戏结束，显示游戏结束信息
    if (!m_running) {
        painter.setFont(QFont("Arial", 16, QFont::Bold)); //设置游戏结束文字字体
        painter.setPen(QPen(QColorConstants::Svg::red, 3)); //设置游戏结束文字颜色为红色
        QString gameOverText = "游戏结束";
        QRect textRect = painter.fontMetrics().boundingRect(gameOverText);
        int x = (widgetWidth - textRect.width()) / 2;
        int y = widgetHeight / 2;
        painter.drawText(x, y, gameOverText);
    }
    
    // 如果游戏暂停，显示暂停信息
    if (m_paused && m_running) {
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.setPen(QPen(QColorConstants::Svg::yellow, 3));
        QString pauseText = "游戏暂停";
        QRect textRect = painter.fontMetrics().boundingRect(pauseText);
        int x = (widgetWidth - textRect.width()) / 2;
        int y = widgetHeight / 2 + 30;
        painter.drawText(x, y, pauseText);
        
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.setPen(QPen(QColorConstants::Svg::white, 2));
        QString hintText = "按空格键继续游戏";
        QRect hintRect = painter.fontMetrics().boundingRect(hintText);
        int hintX = (widgetWidth - hintRect.width()) / 2;
        int hintY = y + 30;
        painter.drawText(hintX, hintY, hintText);
    }
}


//根据玩家ID获取颜色
QColor GameWindow::getPlayerColor(int playerId)
{
    switch (playerId) {
    case 1: return QColor(0xff0000); // 红色，更明显
    case 2: return QColor(0xff00ff); // 紫色
    default: return QColorConstants::Svg::white;
    }
} 


void GameWindow::saveGame(const QString& filename)
{
    if (!m_running) return;
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(static_cast<QWidget*>(this), "保存失败", "无法创建存档文件！");
        return;
    }
    
    QTextStream out(&file);
    
    // 保存各个部分的数据
    saveGameHeader(out);      // 保存游戏基本信息
    saveGamePlayers(out);     // 保存玩家信息
    saveGameMap(out);         // 保存地图状态
    saveGameItems(out);       // 保存道具信息
    saveGameEffects(out);     // 保存游戏效果
    
    file.close();
}

//保存游戏基本信息
void GameWindow::saveGameHeader(QTextStream& out)
{
    out << "QLink Save File\n";
    out << "Version: 1.0\n";
    out << "TwoPlayerMode: " << (m_twoPlayer ? "1" : "0") << "\n\n";
}
    
    // 保存玩家信息
void GameWindow::saveGamePlayers(QTextStream& out)
{
    out << "Player1: " << m_p1.getRow() << " " << m_p1.getCol() << " " << m_p1.getScore() << "\n";
    out << "Player2: " << m_p2.getRow() << " " << m_p2.getCol() << " " << m_p2.getScore() << "\n";
    out << "Time: " << m_time << "\n";
    out << "GameRunning: " << (m_running ? "1" : "0") << "\n";
    out << "Paused: " << (m_paused ? "1" : "0") << "\n\n";
    out << "Activated: " << m_activeRow << " " << m_activeCol << "\n\n";
}

// 保存地图状态
void GameWindow::saveGameMap(QTextStream& out)
{
    out << "MapData:\n";
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            out << m_mapData[r][c];
            if (c < COLS - 1) out << " ";
        }
        out << "\n";
    }
    out << "\n";
}
    
    // 保存道具信息
void GameWindow::saveGameItems(QTextStream& out)
{
    out << "Items: " << m_items.size() << "\n";
    for (const auto& item : m_items) {
        out << item.row << " " << item.col << " " << item.type << "\n";
    }
    out << "\n";
}

// 保存游戏效果
void GameWindow::saveGameEffects(QTextStream& out)
{
    out << "HintActive: " << (m_hintOn ? "1" : "0") << "\n";
    out << "HintTime: " << m_hintTime << "\n";
    out << "HintBlocks: " << m_hintR1 << " " << m_hintC1 << " " << m_hintR2 << " " << m_hintC2 << "\n";
    out << "Player1Dizzy: " << (m_p1.isDizzy() ? "1" : "0") << " " << m_p1.getDizzyTime() << "\n";
    out << "Player2Dizzy: " << (m_p2.isDizzy() ? "1" : "0") << " " << m_p2.getDizzyTime() << "\n";
}

// 加载游戏
void GameWindow::loadGame(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(static_cast<QWidget*>(this), "加载失败", "无法打开存档文件！");
        return;
    }
    
    QTextStream in(&file); // 创建输入流
    
    // 初始化地图数据
    initializeMapData();
    
    // 验证文件头
    if (!loadGameHeader(in)) { 
        file.close();
        return;
    }
    
    loadGamePlayers(in); // 加载玩家信息
    loadGameMap(in); // 加载地图状态
    loadGameItems(in); // 加载道具信息
    loadGameEffects(in); // 加载游戏效果
    
    file.close();
    
    // 设置游戏状态
    m_state = GAME_STATE;
    m_running = true;
    m_paused = false;
    
    // 停止所有定时器
    stopAllTimers();
    
    // 停止背景音乐
    stopBackgroundMusic();
    
    // 注意：不重置游戏效果，因为loadGameEffects已经载入了存档中的效果数据
    
    // 更新窗口标题
    updateWindowTitle("游戏进行中");
    
    // 启动游戏定时器
    m_timer->start(1000); // 每秒更新一次
    
    // 启动道具生成定时器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(10000, 20000);
    m_spawnTimer->start(dist(gen));
    
    // 启动提示定时器（如果需要）
    if (m_hintOn && m_hintTime > 0) {
        m_hintTimer->start(1000);
    }
    
    // 启动眩晕定时器（如果需要）
    if (m_p1.isDizzy() && m_p1.getDizzyTime() > 0) {
        m_dizzy1->start(1000);
    }
    if (m_p2.isDizzy() && m_p2.getDizzyTime() > 0) {
        m_dizzy2->start(1000);
    }
    
    // 播放背景音乐
    playBackgroundMusic();
    
    // 刷新界面
    update();
}

bool GameWindow::loadGameHeader(QTextStream& in)
{
    QString header = in.readLine();
    if (header != "QLink Save File") {
        QMessageBox::warning(static_cast<QWidget*>(this), "加载失败", "不是有效的存档文件！");
        return false;
    }
    
    in.readLine(); // 版本号
    QString twoPlayerLine = in.readLine();
    QStringList twoPlayerParts = twoPlayerLine.split(" ");
    if (twoPlayerParts.size() >= 2) {
        m_twoPlayer = (twoPlayerParts[1] == "1");
    }
    
    in.readLine(); // 空行
    return true;
}
    
void GameWindow::loadGamePlayers(QTextStream& in)
{
    // 读取玩家1信息
    QString player1Line = in.readLine();
    QStringList player1Parts = player1Line.split(" ");
    if (player1Parts.size() >= 4) {
        m_p1.setPosition(player1Parts[1].toInt(), player1Parts[2].toInt());
        m_p1.addScore(-m_p1.getScore() + player1Parts[3].toInt());
    }
    
    // 读取玩家2信息
    QString player2Line = in.readLine();
    QStringList player2Parts = player2Line.split(" ");
    if (player2Parts.size() >= 4) {
        m_p2.setPosition(player2Parts[1].toInt(), player2Parts[2].toInt());
        m_p2.addScore(-m_p2.getScore() + player2Parts[3].toInt());
    }
    
    // 读取游戏状态
    QString timeLine = in.readLine();
    QStringList timeParts = timeLine.split(" ");
    if (timeParts.size() >= 2) {
        m_time = timeParts[1].toInt();
    }
    
    QString gameRunningLine = in.readLine();
    QStringList gameRunningParts = gameRunningLine.split(" ");
    if (gameRunningParts.size() >= 2) {
        m_running = (gameRunningParts[1] == "1");
    }
    
    QString pausedLine = in.readLine();
    QStringList pausedParts = pausedLine.split(" ");
    if (pausedParts.size() >= 2) {
        m_paused = (pausedParts[1] == "1");
    }
    
    in.readLine(); 
    
    // 读取激活的方块
    QString activatedLine = in.readLine();
    QStringList activatedParts = activatedLine.split(" ");
    if (activatedParts.size() >= 3) {
        m_activeRow = activatedParts[1].toInt();
        m_activeCol = activatedParts[2].toInt();
    }
    
    in.readLine(); 
}
    
void GameWindow::loadGameMap(QTextStream& in)
{
    QString mapHeader = in.readLine();
    if (mapHeader == "MapData:") {
        for (int r = 0; r < ROWS; ++r) {
            QString line = in.readLine();
            QStringList values = line.split(" ");
            for (int c = 0; c < COLS && c < values.size(); ++c) {
                m_mapData[r][c] = values[c].toInt();
            }
        }
    }
    in.readLine();
    }
    
void GameWindow::loadGameItems(QTextStream& in)
{
    QString itemsLine = in.readLine();
    QStringList itemsParts = itemsLine.split(" ");
    if (itemsParts.size() >= 2) {
        int itemCount = itemsParts[1].toInt();
        m_items.clear();
        for (int i = 0; i < itemCount; ++i) {
            QString itemLine = in.readLine();
            QStringList itemParts = itemLine.split(" ");
            if (itemParts.size() >= 3) {
                int row = itemParts[0].toInt();
                int col = itemParts[1].toInt();
                int type = itemParts[2].toInt();
                m_items.emplace_back(row, col, type);
            }
        }
    }
    in.readLine();
}
    
void GameWindow::loadGameEffects(QTextStream& in)
{
    // 读取提示效果状态
    QString hintActiveLine = in.readLine();
    QStringList hintActiveParts = hintActiveLine.split(" ");
    if (hintActiveParts.size() >= 2) {
        m_hintOn = (hintActiveParts[1] == "1");
    }
    
    QString hintTimeLine = in.readLine();
    QStringList hintTimeParts = hintTimeLine.split(" ");
    if (hintTimeParts.size() >= 2) {
        m_hintTime = hintTimeParts[1].toInt();
    }
    
    QString hintBlocksLine = in.readLine();
    QStringList hintBlocksParts = hintBlocksLine.split(" ");
    if (hintBlocksParts.size() >= 5) {
        m_hintR1 = hintBlocksParts[1].toInt();
        m_hintC1 = hintBlocksParts[2].toInt();
        m_hintR2 = hintBlocksParts[3].toInt();
        m_hintC2 = hintBlocksParts[4].toInt();
    }
    
    // 读取玩家状态
    QString player1DizzyLine = in.readLine();
    QStringList player1DizzyParts = player1DizzyLine.split(" ");
    if (player1DizzyParts.size() >= 3) {
        bool dizzy = (player1DizzyParts[1] == "1");
        int time = player1DizzyParts[2].toInt();
        m_p1.setDizzy(dizzy, time);
    }
    
    QString player2DizzyLine = in.readLine();
    QStringList player2DizzyParts = player2DizzyLine.split(" ");
    if (player2DizzyParts.size() >= 3) {
        bool dizzy = (player2DizzyParts[1] == "1");
        int time = player2DizzyParts[2].toInt();
        m_p2.setDizzy(dizzy, time);
    }
}

//加载方块图片
void GameWindow::loadBlockImages()
{

    m_blockImages.clear();
    m_imagesLoaded = false;
    
    QStringList imageFiles = {":/1.png", ":/2.png", ":/3.png", ":/4.png", ":/5.png", ":/6.png", ":/7.png", ":/8.png"};

    int loaded = 0;
    for (int i = 0; i < imageFiles.size(); ++i) {
        const QString& imagePath = imageFiles[i];
        QPixmap pixmap(imagePath);
        m_blockImages.push_back(pixmap);
        if (!pixmap.isNull()) {
            ++loaded;
        } else {
        }
    } 
    m_imagesLoaded = (loaded == 8);
    
}

//绘制方块图片
void GameWindow::drawBlockImage(QPainter& painter, const QRectF& rect, int blockType, bool isActivated)
{
    if (!m_imagesLoaded) { 
        // 绘制彩色方块作为备用
        QColor colors[] = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::magenta, Qt::cyan, Qt::darkGreen, Qt::darkBlue};
        painter.setBrush(colors[(blockType - 1) % 8]);
        painter.setPen(Qt::black);
        painter.drawRoundedRect(rect, 5.0, 5.0);
        return;
    }
    
    if (blockType < 1 || blockType > 8) { 
        return;
    }
    
    int imageIndex = blockType - 1; 
    
    if (imageIndex >= m_blockImages.size()) { 
        return;
    }
    
    // 获取对应的图片
    const QPixmap& pixmap = m_blockImages[imageIndex];
    if (pixmap.isNull()) { 
        return;
    }
    
    // 绘制边框
    if (isActivated) {
        // 激活状态：红色边框
        painter.setPen(QPen(Qt::red, 3));
    } else {
        // 普通状态：黑色边框
        painter.setPen(QPen(QColorConstants::Svg::black, 1));
    }
    painter.setBrush(Qt::transparent);
    painter.drawRoundedRect(rect, 5.0, 5.0);
    
    // 绘制图片，缩放到方块大小
    painter.drawPixmap(rect, pixmap, pixmap.rect());
}


//停止所有定时器
void GameWindow::stopAllTimers()
{
    m_timer->stop();
    m_hintTimer->stop();
    m_dizzy1->stop();
    m_dizzy2->stop();
    m_spawnTimer->stop();
}

//重置游戏效果
void GameWindow::resetGameEffects()
{
    m_hintOn = false;
    m_showLine = false;
    m_linePath.clear();
    m_p1.setDizzy(false);
    m_p1.setFlashActive(false);
    m_p2.setDizzy(false);
    m_p2.setFlashActive(false);
}


//更新窗口标题
void GameWindow::updateWindowTitle(const QString& status)
{
    QString title = "QLink";
    if (m_twoPlayer) {
        title += " - 双人模式";
    } else {
        title += " - 单人模式";
    }
    
    if (!status.isEmpty()) {
        title += " (" + status + ")";
    }
    
    setWindowTitle(title);
}

// 初始化音效系统
void GameWindow::initializeAudioSystem()
{
    // 设置音效文件源
    m_eliminationSound->setSource(QUrl("qrc:/elimination.wav"));
    m_itemSound->setSource(QUrl("qrc:/item.wav"));
    m_selectSound->setSource(QUrl("qrc:/select.wav"));
    m_winSound->setSource(QUrl("qrc:/win.wav"));
    
    // 设置音量
    m_eliminationSound->setVolume(0.7f); // 70%音量
    m_itemSound->setVolume(0.7f); // 70%音量
    m_selectSound->setVolume(0.6f); // 60%音量
    m_winSound->setVolume(0.8f); // 80%音量
    
    // 初始化背景音乐
    m_backgroundMusic->setAudioOutput(m_audioOutput);
    m_backgroundMusic->setSource(QUrl("qrc:/music.wav"));
    m_backgroundMusic->setLoops(QMediaPlayer::Infinite); // 循环播放
    m_audioOutput->setVolume(0.3f); // 30%音量，避免与音效冲突
}

// 初始化僵尸GIF动图
void GameWindow::initializeZombieAnimation()
{
    // 创建QMovie对象加载普通僵尸GIF动图
    m_zombieMovie = new QMovie(":/zombie.gif", QByteArray(), this);
    
    if (m_zombieMovie->isValid()) {
        // 设置动画循环模式
        m_zombieMovie->setCacheMode(QMovie::CacheAll);
        
        // 连接帧改变信号到更新槽
        connect(m_zombieMovie, &QMovie::frameChanged, this, [this]() {
            if (!m_isZombieAttacking) { // 只在非攻击状态时更新普通帧
                m_currentZombieFrame = m_zombieMovie->currentPixmap();
                update(); // 刷新界面
            }
        });
        
        // 启动动画
        m_zombieMovie->start();
    }
    
    // 创建QMovie对象加载攻击僵尸GIF动图
    m_zombieEatMovie = new QMovie(":/zombie_eat.gif", QByteArray(), this);
    
    if (m_zombieEatMovie->isValid()) {
        // 设置动画循环模式
        m_zombieEatMovie->setCacheMode(QMovie::CacheAll);
        
        // 连接帧改变信号到更新槽
        connect(m_zombieEatMovie, &QMovie::frameChanged, this, [this]() {
            if (m_isZombieAttacking) { // 只在攻击状态时更新攻击帧
                m_currentZombieEatFrame = m_zombieEatMovie->currentPixmap();
                update(); // 刷新界面
            }
        });
        
        // 攻击动画不自动启动，只在需要时启动
    }
    
    // 设置攻击定时器
    m_attackTimer->setSingleShot(true); // 单次触发
    connect(m_attackTimer, &QTimer::timeout, this, [this]() {
        // 攻击动画结束，恢复普通动画
        m_isZombieAttacking = false;
        if (m_zombieEatMovie) {
            m_zombieEatMovie->stop();
        }
        if (m_zombieMovie) {
            m_zombieMovie->start();
        }
        update();
    });
}

// 加载菜单背景图片
void GameWindow::loadMenuBackground()
{
    // 加载背景图片
    m_menuBackground = QPixmap(":/background.jpg");
    
    if (m_menuBackground.isNull()) {
        // 如果加载失败，创建一个默认背景
        m_menuBackground = QPixmap(800, 600);
        m_menuBackground.fill(QColor(50, 50, 100)); // 深蓝色背景
    } else {
        // 输出背景图片信息用于调试
        qDebug() << "Background image loaded successfully:";
        qDebug() << "Size:" << m_menuBackground.size();
        qDebug() << "Aspect ratio:" << (double)m_menuBackground.width() / m_menuBackground.height();
    }
}

// 根据背景图片比例调整窗口大小
void GameWindow::adjustWindowSizeToBackground()
{
    if (!m_menuBackground.isNull()) {
        // 获取背景图片尺寸
        QSize imageSize = m_menuBackground.size();
        double imageRatio = (double)imageSize.width() / imageSize.height();
        
        // 设置基础窗口大小
        int baseWidth = 1000;
        int baseHeight = 700;
        
        // 根据图片比例计算最佳窗口尺寸
        int newWidth, newHeight;
        if (imageRatio > (double)baseWidth / baseHeight) {
            // 图片更宽，以宽度为准
            newWidth = baseWidth;
            newHeight = (int)(baseWidth / imageRatio);
        } else {
            // 图片更高，以高度为准
            newHeight = baseHeight;
            newWidth = (int)(baseHeight * imageRatio);
        }
        
        // 确保窗口不会太小
        if (newWidth < 800) {
            newWidth = 800;
            newHeight = (int)(800 / imageRatio);
        }
        if (newHeight < 600) {
            newHeight = 600;
            newWidth = (int)(600 * imageRatio);
        }
        
        // 设置窗口大小
        resize(newWidth, newHeight);
        
        // 输出调试信息
        qDebug() << "Window size adjusted to match background image:";
        qDebug() << "Image size:" << imageSize;
        qDebug() << "Image ratio:" << imageRatio;
        qDebug() << "Window size:" << newWidth << "x" << newHeight;
        qDebug() << "Window ratio:" << (double)newWidth / newHeight;
    }
}

// 触发僵尸攻击动画
void GameWindow::triggerZombieAttackAnimation()
{
    if (m_zombieEatMovie && m_zombieEatMovie->isValid()) {
        // 停止普通动画
        if (m_zombieMovie) {
            m_zombieMovie->stop();
        }
        
        // 设置攻击状态
        m_isZombieAttacking = true;
        
        // 启动攻击动画
        m_zombieEatMovie->start();
        
        // 设置攻击动画持续时间（根据GIF长度调整，这里设为1.5秒）
        m_attackTimer->start(1500);
    }
}

// 播放方块消除音效
void GameWindow::playEliminationSound()
{
    if (m_eliminationSound && m_soundEnabled) {
        m_eliminationSound->play();
    }
}

// 播放道具拾取音效
void GameWindow::playItemSound()
{
    if (m_itemSound && m_soundEnabled) {
        m_itemSound->play();
    }
}

// 播放菜单选择音效
void GameWindow::playSelectSound()
{
    if (m_selectSound && m_soundEnabled) {
        m_selectSound->play();
    }
}

// 播放游戏胜利音效
void GameWindow::playWinSound()
{
    if (m_winSound && m_soundEnabled) {
        m_winSound->play();
    }
}

// 播放背景音乐
void GameWindow::playBackgroundMusic()
{
    if (m_backgroundMusic && !m_backgroundMusicPlaying && m_musicEnabled) {
        m_backgroundMusic->play();
        m_backgroundMusicPlaying = true;
    }
}

// 停止背景音乐
void GameWindow::stopBackgroundMusic()
{
    if (m_backgroundMusic && m_backgroundMusicPlaying) {
        m_backgroundMusic->stop();
        m_backgroundMusicPlaying = false;
    }
}

// 初始化紧凑菜单
void GameWindow::initializeCompactMenu()
{
    // 创建菜单容器
    m_menuWidget = new QWidget(this);
    m_menuWidget->setFixedHeight(30);
    m_menuWidget->setStyleSheet(
        "QWidget { background-color: #2b2b2b; border-bottom: 1px solid #555555; }"
    );
    
    // 创建水平布局
    m_menuLayout = new QHBoxLayout(m_menuWidget);
    m_menuLayout->setContentsMargins(5, 2, 5, 2);
    m_menuLayout->setSpacing(0);
    
    // 创建按钮样式
    QString buttonStyle = 
        "QPushButton { "
        "    background-color: transparent; "
        "    border: none; "
        "    color: #ffffff; "
        "    padding: 5px 12px; "
        "    font-size: 12px; "
        "    text-align: left; "
        "} "
        "QPushButton:hover { "
        "    background-color: #404040; "
        "} "
        "QPushButton:pressed { "
        "    background-color: #555555; "
        "}";
    
    // 创建游戏按钮
    m_gameButton = new QPushButton("游戏(G)", m_menuWidget);
    m_gameButton->setStyleSheet(buttonStyle);
    m_gameButton->setFixedHeight(26);
    m_menuLayout->addWidget(m_gameButton);
    
    // 创建帮助按钮
    m_helpButton = new QPushButton("帮助(H)", m_menuWidget);
    m_helpButton->setStyleSheet(buttonStyle);
    m_helpButton->setFixedHeight(26);
    m_menuLayout->addWidget(m_helpButton);
    
    // 创建存档按钮
    m_saveButton = new QPushButton("存档(J)", m_menuWidget);
    m_saveButton->setStyleSheet(buttonStyle);
    m_saveButton->setFixedHeight(26);
    m_menuLayout->addWidget(m_saveButton);
    
    // 创建设置按钮
    m_settingsButton = new QPushButton("设置", m_menuWidget);
    m_settingsButton->setStyleSheet(buttonStyle);
    m_settingsButton->setFixedHeight(26);
    m_menuLayout->addWidget(m_settingsButton);
    
    // 添加弹性空间
    m_menuLayout->addStretch();
    
    // 创建下拉菜单
    createGameMenu();
    createHelpMenu();
    createSaveMenu();
    createSettingsMenu();
    
    // 连接按钮信号
    connect(m_gameButton, &QPushButton::clicked, this, [this]() {
        if (m_gameMenu) {
            m_gameMenu->exec(m_gameButton->mapToGlobal(QPoint(0, m_gameButton->height())));
        }
    });
    
    connect(m_helpButton, &QPushButton::clicked, this, [this]() {
        if (m_helpMenu) {
            m_helpMenu->exec(m_helpButton->mapToGlobal(QPoint(0, m_helpButton->height())));
        }
    });
    
    connect(m_saveButton, &QPushButton::clicked, this, [this]() {
        if (m_saveMenu) {
            m_saveMenu->exec(m_saveButton->mapToGlobal(QPoint(0, m_saveButton->height())));
        }
    });
    
    connect(m_settingsButton, &QPushButton::clicked, this, [this]() {
        if (m_settingsMenu) {
            m_settingsMenu->exec(m_settingsButton->mapToGlobal(QPoint(0, m_settingsButton->height())));
        }
    });
    
    // 将菜单添加到窗口顶部
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_menuWidget);
    
    // 设置中央widget（游戏区域）
    QWidget* centralWidget = new QWidget();
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(centralWidget);
    
    // 设置布局
    setLayout(mainLayout);
    
    // 确保游戏区域能够接收鼠标和键盘事件
    centralWidget->setMouseTracking(true);
    centralWidget->setFocusPolicy(Qt::StrongFocus);
}

// 创建游戏菜单
void GameWindow::createGameMenu()
{
    m_gameMenu = new QMenu(this);
    m_gameMenu->setStyleSheet(
        "QMenu { "
        "    background-color: #2b2b2b; "
        "    border: 1px solid #555555; "
        "    color: #ffffff; "
        "} "
        "QMenu::item { "
        "    padding: 8px 20px; "
        "    background-color: transparent; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #404040; "
        "}"
    );
    
    QAction* returnToMenu = m_gameMenu->addAction("返回菜单");
    QAction* restart = m_gameMenu->addAction("重新开始");
    m_gameMenu->addSeparator();
    QAction* exit = m_gameMenu->addAction("退出游戏");
    
    connect(returnToMenu, &QAction::triggered, this, [this]() {
        if (m_running) {
            end();
            showMenu();
        }
    });
    
    connect(restart, &QAction::triggered, this, [this]() {
        if (m_running) {
            start();
        }
    });
    
    connect(exit, &QAction::triggered, this, &QWidget::close);
}

// 创建帮助菜单
void GameWindow::createHelpMenu()
{
    m_helpMenu = new QMenu(this);
    m_helpMenu->setStyleSheet(
        "QMenu { "
        "    background-color: #2b2b2b; "
        "    border: 1px solid #555555; "
        "    color: #ffffff; "
        "} "
        "QMenu::item { "
        "    padding: 8px 20px; "
        "    background-color: transparent; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #404040; "
        "}"
    );
    
    QAction* help = m_helpMenu->addAction("操作指引");
    QAction* hints = m_helpMenu->addAction("操作提示");
    
    connect(help, &QAction::triggered, this, [this]() {
        QString helpText = 
            "=== QLink 游戏操作指引 ===\n\n"
            "【基本操作】\n"
            "• 使用 WASD 或方向键移动角色\n"
            "• 点击方块进行激活和消除\n"
            "• 空格键暂停/继续游戏\n"
            "• ESC 键返回菜单\n\n"
            "【游戏规则】\n"
            "• 连接两个相同图案的方块进行消除\n"
            "• 连接路径最多只能有两个拐角\n"
            "• 消除方块获得分数\n"
            "• 在时间限制内消除所有方块获胜\n\n"
            "【道具说明】\n"
            "• +1s: 增加游戏时间\n"
            "• Shuffle: 重新排列方块\n"
            "• Hint: 显示可消除的方块\n"
            "• Freeze: 冻结对手3秒（双人模式）\n"
            "• Dizzy: 让对手方向颠倒10秒（双人模式）\n"
            "• Flash: 5秒内可瞬移到任意位置（单人模式）";
        
        QMessageBox::information(this, "操作指引", helpText);
    });
    
    connect(hints, &QAction::triggered, this, [this]() {
        QString hintsText = 
            "=== 操作提示 ===\n\n"
            "WASD/方向键: 移动角色\n"
            "空格键: 暂停/继续游戏\n"
            "R: 重新开始\n"
            "T: 切换模式";
        
        QMessageBox::information(this, "操作提示", hintsText);
    });
}

// 创建存档菜单
void GameWindow::createSaveMenu()
{
    m_saveMenu = new QMenu(this);
    m_saveMenu->setStyleSheet(
        "QMenu { "
        "    background-color: #2b2b2b; "
        "    border: 1px solid #555555; "
        "    color: #ffffff; "
        "} "
        "QMenu::item { "
        "    padding: 8px 20px; "
        "    background-color: transparent; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #404040; "
        "}"
    );
    
    QAction* save = m_saveMenu->addAction("保存游戏");
    QAction* load = m_saveMenu->addAction("载入游戏");
    QAction* deleteSave = m_saveMenu->addAction("删除存档");
    
    connect(save, &QAction::triggered, this, [this]() {
        // 显示保存存档槽位选择
        m_showSaveSlots = true;
        m_showLoadSlots = false; // 确保载入界面关闭
        m_selectedSaveSlot = SAVE_SLOT_1;
        m_backButtonSelected = false; // 重置返回按钮状态
        update();
    });
    
    connect(load, &QAction::triggered, this, [this]() {
        // 显示载入存档槽位选择
        m_showLoadSlots = true;
        m_showSaveSlots = false; // 确保保存界面关闭
        m_selectedSaveSlot = SAVE_SLOT_1;
        m_backButtonSelected = false; // 重置返回按钮状态
        update();
    });
    
    connect(deleteSave, &QAction::triggered, this, [this]() {
        // 显示删除存档槽位选择
        m_showDeleteSlots = true;
        m_showSaveSlots = false; // 确保其他界面关闭
        m_showLoadSlots = false;
        m_selectedSaveSlot = SAVE_SLOT_1;
        m_backButtonSelected = false; // 重置返回按钮状态
        update();
    });
}

// 创建设置菜单
void GameWindow::createSettingsMenu()
{
    m_settingsMenu = new QMenu(this);
    m_settingsMenu->setStyleSheet(
        "QMenu { "
        "    background-color: #2b2b2b; "
        "    border: 1px solid #555555; "
        "    color: #ffffff; "
        "} "
        "QMenu::item { "
        "    padding: 8px 20px; "
        "    background-color: transparent; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #404040; "
        "}"
    );
    
    QAction* soundToggle = m_settingsMenu->addAction("音效: 开启");
    QAction* musicToggle = m_settingsMenu->addAction("音乐: 开启");
    QAction* fullscreen = m_settingsMenu->addAction("全屏模式");
    
    soundToggle->setCheckable(true);
    soundToggle->setChecked(true);
    musicToggle->setCheckable(true);
    musicToggle->setChecked(true);
    
    connect(soundToggle, &QAction::triggered, this, [this, soundToggle]() {
        m_soundEnabled = soundToggle->isChecked();
        if (m_soundEnabled) {
            soundToggle->setText("音效: 开启");
        } else {
            soundToggle->setText("音效: 关闭");
        }
    });
    
    connect(musicToggle, &QAction::triggered, this, [this, musicToggle]() {
        m_musicEnabled = musicToggle->isChecked();
        if (m_musicEnabled) {
            musicToggle->setText("音乐: 开启");
            // 如果当前在菜单状态且音乐未播放，则开始播放
            if (m_state == MENU_STATE && !m_backgroundMusicPlaying) {
                playBackgroundMusic();
            }
        } else {
            musicToggle->setText("音乐: 关闭");
            // 立即停止背景音乐
            stopBackgroundMusic();
        }
    });
    
    connect(fullscreen, &QAction::triggered, this, [this]() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
}
