#include"jps.h"


//判断邻居类型，是否是最佳邻居和强迫邻居
//入参：单元地图8位二进制格式(十进制范围0-255)，父节点位置(0-8)、检测的邻居的位置(0-8)
//当前点在单元地图的4位置
bool* Jps::Prune(short unitMap,char p,char n){
    static bool retType[2];//返回的类型
    char obstaclePos = 0;

 #if 0
    //单元地图预处理
    char unitMapTmp =0;
    if(p ==0){//单元地图顺时针转180度
        unitMapTmp = unitMapTmp | (unitMap<<8 &(1<<8) );//0->8
        unitMapTmp = unitMapTmp | (unitMap<<6 &(1<<7) );//1->7
        unitMapTmp = unitMapTmp | (unitMap<<4 &(1<<6) );//2->6
        unitMapTmp = unitMapTmp | (unitMap<<2 &(1<<5) );//3->5
        unitMapTmp = unitMapTmp | (unitMap>>2 &(1<<3) );//5->3
        unitMapTmp = unitMapTmp | (unitMap>>4 &(1<<2) );//6->2
        unitMapTmp = unitMapTmp | (unitMap>>6 &(1<<1) );//7->1
        unitMapTmp = unitMapTmp | (unitMap>>8 &(1<<0) );//8->0
        p = 8;
    }
#endif // 0

    if(p == 0){
        if(n ==7 ||n ==5 || n ==8){
            retType[0] = true;
            retType[1] = false;
        }

        if(n ==2){
            obstaclePos = unitMap>>1 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==6){
            obstaclePos = unitMap>>3 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }

        if(n == 1||n ==3){
            retType[0] = false;
            retType[1] = false;
        }
    }

    if(p == 1){
        if(n ==7){
            retType[0] = true;
            retType[1] = false;
        }
        if(n ==6){
            obstaclePos = unitMap>>3 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==8){
            obstaclePos = unitMap>>5 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }

        if(n == 0||n ==2||n ==3||n ==5){
            retType[0] = false;
            retType[1] = false;
        }
    }

    if(p == 2){
        if(n ==7 ||n ==6 || n ==3){
            retType[0] = true;
            retType[1] = false;
        }
        if(n ==0){
            obstaclePos = unitMap>>1 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==8){
            obstaclePos = unitMap>>5 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }

        if(n == 1||n ==5){
            retType[0] = false;
            retType[1] = false;
        }
    }

    if(p == 3){
        if(n ==5){
            retType[0] = true;
            retType[1] = false;
        }
        if(n ==2){
            obstaclePos = unitMap>>1 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==8){
            obstaclePos = unitMap>>7 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n == 0||n ==1||n ==6||n ==7){
            retType[0] = false;
            retType[1] = false;
        }
    }

    if(p == 5){
        if(n ==3){
            retType[0] = true;
            retType[1] = false;
        }
        if(n ==0){
            obstaclePos = unitMap>>1 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==6){
            obstaclePos = unitMap>>7 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }

        if(n == 1||n ==2||n ==7||n ==8){
            retType[0] = false;
            retType[1] = false;
        }
    }


    if(p == 6){
        if(n ==1 ||n ==2 || n ==5){
            retType[0] = true;
            retType[1] = false;
        }
        if(n ==0){
            obstaclePos = unitMap>>3 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==8){
            obstaclePos = unitMap>>7 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }

        if(n == 3||n ==7){
            retType[0] = false;
            retType[1] = false;
        }
    }

    if(p == 7){
        if(n ==1){
            retType[0] = true;
            retType[1] = false;
        }
        if(n ==0){
            obstaclePos = unitMap>>3 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==2){
            obstaclePos = unitMap>>5 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n == 3||n ==5||n ==6||n ==8){
            retType[0] = false;
            retType[1] = false;
        }

    }

    if(p == 8){
        if(n ==0 ||n ==1 || n ==3){
            retType[0] = true;
            retType[1] = false;
        }
        if(n ==2){
            obstaclePos = unitMap>>5 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n ==6){
            obstaclePos = unitMap>>7 & 0x01;
            if( 1 == obstaclePos){
                retType[0] = true;
                retType[1] = true;
            }
            if( 0 == obstaclePos){
                retType[0] = false;
                retType[1] = false;
            }
        }
        if(n == 5||n ==7){
            retType[0] = false;
            retType[1] = false;
        }
    }

    return retType;
}

void Jps::Init(int **_map,int _height,int _width){

    //初始化空节点
    nullNode.isnull = true;
    nullNode.isMidJumpNode = false;

    height = _height;
    width = _width;

    //建立辅助地图
    pathMap = new PathNode**[height];
    for(int i=0;i<height;i++){
        pathMap[i] = new PathNode*[width];
        for(int j=0;j<width;j++){
            pathMap[i][j] = new PathNode;
            memset(pathMap[i][j],0, sizeof(PathNode));
            pathMap[i][j]->row = i;
            pathMap[i][j]->col = j;
            pathMap[i][j]->isfind = false;
            pathMap[i][j]->isroute = false;
            pathMap[i][j]->value = _map[i][j];
        }
    }

}


