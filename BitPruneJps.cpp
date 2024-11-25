
#include "BitPruneJps.h"

BitPruneJps::BitPruneJps()
    : m_map(nullptr), m_mapWidth(0), m_mapHeight(0), m_beginNode(nullptr), m_endNode(nullptr)
{
}

BitPruneJps::~BitPruneJps()
{
    // delete m_map
    if (m_map)
    {
        deleteMap();
    }
}

void BitPruneJps::InitMap(int **map, unsigned int width, unsigned int height)
{
    if (m_map)
    {
        deleteMap();
    }

    m_mapWidth = width;
    m_mapHeight = height;

    // 建立辅助地图
    m_map = new Node *[height];
    for (int y = 0; y < height; y++)
    {
        m_map[y] = new Node[width];
        for (int x = 0; x < width; x++)
        {
            Node &node = m_map[y][x];
            node.X = x;
            node.Y = y;
            node.IsObstacle = map[y][x] == 1 ? true : false;
        }
    }
}

void BitPruneJps::FindPath(unsigned int beginX, unsigned int beginY, unsigned int endX, unsigned int endY)
{
    resetMap();
    m_beginNode = &m_map[beginY][beginX];
    m_endNode = &m_map[endY][endX];

    bool findGoal = false; // 是否找到了结束目标

    vector<DirectionEnum> jumpDirs;         // 存放当前需要跳跃的方向
    vector<DirectionEnum>::iterator dirsIt; // 用于检索反向树的迭代器

    Node *currentNodePtr; // 当前节点
    // vector<PathNode> openTree;            // 开放列表，关闭列表是用辅助地图各点的isfind属性维护的
    vector<Node *>::iterator it;       // 用于迭代
    vector<Node *>::iterator minFIter; // 存放最小f值节点

    currentNodePtr = m_beginNode; // 当前点为开始点
    currentNodePtr->IsInClosedList = true;

    // 为了找到终点，将终点设为障碍，当能检测到终点周围的跳点时，下一步找到终点
    m_map[m_endNode->Y][m_endNode->X].IsObstacle = 1;

    // 初始方向树（八个方向）
    jumpDirs = DirectionEnums;

    // 寻路
    while (true)
    {
        // 利用当前点，以及parent方向，往所有“真。邻居“方向跳跃
        for (dirsIt = jumpDirs.begin(); dirsIt != jumpDirs.end(); dirsIt++)
        {
            Node nodeTmp = *currentNodePtr; // 临时检索节点，用于当前方向上的检索
            // cout<<"方向："<<(*dirsIt)<<" "<<endl;
            // 返回的跳点组
            Node *jumpNodes = {};
            if (*(dirsIt) == Up || *(dirsIt) == Down || *(dirsIt) == Left || *(dirsIt) == Right)
            {
                jumpNodes[0] = JumpStraight(nodeTmp, (*dirsIt));
            }
            if (*(dirsIt) == LeftUp || *(dirsIt) == LeftDown || *(dirsIt) == RightUp || *(dirsIt) == RightDown)
            {
                jumpNodes = JumpOblique(nodeTmp, (*dirsIt));
            }

            // 从返回的节点中，搜寻跳点，并更新g、h、f值
            for (int i = 0; i < 2; i++)
            {
                Node jumpNode = jumpNodes[i];
                // 如果返回的是有效节点，且不在关闭列表中（未找过）
                if (false == jumpNode.IsNull &&
                    false == m_map[jumpNode.Y][jumpNode.X].IsInClosedList && // 不在关闭列表中（未找过）
                    false == jumpNode.IsMidJumpNode)                         // 不是中间跳点
                {
                    Node &nodeTmpInMap = m_map[nodeTmp.Y][nodeTmp.X];
                    Node parentTmp; // 临时父节点，用于计算g值，判断是否更新父节点
                    if (nodeTmpInMap.IsMidJumpNode)
                    { // 如果发现当前点为中间跳点,就把后续节点的父节点设为当前节点的父节点
                        parentTmp.Y = nodeTmpInMap.ParentNode->Y;
                        parentTmp.X = nodeTmpInMap.ParentNode->X;
                    }
                    if (false == nodeTmpInMap.IsMidJumpNode)
                    { // 如果发现当前点不是中间跳点
                        parentTmp.Y = nodeTmpInMap.Y;
                        parentTmp.X = nodeTmpInMap.X;
                    }
                    Node &parentNodeTmpInMap = m_map[parentTmp.Y][parentTmp.X];
                    jumpNode.G = parentNodeTmpInMap.G + GetDis(parentNodeTmpInMap, jumpNodes[i]);
                    // 如果该点已经在开放列表中，比较g值，取g值较小的点的属性，并不用再次加入开放列表
                    Node &jumpNodeInMap = m_map[jumpNode.Y][jumpNode.X];
                    if (jumpNodeInMap.IsInOpenedList)
                    {
                        if (jumpNodeInMap.G > jumpNode.G)
                        {
                            jumpNodeInMap.G = jumpNode.G;
                            jumpNodeInMap.UpdateF();
                            jumpNodeInMap.ParentNode = &parentNodeTmpInMap;
                        }
                    }
                    // 如果不在开放列表中
                    if (false == jumpNodeInMap.IsInOpenedList)
                    {
                        // 更新辅助地图中对应探测点的节点属性
                        jumpNodeInMap.G = jumpNode.G;
                        jumpNodeInMap.H = getH(jumpNodeInMap);
                        jumpNodeInMap.UpdateF();
                        jumpNodeInMap.ParentNode = &parentNodeTmpInMap;
                        jumpNodeInMap.IsInOpenedList = true;

                        // 将探测点加入开放列表
                        m_openedNodeList.push_back(&jumpNodeInMap);
                    }
                    // 判断是否找到了终点--跳点周围存在终点
                    if (m_endNode->Y == jumpNode.Y)
                    {
                        if (m_endNode->X == jumpNode.X - 1 ||
                            m_endNode->X == jumpNode.X + 1)
                        {
                            findGoal = true;
                            m_endNode->ParentNode = &jumpNodeInMap;
                            break;
                        }
                    }
                    if (m_endNode->X == jumpNode.X)
                    {
                        if (m_endNode->Y == jumpNode.Y - 1 ||
                            m_endNode->Y == jumpNode.Y + 1)
                        {
                            findGoal = true;
                            m_endNode->ParentNode = &jumpNodeInMap;
                            break;
                        }
                    }

                } // end--//如果返回的是有效节点，且不在关闭列表中（未找过）

            } // end--从返回的节点中，搜寻跳点，并更新g、h、f值

            if (findGoal)
                break;
        } // end---for(dirsIt = jumpDirs.begin();dirsIt != jumpDirs.end(); dirsIt++)

        if (findGoal)
            break;

        if (m_openedNodeList.size() == 0)
            break;
        // 找下一点
        minFIter = m_openedNodeList.begin();
        // cout<<endl<<"找下一点"<<endl;
        for (it = m_openedNodeList.begin(); it != m_openedNodeList.end(); it++)
        {
            // cout<<(*it).row<<","<<(*it).X<<endl;
            if ((*it)->F < (*minFIter)->F)
            {
                minFIter = it;
            }
        }

#if 0 // 调试
        cout<<endl<<"找到的下一点: ";
        cout<<(*minF_iter).row<<","<<(*minF_iter).X<<endl;
        cout<<"此节点父节点坐标：";
        PathNode tmp = { (*minF_iter).row,(*minF_iter).X};
        while(NULL != pathMap[tmp.row][tmp.X]->parent){
            int t_row = tmp.row,t_col = tmp.X;
            tmp.row = pathMap[t_row][t_col]->parent->row;
            tmp.X = pathMap[t_row][t_col]->parent->col;
            cout<<tmp.row<<","<<tmp.X<<" ";
        }
        cout<<endl;
#endif

        currentNodePtr = (*minFIter);
        currentNodePtr->IsInClosedList = true;
        m_openedNodeList.erase(minFIter);

        if (currentNodePtr == m_endNode)
        {
            break;
        }

        DirectionEnum jumpedDir;
        int deltaY = currentNodePtr->Y - currentNodePtr->ParentNode->Y;
        int deltaX = currentNodePtr->X - currentNodePtr->ParentNode->X;
        if (0 > deltaY && 0 == deltaX)
            jumpedDir = Up;
        if (0 < deltaY && 0 == deltaX)
            jumpedDir = Down;
        if (0 == deltaY && 0 > deltaX)
            jumpedDir = Left;
        if (0 == deltaY && 0 < deltaX)
            jumpedDir = Right;
        if (0 > deltaY && 0 > deltaX)
            jumpedDir = LeftUp;
        if (0 < deltaY && 0 > deltaX)
            jumpedDir = LeftDown;
        if (0 > deltaY && 0 < deltaX)
            jumpedDir = RightUp;
        if (0 < deltaY && 0 < deltaX)
            jumpedDir = RightDown;

        // 获取当前节点即将要搜寻的方向，jumpDirs
        jumpDirs = getForcedNeighbourDirs(currentNodePtr->X, currentNodePtr->Y, jumpedDir);
    } // end-寻路-while(1)

    // 路径回溯
    Node *pathNodePtr = m_endNode;
    while (true)
    {
        pathNodePtr->IsInPath = true;
        m_pathNodeList.insert(m_pathNodeList.begin(), pathNodePtr);

        pathNodePtr = pathNodePtr->ParentNode;
        if (nullptr == pathNodePtr)
        {
            break;
        }
    }
}

