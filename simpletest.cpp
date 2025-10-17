#include "simpletest.h"
#include <vector>


void SimpleTest::initTestCase()
{
    m_judger = new LinkJudger(16, 24);  
    QVERIFY(m_judger != nullptr);
}


void SimpleTest::cleanupTestCase()
{
    delete m_judger;
    m_judger = nullptr;
}

// 创建用于测试的小地图
void SimpleTest::createTestMap(std::vector<std::vector<int>>& map, int rows, int cols)
{
    map.clear();
    map.resize(rows, std::vector<int>(cols, 0));
}


// 直线连接测试 


void SimpleTest::testCanLineConnection_Horizontal()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4); // 创建一个4x4的地图。
    
    // 测试水平直线(无障碍),测试起点为(1,0),终点为(1,3)。
    map[1][0] = 1;  // 起点
    map[1][3] = 1;  // 终点
  
    
    QVERIFY(m_judger->canLineConnection(1, 0, 1, 3, map) == true);
    QVERIFY(m_judger->canEliminate(1, 0, 1, 3, map) == true);
    
    map[1][2] = 2;  
    map[0][1] = 2; map[2][1] = 2;
    map[0][2] = 2; map[2][2] = 2;
    map[0][0] = 2; map[0][3] = 2; map[2][0] = 2; map[2][3] = 2;
    
    QVERIFY(m_judger->canLineConnection(1, 0, 1, 3, map) == false);
    QVERIFY(m_judger->canEliminate(1, 0, 1, 3, map) == false);
}

//测试同一列的相同类型方块是否可以直线连接。
void SimpleTest::testCanLineConnection_Vertical()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4);
    
    // 测试垂直直线(无障碍),测试起点为(0,1),终点为(3,1)。
    map[0][1] = 1;  // 起点
    map[3][1] = 1;  // 终点
    
    QVERIFY(m_judger->canLineConnection(0, 1, 3, 1, map) == true);
    QVERIFY(m_judger->canEliminate(0, 1, 3, 1, map) == true);
    
    map[2][1] = 2;  
    map[0][0] = 2; map[1][0] = 2; map[2][0] = 2; map[3][0] = 2;
    map[0][2] = 2; map[1][2] = 2; map[2][2] = 2; map[3][2] = 2;
    
    QVERIFY(m_judger->canLineConnection(0, 1, 3, 1, map) == false);
    QVERIFY(m_judger->canEliminate(0, 1, 3, 1, map) == false);
}


//测试方块相邻的情况下的直线连接,测试起点为(1,1),终点为(1,2)。
void SimpleTest::testCanLineConnection_EdgeCases()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
    map[1][1] = 1;
    map[1][2] = 1;
    
    QVERIFY(m_judger->canLineConnection(1, 1, 1, 2, map) == true); 
    QVERIFY(m_judger->canLineConnection(1, 1, 1, 1, map) == true);
    QVERIFY(m_judger->canEliminate(1, 1, 1, 2, map) == true); 
    QVERIFY(m_judger->canEliminate(1, 1, 1, 1, map) == false);
}

// 单拐角连接测试

//测试L形路径(无障碍)的情况,测试起点为(0,0),终点为(2,2)。
void SimpleTest::testHasOneCornerPath_Valid()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4);
    
    // 测试L形路径(无障碍),测试起点为(0,0),终点为(2,2)。
    map[0][0] = 1;  // 起点
    map[2][2] = 1;  // 终点
    // 路径: (0,0) -> (0,2) -> (2,2)
    
    QVERIFY(m_judger->canEliminate(0, 0, 2, 2, map) == true);
}

//测试L形路径(有障碍)的情况,测试起点为(0,0),终点为(2,2),障碍为(0,1)和(1,0)。
void SimpleTest::testHasOneCornerPath_Invalid()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4);
    
    // 测试L形路径 有障碍，阻挡所有可能的单拐角路径
    map[0][0] = 1;  
    map[2][2] = 1;  
    map[0][1] = 2;  
    map[1][0] = 2;  
    
    QVERIFY(m_judger->canEliminate(0, 0, 2, 2, map) == false);
}

//专门测试L形路径(贴边转弯)的情况,测试起点为(0,0),终点为(2,2),拐角为(0,2)。
void SimpleTest::testHasOneCornerPath_EdgeCases()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
  
    map[0][0] = 1; // 起点
    map[2][2] = 1; // 终点

    QVERIFY(m_judger->canEliminate(0, 0, 2, 2, map) == true);
}

// 双拐角连接测试
void SimpleTest::testHasTwoCornersPath_Valid()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 5, 5);
    
    map[1][1] = 1;  // 起点
    map[3][3] = 1;  // 终点

    map[1][3] = 2;  // 阻断经 (1,3) 拐角的水平段
    map[3][1] = 2;  // 阻断经 (3,1) 拐角的垂直段

    QVERIFY(m_judger->canEliminate(1, 1, 3, 3, map) == true);
}