//直跳跃
//入参：辅助地图、当前节点、直跳跃方向-x方向值，y方向值
//返回跳点
Jps::PathNode Jps::JumpStraight(PathNode*** _pathMap,PathNode currenNode,Direct dir){
    int delta_x = 0;
    int delta_y = 0;
    short unitMap = 0;
    char valueT = 0;//存储辅助地图中点的临时值，用于算出单元地图值
    bool* nodeTyp;
    char parent;//单元地图中，父节点
    char neighbGroup[9] = {9,9,9,9,9,9,9,9,9};//单元地图中,要探测的邻居组，初始化为非(0-8)的值，为9的点为不可行点
    switch(dir)
    {
    case p_up:
        delta_x = 0;
        delta_y = -1;
        parent = 7;
        break;
    case p_down:
        delta_x = 0;
        delta_y = 1;
        parent = 1;
        break;
    case p_left:
        delta_x = -1;
        delta_y = 0;
        parent = 5;
        break;
    case p_right:
        delta_x = 1;
        delta_y = 0;
        parent = 3;
        break;
    default:
        break;
    }

    PathNode nodeTmp = currenNode;//沿指定方向搜寻的点
    //沿指定方向搜寻，知道找到跳点，或碰到障碍物，或超出地图
    while(1){
        nodeTmp.row += delta_y;
        nodeTmp.col += delta_x;
        cout<<"直跳跃："<<nodeTmp.row<<","<<nodeTmp.col<<endl;
        //如果搜寻到终点就返回
        if(nodeTmp.row == endNode.row &&
           nodeTmp.col == endNode.col) return nodeTmp;
        //如果搜寻点，是障碍物，或者出了地图，返回空
        if(height <= nodeTmp.row || 0 > nodeTmp.row||
           width <= nodeTmp.col || 0 > nodeTmp.col ||
            1 == _pathMap[nodeTmp.row][nodeTmp.col]->value
           ) return nullNode;

        //获取搜寻点周围3x3单元地图，并找到邻居组
        unitMap = 0;//清空单元地图
        for(int i=0; i<9; i++){//初始化邻居组
            neighbGroup[i] = 9;
        }

        for(int i = 0;i <3; i++){
            for(int j= 0;j <3; j++){
                int row_t = nodeTmp.row +i-1;//获取周围的点坐标
                int col_t = nodeTmp.col +j-1;
                if(height > row_t && 0 <= row_t &&
                    width > col_t && 0 <= col_t
                    ){//确保周围点没超出地图
                    valueT = _pathMap[row_t][col_t]->value;
                    unitMap = unitMap|valueT<<(i*3 +j);
                    if(1 != valueT){//不为障碍
                        neighbGroup[i*3+j] = (i*3 +j);
                    }
                }
            }
        }//end-获取搜寻点周围3x3单元地图，并找到邻居组

        //获取当前搜寻点周围点类型
        for(int i=0;i <9;i++){
            if(9 != neighbGroup[i] &&
               parent != neighbGroup[i] &&
               4 != neighbGroup[i]
               ){//如果邻居组中点不为：空(9)、当前搜寻点(4)、父节点
                nodeTyp = Prune(unitMap, parent, neighbGroup[i]);
                if(1 == nodeTyp[1]){//如果存在强迫邻居，返回当前搜寻点
                    return nodeTmp;
                }

            }
        }//end-获取当前搜寻点周围点类型


    }//while(o)-end
}