void BitPruneJps::PrintPath()
{
    cout << endl
         << "最短路径：";
    vector<Node *>::const_iterator cIter = m_pathNodeList.cbegin();
    for (; cIter != m_pathNodeList.cend(); cIter++)
    {
        cout << (*cIter)->X << "," << (*cIter)->Y << " ";
    }
    cout << endl;
}

void BitPruneJps::PrintPathMap()
{
    // 打印路线地图
    for (int y = 0; y < m_mapHeight; y++)
    {
        for (int x = 0; x < m_mapWidth; x++)
        {
            if (m_map[y][x].IsInPath)
                cout << "*";
            else
                cout << m_map[y][x].IsObstacle;
        }
        cout << endl;
    }
}

void BitPruneJps::resetMap()
{
    for (int y = 0; y < m_mapHeight; y++)
    {
        for (int x = 0; x < m_mapWidth; x++)
        {
            m_map[y][x].Reset();
        }
    }

    m_openedNodeList.clear();
    m_pathNodeList.clear();
}

void BitPruneJps::deleteMap()
{
    for (int y = 0; y < m_mapHeight; y++)
    {
        delete[] m_map[y];
    }
    delete[] m_map;
    m_map = nullptr;
}

int BitPruneJps::getH(const Node &node)
{
    unsigned int x = abs(long(node.X) - long(m_endNode->X)); // 取水平距离差绝对值
    unsigned int y = abs(long(node.Y) - long(m_endNode->Y)); // 取竖直距离差绝对值
    return (x + y) * 10;
}

