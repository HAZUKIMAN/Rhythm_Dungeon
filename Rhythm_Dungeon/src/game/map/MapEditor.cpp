#include"MapEditor.h"
#include <corecrt_math.h>
#include "../../lib/Input/Input.h"


// コンストラクタ
MapEditor::MapEditor()
{
    Init();
}
// デストラクタ
MapEditor::~MapEditor()
{
    Fin();
}

//-------------------------------
//		初期化
//-------------------------------
void MapEditor::Init()
{
    m_iModelHdl = -1;
    m_iModelHdl_Wall = -1;

    memset(map, 0, sizeof(map));
}


//-------------------------------
//		データロード
//-------------------------------
void MapEditor::Load()
{
    VECTOR size = VGet(0.05f, 0.05f, 0.05f);

    if (m_iModelHdl == -1)
    {
        m_iModelHdl = MV1LoadModel("Data/object/stage/StageTexture.mv1");
        m_iModelHdl_Wall = MV1LoadModel("Data/object/stage/Wall.mv1");
    }

    LoadMap("map.dat");

    MV1SetScale(m_iModelHdl, size);
    MV1SetScale(m_iModelHdl_Wall, size);
}

//--------------------------------
//      実行処理
//--------------------------------
int  MapEditor::Step()
{
    //データの保存・読み取り
    if (Input::Key::Push(KEY_INPUT_P)) {
        SaveMap("map.dat");
    }

    if (Input::Key::Push(KEY_INPUT_L)) {
        LoadMap("map.dat");
    }
   

    return 0;
}


//--------------------------------
//      更新処理
//---------------------------------
void MapEditor::Update()
{
    //ビット
    //MOUSE_INPUT_LEFT;   // 左クリック
    //MOUSE_INPUT_RIGHT;  // 右クリック
    //MOUSE_INPUT_MIDDLE; // 中クリック

    int mouseState = GetMouseInput();

    bool needRebuild = false;//生成時のみtrue

    VECTOR hitPos;

    //マウスの位置当たり判定
    if (GetMouseHitPosition(&hitPos)) {

        printf("Hit: %f %f %f\n", hitPos.x, hitPos.y, hitPos.z);

        int gx, gz;

        if (GetGridPos(hitPos, &gx, &gz)) {

            // 左クリック（押した瞬間）
            //今押してる状態かつ前は押してない状態
            if ((mouseState & MOUSE_INPUT_LEFT)) {

                map[gz][gx] = TILE_FLOOR;
                needRebuild = true;
            }
            // 右クリック
            //今押してる状態かつ前は押してない状態
            if ((mouseState & MOUSE_INPUT_RIGHT)) {

                map[gz][gx] = TILE_WALL;
                needRebuild = true;
            }
            // 中クリック
            //今押してる状態かつ前は押してない状態
            if ((mouseState & MOUSE_INPUT_MIDDLE)) {

                map[gz][gx] = TILE_NONE;
                needRebuild = true;
            }

        }
    }

    //モデルの生成時のみ
    if (needRebuild) {
        BuildInstances();
        needRebuild = false;
    }
}

//-------------------------------
//		描画
//-------------------------------
void MapEditor::Draw()
{
   
    //ブロックの表示
    //インスタンスで配置情報だけをとって表示
    for (auto& inst : instances) {
        MV1SetPosition(inst.m_iModelHdl, inst.m_vPosition);         //床用
        //MV1SetPosition(inst.m_iModelHdl_Wall, inst.m_vPosition);  //壁用
        MV1DrawModel(inst.m_iModelHdl);
    }

    //選択しているところを赤くする
    DrawSelectedTile();

    //ザップフャイルの作成
    DrawString(1400,100,"Pでセーブ\nLでロード",RED);
    //ザップフャイルの作成
    DrawString(200, 100, "左クリックかつOでプレイヤーの位置用のオブジェクトを配置\n左クリックかつUでオブジェクト関係の削除", RED);

}


//-------------------------------
//		終了処理
//-------------------------------
void MapEditor::Fin()
{
    if (m_iModelHdl != -1)
    {
        MV1DeleteModel(m_iModelHdl);
        m_iModelHdl = -1;
    }
    if (m_iModelHdl_Wall != -1)
    {
        MV1DeleteModel(m_iModelHdl_Wall);
        m_iModelHdl_Wall = -1;
    }
}