void SimpleTest::testHasTwoCornersPath_Invalid()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 5, 5);
    
    map[1][1] = 1;  
    map[3][3] = 1; 

    map[1][2] = 2;  
    map[1][3] = 2;
    map[2][1] = 2;
    map[3][1] = 2;  

    QVERIFY(m_judger->canEliminate(1, 1, 3, 3, map) == false);
}

void SimpleTest::testHasTwoCornersPath_EdgeCases()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4);
    
    // 测试边界情况 - 必须阻断直线和单拐角连接，只允许双拐角连接
    map[0][0] = 1;  // 起点
    map[3][3] = 1;  // 终点

    map[0][3] = 2;
    map[3][0] = 2;
    QVERIFY(m_judger->canEliminate(0, 0, 3, 3, map) == true);
}

//综合消除判定测试
void SimpleTest::testCanEliminate_NoPath()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4);
    
    // 测试无路径情况,完全被阻挡
    map[0][0] = 1;  // 起点
    map[3][3] = 1;  // 终点
    map[0][1] = 2;  // 阻挡所有可能的路径
    map[0][2] = 2;
    map[1][0] = 2;
    map[1][1] = 2;
    map[1][2] = 2;
    map[1][3] = 2;
    map[2][0] = 2;
    map[2][1] = 2;
    map[2][2] = 2;
    map[2][3] = 2;
    map[3][0] = 2;
    map[3][1] = 2;
    map[3][2] = 2;
    
    QVERIFY(m_judger->canEliminate(0, 0, 3, 3, map) == false);
}

//测试相同位置的方块能否消除
void SimpleTest::testCanEliminate_SamePosition()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
    map[1][1] = 1;
    
    QVERIFY(m_judger->canEliminate(1, 1, 1, 1, map) == false); 
}

//测试不同类型的方块能否消除
void SimpleTest::testCanEliminate_DifferentTypes()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
    map[0][0] = 1;  
    map[0][2] = 2;  
    
    QVERIFY(m_judger->canEliminate(0, 0, 0, 2, map) == false);
}

//测试空方块能否消除
void SimpleTest::testCanEliminate_EmptyBlocks()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
    map[0][0] = 0;  
    map[0][2] = 1;  
    
    QVERIFY(m_judger->canEliminate(0, 0, 0, 2, map) == false);
}

//测试越界位置能否消除
void SimpleTest::testCanEliminate_OutOfBounds()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
    map[0][0] = 1;
    
    QVERIFY(m_judger->canEliminate(0, 0, 5, 5, map) == false);
    QVERIFY(m_judger->canEliminate(-1, 0, 0, 0, map) == false);
}

// 边界条件测试

void SimpleTest::testBoundaryConditions()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4);
    
    // 测试边界位置
    map[0][0] = 1;
    map[0][3] = 1;
    map[3][0] = 1;
    map[3][3] = 1;
    
    QVERIFY(m_judger->canEliminate(0, 0, 0, 3, map) == true);
    QVERIFY(m_judger->canEliminate(0, 0, 3, 0, map) == true);
    QVERIFY(m_judger->canEliminate(0, 3, 3, 3, map) == true);
    QVERIFY(m_judger->canEliminate(3, 0, 3, 3, map) == true);
}

//测试角落位置能否消除
void SimpleTest::testCornerPositions()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
    map[0][0] = 1;
    map[2][2] = 1;
    
    QVERIFY(m_judger->canEliminate(0, 0, 2, 2, map) == true);
}

//测试边缘位置能否消除
void SimpleTest::testEdgePositions()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 4, 4);
    
    // 测试边缘位置
    map[0][1] = 1;
    map[0][2] = 1;
    
    QVERIFY(m_judger->canEliminate(0, 1, 0, 2, map) == true);
}

// 异常情况测试

//测试无效输入是否会导致崩溃
void SimpleTest::testInvalidInputs()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 3, 3);
    
    QVERIFY(m_judger->canEliminate(0, 0, 0, 0, map) == false);  // 相同位置
    QVERIFY(m_judger->canEliminate(-1, 0, 0, 0, map) == false); // 负坐标
    QVERIFY(m_judger->canEliminate(0, 0, 10, 10, map) == false); // 超出范围
}

//测试极端情况是否会崩溃
void SimpleTest::testExtremeCases()
{
    std::vector<std::vector<int>> map;
    createTestMap(map, 2, 2);
    
    // 测试最小地图的情况
    map[0][0] = 1;
    map[1][1] = 1;
    
    QVERIFY(m_judger->canEliminate(0, 0, 1, 1, map) == true);
    
    // 测试完全被阻挡的情况
    map[0][1] = 2;
    map[1][0] = 2;
    
    QVERIFY(m_judger->canEliminate(0, 0, 1, 1, map) == false);
}

QTEST_MAIN(SimpleTest)
