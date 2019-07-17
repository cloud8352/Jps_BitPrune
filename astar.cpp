#include"astar.h"

//初始化
void Astar::Init(int **_map,int height,int width,MyPoint _beginPoint,MyPoint _endPoint){
    map = _map;
    ROW = height;
    COL = width;

    VerticalDist = 10; //每格到相邻格直线距离10
    ObliqueDist = 14;  //每格到相邻格斜线距离14

    beginPoint = _beginPoint;
    endPoint = _endPoint;

    //建立辅助地图
    pathMap = new PathNode*[ROW];
    for(int i=0;i<ROW;i++){
        pathMap[i] = new PathNode[COL];
        for(int j=0;j<COL;j++){
            pathMap[i][j].isfind = false;
            pathMap[i][j].isroute = false;
            pathMap[i][j].value = map[i][j];
        }
    }

    beginTreeNode = new MyTreeNode; //开放列表的起始节点
    memset(beginTreeNode, 0, sizeof(MyTreeNode));
    //向树中加入起点
    beginTreeNode->pos = beginPoint;
    //标记走过
    pathMap[beginTreeNode->pos.row][beginTreeNode->pos.col].isfind = true;

    pTemp = beginTreeNode;//初始化当前树节点为起始节点
    pTempChild = NULL;//探路点
}

