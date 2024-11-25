
#include "BitPruneJps.h"

int builtinClz(unsigned int x)
{
    if (0 == x)
    {
        return 31;
    }
    return __builtin_clz(x);
}

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

void BitPruneJps::FindPath(int beginX, int beginY, int endX, int endY)
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
            Node jumpNodes[2] = {};
            vector<Node> jumpNodeList;
            if (*(dirsIt) == Up || *(dirsIt) == Down || *(dirsIt) == Left || *(dirsIt) == Right)
            {
                Node node = jumpStraight(nodeTmp, (*dirsIt));
                jumpNodeList.push_back(node);
            }
            if (*(dirsIt) == LeftUp || *(dirsIt) == LeftDown || *(dirsIt) == RightUp || *(dirsIt) == RightDown)
            {
                jumpNodeList = jumpOblique(nodeTmp, (*dirsIt));
            }

            // 从返回的节点中，搜寻跳点，并更新g、h、f值
            for (Node jumpNode : jumpNodeList)
            {
                // 如果返回的是有效节点，且不在关闭列表中（未找过）
                if (false == jumpNode.IsNull &&
                    false == m_map[jumpNode.Y][jumpNode.X].IsInClosedList && // 不在关闭列表中（未找过）
                    false == jumpNode.IsMidJumpNode                          // 不是中间跳点
                )
                {
                    Node &nodeTmpInMap = m_map[nodeTmp.Y][nodeTmp.X];
                    jumpNode.G = nodeTmpInMap.G + getDis(nodeTmpInMap, jumpNode);
                    // 如果该点已经在开放列表中，比较g值，取g值较小的点的属性，并不用再次加入开放列表
                    Node &jumpNodeInMap = m_map[jumpNode.Y][jumpNode.X];
                    if (jumpNodeInMap.IsInOpenedList)
                    {
                        if (jumpNodeInMap.G > jumpNode.G)
                        {
                            jumpNodeInMap.G = jumpNode.G;
                            jumpNodeInMap.UpdateF();
                            jumpNodeInMap.ParentNode = &nodeTmpInMap;
                        }
                    }
                    // 如果不在开放列表中
                    if (false == jumpNodeInMap.IsInOpenedList)
                    {
                        // 更新辅助地图中对应探测点的节点属性
                        jumpNodeInMap.G = jumpNode.G;
                        jumpNodeInMap.H = getH(jumpNodeInMap);
                        jumpNodeInMap.UpdateF();
                        jumpNodeInMap.ParentNode = &nodeTmpInMap;
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
            // cout<< (*it)->X <<","<< (*it)->Y <<endl;
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

        // 获取当前节点即将要搜寻的方向，jumpDirs
        jumpDirs = DirectionEnums;
        DirectionEnum jumpDirNeedRm;
        int deltaY = currentNodePtr->Y - currentNodePtr->ParentNode->Y;
        int deltaX = currentNodePtr->X - currentNodePtr->ParentNode->X;
        if (0 > deltaY && 0 == deltaX)
            jumpDirNeedRm = Down;
        if (0 < deltaY && 0 == deltaX)
        {
            jumpDirNeedRm = Up;
        }
        if (0 == deltaY && 0 > deltaX)
        {
            jumpDirNeedRm = Right;
        }
        if (0 == deltaY && 0 < deltaX)
        {
            jumpDirNeedRm = Left;
        }
        if (0 > deltaY && 0 > deltaX)
        {
            jumpDirNeedRm = RightDown;
        }
        if (0 < deltaY && 0 > deltaX)
        {
            jumpDirNeedRm = RightUp;
        }
        if (0 > deltaY && 0 < deltaX)
        {
            jumpDirNeedRm = LeftDown;
        }
        if (0 < deltaY && 0 < deltaX)
        {
            jumpDirNeedRm = LeftUp;
        }
        vector<DirectionEnum>::iterator dirIterNeedRm = std::find_if(jumpDirs.begin(), jumpDirs.end(), [jumpDirNeedRm](DirectionEnum dir) -> bool
                                                                     { return jumpDirNeedRm == dir; });
        jumpDirs.erase(dirIterNeedRm);
    } // end-寻路-while(1)

    // 剪枝路径回溯
    Node *pathNodePtr = m_endNode;
    while (true)
    {
        pathNodePtr->IsInPath = true;
        m_prunedPathNodeList.insert(m_prunedPathNodeList.begin(), pathNodePtr);

        pathNodePtr = pathNodePtr->ParentNode;
        if (nullptr == pathNodePtr)
        {
            break;
        }
    }

    // 完整路径回溯
    fillPath();
    pathNodePtr = m_endNode;
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
    m_prunedPathNodeList.clear();
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

int BitPruneJps::getDis(const Node &startNode, const Node &endNode)
{
    int dis = sqrt(pow(long(endNode.X) - long(startNode.X), 2) + pow(long(endNode.Y) - long(startNode.Y), 2)) * 10; // pow次方函数
    return dis;
}

int BitPruneJps::getH(const Node &node)
{
    int x = abs(node.X - m_endNode->X); // 取水平距离差绝对值
    int y = abs(node.Y - m_endNode->Y); // 取竖直距离差绝对值
    return (x + y) * 10;
}

BitPruneJps::Node BitPruneJps::jumpStraight(const Node &currenNode, const DirectionEnum &dir)
{
    Node nodeTmp = currenNode;
    Node jumpNode;

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
                // 如果待构建的位地图点水平位置超出了地图， 则位值置1
                if ((nodeTmp.X + i) >= m_mapWidth)
                {
                    bitMapUp = bitMapUp << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | 1;
                    continue;
                }
                if (0 <= nodeTmp.Y - 1 && m_mapHeight > nodeTmp.Y + 1)
                { // 三行没超出辅助节点地图
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

            n = builtinClz(bitMap);
            barrierUpBit = bitMapUp >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierLeftUpBit = bitMapUp >> (32 - n - 1 + 1) & 1;
            barrierDownBit = bitMapDown >> (32 - n - 1) & 1;
            barrierLeftDownBit = bitMapDown >> (32 - n - 1 + 1) & 1;
            j = builtinClz(((bitMapUp >> 1) & (~bitMapUp)) | ((bitMapDown >> 1) & (~bitMapDown)));
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
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y;
                    jumpNode.X = nodeTmp.X + j - 1;
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
            }
        }
        if (Left == dir)
        {
            // 建立位地图
            for (int i = 0; i < 32; i++)
            {
                // 如果待构建的位地图点水平位置超出了地图， 则位值置1
                if ((nodeTmp.X - i) < 0)
                {
                    bitMapUp = bitMapUp << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapDown = bitMapDown << 1 | 1;
                    continue;
                }
                if (0 <= nodeTmp.Y - 1 && m_mapHeight > nodeTmp.Y + 1)
                { // 三行没超出辅助节点地图
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

            n = builtinClz(bitMap);
            barrierUpBit = bitMapUp >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierRightUpBit = bitMapUp >> (32 - n - 1 + 1) & 1;
            barrierDownBit = bitMapDown >> (32 - n - 1) & 1;
            barrierRightDownBit = bitMapDown >> (32 - n - 1 + 1) & 1;
            j = builtinClz(((bitMapUp >> 1) & (~bitMapUp)) | ((bitMapDown >> 1) & (~bitMapDown)));
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
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y;
                    jumpNode.X = nodeTmp.X - j + 1;
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
            }

        } // end-if(p_left == dir)
        if (Down == dir)
        {
            // 建立位地图
            for (int i = 0; i < 32; i++)
            {
                // 如果待构建的位地图点垂直位置超出了地图， 则位值置1
                if ((nodeTmp.Y + i) >= m_mapHeight)
                {
                    bitMapLeft = bitMapLeft << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | 1;
                    continue;
                }
                if (0 <= nodeTmp.X - 1 && m_mapWidth > nodeTmp.X + 1)
                { // 三列没超出辅助节点地图
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

            n = builtinClz(bitMap);
            barrierLeftBit = bitMapLeft >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierLeftUpBit = bitMapLeft >> (32 - n - 1 + 1) & 1;
            barrierRightBit = bitMapRight >> (32 - n - 1) & 1;
            barrierRightUpBit = bitMapRight >> (32 - n - 1 + 1) & 1;

            j = builtinClz(((bitMapLeft >> 1) & (~bitMapLeft)) | ((bitMapRight >> 1) & (~bitMapRight)));
            if (31 == n && 31 == j)
            { // builtinClz( (unsigned int)0 ) 的值为31
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
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y + j - 1;
                    jumpNode.X = nodeTmp.X;
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
            }
        } // end-if(p_down == dir)
        if (Up == dir)
        {
            // 建立位地图
            for (int i = 0; i < 32; i++)
            {
                // 如果待构建的位地图点垂直位置超出了地图， 则位值置1
                if ((nodeTmp.Y - i) < 0)
                {
                    bitMapLeft = bitMapLeft << 1 | 1;
                    bitMap = bitMap << 1 | 1;
                    bitMapRight = bitMapRight << 1 | 1;
                    continue;
                }
                if (0 <= nodeTmp.X - 1 && m_mapWidth > nodeTmp.X + 1)
                { // 三列当前列没超出辅助节点地图
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

            n = builtinClz(bitMap);
            barrierLeftBit = bitMapLeft >> (32 - n - 1) & 1; // 从位地图中得到障碍周围点值
            barrierLeftDownBit = bitMapLeft >> (32 - n - 1 + 1) & 1;
            barrierRightBit = bitMapRight >> (32 - n - 1) & 1;
            barrierRightDownBit = bitMapRight >> (32 - n - 1 + 1) & 1;

            j = builtinClz(((bitMapLeft >> 1) & (~bitMapLeft)) | ((bitMapRight >> 1) & (~bitMapRight)));
            if (31 == n && 31 == j)
            { // builtinClz( (unsigned int)0 ) 的值为31
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
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
                if (n > j)
                {
                    jumpNode.Y = nodeTmp.Y - j + 1;
                    jumpNode.X = nodeTmp.X;
                    jumpNode.IsNull = false;
                    return jumpNode;
                }
            }
        } // end-if(p_up == dir)
    }

    return Node();
}

vector<BitPruneJps::Node> BitPruneJps::jumpOblique(const Node &currenNode, const DirectionEnum &dir)
{
    vector<Node> retNodeList;

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

        nodeTmp.Y += deltaY;
        nodeTmp.X += deltaX;
        // 如果搜寻点，是障碍物，或者出了地图，返回空
        if (m_mapHeight <= nodeTmp.Y || 0 > nodeTmp.Y ||
            m_mapWidth <= nodeTmp.X || 0 > nodeTmp.X ||
            m_map[nodeTmp.Y][nodeTmp.X].IsObstacle)
        {
            break;
        }

        // 往当前点垂直水平分量方向做直跳跃，如果不反回空，返回当前点
        Node jumpNode; // 用于保存直跳跃的返回节点
        for (int i = 0; i < 2; i++)
        {
            jumpNode = jumpStraight(nodeTmp, straightDirs[i]); //----------------修改
            if (false == jumpNode.IsNull)
            {
                retNodeList.push_back(jumpNode);
            }
        }
    }

    return retNodeList;
}

void BitPruneJps::fillNode(Node *const nodePtr)
{
    if (!nodePtr->ParentNode)
    {
        return;
    }
    BitPruneJps::Node *srcNodePtr = nodePtr->ParentNode;
    int childNodeTmpX = srcNodePtr->X;
    int childNodeTmpY = srcNodePtr->Y;
    BitPruneJps::Node *destNodePtr = nodePtr;
    // 从父节点开始检索
    BitPruneJps::Node *indexNodePtr = srcNodePtr;
    while (true)
    {
        if (indexNodePtr == destNodePtr)
        {
            break;
        }

        int deltaY = destNodePtr->Y - indexNodePtr->Y;
        int deltaX = destNodePtr->X - indexNodePtr->X;
        if (0 > deltaY && 0 == deltaX)
        {
            childNodeTmpY = childNodeTmpY - 1;
        }
        if (0 < deltaY && 0 == deltaX)
        {
            childNodeTmpY = childNodeTmpY + 1;
        }
        if (0 == deltaY && 0 > deltaX)
        {
            childNodeTmpX = childNodeTmpX - 1;
        }
        if (0 == deltaY && 0 < deltaX)
        {
            childNodeTmpX = childNodeTmpX + 1;
        }
        if (0 > deltaY && 0 > deltaX)
        {
            childNodeTmpX = childNodeTmpX - 1;
            childNodeTmpY = childNodeTmpY - 1;
        }
        if (0 < deltaY && 0 > deltaX)
        {
            childNodeTmpX = childNodeTmpX - 1;
            childNodeTmpY = childNodeTmpY + 1;
        }
        if (0 > deltaY && 0 < deltaX)
        {
            childNodeTmpX = childNodeTmpX + 1;
            childNodeTmpY = childNodeTmpY - 1;
        }
        if (0 < deltaY && 0 < deltaX)
        {
            childNodeTmpX = childNodeTmpX + 1;
            childNodeTmpY = childNodeTmpY + 1;
        }

        m_map[childNodeTmpY][childNodeTmpX].ParentNode = indexNodePtr;
        indexNodePtr = &m_map[childNodeTmpY][childNodeTmpX];
    }
}

void BitPruneJps::fillPath()
{
    Node *nodePtr = m_endNode;
    while (true)
    {
        Node *parentNodePtr = nodePtr->ParentNode;
        if (nullptr == parentNodePtr)
        {
            break;
        }

        fillNode(nodePtr);
        nodePtr = parentNodePtr;
    }
}