//直跳跃（位运算优化）
//入参：辅助地图、当前节点、直跳跃方向-x方向值，y方向值
//返回跳点
Jps::PathNode Jps::JumpStraightBit(PathNode*** _pathMap,PathNode currenNode,Direct dir){
    PathNode nodeTmp = currenNode;
    PathNode jumpNode = currenNode;

    unsigned int bitMap = 0;//临时节点当前行或列跳点判断位地图
    unsigned int bitMapUp = 0;//临时节点上一行跳点判断位地图
    unsigned int bitMapDown = 0;//临时节点下一行跳点判断位地图
    unsigned int bitMapLeft = 0;
    unsigned int bitMapRight = 0;

    char barrierUpBit = 0;//障碍往上一点值
    char barrierDownBit = 0;//障碍下一点值
    char barrierLeftBit = 0;//障碍左一点值
    char barrierRightBit = 0;//障碍右一点值
    char barrierLeftUpBit = 0;//障碍左上值
    char barrierLeftDownBit = 0;//障碍左下值
    char barrierRightUpBit = 0;//障碍右上值
    char barrierRightDownBit = 0;//障碍右下值

    int n,j;
    //根据方向初始化开始搜寻的点
    switch(dir)
    {
    case p_up:
        nodeTmp.row = currenNode.row -1;
        nodeTmp.col = currenNode.col;
        break;
    case p_down:
        nodeTmp.row = currenNode.row +1;
        nodeTmp.col = currenNode.col;
        break;
    case p_left:
        nodeTmp.row = currenNode.row;
        nodeTmp.col = currenNode.col -1;
        break;
    case p_right:
        nodeTmp.row = currenNode.row;
        nodeTmp.col = currenNode.col +1;
        break;
    default:
        break;
    }

    while(1){
        //清空位地图
        bitMapUp = 0;
        bitMap = 0;
        bitMapDown = 0;
        bitMapLeft = 0;
        bitMapRight = 0;
        //清空障碍物周围点值
        barrierUpBit = 0;//障碍往上一点值
        barrierDownBit = 0;//障碍下一点值
        barrierLeftBit = 0;//障碍左一点值
        barrierRightBit = 0;//障碍右一点值
        barrierLeftUpBit = 0;//障碍左上值
        barrierLeftDownBit = 0;//障碍左下值
        barrierRightUpBit = 0;//障碍右上值
        barrierRightDownBit = 0;//障碍右下值


        //如果临时节点不在地图中，则说明此次搜寻没有跳点，直接返回空节点；
        if(0 > nodeTmp.row ||height <= nodeTmp.row ||0 > nodeTmp.col ||width <= nodeTmp.col) return nullNode;

        if(p_right == dir){
            //建立位地图
            for(int i=0;i < 32;i++){
                if(0 <= nodeTmp.col +i && width >nodeTmp.col +i){//如果列数在地图列数范围内
                    if(0 <= nodeTmp.row -1 && height >nodeTmp.row +1){//没超出辅助节点地图
                        bitMapUp = bitMapUp<<1 |_pathMap[nodeTmp.row -1][nodeTmp.col +i]->value;
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row -0][nodeTmp.col +i]->value;
                        bitMapDown = bitMapDown<<1 |_pathMap[nodeTmp.row +1][nodeTmp.col +i]->value;
                    }
                    if(0 > nodeTmp.row +1 ||height <= nodeTmp.row -1){//三行全超出地图，全置1，等价为障碍
                        bitMapUp = bitMapUp<<1 |1;
                        bitMap   = bitMap<<1 |1;
                        bitMapDown = bitMapDown<<1 |1;
                    }
                    if(0 == nodeTmp.row +1){//上两行超出地图，上两行置1，等价为障碍
                        bitMapUp = bitMapUp<<1 |1;
                        bitMap   = bitMap   <<1 |1;
                        bitMapDown = bitMapDown<<1 |_pathMap[nodeTmp.row +1][nodeTmp.col +i]->value;
                    }
                    if(height -1 == nodeTmp.row -1){//下两行超出地图，下两行置1，等价为障碍
                        bitMapUp = bitMapUp<<1 |_pathMap[nodeTmp.row -1][nodeTmp.col +i]->value;
                        bitMap   = bitMap   <<1 |1;
                        bitMapDown = bitMapDown<<1 |1;
                    }
                    if(0 == nodeTmp.row ||height -1 == nodeTmp.row){//中间行在地图上或下边界上
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row -0][nodeTmp.col +i]->value;
                        if(-1 == nodeTmp.row -1) bitMapUp = bitMapUp<<1 |1;//如果上一行是上边界的上一行，置1
                        else bitMapUp = bitMapUp<<1 |_pathMap[nodeTmp.row -1][nodeTmp.col +i]->value;
                        if(height == nodeTmp.row +1) bitMapDown = bitMapDown<<1 |1;//如果下一行是下边界的下一行，置1
                        else bitMapDown = bitMapDown<<1 |_pathMap[nodeTmp.row +1][nodeTmp.col +i]->value;
                    }

                }
                else{//如果列数不在地图列数范围内
                    bitMapUp = bitMapUp<<1 |1;
                    bitMap   = bitMap<<1 |1;
                    bitMapDown = bitMapDown<<1 |1;
                }
            }//end - 建立位地图
            //bit map test
            cout<<endl<<"bit map test:"<<bitMapUp<<" "<<bitMap<<" "<<bitMapDown<<endl;
            cout<<""<<currenNode.row<<","<<currenNode.col<<endl;
            cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierUpBit = bitMapUp>>(32 -n-1) &1;//从位地图中得到障碍周围点值
            barrierLeftUpBit = bitMapUp>>(32 -n-1 +1) &1;
            barrierDownBit = bitMapDown>>(32 -n-1) &1;
            barrierLeftDownBit = bitMapDown>>(32 -n-1 +1) &1;
            j = __builtin_clz(  ( (bitMapUp>>1)& (~bitMapUp)) |( (bitMapDown>>1)& (~bitMapDown)) );
            if(31 == n && 31 == j){//如果距离当前1-32格内没找到跳点，就切换到第30格
                nodeTmp.row = nodeTmp.row;
                nodeTmp.col = nodeTmp.col + 30;
            }
            else{
                if(n <= j){
                    if( (1 == barrierLeftUpBit ||1 == barrierUpBit) &&
                       (1 == barrierLeftDownBit ||1 == barrierDownBit)
                       ) return nullNode;//如果该障碍点的(左上 ||上) && (左下 ||下)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.row = nodeTmp.row;
                    jumpNode.col = nodeTmp.col +n-1;
                    return jumpNode;
                }
                if(n > j){
                    jumpNode.row = nodeTmp.row;
                    jumpNode.col = nodeTmp.col +j-1;
                    return jumpNode;
                }
            }

        }
        if(p_left == dir){
            //建立位地图
            for(int i=0;i < 32;i++){
                if(0 <= nodeTmp.col -i && width >nodeTmp.col -i){//如果列数在地图列数范围内
                    if(0 <= nodeTmp.row -1 && height >nodeTmp.row +1){//没超出辅助节点地图
                        bitMapUp = bitMapUp<<1 |_pathMap[nodeTmp.row -1][nodeTmp.col -i]->value;
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row -0][nodeTmp.col -i]->value;
                        bitMapDown = bitMapDown<<1 |_pathMap[nodeTmp.row +1][nodeTmp.col -i]->value;
                    }
                    if(0 > nodeTmp.row +1 ||height <= nodeTmp.row -1){//三行全超出地图，全置1，等价为障碍
                        bitMapUp = bitMapUp<<1 |1;
                        bitMap   = bitMap<<1 |1;
                        bitMapDown = bitMapDown<<1 |1;
                    }
                    if(0 == nodeTmp.row +1){//上两行超出地图，上两行置1，等价为障碍
                        bitMapUp = bitMapUp<<1 |1;
                        bitMap   = bitMap   <<1 |1;
                        bitMapDown = bitMapDown<<1 |_pathMap[nodeTmp.row +1][nodeTmp.col -i]->value;
                    }
                    if(height -1 == nodeTmp.row -1){//下两行超出地图，下两行置1，等价为障碍
                        bitMapUp = bitMapUp<<1 |_pathMap[nodeTmp.row -1][nodeTmp.col -i]->value;
                        bitMap   = bitMap   <<1 |1;
                        bitMapDown = bitMapDown<<1 |1;
                    }
                    if(0 == nodeTmp.row ||height -1 == nodeTmp.row){//中间行在地图上或下边界上
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row -0][nodeTmp.col -i]->value;
                        if(-1 == nodeTmp.row -1) bitMapUp = bitMapUp<<1 |1;//如果上一行是上边界的上一行，置1
                        else bitMapUp = bitMapUp<<1 |_pathMap[nodeTmp.row -1][nodeTmp.col -i]->value;
                        if(height == nodeTmp.row +1) bitMapDown = bitMapDown<<1 |1;//如果下一行是下边界的下一行，置1
                        else bitMapDown = bitMapDown<<1 |_pathMap[nodeTmp.row +1][nodeTmp.col -i]->value;
                    }

                }
                else{//如果列数不在地图列数范围内
                    bitMapUp = bitMapUp<<1 |1;
                    bitMap   = bitMap<<1 |1;
                    bitMapDown = bitMapDown<<1 |1;
                }
            }//end - 建立位地图
            //bit map test
            cout<<endl<<"bit map test:"<<bitMapUp<<" "<<bitMap<<" "<<bitMapDown<<endl;
            cout<<""<<currenNode.row<<","<<currenNode.col<<endl;
            cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierUpBit = bitMapUp>>(32 -n-1) &1;//从位地图中得到障碍周围点值
            barrierRightUpBit = bitMapUp>>(32 -n-1 +1) &1;
            barrierDownBit = bitMapDown>>(32 -n-1) &1;
            barrierRightDownBit = bitMapDown>>(32 -n-1 +1) &1;
            j = __builtin_clz(  ( (bitMapUp>>1)& (~bitMapUp)) |( (bitMapDown>>1)& (~bitMapDown)) );
            if(31 == n && 31 == j){//如果距离当前1-32格内没找到跳点，就切换到第30格
                nodeTmp.row = nodeTmp.row;
                nodeTmp.col = nodeTmp.col - 30;
            }
            else{
                if(n <= j){
                    if( (1 == barrierRightUpBit ||1 == barrierUpBit) &&
                       (1 == barrierRightDownBit ||1 == barrierDownBit)
                       ) return nullNode;//如果该障碍点的(右上 ||上) && (右下 ||下)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.row = nodeTmp.row;
                    jumpNode.col = nodeTmp.col -n+1;
                    return jumpNode;
                }
                if(n > j){
                    jumpNode.row = nodeTmp.row;
                    jumpNode.col = nodeTmp.col -j+1;
                    return jumpNode;
                }
            }

        }//end-if(p_left == dir)
        if(p_down == dir){
            //建立位地图
            for(int i=0;i < 32;i++){
                if(0 <= nodeTmp.row +i && height >nodeTmp.row +i){//如果行数在地图行数范围内
                    if(0 <= nodeTmp.col -1 && width >nodeTmp.col +1){//没超出辅助节点地图
                        bitMapLeft = bitMapLeft<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col -1]->value;
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col + 0]->value;
                        bitMapRight = bitMapRight<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col +1]->value;
                    }
                    if(0 > nodeTmp.col +1 ||width <= nodeTmp.col -1){//三列全超出地图，全置1，等价为障碍
                        bitMapLeft = bitMapLeft<<1 |1;
                        bitMap   = bitMap<<1 |1;
                        bitMapRight = bitMapRight<<1 |1;
                    }
                    if(0 == nodeTmp.col +1){//左两列超出地图，左两行置1，等价为障碍
                        bitMapLeft = bitMapLeft<<1 |1;
                        bitMap   = bitMap   <<1 |1;
                        bitMapRight = bitMapRight<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col +1]->value;
                    }
                    if(width -1 == nodeTmp.col -1){//右两列超出地图，右两行置1，等价为障碍
                        bitMapLeft = bitMapLeft<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col -1]->value;
                        bitMap   = bitMap   <<1 |1;
                        bitMapRight = bitMapRight<<1 |1;
                    }
                    if(0 == nodeTmp.col ||width -1 == nodeTmp.col){//中间列在地图左或右边界上
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col]->value;
                        if(-1 == nodeTmp.col -1) bitMapLeft = bitMapLeft<<1 |1;//如果左一列是左边界的左一列，置1
                        else bitMapLeft = bitMapLeft<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col -1]->value;
                        if(width == nodeTmp.col +1) bitMapRight = bitMapRight<<1 |1;//如果右一列是右边界的右一列，置1
                        else bitMapRight = bitMapRight<<1 |_pathMap[nodeTmp.row +i][nodeTmp.col +1]->value;
                    }

                }
                else{//如果行数不在地图列数范围内
                    bitMapLeft = bitMapLeft<<1 |1;
                    bitMap   = bitMap<<1 |1;
                    bitMapRight = bitMapRight<<1 |1;
                }
            }//end - 建立位地图
            //bit map test
            cout<<endl<<"bit map test:"<<bitMapLeft<<" "<<bitMap<<" "<<bitMapRight<<endl;
            cout<<""<<currenNode.row<<","<<currenNode.col<<endl;
            cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierLeftBit = bitMapLeft>>(32 -n-1) &1;//从位地图中得到障碍周围点值
            barrierLeftUpBit = bitMapLeft>>(32 -n-1 +1) &1;
            barrierRightBit = bitMapRight>>(32 -n-1) &1;
            barrierRightUpBit = bitMapRight>>(32 -n-1 +1) &1;

            j = __builtin_clz( ( (bitMapLeft>>1)& (~bitMapLeft)) |( (bitMapRight>>1)& (~bitMapRight)) );
            if(31 == n && 31 == j){ //__builtin_clz( (unsigned int)0 ) 的值为31
                nodeTmp.row = nodeTmp.row + 30;
                nodeTmp.col = nodeTmp.col;
            }
            else{
                if(n <= j){
                    if( (1 == barrierLeftUpBit ||1 == barrierLeftBit) &&
                       (1 == barrierRightUpBit ||1 == barrierRightBit)
                       ) return nullNode;//如果该障碍点的(左上 ||左) && (右上 ||右)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.row = nodeTmp.row +n-1;
                    jumpNode.col = nodeTmp.col;
                    return jumpNode;
                }
                if(n > j){
                    jumpNode.row = nodeTmp.row +j-1;
                    jumpNode.col = nodeTmp.col;
                    return jumpNode;
                }
            }
        }//end-if(p_down == dir)
        if(p_up == dir){
            //建立位地图
            for(int i=0;i < 32;i++){
                if(0 <= nodeTmp.row -i && height >nodeTmp.row -i){//如果行数在地图行数范围内
                    if(0 <= nodeTmp.col -1 && width >nodeTmp.col +1){//当前列没超出辅助节点地图
                        bitMapLeft = bitMapLeft<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col -1]->value;
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col + 0]->value;
                        bitMapRight = bitMapRight<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col +1]->value;
                    }
                    if(0 > nodeTmp.col +1 ||width <= nodeTmp.col -1){//三列全超出地图，全置1，等价为障碍
                        bitMapLeft = bitMapLeft<<1 |1;
                        bitMap   = bitMap<<1 |1;
                        bitMapRight = bitMapRight<<1 |1;
                    }
                    if(0 == nodeTmp.col +1){//左两列超出地图，左两列置1，等价为障碍
                        bitMapLeft = bitMapLeft<<1 |1;
                        bitMap   = bitMap   <<1 |1;
                        bitMapRight = bitMapRight<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col +1]->value;
                    }
                    if(width -1 == nodeTmp.col -1){//右两列超出地图，右两列置1，等价为障碍
                        bitMapLeft = bitMapLeft<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col -1]->value;
                        bitMap   = bitMap   <<1 |1;
                        bitMapRight = bitMapRight<<1 |1;
                    }
                    if(0 == nodeTmp.col ||width -1 == nodeTmp.col){//中间列在地图左或右边界上
                        bitMap   = bitMap<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col]->value;
                        if(-1 == nodeTmp.col -1) bitMapLeft = bitMapLeft<<1 |1;//如果左一列是左边界的左一列，置1
                        else bitMapLeft = bitMapLeft<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col -1]->value;
                        if(width == nodeTmp.col +1) bitMapRight = bitMapRight<<1 |1;//如果右一列是右边界的右一列，置1
                        else bitMapRight = bitMapRight<<1 |_pathMap[nodeTmp.row -i][nodeTmp.col +1]->value;
                    }

                }
                else{//如果列数不在地图列数范围内
                    bitMapLeft = bitMapLeft<<1 |1;
                    bitMap   = bitMap<<1 |1;
                    bitMapRight = bitMapRight<<1 |1;
                }
            }//end - 建立位地图
            //bit map test
            cout<<endl<<"bit map test:"<<bitMapLeft<<" "<<bitMap<<" "<<bitMapRight<<endl;
            cout<<""<<currenNode.row<<","<<currenNode.col<<endl;
            cout<<"__builtin_clz(bitMap):"<<__builtin_clz(bitMap)<<endl;

            n = __builtin_clz(bitMap);
            barrierLeftBit = bitMapLeft>>(32 -n-1) &1;//从位地图中得到障碍周围点值
            barrierLeftDownBit = bitMapLeft>>(32 -n-1 +1) &1;
            barrierRightBit = bitMapRight>>(32 -n-1) &1;
            barrierRightDownBit = bitMapRight>>(32 -n-1 +1) &1;

            j = __builtin_clz( ( (bitMapLeft>>1)& (~bitMapLeft)) |( (bitMapRight>>1)& (~bitMapRight)) );
            if(31 == n && 31 == j){ //__builtin_clz( (unsigned int)0 ) 的值为31
                nodeTmp.row = nodeTmp.row + 30;
                nodeTmp.col = nodeTmp.col;
            }
            else{
                if(n <= j){
                    if( (1 == barrierLeftDownBit ||1 == barrierLeftBit) &&
                       (1 == barrierRightDownBit ||1 == barrierRightBit)
                       ) return nullNode;//如果该障碍点的(左上 ||左) && (右上 ||右)的点为障碍，证明此次搜寻没有跳点，直接返回空节点；
                    jumpNode.row = nodeTmp.row -n+1;
                    jumpNode.col = nodeTmp.col;
                    return jumpNode;
                }
                if(n > j){
                    jumpNode.row = nodeTmp.row -j+1;
                    jumpNode.col = nodeTmp.col;
                    return jumpNode;
                }
            }
        }//end-if(p_up == dir)

    }

    return nullNode;
}


