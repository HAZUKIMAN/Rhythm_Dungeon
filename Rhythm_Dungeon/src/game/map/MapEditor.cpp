#include"MapEditor.h"
#include <corecrt_math.h>
#include "../common.h"


constexpr int MAP_W = 50;
constexpr int MAP_H = 50;
constexpr float WALL_HIGHT = 5.0f;

constexpr float TILE_SIZE = 5.0f;

enum TileType {
    TILE_NONE,
    TILE_FLOOR,
    TILE_WALL
};

TileType map[MAP_H][MAP_W];

// コンストラクタ
MapEditor::MapEditor()
{

}
// デストラクタ
MapEditor::~MapEditor()
{

}

// 初期化
void MapEditor::Init()
{
    m_iModelHdl = -1;
}


// データロード
void MapEditor::Load()
{
    VECTOR size = VGet(0.05f, 0.05f, 0.05f);

    if (m_iModelHdl == -1)
    {
        m_iModelHdl = MV1LoadModel("Data/object/stage/StageTexture.mv1");
    }

    MV1SetScale(m_iModelHdl, size);
}


// 実行処理
int  MapEditor::Step()
{

//------------------------------------
//    データの保存・読み取り
//------------------------------------
    if (CheckHitKey(KEY_INPUT_S)) {
        SaveMap("map.dat");
    }

    if (CheckHitKey(KEY_INPUT_L)) {
        LoadMap("map.dat");
    }
   
    return 0;
}

// 更新処理
void MapEditor::Update()
{
    //ビット
    //MOUSE_INPUT_LEFT;   // 左クリック
    //MOUSE_INPUT_RIGHT;  // 右クリック
    //MOUSE_INPUT_MIDDLE; // 中クリック

    static int prevMouseState = 0;

    int mouseState = GetMouseInput();

    VECTOR hitPos;

    //マウスの位置当たり判定
    if (GetMouseHitPosition(&hitPos)) {

        printf("Hit: %f %f %f\n", hitPos.x, hitPos.y, hitPos.z);

        int gx, gz;

        if (GetGridPos(hitPos, &gx, &gz)) {

            // 左クリック（押した瞬間）
            //今押してる状態かつ前は押してない状態
            if ((mouseState & MOUSE_INPUT_LEFT) &&
                !(prevMouseState & MOUSE_INPUT_LEFT)) {

                map[gz][gx] = TILE_FLOOR;
            }

            // 右クリック
            //今押してる状態かつ前は押してない状態
            if ((mouseState & MOUSE_INPUT_RIGHT) &&
                !(prevMouseState & MOUSE_INPUT_RIGHT)) {

                map[gz][gx] = TILE_WALL;
            }

            // 中クリック
            //今押してる状態かつ前は押してない状態
            if ((mouseState & MOUSE_INPUT_MIDDLE) &&
                !(prevMouseState & MOUSE_INPUT_MIDDLE)) {

                map[gz][gx] = TILE_NONE;
            }
        }


    }

    prevMouseState = mouseState;

}

// 描画
void MapEditor::Draw()
{
    //------------------------------------------------------
    //モデル作るのが大変なのでとりあえずキューブで誤魔化す。
    //-------------------------------------------------------
    //ブロックの表示
    for (int z = 0; z < MAP_H; z++) {
        for (int x = 0; x < MAP_W; x++) {

            float worldX = (x + 0.5f) * TILE_SIZE;
            float worldZ = (z + 0.5f) * TILE_SIZE;

            if (map[z][x] == TILE_FLOOR) {
                MV1SetPosition(m_iModelHdl, VGet(worldX, 0, worldZ));
                MV1DrawModel(m_iModelHdl);
               /* DrawCube3D(
                    VGet(x * 5, 0, z * 5),
                    VGet(x * 5 + 5, 1, z * 5 + 5),
                    GetColor(100, 100, 100), GetColor(100, 100, 100),true);*/
            }

            if (map[z][x] == TILE_WALL) {
                MV1SetPosition(m_iModelHdl, VGet(worldX, WALL_HIGHT, worldZ));
                MV1DrawModel(m_iModelHdl);
                /*DrawCube3D(
                    VGet(x * 5, 0, z * 5),
                    VGet(x * 5 + 5, 5, z * 5 + 5),
                    GetColor(200, 200, 200), GetColor(200, 200, 200), TRUE);*/
            }
        }
    }

    //連続で置きたいならやってもいいよ
    //VECTOR hitPos;
    ////グリッドの表示
    //if (GetMouseHitPosition(&hitPos)) {
    //    printf("Hit: %f %f %f\n", hitPos.x, hitPos.y, hitPos.z);

    //    int gx, gz;

    //    if (GetGridPos(hitPos, &gx, &gz)) {
    //        map[gz][gx] = TILE_FLOOR;
    //    }
    //}

    DrawString(1400,50,"Sでセーブ\nLでロード",WHITE);

}


// 終了処理
void MapEditor::Fin()
{
    if (m_iModelHdl != -1)
    {
        MV1DeleteModel(m_iModelHdl);
        m_iModelHdl = -1;
    }
}


//------------------------------------
//          データの保存
//------------------------------------
void MapEditor::SaveMap(const char* filename)
{
    //-------------------------
    // 個人作成なのでバイナリで
    //-------------------------
    FILE* fp;

    fopen_s(&fp,filename, "wb");

    if (fp == NULL) return;

    // サイズも一緒に保存（重要）
    fwrite(&MAP_W, sizeof(int), 1, fp);
    fwrite(&MAP_H, sizeof(int), 1, fp);

    // マップ本体
    fwrite(map, sizeof(TileType), MAP_W * MAP_H, fp);

    fclose(fp);
}


//------------------------------------
//          マップの読み取り
//------------------------------------
void MapEditor::LoadMap(const char* filename)
{
   //-------------------------
   // 個人作成なのでバイナリで
   //-------------------------
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
}


//マウスの当たり判定の取得
bool MapEditor::GetMouseHitPosition(VECTOR* outPos)
{
    //@メモ
    //---------------------------
   /* レイの式
    P = near + t * (far - near)
    y=0 にぶつかる t
    t = -near.y / (far.y - near.y)*/
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


//グリッドの取得
bool MapEditor::GetGridPos(VECTOR hitPos, int* gx, int* gz)
{
    *gx = (int)floor(hitPos.x / TILE_SIZE);
    *gz = (int)floor(hitPos.z / TILE_SIZE);

    // 範囲チェック（超重要）
    if (*gx < 0 || *gx >= MAP_W ||
        *gz < 0 || *gz >= MAP_H) {
        return false;
    }

    return true;
}