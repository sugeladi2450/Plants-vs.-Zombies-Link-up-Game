#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <QtTest>
#include "linkjudger.h"
#include <vector>

class SimpleTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 直线连接测试
    void testCanLineConnection_Horizontal();
    void testCanLineConnection_Vertical();
    void testCanLineConnection_EdgeCases();

    // 单拐角连接测试
    void testHasOneCornerPath_Valid();
    void testHasOneCornerPath_Invalid();
    void testHasOneCornerPath_EdgeCases();

    // 双拐角连接测试
    void testHasTwoCornersPath_Valid();
    void testHasTwoCornersPath_Invalid();
    void testHasTwoCornersPath_EdgeCases();

    // 综合消除判定测试 - 只保留真正需要测试的边界和异常情况
    void testCanEliminate_NoPath();
    void testCanEliminate_SamePosition();
    void testCanEliminate_DifferentTypes();
    void testCanEliminate_EmptyBlocks();
    void testCanEliminate_OutOfBounds();

    // 边界条件测试
    void testBoundaryConditions();
    void testCornerPositions();
    void testEdgePositions();

    // 异常情况测试
    void testInvalidInputs();
    void testExtremeCases();

private:
    LinkJudger* m_judger; //测试类 SimpleTest 的成员指针，指向一个 LinkJudger 实例。它负责游戏“连线是否可消除”的判定逻辑。
    
    // 测试辅助函数
    void createTestMap(std::vector<std::vector<int>>& map, int rows, int cols);
};

#endif // SIMPLETEST_H