Jps::PathNode* Jps::JumpOblique(PathNode*** _pathMap,PathNode currenNode,Direct dir){
    static PathNode retNode[3];
    int delta_x = 0;
    int delta_y = 0;
    short unitMap = 0;
    char valueT = 0;//存储辅助地图中点的临时值，用于算出单元地图值
    bool* nodeTyp;
    char parent;//单元地图中，父节点
    char neighbGroup[9] = {9,9,9,9,9,9,9,9,9};//单元地图中,要探测的邻居组，初始化为非(0-8)的值，为9的点为不可行点
    Direct straightDirs[2] = {p_up,p_up};
    switch(dir)
    {
    case p_leftup:
        delta_x = -1;
        delta_y = -1;
        parent = 8;
        straightDirs[0] = p_left;
        straightDirs[1] = p_up;
        break;
    case p_leftdown:
        delta_x = -1;
        delta_y = 1;
        parent = 2;
        straightDirs[0] = p_left;
        straightDirs[1] = p_down;
        break;
    case p_rightup:
        delta_x = 1;
        delta_y = -1;
        parent = 6;
        straightDirs[0] = p_right;
        straightDirs[1] = p_up;
        break;
    case p_rightdown:
        delta_x = 1;
        delta_y = 1;
        parent = 0;
        straightDirs[0] = p_right;
        straightDirs[1] = p_down;
        break;
    default:
        break;
    }

    PathNode nodeTmp = currenNode;//沿指定方向搜寻的点
    //沿指定方向搜寻，知道找到跳点，或碰到障碍物，或超出地图
    while(1){
        nodeTmp.row += delta_y;
        nodeTmp.col += delta_x;
        cout<<"斜跳跃："<<nodeTmp.row<<","<<nodeTmp.col<<endl;
        //如果搜寻到终点就返回
        if(nodeTmp.row == endNode.row &&
           nodeTmp.col == endNode.col){
            retNode[0] = nodeTmp;
            return retNode;
           }
        //如果搜寻点，是障碍物，或者出了地图，返回空
        if(height <= nodeTmp.row || 0 > nodeTmp.row||
           width <= nodeTmp.col || 0 > nodeTmp.col ||
            1 == _pathMap[nodeTmp.row][nodeTmp.col]->value
           ){
            retNode[0] = nullNode;
            return retNode;
           }

        //获取搜寻点周围3x3单元地图，并找到邻居组
        unitMap = 0;//清空单元地图
        for(int i=0; i<9; i++){//初始化邻居组
            neighbGroup[i] = 9;
        }

        for(int i = 0;i <3; i++){
            for(int j= 0;j <3; j++){
                int row_t = nodeTmp.row +i-1;//获取周围的点坐标
                int col_t = nodeTmp.col +j-1;
                if(height > row_t && 0 <= row_t &&
                    width > col_t && 0 <= col_t
                    ){//确保周围点没超出地图
                    valueT = _pathMap[row_t][col_t]->value;
                    unitMap = unitMap|valueT<<(i*3 +j);
                    if(1 != valueT){//不为障碍
                        neighbGroup[i*3+j] = (i*3 +j);
                    }
                }
            }
        }//end-获取搜寻点周围3x3单元地图，并找到邻居组

        //获取当前搜寻点周围点类型，如果存在强迫邻居，返回当前搜寻点
        for(int i=0;i <9;i++){
            if(9 != neighbGroup[i] &&
               parent != neighbGroup[i] &&
               4 != neighbGroup[i]
               ){//如果邻居组中点不为：空(9)、当前搜寻点(4)、父节点
                nodeTyp = Prune(unitMap, parent, neighbGroup[i]);
                if(1 == nodeTyp[1]){//如果存在强迫邻居，返回当前搜寻点
                    retNode[0] = nodeTmp;
                    retNode[0].isMidJumpNode = false;
                    return retNode;
                }

            }
        }//end-获取当前搜寻点周围点类型

        //往当前点的直线“真。邻居“，做直跳跃，如果不反回空，返回当前点
        PathNode jumpNode;//用于保存直跳跃的返回节点
        bool willRet = false;
        for(int i=0;i <2; i++){
            jumpNode = JumpStraightBit(_pathMap, nodeTmp, straightDirs[i]);    //----------------修改
            if(false == jumpNode.isnull){
                willRet = true;
                retNode[0] = nodeTmp;
                retNode[0].isMidJumpNode = true;
                retNode[i +1] = jumpNode;
            }
            if(jumpNode.isnull){
                retNode[i +1] = jumpNode;
            }
        }
        if(willRet) return retNode;


    }
}

