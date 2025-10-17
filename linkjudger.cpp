#include "linkjudger.h"
#include <algorithm>

LinkJudger::LinkJudger(int rows, int cols) : m_rows(rows), m_cols(cols)
{
}

bool LinkJudger::canEliminate(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map)
{
    const int mapRows = static_cast<int>(map.size());
    if (mapRows == 0) {
        return false;
    }
    if (r1 < 0 || r1 >= mapRows || r2 < 0 || r2 >= mapRows) {
        return false;
    }
    const int row1Cols = static_cast<int>(map[r1].size());
    const int row2Cols = static_cast<int>(map[r2].size());
    if (c1 < 0 || c1 >= row1Cols || c2 < 0 || c2 >= row2Cols) {
        return false;
    }
    
    // 基本验证
    if (!isValidElimination(r1, c1, r2, c2, map)) {
        return false;
    }
    
    // 按优先级检查连接方式：直线 -> 单拐角 -> 双拐角
    if (canLineConnection(r1, c1, r2, c2, map)) {
        return true;
    }
    
    if (canOneCornerConnection(r1, c1, r2, c2, map)) {
        return true;
    }
    
    if (canTwoCornersConnection(r1, c1, r2, c2, map)) {
        return true;
    }
    
    return false;
}

//检查是否是有效的消除
bool LinkJudger::isValidElimination(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map)
{
    //  检查是否越界
    const int mapRows = static_cast<int>(map.size());
    if (mapRows == 0) {
        return false;
    }
    if (r1 < 0 || r1 >= mapRows || r2 < 0 || r2 >= mapRows) {
        return false;
    }
    const int row1Cols = static_cast<int>(map[r1].size());
    const int row2Cols = static_cast<int>(map[r2].size());
    if (c1 < 0 || c1 >= row1Cols || c2 < 0 || c2 >= row2Cols) {
        return false;
    }

    if (r1 == r2 && c1 == c2) {
        return false; // 同一格不能消除
    }

    if (map[r1][c1] == 0 || map[r2][c2] == 0) {
        return false; //空方块不能消除
    }
    if (map[r1][c1] != map[r2][c2]) {
        return false; //不同类型不能消除
    }
    
    return true;
}
    
//检查一个拐角能否连接
bool LinkJudger::canOneCornerConnection(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map)
{
    // 使用实际地图大小进行边界检查
    int mapRows = static_cast<int>(map.size());
    if (mapRows == 0) return false;
    int mapCols = static_cast<int>(map[0].size());
    
 
    if (r1 < 0 || r1 >= mapRows || c1 < 0 || c1 >= mapCols ||
        r2 < 0 || r2 >= mapRows || c2 < 0 || c2 >= mapCols) {
        return false;
    }
    
    // 检查水平-垂直路径
    if (r1 >= 0 && r1 < mapRows && c2 >= 0 && c2 < mapCols && 
        map.size() > static_cast<size_t>(r1) && map[r1].size() > static_cast<size_t>(c2)) {
        if (canLineConnection(r1, c1, r1, c2, map) && canLineConnection(r1, c2, r2, c2, map) && map[r1][c2] == 0) {
            return true;
        }
    }
    
    // 检查垂直-水平路径
    if (r2 >= 0 && r2 < mapRows && c1 >= 0 && c1 < mapCols && 
        map.size() > static_cast<size_t>(r2) && map[r2].size() > static_cast<size_t>(c1)) {
        if (canLineConnection(r1, c1, r2, c1, map) && canLineConnection(r2, c1, r2, c2, map) && map[r2][c1] == 0) {
            return true;
        }
    }
    
    return false;
}

//检查地图是否还有方块可以消除
bool LinkJudger::hasSolutions(const std::vector<std::vector<int>>& map)
{
    for (int r1 = 0; r1 < m_rows; ++r1) {
        for (int c1 = 0; c1 < m_cols; ++c1) {
            for (int r2 = r1; r2 < m_rows; ++r2) {
                for (int c2 = (r2 == r1) ? c1 + 1 : 0; c2 < m_cols; ++c2) { //c2从c1+1开始，避免重复检查
                    if (canEliminate(r1, c1, r2, c2, map)) { //如果两个方块可以消除，则地图还有方块可以消除
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//检查直线连接
bool LinkJudger::canLineConnection(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map)
{
  
    int mapRows = static_cast<int>(map.size());
    if (mapRows == 0) return false;
    int mapCols = static_cast<int>(map[0].size());
    
    // 确保坐标在有效范围内
    if (r1 < 0 || r1 >= mapRows || c1 < 0 || c1 >= mapCols ||
        r2 < 0 || r2 >= mapRows || c2 < 0 || c2 >= mapCols) {
        return false;
    }
    
    if (r1 == r2) {
        // 水平连接
        int minC = std::min(c1, c2);
        int maxC = std::max(c1, c2);
        for (int c = minC + 1; c < maxC; ++c) {
            if (map[r1][c] != 0) {
                return false;
            }
        }
        return true;
    } else if (c1 == c2) {
        // 垂直连接
        int minR = std::min(r1, r2);
        int maxR = std::max(r1, r2);
        for (int r = minR + 1; r < maxR; ++r) {
            if (map[r][c1] != 0) {
                return false;
            
            }
        }
        return true;
    }
    return false;
}

//检查两个拐角能否连接
bool LinkJudger::canTwoCornersConnection(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map)
{
    int mapRows = static_cast<int>(map.size());
    if (mapRows == 0) return false;
    int mapCols = static_cast<int>(map[0].size());
    
    // 确保坐标在有效范围内
    if (r1 < 0 || r1 >= mapRows || c1 < 0 || c1 >= mapCols ||
        r2 < 0 || r2 >= mapRows || c2 < 0 || c2 >= mapCols) {
        return false;
    }
    
    // 路径1: 水平-垂直-水平 (r1,c1) -> (r1,c) -> (r2,c) -> (r2,c2)
    for (int c = 0; c < mapCols; ++c) {
        if (c == c1 || c == c2) continue;
        
        if (map[r1][c] != 0) continue;  // 第一个拐角必须是空白
        if (map[r2][c] != 0) continue;  // 第二个拐角必须是空白
        
        // 检查 (r1,c1) -> (r1,c) 是否畅通
        if (!canLineConnection(r1, c1, r1, c, map)) continue;
        
        // 检查 (r1,c) -> (r2,c) 是否畅通
        if (!canLineConnection(r1, c, r2, c, map)) continue;
        
        // 检查 (r2,c) -> (r2,c2) 是否畅通
        if (!canLineConnection(r2, c, r2, c2, map)) continue;
        
        return true;
    }
    
    // 路径2: 垂直-水平-垂直 (r1,c1) -> (r,c1) -> (r,c2) -> (r2,c2)
    for (int r = 0; r < mapRows; ++r) {
        if (r == r1 || r == r2) continue;
        
        if (map[r][c1] != 0) continue;  // 第一个拐角必须是空白
        if (map[r][c2] != 0) continue;  // 第二个拐角必须是空白
        
        // 检查 (r1,c1) -> (r,c1) 是否畅通
        if (!canLineConnection(r1, c1, r, c1, map)) continue;
        
        // 检查 (r,c1) -> (r,c2) 是否畅通
        if (!canLineConnection(r, c1, r, c2, map)) continue;
        
        // 检查 (r,c2) -> (r2,c2) 是否畅通
        if (!canLineConnection(r, c2, r2, c2, map)) continue;
        
        return true;
    }
    
    return false;
}