//初始化后，获取路径
void Astar::FindPath(){
    while(1){
        //找出探路点周围8个可行点，保存到开放列表
        for(int i=0;i<8;i++){
            bool canWalkObliquely = true;//斜对角是否可行走
            pTempChild = new MyTreeNode;
            memset(pTempChild, 0, sizeof(MyTreeNode));
            pTempChild->pos = pTemp->pos;
            switch(i)
            {
            case p_up:
                pTempChild->pos.row = pTemp->pos.row -1;//只有行减1
                pTempChild->pos.col = pTemp->pos.col;
                pTempChild->pos.g = pTemp->pos.g + VerticalDist;
                break;
            case p_down:
                pTempChild->pos.row = pTemp->pos.row +1;
                pTempChild->pos.col = pTemp->pos.col;
                pTempChild->pos.g = pTemp->pos.g + VerticalDist;
                break;
            case p_left:
                pTempChild->pos.row = pTemp->pos.row;//行不变，列减1
                pTempChild->pos.col = pTemp->pos.col -1;
                pTempChild->pos.g = pTemp->pos.g + VerticalDist;
                break;
            case p_right:
                pTempChild->pos.row = pTemp->pos.row;//行不变，列加1
                pTempChild->pos.col = pTemp->pos.col +1;
                pTempChild->pos.g = pTemp->pos.g + VerticalDist;
                break;
            case p_leftup:
                if(isBarrier(pTemp->pos.row -1, pTemp->pos.col, pathMap) ||//判断当前点上边点是否为障碍
                    isBarrier(pTemp->pos.row, pTemp->pos.col -1, pathMap)   //判断当前点左边点是否为障碍
                   ){//判断斜角是否可走
                    canWalkObliquely = false;
                    break;
                   }
                pTempChild->pos.row = pTemp->pos.row -1;
                pTempChild->pos.col = pTemp->pos.col -1;
                pTempChild->pos.g = pTemp->pos.g + ObliqueDist;
                break;
            case p_leftdown:
                if(isBarrier(pTemp->pos.row +1, pTemp->pos.col, pathMap) ||//判断当前点下边点是否为障碍
                    isBarrier(pTemp->pos.row, pTemp->pos.col -1, pathMap)   //判断当前点左边点是否为障碍
                   ){//判断斜角是否可走
                    canWalkObliquely = false;
                    break;
                   }
                pTempChild->pos.row = pTemp->pos.row +1;
                pTempChild->pos.col = pTemp->pos.col -1;
                pTempChild->pos.g = pTemp->pos.g + ObliqueDist;
                break;
            case p_rightup:
                if(isBarrier(pTemp->pos.row -1, pTemp->pos.col, pathMap) ||//判断当前点上边点是否为障碍
                    isBarrier(pTemp->pos.row, pTemp->pos.col +1, pathMap)   //判断当前点右边点是否为障碍
                   ){//判断斜角是否可走
                    canWalkObliquely = false;
                    break;
                   }
                pTempChild->pos.row = pTemp->pos.row -1;
                pTempChild->pos.col = pTemp->pos.col +1;
                pTempChild->pos.g = pTemp->pos.g + ObliqueDist;
                break;
            case p_rightdown:
                if(isBarrier(pTemp->pos.row +1, pTemp->pos.col, pathMap) ||//判断当前点下边点是否为障碍
                    isBarrier(pTemp->pos.row, pTemp->pos.col +1, pathMap)   //判断当前点右边点是否为障碍
                   ){//判断斜角是否可走
                    canWalkObliquely = false;
                    break;
                   }
                pTempChild->pos.row = pTemp->pos.row +1;
                pTempChild->pos.col = pTemp->pos.col +1;
                pTempChild->pos.g = pTemp->pos.g + ObliqueDist;
                break;
            }
            //能走就加入当前节点的子节点组，并存入开放树openTree
            if(isRoad(pTempChild->pos, pathMap) && //是否可行
               canWalkObliquely //对角是否可走
               ){
                //检查是否已经在开放列表中
                bool isInOpenLst = false;
                for(it=openTree.begin();it != openTree.end();it++){
                    if( (*it)->pos.row == pTempChild->pos.row &&
                        (*it)->pos.col == pTempChild->pos.col
                       ){
                        isInOpenLst = true;
                        break;
                       }
                }
                if(isInOpenLst){
                    if( (*it)->pos.g > pTempChild->pos.g){
                        (*it)->pos.g = pTempChild->pos.g;//如果当前点g值大于开放列表中对映点的g值，就修改g
                        (*it)->pos.GetF();
                        (*it)->parent = pTemp;
                        pTemp->child.push_back(pTempChild);
                    }
                }
                if(isInOpenLst == false){
                    //计算h值
                    pTempChild->pos.h = GetH(pTempChild->pos,endPoint);
                    //计算f值
                    pTempChild->pos.GetF();
                    //入树
                    pTemp->child.push_back(pTempChild);
                    pTempChild->parent = pTemp;
                    //存入数组
                    openTree.push_back(pTempChild);

                }
            }

        }//--end--找出探路点周围8个可行点，保存到开放列表

        //找出当前点周围最小f值可行点
        it = openTree.begin();
        minF_Iter = it;
        for(it = openTree.begin();it != openTree.end();it++){
            if( (*minF_Iter)->pos.f > (*it)->pos.f){
                minF_Iter = it;
            }
        }

        //换层
        if((*minF_Iter)->pos.row == endPoint.row &&
           (*minF_Iter)->pos.col == endPoint.col
           )
            break;
        pTemp = (*minF_Iter);

        //标记走过
        pathMap[pTemp->pos.row][pTemp->pos.col].isfind = true;

        //把最小f值可行点从数组(open_list)中删除
        openTree.erase(minF_Iter);

        if(openTree.size() == 0) break;

    }//end--while(1)寻路

    //路径回溯
    cout<<"最短路径：";
    MyTreeNode* node_line = (*minF_Iter);
    while(1){
        pathMap[node_line->pos.row][node_line->pos.col].isroute = true;
        cout<<node_line->pos.row<<","<<node_line->pos.col<<" ";
        node_line = node_line->parent;
        if(node_line == NULL) break;

    }
    cout<<endl;
    //打印路线地图
    for(int i=0;i < ROW;i++){
        for(int j=0;j < COL;j++){
            if(pathMap[i][j].isroute)
                cout<<"*";
            else cout<<pathMap[i][j].value;
        }
        cout<<endl;
    }

}