#if 0
vector<Jps::PathNode> Jps::FindPath(PathNode _startNode,PathNode _endNode){
    //设置开始结束点
    startNode = _startNode;
    endNode = _endNode;

    vector<Direct> jumpDirs;//存放当前需要跳跃的方向
    vector<Direct>::iterator dirsIt;//用于检索反向树的迭代器
    PathNode jumpNode;//返回的跳点
    bool* nodeTyp;//返回的邻居类型

    PathNode currentNode;//当前节点
    vector<PathNode> openTree;//开放列表，关闭列表是用辅助地图各点的isfind属性维护的
    vector<PathNode>::iterator it;//用于迭代
    vector<PathNode>::iterator minF_iter;//存放最小f值节点

    currentNode =  startNode;//当前点为开始点
    pathMap[currentNode.row][currentNode.col]->isfind = true;

    //初始方向树（八个方向）
    for(int i=0;i <8;i++){
        jumpDirs.push_back( (Direct)i);
    }

    //寻路
    while(1){

        //利用当前点，以及parent方向，往所有“真。邻居“方向跳跃
        for(dirsIt = jumpDirs.begin();dirsIt != jumpDirs.end(); dirsIt++){
            cout<<"方向："<<(*dirsIt)<<" "<<endl;
            if( *(dirsIt) == p_up|| *(dirsIt) == p_down|| *(dirsIt) == p_left|| *(dirsIt) == p_right){
                jumpNode = JumpStraight(pathMap, currentNode, (*dirsIt) );
            }
            if( *(dirsIt) == p_leftup|| *(dirsIt) == p_leftdown|| *(dirsIt) == p_rightup|| *(dirsIt) == p_rightdown){
                jumpNode = JumpOblique(pathMap, currentNode, (*dirsIt) );
            }

            //如果返回的是有效节点，且不在关闭列表中（未找过）
            if(false == jumpNode.isnull && false == pathMap[jumpNode.row][jumpNode.col]->isfind){

                jumpNode.g = pathMap[currentNode.row][currentNode.col]->g +GetDis( currentNode, jumpNode);
                //如果该点已经在开放列表中，比较g值，取g值较小的点的属性，并不用再次加入开放列表
                if(pathMap[jumpNode.row][jumpNode.col]->inopen){
                    if(pathMap[jumpNode.row][jumpNode.col]->g > jumpNode.g){
                        pathMap[jumpNode.row][jumpNode.col]->g = jumpNode.g;
                        pathMap[jumpNode.row][jumpNode.col]->GetF();

                        pathMap[jumpNode.row][jumpNode.col]->parent = pathMap[currentNode.row][currentNode.col];
                    }

                }
                //如果不在开放列表中
                if(false == pathMap[jumpNode.row][jumpNode.col]->inopen){
                    jumpNode.h = GetH(jumpNode, endNode);
                    jumpNode.GetF();
                    jumpNode.inopen = true;

                    //将探测点加入开放列表
                    openTree.push_back(jumpNode);
                    //更新辅助地图中对应探测点的节点属性
                    pathMap[jumpNode.row][jumpNode.col]->g = jumpNode.g;
                    pathMap[jumpNode.row][jumpNode.col]->h = jumpNode.h;
                    pathMap[jumpNode.row][jumpNode.col]->f = jumpNode.f;
                    pathMap[jumpNode.row][jumpNode.col]->parent = pathMap[currentNode.row][currentNode.col];
                    pathMap[jumpNode.row][jumpNode.col]->inopen = jumpNode.inopen;
                }


                //system("pause");

            }


        }

        if(openTree.size() == 0) break;
        //找下一点
        minF_iter = openTree.begin();
        cout<<endl<<"找下一点"<<endl;
        for(it =openTree.begin();it !=openTree.end(); it++){
            cout<<(*it).row<<","<<(*it).col<<endl;
            if(pathMap[(*it).row][(*it).col]->f < pathMap[(*minF_iter).row][(*minF_iter).col]->f){
                minF_iter = it;
            }
        }

        cout<<endl<<"找到的下一点: ";
        cout<<(*minF_iter).row<<","<<(*minF_iter).col<<endl;

        currentNode = (*minF_iter);

        pathMap[currentNode.row][currentNode.col]->isfind = true;

        if(currentNode.row == endNode.row && currentNode.col == endNode.col) break;

        openTree.erase(minF_iter);

        //获取当前节点即将要搜寻的方向，jumpDirs
        jumpDirs.clear();
        int delta_y = currentNode.row - pathMap[currentNode.row][currentNode.col]->parent->row;
        int delta_x = currentNode.col - pathMap[currentNode.row][currentNode.col]->parent->col;
        char p;//单元地图中父点
        short unitMap = 0;
        char neighbGroup[9] = {9,9,9,9,9,9,9,9,9};//单元地图中,要探测的邻居组，初始化为非(0-8)的值，为9的点为不可行点

        if(0 > delta_y && 0 ==delta_x) p = 7;//搜寻方向为上
        if(0 < delta_y && 0 ==delta_x) p = 1;//搜寻方向为下
        if(0 == delta_y && 0 >delta_x) p = 3;//搜寻方向为左
        if(0 > delta_y && 0 <delta_x) p = 5;//搜寻方向为右

        if(0 > delta_y && 0 >delta_x) p = 8;//搜寻方向为左上
        if(0 < delta_y && 0 >delta_x) p = 2;//搜寻方向为左下
        if(0 > delta_y && 0 <delta_x) p = 6;//搜寻方向为右上
        if(0 < delta_y && 0 <delta_x) p = 0;//

        //获取搜寻点周围3x3单元地图，并找到邻居组

        for(int i = 0;i <3; i++){
            for(int j= 0;j <3; j++){
                int row_t = currentNode.row +i-1;//获取周围的点坐标
                int col_t = currentNode.col +j-1;
                if(height > row_t && 0 <= row_t &&
                    width > col_t && 0 <= col_t
                    ){//确保周围点没超出地图
                    int valueT = pathMap[row_t][col_t]->value;
                    unitMap = unitMap|valueT<<(i*3 +j);
                    if(1 != valueT){//不为障碍
                        neighbGroup[i*3+j] = (i*3 +j);
                    }
                }
            }
        }//end-获取搜寻点周围3x3单元地图，并找到邻居组

        //获取当前搜寻点周围点类型，并赋值探测方向组
        for(int i=0;i <9;i++){
            if(9 != neighbGroup[i] &&
               p != neighbGroup[i] &&
               4 != neighbGroup[i]
               ){//如果邻居组中点不为：空(9)、当前搜寻点(4)、父节点
                nodeTyp = Prune(unitMap, p, neighbGroup[i]);
                if(1 == nodeTyp[0]){//如果存在关键邻居，就向探测方向组中加入当前方向
                    if(1==i) jumpDirs.push_back(p_up);
                    if(7==i) jumpDirs.push_back(p_down);
                    if(3==i) jumpDirs.push_back(p_left);
                    if(5==i) jumpDirs.push_back(p_right);

                    if(0==i) jumpDirs.push_back(p_leftup);
                    if(6==i) jumpDirs.push_back(p_leftdown);
                    if(2==i) jumpDirs.push_back(p_rightup);
                    if(8==i) jumpDirs.push_back(p_rightdown);

                }

            }
        }//end-获取当前搜寻点周围点类型，并赋值探测方向组

        //system("pause");

    }

    //返回路径
    vector<PathNode> retPathTmp;
    vector<PathNode> retPath;
    PathNode nodeTmp = endNode;
    while(1){
        int row_t = nodeTmp.row;
        int col_t = nodeTmp.col;
        retPathTmp.push_back(nodeTmp);
        if(NULL == pathMap[nodeTmp.row][nodeTmp.col]->parent) break;
        nodeTmp.row = pathMap[row_t][col_t]->parent->row;
        nodeTmp.col = pathMap[row_t][col_t]->parent->col;
    }
    //将路径整理为从开始点出发的顺序
    cout<<endl;
    for(it =retPathTmp.end()-1;it != retPathTmp.begin() -1; it--){
        retPath.push_back(*it);
        cout<<(*it).row<<","<<(*it).col<<" ";
    }
    cout<<endl;

    vector<PathNode>().swap(retPathTmp);//释放内存
    return retPath;


}
#endif

