#include <iostream> //"#"代表预处理命令
#include<cstring>
#include<fstream>//读写头文件
#include<time.h>
#include<windows.h>
#include"astar.h"
#include"jps.h"
using namespace std;    //使用standard命名空间


int main(){
    system("mode con cols=120 lines=600");
    //行row，列col
    int height  = 50;
    int width   = 100;

    int start_x =1,start_y =1;
    int end_x   =6,end_y  =45;
    cout<<"地图尺寸（height*width）:"<<height<<"*"<<width;
    cout<<endl<<"开始点（y，x）："<<start_y<<","<<start_x<<endl;
    cout<<"结束点（y，x）："<<end_y<<","<<end_x<<endl;

    time_t time_start_ms,time_end_ms;//时间记录ms

    //读取地图
    string filepath="map/map50x100.txt";
    ifstream fin(filepath.c_str());
    if(!fin) {cout<<endl<<"文件不存在"<<endl; system("pause");}

    int **pMap;//地图二维指针数组
    pMap = new int* [height];
    for(int i=0;i < height;i++){
        pMap[i] = new int[width];
        for(int j=0;j < width;j++){
            char c;
            fin>>c;
            if('.' == c) pMap[i][j] = 0;
            else pMap[i][j] = 1;
            cout<<pMap[i][j];
        }
        cout<<endl;
    }
    system("pause");

    Astar::MyPoint startPoint = {start_y,start_x};
    Astar::MyPoint endPoint = {end_y, end_x};

    Astar astar;

    time_start_ms = clock();//a星寻路开始时间

    astar.Init(pMap, height, width, startPoint, endPoint);

    astar.FindPath();

    time_end_ms = clock();//a星寻路结束时间
    cout<<"a星寻路使用时间："<<difftime(time_end_ms, time_start_ms)<<"ms";

    astar.PrintRoute();
    astar.PrintRouteMap();

    system("pause");

    //JPS
    cout<<"------------JPS---------------"<<"\n";
    Jps jps;
    Jps::PathNode jStart = {start_y,start_x};
    Jps::PathNode jEnd = {end_y, end_x};

    time_start_ms = clock();//JpsPrune寻路开始时间

    jps.Init(pMap, height, width);
    cout<<"--------FindPathPrune---------"<<"\n";
    jps.FindPathPrune(jStart, jEnd);

    time_end_ms = clock();//JpsPrune寻路结束时间
    cout<<"JpsPrune寻路使用时间："<<difftime(time_end_ms, time_start_ms)<<"ms";
    jps.PrintRoute();

    system("pause");
    return 0;
}