vector<DirectionEnum> BitPruneJps::getForcedNeighbourDirs(unsigned int x, unsigned int y,
                                                          DirectionEnum jumpedDir)
{
    vector<DirectionEnum> retDirs;

    char p; // 单元地图中父点
    short unitMap = 0;
    char neighbGroup[9] = {9, 9, 9, 9, 9, 9, 9, 9, 9}; // 单元地图中,要探测的邻居组，初始化为非(0-8)的值，为9的点为不可行点

    switch (jumpedDir)
    {
    case Up:
        p = 7;
        break;
    case Down:
        p = 1;
        break;
    case Left:
        p = 5;
        break;
    case Right:
        p = 3;
        break;
    case LeftUp:
        p = 8;
        break;
    case LeftDown:
        p = 2;
        break;
    case RightUp:
        p = 6;
        break;
    case RightDown:
        p = 0;
        break;
    default:
        break;
    }

    // 获取搜寻点周围3x3单元地图，并找到邻居组
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int yTmp = y + i - 1; // 获取周围的点坐标
            int xTmp = x + j - 1;
            if (m_mapHeight > yTmp && 0 <= yTmp &&
                m_mapWidth > xTmp && 0 <= xTmp)
            { // 确保周围点没超出地图
                int valueT = m_map[yTmp][xTmp].IsObstacle;
                unitMap = unitMap | valueT << (i * 3 + j);
                if (1 != valueT)
                { // 不为障碍
                    neighbGroup[i * 3 + j] = (i * 3 + j);
                }
            }
        }
    } // end-获取搜寻点周围3x3单元地图，并找到邻居组

    // 获取当前搜寻点周围点类型，并赋值探测方向组
    for (int i = 0; i < 9; i++)
    {
        if (9 != neighbGroup[i] &&
            p != neighbGroup[i] &&
            4 != neighbGroup[i])
        { // 如果邻居组中点不为：空(9)、当前搜寻点(4)、父节点
            bool *nodeTyp = getNeighbourType(unitMap, p, neighbGroup[i]);
            if (1 == nodeTyp[0])
            { // 如果存在关键邻居，就向探测方向组中加入当前方向
                if (1 == i)
                    retDirs.push_back(Up);
                if (7 == i)
                    retDirs.push_back(Down);
                if (3 == i)
                    retDirs.push_back(Left);
                if (5 == i)
                    retDirs.push_back(Right);

                if (0 == i)
                    retDirs.push_back(LeftUp);
                if (6 == i)
                    retDirs.push_back(LeftDown);
                if (2 == i)
                    retDirs.push_back(RightUp);
                if (8 == i)
                    retDirs.push_back(RightDown);
            }
        }
    } // end-获取当前搜寻点周围点类型，并赋值探测方向组
    return retDirs;
}

