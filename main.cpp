
#include "AStar.h"
#include "BitPruneJps.h"

#include <iostream> //"#"代表预处理命令
#include <cstring>
#include <fstream> //读写头文件
#include <chrono>

using namespace std; // 使用standard命名空间
using namespace std::chrono;

// 地图二维指针数组
int **MapTable = nullptr;
unsigned int MapWidth = 0;
unsigned int MapHeight = 0;

#ifdef _WIN32
void waitForContinue()
{
    system("pause");
}
#else
#include <termios.h>

void getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(0, &oldattr); // 获取当前的终端设置
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // 设置为非标准模式
    tcsetattr(0, TCSANOW, &newattr);     // 设置新的终端设置

    ch = getchar(); // 读取一个字符

    tcsetattr(0, TCSANOW, &oldattr); // 恢复原来的终端设置
    std::cout << "\n";               // 输出换行，用于清除回车字符
}

void waitForContinue()
{
    cout << "按任意键继续" << endl;
    getch();
}
#endif

void deleteMap()
{
    for (int y = 0; y < MapHeight; y++)
    {
        delete[] MapTable[y];
    }
    delete[] MapTable;
    MapTable = nullptr;
}

bool loadMap(const string &path, unsigned int width, unsigned int height)
{
    if (MapTable)
    {
        deleteMap();
    }

    // 读取地图
    ifstream fin(path.c_str());
    if (!fin)
    {
        cout << endl
             << "文件不存在" << endl;
        return false;
    }

    MapTable = new int *[height];
    for (int i = 0; i < height; i++)
    {
        MapTable[i] = new int[width];
        for (int j = 0; j < width; j++)
        {
            char c;
            fin >> c;
            if ('.' == c)
                MapTable[i][j] = 0;
            else
                MapTable[i][j] = 1;
            cout << MapTable[i][j];
        }
        cout << endl;
    }
    MapWidth = width;
    MapHeight = height;

    return true;
}

int main()
{
    system("chcp 65001");
    system("mode con cols=120 lines=600");
    // 行row，列col
    int width = 101;
    int height = 101;

    int start_x = 60, start_y = 1;
    int end_x = 60, end_y = 99;
    cout << "地图尺寸（width*height):" << width << "*" << height;
    cout << endl
         << "开始点（x，y）：" << start_x << "," << start_y << endl;
    cout << "结束点（x，y）：" << end_x << "," << end_y << endl;

    // 时间记录ms
    time_point beginTimePoint = std::chrono::high_resolution_clock::now();
    time_point endTimePoint = beginTimePoint;

    bool succeed = loadMap("map/map101x101.txt", width, height);
    if (!succeed)
    {
        return 0;
    }
    waitForContinue();

    AStar aStar;
    aStar.InitMap(MapTable, width, height);
    // a星寻路开始时间
    beginTimePoint = std::chrono::high_resolution_clock::now();
    aStar.FindPath(start_x, start_y, end_x, end_y);

    // a星寻路结束时间
    endTimePoint = std::chrono::high_resolution_clock::now();
    milliseconds elapsedTime = duration_cast<milliseconds>(endTimePoint - beginTimePoint);
    cout << "a星寻路使用时间：" << elapsedTime.count() << "ms";

    aStar.PrintPath();
    aStar.PrintPathMap();
    waitForContinue();

    // BitPruneJps
    cout << "------------BitPruneJps---------------" << "\n";
    BitPruneJps jps;
    jps.InitMap(MapTable, height, width);
    // BitPruneJps 寻路开始时间
    beginTimePoint = std::chrono::high_resolution_clock::now();
    jps.FindPath(start_x, start_y, end_x, end_y);

    // BitPruneJps 寻路结束时间
    endTimePoint = std::chrono::high_resolution_clock::now();
    elapsedTime = duration_cast<milliseconds>(endTimePoint - beginTimePoint);
    cout << "BitPruneJps 寻路使用时间：" << elapsedTime.count() << "ms";
    jps.PrintPath();
    jps.PrintPathMap();

    waitForContinue();
    return 0;
}
