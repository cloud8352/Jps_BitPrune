#include <iostream>
#include<windows.h>
#include<vector>
#include<cmath>

using namespace std;
using std::vector;

class Jps
{
public:

    //方向枚举
    enum Direct{
        p_up,p_down,p_left,p_right,p_leftup,p_leftdown,p_rightup,p_rightdown
    };

    //辅助地图节点
    struct PathNode{
        int row;//行
        int col;
        int g,h,f;
        void GetF(){
            f = g + h;
        }
        int     value;//
        bool    isroute;//是否是最短路径中的一点
        bool    isfind;//是否走过
        bool    inopen;
        bool    isnull;//是否是空节点
        PathNode* parent;//父节点
        vector<PathNode*> keyNeighbours; //关键邻居节点
    };

    int height,width;

    PathNode*** pathMap;//辅助地图
    PathNode startNode;
    PathNode endNode;

    PathNode nullNode;//空节点


    //计算两点直线距离
    int GetDis(const PathNode& startNode,const PathNode& endNode){
        int dis = sqrt( pow( (endNode.col -startNode.col),2) +pow( (endNode.row -startNode.row),2) )*10;//pow次方函数
        return dis;
    }
    //计算h值
    int GetH(const PathNode& startNode,const PathNode& endNode){
        int x = abs(startNode.col - endNode.col);//取水平距离差绝对值
        int y = abs(startNode.row - endNode.row);//取竖直距离差绝对值
        return (x + y)*10;
    }


    bool* Prune(short unitMap,char p,char n);
    void Init(int **_map,int _height,int _width);
    PathNode JumpStraight(PathNode*** _pathMap,PathNode currenNode,Direct dir);
    PathNode JumpOblique(PathNode*** _pathMap,PathNode currenNode,Direct dir);
    vector<PathNode> FindPath(PathNode _startNode,PathNode _endNode);

    PathNode JumpStraightBit(PathNode*** _pathMap,PathNode currenNode,Direct dir);//利用位运算优化后的，直线寻跳点

};