bool *BitPruneJps::getNeighbourType(short unitMap, char p, char n)
{
    static bool retType[2]; // 返回的类型
    char obstaclePos = 0;

    if (p == 0)
    {
        if (n == 7 || n == 5 || n == 8)
        {
            retType[0] = true;
            retType[1] = false;
        }

        if (n == 2)
        {
            obstaclePos = unitMap >> 1 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 6)
        {
            obstaclePos = unitMap >> 3 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }

        if (n == 1 || n == 3)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    if (p == 1)
    {
        if (n == 7)
        {
            retType[0] = true;
            retType[1] = false;
        }
        if (n == 6)
        {
            obstaclePos = unitMap >> 3 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 8)
        {
            obstaclePos = unitMap >> 5 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }

        if (n == 0 || n == 2 || n == 3 || n == 5)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    if (p == 2)
    {
        if (n == 7 || n == 6 || n == 3)
        {
            retType[0] = true;
            retType[1] = false;
        }
        if (n == 0)
        {
            obstaclePos = unitMap >> 1 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 8)
        {
            obstaclePos = unitMap >> 5 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }

        if (n == 1 || n == 5)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    if (p == 3)
    {
        if (n == 5)
        {
            retType[0] = true;
            retType[1] = false;
        }
        if (n == 2)
        {
            obstaclePos = unitMap >> 1 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 8)
        {
            obstaclePos = unitMap >> 7 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 0 || n == 1 || n == 6 || n == 7)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    if (p == 5)
    {
        if (n == 3)
        {
            retType[0] = true;
            retType[1] = false;
        }
        if (n == 0)
        {
            obstaclePos = unitMap >> 1 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 6)
        {
            obstaclePos = unitMap >> 7 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }

        if (n == 1 || n == 2 || n == 7 || n == 8)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    if (p == 6)
    {
        if (n == 1 || n == 2 || n == 5)
        {
            retType[0] = true;
            retType[1] = false;
        }
        if (n == 0)
        {
            obstaclePos = unitMap >> 3 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 8)
        {
            obstaclePos = unitMap >> 7 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }

        if (n == 3 || n == 7)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    if (p == 7)
    {
        if (n == 1)
        {
            retType[0] = true;
            retType[1] = false;
        }
        if (n == 0)
        {
            obstaclePos = unitMap >> 3 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 2)
        {
            obstaclePos = unitMap >> 5 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 3 || n == 5 || n == 6 || n == 8)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    if (p == 8)
    {
        if (n == 0 || n == 1 || n == 3)
        {
            retType[0] = true;
            retType[1] = false;
        }
        if (n == 2)
        {
            obstaclePos = unitMap >> 5 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 6)
        {
            obstaclePos = unitMap >> 7 & 0x01;
            if (1 == obstaclePos)
            {
                retType[0] = true;
                retType[1] = true;
            }
            if (0 == obstaclePos)
            {
                retType[0] = false;
                retType[1] = false;
            }
        }
        if (n == 5 || n == 7)
        {
            retType[0] = false;
            retType[1] = false;
        }
    }

    return retType;
}

BitPruneJps::Node BitPruneJps::JumpStraight(const Node &currenNode, const DirectionEnum &dir)
{
    Node nodeTmp = currenNode;
    Node jumpNode = currenNode;

    unsigned int bitMap = 0;     // 临时节点当前行或列跳点判断位地图
    unsigned int bitMapUp = 0;   // 临时节点上一行跳点判断位地图
    unsigned int bitMapDown = 0; // 临时节点下一行跳点判断位地图
    unsigned int bitMapLeft = 0;
    unsigned int bitMapRight = 0;

    char barrierUpBit = 0;        // 障碍往上一点值
    char barrierDownBit = 0;      // 障碍下一点值
    char barrierLeftBit = 0;      // 障碍左一点值
    char barrierRightBit = 0;     // 障碍右一点值
    char barrierLeftUpBit = 0;    // 障碍左上值
    char barrierLeftDownBit = 0;  // 障碍左下值
    char barrierRightUpBit = 0;   // 障碍右上值
    char barrierRightDownBit = 0; // 障碍右下值

    int n, j;
    // 根据方向初始化开始搜寻的点
    switch (dir)
    {
    case Up:
        nodeTmp.Y = currenNode.Y - 1;
        nodeTmp.X = currenNode.X;
        break;
    case Down:
        nodeTmp.Y = currenNode.Y + 1;
        nodeTmp.X = currenNode.X;
        break;
    case Left:
        nodeTmp.Y = currenNode.Y;
        nodeTmp.X = currenNode.X - 1;
        break;
    case Right:
        nodeTmp.Y = currenNode.Y;
        nodeTmp.X = currenNode.X + 1;
        break;
    default:
        break;
    }

    // 如果搜寻点，是障碍物，或者出了地图，返回空
    if (m_mapHeight <= nodeTmp.Y || 0 > nodeTmp.Y ||
        m_mapWidth <= nodeTmp.X || 0 > nodeTmp.X ||
        m_map[nodeTmp.Y][nodeTmp.X].IsObstacle)
    {
        return Node();
    }

    while (true)
    {
        // 清空位地图
        bitMapUp = 0;
        bitMap = 0;
        bitMapDown = 0;
        bitMapLeft = 0;
        bitMapRight = 0;
        // 清空障碍物周围点值
        barrierUpBit = 0;        // 障碍往上一点值
        barrierDownBit = 0;      // 障碍下一点值
        barrierLeftBit = 0;      // 障碍左一点值
        barrierRightBit = 0;     // 障碍右一点值
        barrierLeftUpBit = 0;    // 障碍左上值
        barrierLeftDownBit = 0;  // 障碍左下值
        barrierRightUpBit = 0;   // 障碍右上值
        barrierRightDownBit = 0; // 障碍右下值

        // 如果临时节点不在地图中，或为障碍点，则说明此次搜寻没有跳点，直接返回空节点；
        if (m_mapHeight <= nodeTmp.Y || 0 > nodeTmp.Y ||
            m_mapWidth <= nodeTmp.X || 0 > nodeTmp.X ||
            m_map[nodeTmp.Y][nodeTmp.X].IsObstacle)
        {
            return Node();
        }

        if (Right == dir)
        {
            // 建立位地图
            for (int i = 0; i < 32; i++)
            {
                if (0 <= nodeTmp.Y - 1 && m_mapHeight > nodeTmp.Y + 1)
                { // 没超出辅助节点地图
                    bitMapUp = bitMapUp << 1 | m_map[nodeTmp.Y - 1][nodeTmp.X + i].IsObstacle;
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y - 0][nodeTmp.X + i].IsObstacle;
                    bitMapDown = bitMapDown << 1 | m_map[nodeTmp.Y + 1][nodeTmp.X + i].IsObstacle;
                }
                if (0 > nodeTmp.Y + 1 || m_mapHeight <= nodeTmp.Y - 1)
                { // 三行全超出地图，全置1，等价为障碍
                    bitMapUp = bitMapUp << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | 1;
                }
                if (0 == nodeTmp.Y + 1)
                { // 上两行超出地图，上两行置1，等价为障碍
                    bitMapUp = bitMapUp << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | m_map[nodeTmp.Y + 1][nodeTmp.X + i].IsObstacle;
                }
                if (m_mapHeight - 1 == nodeTmp.Y - 1)
                { // 下两行超出地图，下两行置1，等价为障碍
                    bitMapUp = bitMapUp << 1 | m_map[nodeTmp.Y - 1][nodeTmp.X + i].IsObstacle;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | 1;
                }
                if (0 == nodeTmp.Y || m_mapHeight - 1 == nodeTmp.Y)
                { // 中间行在地图上或下边界上
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y - 0][nodeTmp.X + i].IsObstacle;
                    if (-1 == nodeTmp.Y - 1)
                        bitMapUp = bitMapUp << 1 | 1; // 如果上一行是上边界的上一行，置1
                    else
                        bitMapUp = bitMapUp << 1 | m_map[nodeTmp.Y - 1][nodeTmp.X + i].IsObstacle;
                    if (m_mapHeight == nodeTmp.Y + 1)
                        bitMapDown = bitMapDown << 1 | 1; // 如果下一行是下边界的下一行，置1
                    else
                        bitMapDown = bitMapDown << 1 | m_map[nodeTmp.Y + 1][nodeTmp.X + i].IsObstacle;
                }
            } // end - 建立位地图
            // bit map test
            // cout<<endl<<"bit map test:"<<bitMapUp<<" "<<bitMap<<" "<<bitMapDown<<endl;
            // cout<<""<<currenNode.Y<<","<<currenNode.X<<endl;
            // cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierUpBit = bitMapUp >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierLeftUpBit = bitMapUp >> (32 - n - 1 + 1) & 1;
            barrierDownBit = bitMapDown >> (32 - n - 1) & 1;
            barrierLeftDownBit = bitMapDown >> (32 - n - 1 + 1) & 1;
            j = __builtin_clz(((bitMapUp >> 1) & (~bitMapUp)) | ((bitMapDown >> 1) & (~bitMapDown)));
            if (31 == n && 31 == j)
            { // 如果距离当前1-32格内没找到跳点，就切换到第30格
                nodeTmp.Y = nodeTmp.Y;
                nodeTmp.X = nodeTmp.X + 30;
            }
            else
            {
                if (n <= j)
                {
                    if ((1 == barrierLeftUpBit || 1 == barrierUpBit) &&
                        (1 == barrierLeftDownBit || 1 == barrierDownBit))
                        return Node(); // 如果该障碍点的(左上 ||上) && (左下 ||下)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.Y = nodeTmp.Y;
                    jumpNode.X = nodeTmp.X + n - 1;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y;
                    jumpNode.X = nodeTmp.X + j - 1;
                    return jumpNode;
                }
            }
        }
        if (Left == dir)
        {
            // 建立位地图
            for (int i = 0; i < 32; i++)
            {
                if (0 <= nodeTmp.Y - 1 && m_mapHeight > nodeTmp.Y + 1)
                { // 没超出辅助节点地图
                    bitMapUp = bitMapUp << 1 | m_map[nodeTmp.Y - 1][nodeTmp.X - i].IsObstacle;
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y - 0][nodeTmp.X - i].IsObstacle;
                    bitMapDown = bitMapDown << 1 | m_map[nodeTmp.Y + 1][nodeTmp.X - i].IsObstacle;
                }
                if (0 > nodeTmp.Y + 1 || m_mapHeight <= nodeTmp.Y - 1)
                { // 三行全超出地图，全置1，等价为障碍
                    bitMapUp = bitMapUp << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | 1;
                }
                if (0 == nodeTmp.Y + 1)
                { // 上两行超出地图，上两行置1，等价为障碍
                    bitMapUp = bitMapUp << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | m_map[nodeTmp.Y + 1][nodeTmp.X - i].IsObstacle;
                }
                if (m_mapHeight - 1 == nodeTmp.Y - 1)
                { // 下两行超出地图，下两行置1，等价为障碍
                    bitMapUp = bitMapUp << 1 | m_map[nodeTmp.Y - 1][nodeTmp.X - i].IsObstacle;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | 1;
                }
                if (0 == nodeTmp.Y || m_mapHeight - 1 == nodeTmp.Y)
                { // 中间行在地图上或下边界上
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y - 0][nodeTmp.X - i].IsObstacle;
                    if (-1 == nodeTmp.Y - 1)
                        bitMapUp = bitMapUp << 1 | 1; // 如果上一行是上边界的上一行，置1
                    else
                        bitMapUp = bitMapUp << 1 | m_map[nodeTmp.Y - 1][nodeTmp.X - i].IsObstacle;
                    if (m_mapHeight == nodeTmp.Y + 1)
                        bitMapDown = bitMapDown << 1 | 1; // 如果下一行是下边界的下一行，置1
                    else
                        bitMapDown = bitMapDown << 1 | m_map[nodeTmp.Y + 1][nodeTmp.X - i].IsObstacle;
                }
            } // end - 建立位地图
            // bit map test
            // cout<<endl<<"bit map test:"<<bitMapUp<<" "<<bitMap<<" "<<bitMapDown<<endl;
            // cout<<""<<currenNode.Y<<","<<currenNode.X<<endl;
            // cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierUpBit = bitMapUp >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierRightUpBit = bitMapUp >> (32 - n - 1 + 1) & 1;
            barrierDownBit = bitMapDown >> (32 - n - 1) & 1;
            barrierRightDownBit = bitMapDown >> (32 - n - 1 + 1) & 1;
            j = __builtin_clz(((bitMapUp >> 1) & (~bitMapUp)) | ((bitMapDown >> 1) & (~bitMapDown)));
            if (31 == n && 31 == j)
            { // 如果距离当前1-32格内没找到跳点，就切换到第30格
                nodeTmp.Y = nodeTmp.Y;
                nodeTmp.X = nodeTmp.X - 30;
            }
            else
            {
                if (n <= j)
                {
                    if ((1 == barrierRightUpBit || 1 == barrierUpBit) &&
                        (1 == barrierRightDownBit || 1 == barrierDownBit))
                        return Node(); // 如果该障碍点的(右上 ||上) && (右下 ||下)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.Y = nodeTmp.Y;
                    jumpNode.X = nodeTmp.X - n + 1;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y;
                    jumpNode.X = nodeTmp.X - j + 1;
                    return jumpNode;
                }
            }

        } // end-if(p_left == dir)
        if (Down == dir)
        {
            // 建立位地图
            for (int i = 0; i < 32; i++)
            {
                if (0 <= nodeTmp.X - 1 && m_mapWidth > nodeTmp.X + 1)
                { // 没超出辅助节点地图
                    bitMapLeft = bitMapLeft << 1 | m_map[nodeTmp.Y + i][nodeTmp.X - 1].IsObstacle;
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y + i][nodeTmp.X + 0].IsObstacle;
                    bitMapRight = bitMapRight << 1 | m_map[nodeTmp.Y + i][nodeTmp.X + 1].IsObstacle;
                }
                if (0 > nodeTmp.X + 1 || m_mapWidth <= nodeTmp.X - 1)
                { // 三列全超出地图，全置1，等价为障碍
                    bitMapLeft = bitMapLeft << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | 1;
                }
                if (0 == nodeTmp.X + 1)
                { // 左两列超出地图，左两行置1，等价为障碍
                    bitMapLeft = bitMapLeft << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | m_map[nodeTmp.Y + i][nodeTmp.X + 1].IsObstacle;
                }
                if (m_mapWidth - 1 == nodeTmp.X - 1)
                { // 右两列超出地图，右两行置1，等价为障碍
                    bitMapLeft = bitMapLeft << 1 | m_map[nodeTmp.Y + i][nodeTmp.X - 1].IsObstacle;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | 1;
                }
                if (0 == nodeTmp.X || m_mapWidth - 1 == nodeTmp.X)
                { // 中间列在地图左或右边界上
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y + i][nodeTmp.X].IsObstacle;
                    if (-1 == nodeTmp.X - 1)
                        bitMapLeft = bitMapLeft << 1 | 1; // 如果左一列是左边界的左一列，置1
                    else
                        bitMapLeft = bitMapLeft << 1 | m_map[nodeTmp.Y + i][nodeTmp.X - 1].IsObstacle;
                    if (m_mapWidth == nodeTmp.X + 1)
                        bitMapRight = bitMapRight << 1 | 1; // 如果右一列是右边界的右一列，置1
                    else
                        bitMapRight = bitMapRight << 1 | m_map[nodeTmp.Y + i][nodeTmp.X + 1].IsObstacle;
                }
            } // end - 建立位地图
            // bit map test
            // cout<<endl<<"bit map test:"<<bitMapLeft<<" "<<bitMap<<" "<<bitMapRight<<endl;
            // cout<<""<<currenNode.Y<<","<<currenNode.X<<endl;
            // cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierLeftBit = bitMapLeft >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierLeftUpBit = bitMapLeft >> (32 - n - 1 + 1) & 1;
            barrierRightBit = bitMapRight >> (32 - n - 1) & 1;
            barrierRightUpBit = bitMapRight >> (32 - n - 1 + 1) & 1;

            j = __builtin_clz(((bitMapLeft >> 1) & (~bitMapLeft)) | ((bitMapRight >> 1) & (~bitMapRight)));
            if (31 == n && 31 == j)
            { //__builtin_clz( (unsigned int)0 ) 的值为31
                nodeTmp.Y = nodeTmp.Y + 30;
                nodeTmp.X = nodeTmp.X;
            }
            else
            {
                if (n <= j)
                {
                    if ((1 == barrierLeftUpBit || 1 == barrierLeftBit) &&
                        (1 == barrierRightUpBit || 1 == barrierRightBit))
                        return Node(); // 如果该障碍点的(左上 ||左) && (右上 ||右)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.Y = nodeTmp.Y + n - 1;
                    jumpNode.X = nodeTmp.X;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y + j - 1;
                    jumpNode.X = nodeTmp.X;
                    return jumpNode;
                }
            }
        } // end-if(p_down == dir)
        if (Up == dir)
        {
            // 建立位地图
            for (int i = 0; i < 32; i++)
            {
                if (0 <= nodeTmp.X - 1 && m_mapWidth > nodeTmp.X + 1)
                { // 当前列没超出辅助节点地图
                    bitMapLeft = bitMapLeft << 1 | m_map[nodeTmp.Y - i][nodeTmp.X - 1].IsObstacle;
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y - i][nodeTmp.X + 0].IsObstacle;
                    bitMapRight = bitMapRight << 1 | m_map[nodeTmp.Y - i][nodeTmp.X + 1].IsObstacle;
                }
                if (0 > nodeTmp.X + 1 || m_mapWidth <= nodeTmp.X - 1)
                { // 三列全超出地图，全置1，等价为障碍
                    bitMapLeft = bitMapLeft << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | 1;
                }
                if (0 == nodeTmp.X + 1)
                { // 左两列超出地图，左两列置1，等价为障碍
                    bitMapLeft = bitMapLeft << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | m_map[nodeTmp.Y - i][nodeTmp.X + 1].IsObstacle;
                }
                if (m_mapWidth - 1 == nodeTmp.X - 1)
                { // 右两列超出地图，右两列置1，等价为障碍
                    bitMapLeft = bitMapLeft << 1 | m_map[nodeTmp.Y - i][nodeTmp.X - 1].IsObstacle;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | 1;
                }
                if (0 == nodeTmp.X || m_mapWidth - 1 == nodeTmp.X)
                { // 中间列在地图左或右边界上
                    bitMap = bitMap << 1 | m_map[nodeTmp.Y - i][nodeTmp.X].IsObstacle;
                    if (-1 == nodeTmp.X - 1)
                        bitMapLeft = bitMapLeft << 1 | 1; // 如果左一列是左边界的左一列，置1
                    else
                        bitMapLeft = bitMapLeft << 1 | m_map[nodeTmp.Y - i][nodeTmp.X - 1].IsObstacle;
                    if (m_mapWidth == nodeTmp.X + 1)
                        bitMapRight = bitMapRight << 1 | 1; // 如果右一列是右边界的右一列，置1
                    else
                        bitMapRight = bitMapRight << 1 | m_map[nodeTmp.Y - i][nodeTmp.X + 1].IsObstacle;
                }
            } // end - 建立位地图
            // bit map test
            // cout<<endl<<"bit map test:"<<bitMapLeft<<" "<<bitMap<<" "<<bitMapRight<<endl;
            // cout<<""<<currenNode.Y<<","<<currenNode.X<<endl;
            // cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierLeftBit = bitMapLeft >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierLeftDownBit = bitMapLeft >> (32 - n - 1 + 1) & 1;
            barrierRightBit = bitMapRight >> (32 - n - 1) & 1;
            barrierRightDownBit = bitMapRight >> (32 - n - 1 + 1) & 1;

            j = __builtin_clz(((bitMapLeft >> 1) & (~bitMapLeft)) | ((bitMapRight >> 1) & (~bitMapRight)));
            if (31 == n && 31 == j)
            { //__builtin_clz( (unsigned int)0 ) 的值为31
                nodeTmp.Y = nodeTmp.Y + 30;
                nodeTmp.X = nodeTmp.X;
            }
            else
            {
                if (n <= j)
                {
                    if ((1 == barrierLeftDownBit || 1 == barrierLeftBit) &&
                        (1 == barrierRightDownBit || 1 == barrierRightBit))
                        return Node(); // 如果该障碍点的(左上 ||左) && (右上 ||右)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.Y = nodeTmp.Y - n + 1;
                    jumpNode.X = nodeTmp.X;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y - j + 1;
                    jumpNode.X = nodeTmp.X;
                    return jumpNode;
                }
            }
        } // end-if(p_up == dir)
    }

    return Node();
}

