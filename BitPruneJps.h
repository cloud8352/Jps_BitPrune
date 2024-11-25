#pragma once

#include "Common.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;
using std::vector;

class BitPruneJps
{
public:
    // 辅助地图节点
    struct Node
    {
        unsigned int X; // 水平坐标
        unsigned int Y; // 垂直坐标
        unsigned int G, H, F;
        bool IsObstacle;     // 是否为障碍
        bool IsInPath;       // 是否是最短路径中的一点
        bool IsInClosedList; //
        bool IsInOpenedList;

        Node *ParentNode;
        vector<Node *> NextNodeList; // 用于存入该点的周围可行点
        bool IsNull;                 // 是否是空节点
        bool IsMidJumpNode;          // 是否是中间跳点

        Node()
        {
            X = 0;
            Y = 0;
            G = H = F = 0;
            IsObstacle = false;
            IsInPath = false;
            IsInClosedList = IsInOpenedList = false;
            ParentNode = nullptr;
            IsNull = true;
            IsMidJumpNode = false;
        }
        void Reset()
        {
            G = H = F = 0;
            IsInPath = false;
            IsInClosedList = IsInOpenedList = false;
            ParentNode = nullptr;
            IsNull = true;
            IsMidJumpNode = false;
        }
        void UpdateF()
        {
            F = G + H;
        }
    };
    explicit BitPruneJps();
    virtual ~BitPruneJps();
    void InitMap(int **map, unsigned int width, unsigned int height);
    void FindPath(unsigned int beginX, unsigned int beginY,
                  unsigned int endX, unsigned int endY);
    void PrintPath();
    void PrintPathMap();

private:
    // 重置地图
    void resetMap();
    void deleteMap();
    // 计算两点直线距离
    int GetDis(const Node &startNode, const Node &endNode)
    {
        int dis = sqrt(pow(long(endNode.X) - long(startNode.X), 2) + pow(long(endNode.Y) - long(startNode.Y), 2)) * 10; // pow次方函数
        return dis;
    }
    // 计算h值
    int getH(const Node &node);
    vector<DirectionEnum> getForcedNeighbourDirs(unsigned int x, unsigned int y,
                                                 DirectionEnum jumpedDir);
    bool *getNeighbourType(short unitMap, char p, char n);
    Node JumpStraight(const Node &currenNode, const DirectionEnum &dir);
    Node *JumpOblique(const Node &currenNode, const DirectionEnum &dir);

private:
    // 建立辅助地图
    Node **m_map;
    unsigned int m_mapWidth;
    unsigned int m_mapHeight;
    Node *m_beginNode;
    Node *m_endNode;

    vector<Node *> m_openedNodeList; // 开放节点列表
    vector<Node *> m_pathNodeList;   // 储存最终路径
};