//寻路，运用剪枝
vector<Jps::PathNode> Jps::FindPathPrune(PathNode _startNode,PathNode _endNode){
    //设置开始结束点
    startNode = _startNode;
    endNode = _endNode;

    vector<Direct> jumpDirs;//存放当前需要跳跃的方向
    vector<Direct>::iterator dirsIt;//用于检索反向树的迭代器
    PathNode jumpNode;//返回的跳点
    PathNode* jumpNodes = new PathNode[3];//返回的跳点组
    bool* nodeTyp;//返回的邻居类型

    PathNode currentNode;//当前节点
    vector<PathNode> openTree;//开放列表，关闭列表是用辅助地图各点的isfind属性维护的
    vector<PathNode>::iterator it;//用于迭代
    vector<PathNode>::iterator minF_iter;//存放最小f值节点

    currentNode =  startNode;//当前点为开始点
    pathMap[currentNode.row][currentNode.col]->isfind = true;

    //初始方向树（八个方向）
    for(int i=0;i <8;i++){
        jumpDirs.push_back( (Direct)i);
    }

    //寻路
    while(1){
        //利用当前点，以及parent方向，往所有“真。邻居“方向跳跃
        for(dirsIt = jumpDirs.begin();dirsIt != jumpDirs.end(); dirsIt++){
            PathNode nodeTmp = currentNode;//临时检索节点，用于当前方向上的检索
            while(1){//在当前方向一直寻找跳点，直到没有找到跳点
                cout<<"方向："<<(*dirsIt)<<" "<<endl;
                for(int i=0;i <3; i++){
                    jumpNodes[i] = nullNode;
                }
                if( *(dirsIt) == p_up|| *(dirsIt) == p_down|| *(dirsIt) == p_left|| *(dirsIt) == p_right){
                    jumpNodes[0] = JumpStraightBit(pathMap, nodeTmp, (*dirsIt) );
                }
                if( *(dirsIt) == p_leftup|| *(dirsIt) == p_leftdown|| *(dirsIt) == p_rightup|| *(dirsIt) == p_rightdown){
                    jumpNodes = JumpOblique(pathMap, nodeTmp, (*dirsIt) );
                }

                //从返回的节点中，搜寻跳点，并更新g、h、f值
                for(int i =0;i <3; i++){
                    //如果返回的是有效节点，且不在关闭列表中（未找过）
                    if(false == jumpNodes[i].isnull &&
                       false == pathMap[jumpNodes[i].row][jumpNodes[i].col]->isfind &&//不在关闭列表中（未找过）
                       false == jumpNodes[i].isMidJumpNode)//不是中间跳点
                       {
                        PathNode parentTmp;//临时父节点，用于计算g值，判断是否更新父节点
                        if(pathMap[nodeTmp.row][nodeTmp.col]->isMidJumpNode){//如果发现当前点为中间跳点,就把后续节点的父节点设为当前节点的父节点
                            parentTmp.row = pathMap[nodeTmp.row][nodeTmp.col]->parent->row;
                            parentTmp.col = pathMap[nodeTmp.row][nodeTmp.col]->parent->col;
                        }
                        if(false == pathMap[nodeTmp.row][nodeTmp.col]->isMidJumpNode){//如果发现当前点不是中间跳点
                            parentTmp.row = pathMap[nodeTmp.row][nodeTmp.col]->row;
                            parentTmp.col = pathMap[nodeTmp.row][nodeTmp.col]->col;
                        }
                        jumpNodes[i].g = pathMap[parentTmp.row][parentTmp.col]->g +GetDis( parentTmp, jumpNode);
                        //如果该点已经在开放列表中，比较g值，取g值较小的点的属性，并不用再次加入开放列表
                        if(pathMap[jumpNodes[i].row][jumpNodes[i].col]->inopen){
                            if(pathMap[jumpNodes[i].row][jumpNodes[i].col]->g > jumpNodes[i].g){
                                pathMap[jumpNodes[i].row][jumpNodes[i].col]->g = jumpNodes[i].g;
                                pathMap[jumpNodes[i].row][jumpNodes[i].col]->GetF();

                                pathMap[jumpNodes[i].row][jumpNodes[i].col]->parent = pathMap[parentTmp.row][parentTmp.col];
                            }

                        }
                        //如果不在开放列表中
                        if(false == pathMap[jumpNodes[i].row][jumpNodes[i].col]->inopen){
                            jumpNodes[i].h = GetH(jumpNodes[i], endNode);
                            jumpNodes[i].GetF();
                            jumpNodes[i].inopen = true;

                            //将探测点加入开放列表
                            openTree.push_back(jumpNodes[i]);
                            //更新辅助地图中对应探测点的节点属性
                            pathMap[jumpNodes[i].row][jumpNodes[i].col]->g = jumpNodes[i].g;
                            pathMap[jumpNodes[i].row][jumpNodes[i].col]->h = jumpNodes[i].h;
                            pathMap[jumpNodes[i].row][jumpNodes[i].col]->f = jumpNodes[i].f;
                            pathMap[jumpNodes[i].row][jumpNodes[i].col]->parent = pathMap[parentTmp.row][parentTmp.col];
                            pathMap[jumpNodes[i].row][jumpNodes[i].col]->inopen = jumpNodes[i].inopen;
                        }

                    }//end--//如果返回的是有效节点，且不在关闭列表中（未找过）


                }

                if(jumpNodes[0].isMidJumpNode){//如果找到中间跳点，标记辅助地图中该点的中间跳点属性为true
                    pathMap[jumpNodes[0].row][jumpNodes[0].col]->isMidJumpNode = true;
                    if(pathMap[nodeTmp.row][nodeTmp.col]->isMidJumpNode)//如果当前检索点为中间跳点，则设置找到的中间跳点的父节点为当前检索点的父节点
                        pathMap[jumpNodes[0].row][jumpNodes[0].col]->parent = pathMap[nodeTmp.row][nodeTmp.col]->parent;
                    if(false == pathMap[nodeTmp.row][nodeTmp.col]->isMidJumpNode)
                        pathMap[jumpNodes[0].row][jumpNodes[0].col]->parent = pathMap[nodeTmp.row][nodeTmp.col];
                }
                //如果找到跳点，把当前检索点设为该跳点
                if(false == jumpNodes[0].isnull){
                    nodeTmp.row = jumpNodes[0].row;
                    nodeTmp.col = jumpNodes[0].col;
                }

                if(jumpNodes[0].isnull) break;//如果没有找到跳点，就跳出当前方向的循环

            }//end--while(1)//在当前方向一直寻找跳点

        }//end---for(dirsIt = jumpDirs.begin();dirsIt != jumpDirs.end(); dirsIt++)


        if(openTree.size() == 0) break;
        //找下一点
        minF_iter = openTree.begin();
        cout<<endl<<"找下一点"<<endl;
        for(it =openTree.begin();it !=openTree.end(); it++){
            cout<<(*it).row<<","<<(*it).col<<endl;
            if(pathMap[(*it).row][(*it).col]->f < pathMap[(*minF_iter).row][(*minF_iter).col]->f){
                minF_iter = it;
            }
        }

        cout<<endl<<"找到的下一点: ";
        cout<<(*minF_iter).row<<","<<(*minF_iter).col<<endl;

        currentNode = (*minF_iter);

        pathMap[currentNode.row][currentNode.col]->isfind = true;

        if(currentNode.row == endNode.row && currentNode.col == endNode.col) break;

        openTree.erase(minF_iter);

        //获取当前节点即将要搜寻的方向，jumpDirs
        jumpDirs.clear();
        int delta_y = currentNode.row - pathMap[currentNode.row][currentNode.col]->parent->row;
        int delta_x = currentNode.col - pathMap[currentNode.row][currentNode.col]->parent->col;
        char p;//单元地图中父点
        short unitMap = 0;
        char neighbGroup[9] = {9,9,9,9,9,9,9,9,9};//单元地图中,要探测的邻居组，初始化为非(0-8)的值，为9的点为不可行点

        if(0 > delta_y && 0 ==delta_x) p = 7;//搜寻方向为上
        if(0 < delta_y && 0 ==delta_x) p = 1;//搜寻方向为下
        if(0 == delta_y && 0 >delta_x) p = 3;//搜寻方向为左
        if(0 > delta_y && 0 <delta_x) p = 5;//搜寻方向为右

        if(0 > delta_y && 0 >delta_x) p = 8;//搜寻方向为左上
        if(0 < delta_y && 0 >delta_x) p = 2;//搜寻方向为左下
        if(0 > delta_y && 0 <delta_x) p = 6;//搜寻方向为右上
        if(0 < delta_y && 0 <delta_x) p = 0;//

        //获取搜寻点周围3x3单元地图，并找到邻居组

        for(int i = 0;i <3; i++){
            for(int j= 0;j <3; j++){
                int row_t = currentNode.row +i-1;//获取周围的点坐标
                int col_t = currentNode.col +j-1;
                if(height > row_t && 0 <= row_t &&
                    width > col_t && 0 <= col_t
                    ){//确保周围点没超出地图
                    int valueT = pathMap[row_t][col_t]->value;
                    unitMap = unitMap|valueT<<(i*3 +j);
                    if(1 != valueT){//不为障碍
                        neighbGroup[i*3+j] = (i*3 +j);
                    }
                }
            }
        }//end-获取搜寻点周围3x3单元地图，并找到邻居组

        //获取当前搜寻点周围点类型，并赋值探测方向组
        for(int i=0;i <9;i++){
            if(9 != neighbGroup[i] &&
               p != neighbGroup[i] &&
               4 != neighbGroup[i]
               ){//如果邻居组中点不为：空(9)、当前搜寻点(4)、父节点
                nodeTyp = Prune(unitMap, p, neighbGroup[i]);
                if(1 == nodeTyp[0]){//如果存在关键邻居，就向探测方向组中加入当前方向
                    if(1==i) jumpDirs.push_back(p_up);
                    if(7==i) jumpDirs.push_back(p_down);
                    if(3==i) jumpDirs.push_back(p_left);
                    if(5==i) jumpDirs.push_back(p_right);

                    if(0==i) jumpDirs.push_back(p_leftup);
                    if(6==i) jumpDirs.push_back(p_leftdown);
                    if(2==i) jumpDirs.push_back(p_rightup);
                    if(8==i) jumpDirs.push_back(p_rightdown);

                }

            }
        }//end-获取当前搜寻点周围点类型，并赋值探测方向组

        //system("pause");
    }//end-寻路-while(1)

    //返回路径
    vector<PathNode> retPathTmp;
    vector<PathNode> retPath;
    PathNode nodeTmp = endNode;
    while(1){
        int row_t = nodeTmp.row;
        int col_t = nodeTmp.col;
        retPathTmp.push_back(nodeTmp);
        if(NULL == pathMap[nodeTmp.row][nodeTmp.col]->parent) break;
        nodeTmp.row = pathMap[row_t][col_t]->parent->row;
        nodeTmp.col = pathMap[row_t][col_t]->parent->col;
    }
    //将路径整理为从开始点出发的顺序
    cout<<endl;
    for(it =retPathTmp.end()-1;it != retPathTmp.begin() -1; it--){
        retPath.push_back(*it);
        cout<<(*it).row<<","<<(*it).col<<" ";
    }
    cout<<endl;

    vector<PathNode>().swap(retPathTmp);//释放内存
    return retPath;




}
