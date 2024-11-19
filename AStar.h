#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;
using std::vector;

// 方向枚举
enum DirectionEnum
{
    Up,
    Down,
    Left,
    Right,
    LeftUp,
    LeftDown,
    RightUp,
    RightDown
};
const vector<DirectionEnum> DirectionEnums =
    {Up, Down, Left, Right, LeftUp, LeftDown, RightUp, RightDown};

const int VerticalDist = 10; // 每格到相邻格直线距离
const int ObliqueDist = 14;  // 每格到相邻格斜线距离

class AStar
{
public:
    struct Node
    {
        unsigned int X; // 水平坐标
        unsigned int Y; // 垂直坐标
        unsigned int G, H, F;
        bool IsObstacle; // 是否为障碍
        bool IsInPath;  // 是否是最短路径中的一点
        bool IsInClosedList; // 
        bool IsInOpenedList;

        Node *ParentNode;
        vector<Node *> NextNodeList; // 用于存入该点的周围可行点
        Node()
        {
            X = 0;
            Y = 0;
            G = H = F = 0;
            IsObstacle = false;
            IsInPath = false;
            IsInClosedList = IsInOpenedList = false;
            ParentNode = nullptr;
        }
        void Reset()
        {
            G = H = F = 0;
            IsInPath = false;
            IsInClosedList = IsInOpenedList = false;
            ParentNode = nullptr;
        }
        void UpdateF()
        {
            F = G + H;
        }
    };

    explicit AStar();
    virtual ~AStar();
    void InitMap(int **map, unsigned int width, unsigned int height);
    void FindPath(unsigned int beginX, unsigned int beginY,
                  unsigned int endX, unsigned int endY);
    void PrintPath();
    void PrintPathMap();

private:
    // 重置地图
    void resetMap();
    // 该点是否为障碍
    bool isObstacle(const Node &node);
    // 位置是否存在障碍
    bool whetherPosHasObstacle(unsigned int x, unsigned int y);
    // 计算h值
    int getH(const Node &node);
    void updateOpenedNodeList(const Node &exploringNode);
    void deleteMap();

private:
    // 建立辅助地图
    Node **m_map;
    unsigned int m_mapWidth;
    unsigned int m_mapHeight;
    Node *m_beginNode;
    Node *m_endNode;

    vector<Node *> m_openedNodeList; // 开放节点列表
    vector<Node *> m_pathNodeList; // 储存最终路径
};
