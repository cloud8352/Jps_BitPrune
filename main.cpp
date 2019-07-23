#include <iostream> //"#"代表预处理命令
#include"astar.h"
#include"jps.h"
using namespace std;    //使用standard命名空间


int main(){
    //行row，列col
    int height = 8;
    int width = 15;
    int test_map[height][width] = {
        {1,0,1,1,0,0,0,0,0,0,0,1,1,0,1},//0
        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},//1
        {0,0,0,1,0,0,1,1,0,0,0,1,1,0,0},//2
        {0,0,0,1,0,0,1,1,0,0,1,1,0,0,0},//3
        {0,0,0,1,0,0,1,1,0,1,0,0,1,1,0},//4
        {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},//5
        {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},//6
        {1,0,1,0,0,1,1,1,0,0,0,0,0,0,0}//7

    };

    //地图转换成二维指针
    int **a;
    a = new int* [height];
    for(int i=0;i < height;i++){
        a[i] = new int[width];
        for(int j=0;j < width;j++){
            a[i][j] = test_map[i][j];
        }
    }

    Astar::MyPoint startPoint = {1,1};
    Astar::MyPoint endPoint = {6,14};

    Astar astar;
    astar.Init(a, height, width, startPoint, endPoint);

    astar.FindPath();


    //JPS
    cout<<"------------JPS---------------"<<"\n";
    Jps jps;
    Jps::PathNode jStart = {6, 1};
    Jps::PathNode jEnd = {5, 13};

    jps.Init(a, 8, 15);

    cout<<"--------JumpStraightBit---------"<<"\n";
    Jps::PathNode jumpNode;
    jumpNode = jps.JumpStraightBit(jps.pathMap,jStart,Jps::p_right);
    if(false == jumpNode.isnull)
    cout<<"jumpNode.row,jumpNode.col:"<<jumpNode.row<<","<<jumpNode.col<<endl;

    cout<<"--------FindPathPrune---------"<<"\n";
    jps.FindPathPrune(jStart, jEnd);

    system("pause");
    return 0;
}
