#include "Rail.h"
#include <algorithm>
#include <fstream>

using namespace std;
static constexpr char PATH[] = { "Data/CSV/Rail/rail.csv" };

CRail::CRail()
{
    //作られた瞬間にレール初期化
    Init();
}

void CRail::Init()
{
    m_selectIndex = -1;

    m_points.clear();

    //仮置き
    // [0] 始点
    // [1] 中間
    // [2] 曲がる場所
    // [3] 終点
    //↓これをいずれゲーム内で設置できるようにします
   /* m_points.push_back(VGet(0, 10, 0));
    m_points.push_back(VGet(30, 15, 30));
    m_points.push_back(VGet(60, 25, 50));
    m_points.push_back(VGet(100, 10, 80));*/
    //size() = 4
    LoadCSV();

    if (m_points.empty())
    {
        AddPoint(0,0,0);
        AddPoint(20,0,20);
    }
}


//------------------------
// CSVのロード
//------------------------
void CRail::LoadCSV()
{
    ifstream file(PATH);

    if (!file.is_open())return;

    float x, y, z;
    char comma;

    while (file
        >> x>> comma
        >> y>> comma
        >> z)
    {
        AddPoint(x, y, z);
    }
}

//------------------------
// CSVのセーブ
//------------------------
void CRail::SaveCSV()
{
    ofstream file(PATH);

    for (auto& p : m_points)
    {
        file << p.x << ","
             << p.y << ","
             << p.z << "\n";
    }
}