//------------------------------------
//          データの保存
//------------------------------------
void MapEditor::SaveMap(const char* filename)
{
    //-----------------------------
    // 個人作成なのでバイナリで
    //-----------------------------
    FILE* fp;

    fopen_s(&fp, filename, "wb");

    if (fp == NULL) return;

    // サイズも一緒に保存（重要）
    fwrite(&MAP_W, sizeof(int), 1, fp);
    fwrite(&MAP_H, sizeof(int), 1, fp);

    // マップ本体
    fwrite(map, sizeof(TileType), MAP_W * MAP_H, fp);

}


//------------------------------------
//      マップの読み取り
//------------------------------------
void MapEditor::LoadMap(const char* filename)
{
   //----------------------------
   // 個人作成なのでバイナリで
   //----------------------------
    FILE* fp;

    fopen_s(&fp , filename, "rb");

    if (fp == NULL) return;

    int w, h;
    fread(&w, sizeof(int), 1, fp);
    fread(&h, sizeof(int), 1, fp);

    // サイズチェック（安全対策）
    if (w != MAP_W || h != MAP_H) {
        fclose(fp);
        return;
    }

    fread(map, sizeof(TileType), MAP_W * MAP_H, fp);

    fclose(fp);

    BuildInstances();

    
}

//-----------------------------------
//      マウスの当たり判定の取得
//-----------------------------------
bool MapEditor::GetMouseHitPosition(VECTOR* outPos)
{
   //@メモ
   //---------------------------
   /* レイの式
    P = near + t * (far - near)
    y=0 にぶつかる t
    t = -near.y / (far.y - near.y)*///レイキャスト(簡単に言ってせんで捉える)
    //-----------------------------
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    VECTOR nearPos, farPos;
    nearPos= ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 0.0f));
    farPos = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 1.0f));

    VECTOR dir = VSub(farPos, nearPos);

    // 地面と平行ならヒットしない
    if (fabs(dir.y) < 0.0001f) return false;

    float t = -nearPos.y / dir.y; 

    // カメラの後ろは無視
    if (t < 0.0f) return false;

    // 交点
    *outPos = VAdd(nearPos, VScale(dir, t));

    return true;
}

//---------------------------------
//         グリッドの取得
//---------------------------------
bool MapEditor::GetGridPos(VECTOR hitPos, int* pos_x, int* pos_z)
{
    *pos_x = (int)floor(hitPos.x / TILE_SIZE);//グリッドズレずれを起こさない(-にも対応できるようにfloor)
    *pos_z = (int)floor(hitPos.z / TILE_SIZE);//グリッドズレずれを起こさない(-にも対応できるようにfloor)

    // 範囲チェック（超重要）
    if (*pos_x < 0 || *pos_x >= MAP_W ||
        *pos_z < 0 || *pos_z >= MAP_H) {
        return false;
    }

    return true;
}

//塗りつぶしなし（枠だけ）
//選択しているマスを赤く表示する
void MapEditor::DrawSelectedTile()
{
    VECTOR hitPos;

    if (GetMouseHitPosition(&hitPos)) {

        int pos_x, pos_z;

        if (GetGridPos(hitPos, &pos_x, &pos_z)) {

            float num0_x = pos_x * TILE_SIZE;
            float num0_z = pos_z * TILE_SIZE;

            float num1_x = num0_x + TILE_SIZE;
            float num1_z = num0_z + TILE_SIZE;

            float num0_y = 0.1f;   // 少し浮かせる（Zファイティング防止）意味：地面と重なるとチラつく
            float num1_y = 1.1f;

            DrawCube3D(VGet(num0_x, num0_y, num0_z),VGet(num1_x, num1_y, num1_z),
                GetColor(255, 0, 0), GetColor(255, 0, 0),FALSE);

        }
    }
}


//----------------------------
//  インスタンスの生成用
//----------------------------
void MapEditor::BuildInstances()
{
    //中身をクリア
    instances.clear();

    for (int z = 0; z < MAP_H; z++) {
        for (int x = 0; x < MAP_W; x++) {

            float worldX = (x + 0.5f) * TILE_SIZE;
            float worldZ = (z + 0.5f) * TILE_SIZE;

            //床用(角度とスケールも足してよし！！今は使ってないけどね)
            if (map[z][x] == TILE_FLOOR) {
                instances.push_back({ m_iModelHdl, VGet(worldX, 0, worldZ)});
            }

            //壁用(角度とスケールも足してよし！！今は使ってないけどね)
            if (map[z][x] == TILE_WALL) {
                instances.push_back({ m_iModelHdl_Wall,VGet(worldX, WALL_HIGHT, worldZ)});
            }

        }
    }
}

int MapEditor::GetMap(int z, int x)const
{
    return map[z][x];
}

//--------------------------------
// マップ設定
//--------------------------------
void MapEditor::SetMap(int z, int x, int value)
{
    map[z][x] = (TileType)value;
}


