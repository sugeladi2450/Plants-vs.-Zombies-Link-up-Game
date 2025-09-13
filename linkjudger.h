#ifndef LINKJUDGER_H
#define LINKJUDGER_H

#include <vector>

class LinkJudger {
public:
    LinkJudger(int rows, int cols);
    bool canEliminate(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map);
    bool hasSolutions(const std::vector<std::vector<int>>& map);
    bool canLineConnection(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map);
    
private:
    bool isValidElimination(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map);
    bool canOneCornerConnection(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map);
    bool canTwoCornersConnection(int r1, int c1, int r2, int c2, const std::vector<std::vector<int>>& map);
    int m_rows;
    int m_cols;
};

#endif // LINKJUDGER_H
