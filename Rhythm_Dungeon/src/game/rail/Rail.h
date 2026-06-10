#pragma once
#include "Comon.h"
#include "DxLib.h"
#include <vector>

class CRail
{
public:
    CRail();

    void Init();
    void Draw();
    void Update();

    VECTOR GetRailPos(float t);

    void SaveCSV();
    void LoadCSV();


private:

    void AddPoint(float x, float y, float z);

    VECTOR GetMouseWorldPos();

    std::vector<VECTOR> m_points;

    int m_selectIndex;
};

