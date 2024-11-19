#include "AStar.h"

AStar::AStar()
    : m_map(nullptr), m_mapWidth(0), m_mapHeight(0), m_beginNode(nullptr), m_endNode(nullptr)
{
}

AStar::~AStar()
{
    // delete m_map
    if (m_map) {
        deleteMap();
    }
}

void AStar::InitMap(int **map, unsigned int width, unsigned int height)
{
    if (m_map) {
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

void AStar::FindPath(unsigned int beginX, unsigned int beginY, unsigned int endX, unsigned int endY)
{
    resetMap();
    m_beginNode = &m_map[beginY][beginX];
    m_endNode = &m_map[endY][endX];

    m_beginNode->IsInClosedList = true;
    Node exploringNode = *m_beginNode;
    while (true)
    {
        // 找出探路点周围8个可行点，保存到开放列表
        updateOpenedNodeList(exploringNode);

        if (m_openedNodeList.size() == 0)
        {
            break;
        }

        // 找出当前点周围最小f值可行点
        vector<Node *>::const_iterator cIter = m_openedNodeList.cbegin();
        vector<Node *>::const_iterator minFNodeConstIter = cIter;
        // cout << endl
        //  << "在开放列表中的点: ";
        for (; cIter != m_openedNodeList.cend(); cIter++)
        {
            // cout << (*cIter)->X << "," << (*cIter)->Y << " ";
            if ((*minFNodeConstIter)->F > (*cIter)->F)
            {
                minFNodeConstIter = cIter;
            }
        }

        // 换层
        Node *minFNodePtr = *minFNodeConstIter;
        if (minFNodePtr->X == m_endNode->X &&
            minFNodePtr->Y == m_endNode->Y)
        {
            break;
        }
        exploringNode = *minFNodePtr;
        minFNodePtr->IsInClosedList = true;
        // 将探路点从开放列表中移除
        minFNodePtr->IsInOpenedList = false;
        m_openedNodeList.erase(minFNodeConstIter);
    } // end--while(1)寻路

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

void AStar::PrintPath()
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

void AStar::PrintPathMap()
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

void AStar::resetMap()
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

bool AStar::isObstacle(const Node &node)
{
    return whetherPosHasObstacle(node.X, node.Y);
}

bool AStar::whetherPosHasObstacle(unsigned int x, unsigned int y)
{
    if (x < 0 || x >= m_mapWidth ||
        y < 0 || y >= m_mapHeight) // 超出地图
        return true;
    if (m_map[y][x].IsObstacle) // 该点为障碍
        return true;
    return false;
}

int AStar::getH(const Node &node)
{
    unsigned int x = abs(long(node.X) - long(m_endNode->X)); // 取水平距离差绝对值
    unsigned int y = abs(long(node.Y) - long(m_endNode->Y)); // 取竖直距离差绝对值
    return (x + y) * VerticalDist;
}

void AStar::updateOpenedNodeList(const Node &exploringNode)
{
    Node nextExploringNode;
    // 找出探路点周围8个可行点，保存到开放列表
    for (DirectionEnum dir : DirectionEnums)
    {
        bool canWalkToNextNode = true;   // 是否可行走至下一探路点
        unsigned int gIncreaseValue = 0; // g值增加量
        switch (dir)
        {
        case Up:
        {
            // nextExploringNode = &m_map[exploringNode->Y - 1][exploringNode->X]
            nextExploringNode.X = exploringNode.X;
            nextExploringNode.Y = exploringNode.Y - 1; // 只有行减1
            gIncreaseValue = VerticalDist;
            break;
        }
        case Down:
        {
            nextExploringNode.X = exploringNode.X;
            nextExploringNode.Y = exploringNode.Y + 1; // 只有行加1
            gIncreaseValue = VerticalDist;
            break;
        }
        case Left:
        {
            nextExploringNode.X = exploringNode.X - 1; // 行不变，列减1
            nextExploringNode.Y = exploringNode.Y;
            gIncreaseValue = VerticalDist;
            break;
        }
        case Right:
        {
            nextExploringNode.X = exploringNode.X + 1; // 行不变，列加1
            nextExploringNode.Y = exploringNode.Y;
            gIncreaseValue = VerticalDist;
            break;
        }
        case LeftUp:
        {
            if (whetherPosHasObstacle(exploringNode.X, exploringNode.Y - 1) || // 判断当前点上边点是否为障碍
                whetherPosHasObstacle(exploringNode.X - 1, exploringNode.Y)    // 判断当前点左边点是否为障碍
            )
            {
                canWalkToNextNode = false;
                break;
            }
            nextExploringNode.X = exploringNode.X - 1;
            nextExploringNode.Y = exploringNode.Y - 1;
            gIncreaseValue = ObliqueDist;
            break;
        }
        case LeftDown:
        {
            if (whetherPosHasObstacle(exploringNode.X, exploringNode.Y + 1) || // 判断当前点下边点是否为障碍
                whetherPosHasObstacle(exploringNode.X - 1, exploringNode.Y)    // 判断当前点左边点是否为障碍
            )
            {
                canWalkToNextNode = false;
                break;
            }
            nextExploringNode.X = exploringNode.X - 1;
            nextExploringNode.Y = exploringNode.Y + 1;
            gIncreaseValue = ObliqueDist;
            break;
        }
        case RightUp:
        {
            if (whetherPosHasObstacle(exploringNode.X, exploringNode.Y - 1) || // 判断当前点上边点是否为障碍
                whetherPosHasObstacle(exploringNode.X + 1, exploringNode.Y)    // 判断当前点右边点是否为障碍
            )
            {
                canWalkToNextNode = false;
                break;
            }
            nextExploringNode.X = exploringNode.X + 1;
            nextExploringNode.Y = exploringNode.Y - 1;
            gIncreaseValue = ObliqueDist;
            break;
        }
        case RightDown:
        {
            if (whetherPosHasObstacle(exploringNode.X, exploringNode.Y + 1) || // 判断当前点下边点是否为障碍
                whetherPosHasObstacle(exploringNode.X + 1, exploringNode.Y)    // 判断当前点右边点是否为障碍
            )
            {
                canWalkToNextNode = false;
                break;
            }
            nextExploringNode.X = exploringNode.X + 1;
            nextExploringNode.Y = exploringNode.Y + 1;
            gIncreaseValue = ObliqueDist;
            break;
        }
        }
        // 如果下一个探路点为障碍点，则视为不可行走到
        if (canWalkToNextNode && isObstacle(nextExploringNode))
        {
            canWalkToNextNode = false;
        }
        if (!canWalkToNextNode)
        {
            continue;
        }
        // 如果下一个探路点在关闭列表中，则不加入开放列表
        if (m_map[nextExploringNode.Y][nextExploringNode.X].IsInClosedList)
        {
            continue;
        }

        //// 将下一探路点加入到开放列表
        // 更新下一探路点g值
        nextExploringNode.G = exploringNode.G + gIncreaseValue;
        // 检查是否已经在开放列表中
        Node &exploringNodeInMap = m_map[exploringNode.Y][exploringNode.X];
        Node &nextExploringNodeInMap = m_map[nextExploringNode.Y][nextExploringNode.X];
        bool isInOpenLst = nextExploringNodeInMap.IsInOpenedList;
        if (isInOpenLst)
        {
            // 如果下一探路点g值小于开放列表中对映点的g值
            if (nextExploringNodeInMap.G > nextExploringNode.G)
            {
                nextExploringNodeInMap.G = nextExploringNode.G;
                nextExploringNodeInMap.UpdateF();
                nextExploringNodeInMap.ParentNode = &exploringNodeInMap;
            }
        }
        else
        {
            nextExploringNodeInMap.G = nextExploringNode.G;
            nextExploringNodeInMap.H = getH(nextExploringNodeInMap);
            nextExploringNodeInMap.UpdateF();
            nextExploringNodeInMap.ParentNode = &exploringNodeInMap;

            // 加到开放列表
            nextExploringNodeInMap.IsInOpenedList = true;
            m_openedNodeList.push_back(&nextExploringNodeInMap);
        }

    } //--end--找出探路点周围8个可行点，保存到开放列表
}

void AStar::deleteMap()
{
    for (int y = 0; y < m_mapHeight; y++)
    {
        delete [] m_map[y];
    }
    delete [] m_map;
    m_map = nullptr;
}
