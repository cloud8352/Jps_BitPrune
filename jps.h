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
        bool    isMidJumpNode;//是否是中间跳点
        PathNode* parent;//父节点
        vector<PathNode*> keyNeighbours; //关键邻居节点
    };

    int height,width;

    PathNode*** pathMap;//辅助地图
    PathNode startNode;
    PathNode endNode;

    PathNode nullNode;//空节点

    vector<PathNode> retPath;//储存最终路径

    //计算两点直线距离
    int GetDis(const PathNode& _startNode,const PathNode& _endNode){
        int dis = sqrt( pow( (_endNode.col -_startNode.col),2) +pow( (_endNode.row -_startNode.row),2) )*10;//pow次方函数
        return dis;
    }
    //计算h值
    int GetH(const PathNode& _startNode,const PathNode& _endNode){
        int x = abs(_startNode.col - _endNode.col);//取水平距离差绝对值
        int y = abs(_startNode.row - _endNode.row);//取竖直距离差绝对值
        return (x + y)*10;
    }


    bool* Prune(short unitMap,char p,char n);
    void Init(int **_map,int _height,int _width);
    PathNode JumpStraight(PathNode*** _pathMap,PathNode currenNode,Direct dir);
    PathNode* JumpOblique(PathNode*** _pathMap,PathNode currenNode,Direct dir);
    vector<PathNode> FindPath(PathNode _startNode,PathNode _endNode);

    PathNode JumpStraightBit(PathNode*** _pathMap,PathNode currenNode,Direct dir);//利用位运算优化后的，直线寻跳点
    vector<PathNode> FindPathPrune(PathNode _startNode,PathNode _endNode);//寻路运用剪枝
	void PrintRoute();

};