VECTOR CRail::GetRailPos(float t)
{
    int count = (int)m_points.size() - 1;//レール点の数

    int index = (int)t;                  //整数だけほしい(例1.8でも１になりる)
    float localT = t - index;            //小数部分(例　0.8)つまり80%進んだ

    if (index >= count)
        return m_points.back();          //終点を超えたら終点を返す（落ちないようにしている）スプラのインク貯まり的な


    //スプライン曲線をやっていくよ(簡単にいうとつなぎ目で書くつかないようにした線の事)カーブ
    /*
    p0 = 0      前
    p1 = 1      今
    p2 = 2      次
    p3 = 3      次の次
    */
    VECTOR p0 = m_points[(max)(index - 1, 0)];
    VECTOR p1 = m_points[index];                        //今の位置
    VECTOR p2 = m_points[(min)(index + 1, count)];      //少し先の位置
    VECTOR p3 = m_points[(min)(index + 2, count)];

    float tt = localT * localT;   //tの2乗
    float ttt = tt * localT;      //tの3乗

    VECTOR result;

    result.x =  0.5f * (
        (2 * p1.x) +                                    //今の点を基準にする(適当な数を代入するとわかりやすいかも)
        (-p0.x + p2.x) * localT +                       //前から次への向き　　* localTは進行率です
        (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * tt +  //カーブの保管部分（tt = localT²)ですので進むほど曲がっているように見える
        (-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * ttt);    //(ttt = localT³)なのでさらに細かくする

    result.y = 0.5f * (
        (2 * p1.y) +
        (-p0.y + p2.y) * localT +
        (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * tt +
        (-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * ttt);

    result.z =0.5f * (
        (2 * p1.z) +
        (-p0.z + p2.z) * localT +
        (2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z) * tt +
        (-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * ttt);

    return result;
}

void CRail::Draw()
{

    //点の表示
    for (int i = 0; i < m_points.size(); i++)
    {
        DrawSphere3D( m_points[i], 2.0f, 8, GetColor(255, 0, 0),GetColor(255, 0, 0),TRUE);
    }
    //つなぎの線
    for (float t = 0; t < m_points.size() - 1; t += 0.02f)
    {
        VECTOR p1 = GetRailPos(t);
        VECTOR p2 = GetRailPos(t + 0.02f);

        DrawLine3D( p1, p2, GetColor(0, 255, 255));
    }

    DrawString(100,100,"左クリックで配置\n右クリックで削除\nGで保存\n一応ドラッグ移動できますよ",GetColor(250,250,250));
}

/*＠memu
曲線化をしていきました(スプライン曲線)
一応ほかの人が見て理解できるように説明多めに書きます。
計算式はこちら
(例)
GetRailPos(1.5f)とした場合
つまり10 と 20 の真ん中です。

①int index = (int)t;
    t = 1.5
    index = 1　という形で50%進んだ位置ということです。

②tt = localT²   （0.5 * 0.5 = 0.25）
　ttt = tt * localT(0.25 * 0.5 = 0.125)

 result.x =0.5f *
(
    (2 * p1.x) +
    (-p0.x + p2.x) * localT +
    (2*p0.x -5*p1.x +4*p2.x -p3.x) * tt +
    (-p0.x +3*p1.x -3*p2.x +p3.x) * ttt
);

//----------------------
// １直線の場合
//----------------------
①p0 = 0
②p1 = 10
③p2 = 20
④p3 = 30   とします。

① (2 * p1.x)　                  // 2 * 10 = 20
② (-p0 + p2) * localT           // (-0 + 20) * 0.5                // 20 * 0.5= 10
③(2p0 -5p1 +4p2 -p3) * tt       // (2*0 -5*10 +4* 20 -30)* 0.25   //(0 -50 +80 -30)*0.25 //0 *0.25 = 0
④↑の式より　　=0

①+②+③+④ = 30

全部足すと　30となります。
0.5 * 30より15となり
10と20の間であることがわかります。

//----------------------
// 曲線の場合
//----------------------
0
10
50
30
といった場合は↑の計算

曲線補正0ではなくなるので曲がる補正が働く事になります

*/


//軽いエディターを作ります
//CSVで読み込んだデータを
void CRail::AddPoint( float x, float y,float z)
{
    m_points.push_back( VGet(x,y,z)); 
}


//-----------------------------
// 更新処理
//-----------------------------
void CRail::Update()
{
    static int oldMouse = 0;

    int mouse = GetMouseInput();
    VECTOR mousePos = GetMouseWorldPos();

    //--------------------------------
    // 左クリック
    //--------------------------------
    if ((mouse & MOUSE_INPUT_LEFT) &&
        !(oldMouse & MOUSE_INPUT_LEFT))
    {
        bool hit = false;

        // 点選択
        for (int i = 0; i < m_points.size(); i++)
        {
            //ポイントとの距離
            float dist = VSize( VSub( m_points[i], mousePos));

            if (dist < 5.0f)
            {
                m_selectIndex = i; 
                hit = true;
                break;
            }
        }

        // 何も選択してなければ追加
        if (!hit)
        {
            AddPoint( mousePos.x,mousePos.y, mousePos.z);
        }
    }

    //--------------------------------
    // ドラッグ移動
    //--------------------------------
    if ((mouse & MOUSE_INPUT_LEFT) && m_selectIndex != -1)
    {
        m_points[m_selectIndex] = mousePos;
    }

    //--------------------------------
    // 離した
    //--------------------------------
    if (!(mouse & MOUSE_INPUT_LEFT))
    {
        m_selectIndex = -1;
    }

    //--------------------------------
    // 右クリック削除
    //--------------------------------
    if ((mouse & MOUSE_INPUT_RIGHT) &&
        !(oldMouse & MOUSE_INPUT_RIGHT))
    {
        for (int i = 0; i < m_points.size(); i++)
        {
            float dist = VSize(VSub(m_points[i],mousePos));

            if (dist < 5.0f)
            {
                m_points.erase( m_points.begin() + i);
                break;
            }
        }
    }

    //--------------------------------
    // Gキー保存
    //--------------------------------
    if (CheckHitKey(KEY_INPUT_G))
    {
        SaveCSV();
    }

    oldMouse = mouse;
}


//-----------------------------
// マウスのワールド座標
//-----------------------------
VECTOR CRail::GetMouseWorldPos()
{
    int x, y;

    GetMousePoint(&x, &y);

    VECTOR nearPos = ConvScreenPosToWorldPos( VGet((float)x, (float)y, 0.0f));
    VECTOR farPos  = ConvScreenPosToWorldPos(VGet((float)x, (float)y, 1.0f));

    VECTOR dir = VSub(farPos, nearPos);

    float t = -nearPos.y / dir.y;

    VECTOR pos;

    pos.x = nearPos.x + dir.x * t;
    pos.y = 0.0f;
    pos.z = nearPos.z + dir.z * t;

    return pos;
}