BitPruneJps::Node *BitPruneJps::JumpOblique(const Node &currenNode, const DirectionEnum &dir)
{
    static Node retNodeArray[2];

    int deltaX = 0;
    int deltaY = 0;
    DirectionEnum straightDirs[2] = {Up, Up};
    switch (dir)
    {
    case LeftUp:
        deltaX = -1;
        deltaY = -1;
        straightDirs[0] = Left;
        straightDirs[1] = Up;
        break;
    case LeftDown:
        deltaX = -1;
        deltaY = 1;
        straightDirs[0] = Left;
        straightDirs[1] = Down;
        break;
    case RightUp:
        deltaX = 1;
        deltaY = -1;
        straightDirs[0] = Right;
        straightDirs[1] = Up;
        break;
    case RightDown:
        deltaX = 1;
        deltaY = 1;
        straightDirs[0] = Right;
        straightDirs[1] = Down;
        break;
    default:
        break;
    }

    Node nodeTmp = currenNode; // 沿指定方向搜寻的点
    // 沿指定方向搜寻，直到找到跳点，或碰到障碍物，或超出地图
    while (true)
    {
        for (int i = 0; i < 2; i++)
        { // 初始化retnode，置空节点
            retNodeArray[i] = Node();
        }
        nodeTmp.Y += deltaY;
        nodeTmp.X += deltaX;
        // cout<<"斜跳跃："<<nodeTmp.row<<","<<nodeTmp.col<<endl;
        // 如果搜寻到终点就返回
        if (nodeTmp.Y == m_endNode->Y &&
            nodeTmp.X == m_endNode->X)
        {
            retNodeArray[0] = nodeTmp;
            break;
        }
        // 如果搜寻点，是障碍物，或者出了地图，返回空
        if (m_mapHeight <= nodeTmp.Y || 0 > nodeTmp.Y ||
            m_mapWidth <= nodeTmp.X || 0 > nodeTmp.X ||
            m_map[nodeTmp.Y][nodeTmp.X].IsObstacle)
        {
            break;
        }

        const vector<DirectionEnum> forcedNeighbourDirs =
            getForcedNeighbourDirs(nodeTmp.X, nodeTmp.Y, dir);
        if (forcedNeighbourDirs.size())
        {
            // 如果存在强迫邻居，返回当前搜寻点
            retNodeArray[0] = nodeTmp;
            break;
        }

        // 往当前点垂直水平分量方向做直跳跃，如果不反回空，返回当前点
        Node jumpNode; // 用于保存直跳跃的返回节点
        for (int i = 0; i < 2; i++)
        {
            jumpNode = JumpStraight(nodeTmp, straightDirs[i]); //----------------修改
            if (false == jumpNode.IsNull)
            {
                retNodeArray[0] = nodeTmp;
                retNodeArray[0].IsMidJumpNode = true;
                retNodeArray[1] = jumpNode;
                break;
            }
        }
    }

    return retNodeArray;
}
