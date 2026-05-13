#include"MapEditor.h"
#include <corecrt_math.h>
#include "../../lib/Input/Input.h"


//---------------------------------
// コンストラクタ
//---------------------------------
MapEditor::MapEditor()
{
    Init();
}

//---------------------------------
// デストラクタ
//---------------------------------
MapEditor::~MapEditor()
{
    Fin();
}

//---------------------------------
// 初期化
//---------------------------------
void MapEditor::Init()
{
    m_iModelHdl = -1;
    m_iModelHdl_Wall = -1;

    m_currentY = 0;

    memset(map, 0, sizeof(map));
}

//---------------------------------
// ロード
//---------------------------------
void MapEditor::Load()
{
    VECTOR size = VGet(0.05f, 0.05f, 0.05f);

    if (m_iModelHdl == -1)
    {
        m_iModelHdl = MV1LoadModel("Data/object/stage/StageTexture.mv1");
        m_iModelHdl_Wall = MV1LoadModel("Data/object/stage/Wall.mv1");
    }

    MV1SetScale(m_iModelHdl, size);
    MV1SetScale(m_iModelHdl_Wall, size);

    LoadMap("map.dat");
}

//---------------------------------
// 実行
//---------------------------------
int MapEditor::Step()
{
    if (Input::Key::Push(KEY_INPUT_P))
    {
        SaveMap("map.dat");
    }

    if (Input::Key::Push(KEY_INPUT_L))
    {
        LoadMap("map.dat");
    }

    return 0;
}

//---------------------------------
// 更新
//---------------------------------
void MapEditor::Update()
{
    //---------------------------------
    // 高さ変更
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_Q))
    {
        m_currentY++;

        if (m_currentY >= MAP_Y)
        {
            m_currentY = MAP_Y - 1;
        }
    }

    if (Input::Key::Push(KEY_INPUT_E))
    {
        m_currentY--;

        if (m_currentY < 0)
        {
            m_currentY = 0;
        }
    }

    //---------------------------------
    // マウス
    //---------------------------------
    int mouseState = GetMouseInput();

    bool needRebuild = false;

    VECTOR hitPos;

    //---------------------------------
    // マウスヒット
    //---------------------------------
    if (GetMouseHitPosition(&hitPos))
    {
        int gx, gz;

        if (GetGridPos(hitPos, &gx, &gz))
        {
            //---------------------------------
            // 左クリック
            //---------------------------------
            if (mouseState & MOUSE_INPUT_LEFT)
            {
                map[m_currentY][gz][gx] = TILE_FLOOR;
                needRebuild = true;
            }

            //---------------------------------
            // 右クリック
            //---------------------------------
            if (mouseState & MOUSE_INPUT_RIGHT)
            {
                map[m_currentY][gz][gx] = TILE_WALL;
                needRebuild = true;
            }

            //---------------------------------
            // 中クリック
            //---------------------------------
            if (mouseState & MOUSE_INPUT_MIDDLE)
            {
                map[m_currentY][gz][gx] = TILE_NONE;
                needRebuild = true;
            }
        }
    }

    //---------------------------------
    // 再生成
    //---------------------------------
    if (needRebuild)
    {
        BuildInstances();
    }
}

//---------------------------------
// 描画
//---------------------------------
void MapEditor::Draw()
{
    //---------------------------------
    // インスタンス描画
    //---------------------------------
    for (auto& inst : instances)
    {
        MV1SetPosition(inst.m_iModelHdl, inst.m_vPosition);

        MV1DrawModel(inst.m_iModelHdl);
    }
    //---------------------------------
    // UI
    //---------------------------------
    DrawFormatString(100, 100, RED, "Curredwwant Y : %d", m_currentY);

    DrawString(100,130,"Q/E : Height Change",RED);
}

//---------------------------------
// 終了
//---------------------------------
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

//---------------------------------
// 保存
//---------------------------------
void MapEditor::SaveMap(const char* filename)
{
    FILE* fp;

    fopen_s(&fp, filename, "wb");

    if (fp == NULL)
        return;

    fwrite(&MAP_W, sizeof(int), 1, fp);
    fwrite(&MAP_H, sizeof(int), 1, fp);
    fwrite(&MAP_Y, sizeof(int), 1, fp);

    fwrite( map, sizeof(TileType), MAP_Y * MAP_H * MAP_W, fp);

    fclose(fp);
}

//---------------------------------
// 読み込み
//---------------------------------
void MapEditor::LoadMap(const char* filename)
{
    FILE* fp;

    fopen_s(&fp, filename, "rb");

    if (fp == NULL)
        return;

    int w, h, y;

    fread(&w, sizeof(int), 1, fp);
    fread(&h, sizeof(int), 1, fp);
    fread(&y, sizeof(int), 1, fp);

    if (w != MAP_W ||
        h != MAP_H ||
        y != MAP_Y)
    {
        fclose(fp);
        return;
    }

    fread( map, sizeof(TileType), MAP_Y * MAP_H * MAP_W, fp
    );

    fclose(fp);

    BuildInstances();
}

//---------------------------------
// マウス位置取得
//---------------------------------
bool MapEditor::GetMouseHitPosition(VECTOR* outPos)
{
    int mouseX, mouseY;

    GetMousePoint(&mouseX, &mouseY);

    VECTOR nearPos;
    VECTOR farPos;

    nearPos = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 0.0f));

    farPos = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 1.0f));

    VECTOR dir = VSub(farPos, nearPos);

    if (fabs(dir.y) < 0.0001f)return false;

    float t = -nearPos.y / dir.y;

    if (t < 0.0f)return false;

    *outPos = VAdd(nearPos, VScale(dir, t));

    return true;
}

//---------------------------------
// グリッド取得
//---------------------------------
bool MapEditor::GetGridPos( VECTOR hitPos, int* pos_x, int* pos_z)
{
    *pos_x = (int)floor(hitPos.x / TILE_SIZE);
    *pos_z = (int)floor(hitPos.z / TILE_SIZE);

    if (*pos_x < 0 ||*pos_x >= MAP_W ||
        *pos_z < 0 ||*pos_z >= MAP_H)
    {
        return false;
    }

    return true;
}

//---------------------------------
// インスタンス生成
//---------------------------------
void MapEditor::BuildInstances()
{
    instances.clear();

    for (int y = 0; y < MAP_Y; y++)
    {
        for (int z = 0; z < MAP_H; z++)
        {
            for (int x = 0; x < MAP_W; x++)
            {
                float worldX = (x + 0.5f) * TILE_SIZE;

                float worldY = y * TILE_SIZE;

                float worldZ = (z + 0.5f) * TILE_SIZE;

                //---------------------------------
                // 床
                //---------------------------------
                if (map[y][z][x] == TILE_FLOOR)
                {
                    instances.push_back({m_iModelHdl, VGet(worldX, worldY, worldZ)});
                }

                //---------------------------------
                // 壁
                //---------------------------------
                if (map[y][z][x] == TILE_WALL)
                {
                    instances.push_back({m_iModelHdl_Wall, VGet(worldX, worldY, worldZ)});
                }
            }
        }
    }
}

//---------------------------------
// マップ取得
//---------------------------------
int MapEditor::GetMap(int y,int z,int x) const
{
    return map[y][z][x];
}

//---------------------------------
// マップ設定
//---------------------------------
void MapEditor::SetMap(int y,int z,int x,int value)
{
    map[y][z][x] = (TileType)value;
}


