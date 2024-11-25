#pragma once

#include <vector>

using namespace std;
using std::vector;

// 方向枚举
enum DirectionEnum
{
    Up,
    Down,
    Left,
    Right,
    LeftUp,
    LeftDown,
    RightUp,
    RightDown
};
const vector<DirectionEnum> DirectionEnums =
    {Up, Down, Left, Right, LeftUp, LeftDown, RightUp, RightDown};