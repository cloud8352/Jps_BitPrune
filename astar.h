#include <iostream>
#include<windows.h>
#include<vector>
#include<cmath>

using namespace std;
using std::vector;

class Astar
{
public:
    struct MyPoint{
        int row;//行
        int col;
        int g,h,f;
        void GetF(){
            f = g + h;
        }
    };
    //方向枚举
    enum Direct{
        p_up,p_down,p_left,p_right,p_leftup,p_leftdown,p_rightup,p_rightdown
    };
    //辅助地图节点
    struct PathNode{
        int     value;//
        bool    isroute;//是否是最短路径中的一点
        bool    isfind;//是否走过
    };
    //树的节点类型
    struct MyTreeNode{
        MyPoint pos;
        MyTreeNode* parent;
        vector<MyTreeNode*> child;//用于存入该点的周围可行点？、、、、、、、、
    };

    int VerticalDist; //每格到相邻格直线距离10
    int ObliqueDist;  //每格到相邻格斜线距离14

    int ROW,COL;//地图数组行数，列数
    int **map;//地图数组

    //建立辅助地图
    PathNode **pathMap;
    MyPoint beginPoint;
    MyPoint endPoint;

    MyTreeNode* beginTreeNode;//开放列表的起始节点，即开始节点

    //创建指针：当前点、探路点
    MyTreeNode* pTemp;//当前点
    MyTreeNode* pTempChild;//探路点

    //创建数组，保存子节点
    vector<MyTreeNode*> openTree;//开放列表

    vector<MyTreeNode*>::iterator minF_Iter;//用于存放最小f值可行点的迭代器
    vector<MyTreeNode*>::iterator it;//用于检索的迭代器

    //该点是否可行，可行返回true
    bool isRoad(const MyPoint& point,PathNode **_pathMap){
        if(point.col <0 || point.col >= COL ||
           point.row <0 || point.row >= ROW
           )//超出地图
           return false;
        if(1 == _pathMap[point.row][point.col].value)//该点为障碍
            return false;
        if(_pathMap[point.row][point.col].isfind)//该点已经走过
            return false;
        return true;
    }

    //判断点(row,col)是否为障碍物
    bool isBarrier(int row, int col, PathNode **_pathMap){
        if(col <0 || col >= COL ||
           row <0 || row >= ROW
           )//超出地图
           return true;
        if(1 == _pathMap[row][col].value)//该点为障碍
            return true;
        return false;
    }

    //计算h值
    int GetH(const MyPoint& point,const MyPoint& endpos){
        int x = abs(point.col - endpos.col);//取水平距离差绝对值
        int y = abs(point.row - endpos.row);//取竖直距离差绝对值
        return (x + y)*VerticalDist;
    }

    void Init(int **_map,int height,int width,MyPoint _beginPoint,MyPoint _endPoint);
    void FindPath();

};
