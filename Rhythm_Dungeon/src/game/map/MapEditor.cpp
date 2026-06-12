#include"MapEditor.h"
#include <corecrt_math.h>
#include "../../lib/Input/Input.h"
#include "../../Data.h"


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
    for (int init = 0;init < TILE_NUM;init++)
    {
        m_iModelHdl[init] = -1;
    }
  
    m_currentY = 0;
    // 現在の角度
    m_currentRotY = 0.0f;

    memset(map, 0, sizeof(map));
    memset(m_rotMap, 0 ,sizeof(m_rotMap));
}

//---------------------------------
// ロード
//---------------------------------
void MapEditor::Load(ObjectEditor& objectEditor)
{
    VECTOR size = VGet(0.05f, 0.05f, 0.05f);

    //---------------------------------
      // モデルロード
      //---------------------------------
    if (m_iModelHdl[0] == -1)
    {
        m_iModelHdl[0] = MV1LoadModel("Data/object/stage/Tile.mv1");
    }
    if (m_iModelHdl[1] == -1)
    {
        m_iModelHdl[1] = MV1LoadModel( "Data/object/stage/Tile2.mv1");
    }
    if (m_iModelHdl[2] == -1)
    {
        m_iModelHdl[2] = MV1LoadModel( "Data/object/stage/stairs.mv1");
    }
    if (m_iModelHdl[3] == -1)
    {
        m_iModelHdl[3] = MV1LoadModel("Data/object/stage/Block.mv1");
    }

    for (int load = 0;load < TILE_NUM;load++)
    {
        MV1SetScale(m_iModelHdl[load], size);
    }

   // MV1SetOpacityRate( m_iModelHdl[3], 0.2f);

    LoadMap(Data::GetInstance()->GetStagePath(), objectEditor);

}

//---------------------------------
// 実行
//---------------------------------
int MapEditor::Step(ObjectEditor& objectEditor)
{
    if (Input::Key::Push(KEY_INPUT_P))
    {
        SaveMap(Data::GetInstance()->GetStagePath(), objectEditor);
    }

    if (Input::Key::Push(KEY_INPUT_L))
    {
        LoadMap(Data::GetInstance()->GetStagePath(), objectEditor);
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
    // 左回転
    if (Input::Key::Push(KEY_INPUT_8))
    {
        m_currentRotY -= DX_PI_F / 2;
    }

    // 右回転
    if (Input::Key::Push(KEY_INPUT_9))
    {
        m_currentRotY += DX_PI_F / 2;
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
                map[m_currentY][gz][gx] = TILE_STAIRS;
                m_rotMap[m_currentY][gz][gx] = m_currentRotY;
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

           //---------------------------------
           // スペースクリック
           //---------------------------------
            if (Input::Key::Push(KEY_INPUT_SPACE))
            {
                map[m_currentY][gz][gx] = TILE_FLOOR2;
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
        VECTOR m_vRotation = VGet(0.0f, inst.rotY,0.0f);
        MV1SetRotationXYZ(inst.m_iModelHdl, m_vRotation);
        MV1DrawModel(inst.m_iModelHdl);
    }

    //---------------------------------
    // UI
    //---------------------------------
    DrawFormatString(100, 100, RED, "Curredwwant Y : %d", m_currentY);
    DrawString(100,130,"Q/E : Height Change",RED);

    //---------------------------------
   // 向き表示
   //---------------------------------
    const char* dirText = "RIGHT";

    // 0～360に変換
    float rotDeg = m_currentRotY * 180.0f / DX_PI_F;

    // マイナス対策
    while (rotDeg < 0)
    {
        rotDeg += 360.0f;
    }

    rotDeg = fmod(rotDeg, 360.0f);

    // 方向判定
    if (rotDeg >= 315 || rotDeg < 45)
    {
        dirText = "DOWN";
    }
    else if (rotDeg >= 45 && rotDeg < 135)
    {
        dirText = "LEFT";
    }
    else if (rotDeg >= 135 && rotDeg < 225)
    {
        dirText = "UP";
    }
    else
    {
        dirText = "RIGHT";
    }

    DrawFormatString(1220, 280, WHITE, "向き : %s", dirText);

}

//---------------------------------
// 終了
//---------------------------------
void MapEditor::Fin()
{
    for (int fin = 0; fin < TILE_NUM; fin++)
    {
        if (m_iModelHdl[fin] != -1)
        {
            MV1DeleteModel(m_iModelHdl[fin]);
            m_iModelHdl[fin] = -1;
        }
    }
}

//---------------------------------
// 保存
//---------------------------------
void MapEditor::SaveMap(const char* filename,  ObjectEditor& objectEditor)
{
    FILE* fp = nullptr;
    fopen_s(&fp, filename, "wb");
    if (!fp) return;

    //---------------------------------
    // マップ保存
    //---------------------------------
    fwrite( map, sizeof(int), MAP_W * MAP_H * MAP_Y, fp);

    //---------------------------------
    // 回転情報保存
    //---------------------------------
    fwrite( m_rotMap, sizeof(float), MAP_W * MAP_H * MAP_Y, fp);

    //---------------------------------
    // オブジェクト保存
    //---------------------------------
    const auto& objs = objectEditor.GetObjects();
    int objCount = (int)objs.size();

    fwrite(&objCount, sizeof(int), 1, fp);

    for (auto& obj : objs)
    {
        fwrite( &obj, sizeof(ObjectEditor::Object), 1, fp);
    }

    fclose(fp);
}

//---------------------------------
// 読み込み
//---------------------------------
void MapEditor::LoadMap(const char* filename, ObjectEditor& objectEditor)
{
    FILE* fp = nullptr;

    //---------------------------------
    // 読み込みモード
    //---------------------------------
    fopen_s(&fp, filename, "rb");

    if (!fp)
    {
        printf("マップ読み込み失敗\n"); return;
    }

    //---------------------------------
    // マップ読み込み
    //---------------------------------
    fread(map,sizeof(int),MAP_W * MAP_H * MAP_Y,fp);

    //---------------------------------
    // 回転情報読み込み
    //---------------------------------
    fread( m_rotMap, sizeof(float), MAP_W * MAP_H * MAP_Y, fp);

    //---------------------------------
    // オブジェクト削除
    //---------------------------------
    objectEditor.Clear();

    //---------------------------------
    // オブジェクト数
    //---------------------------------
    int objCount = 0;

    fread(&objCount, sizeof(int), 1, fp);

    //---------------------------------
    // オブジェクト読み込み
    //---------------------------------
    for (int i = 0; i < objCount; i++)
    {
        ObjectEditor::Object obj;
        fread( &obj, sizeof(ObjectEditor::Object), 1,fp);

        objectEditor.AddLoadedObject(obj);
    }

    fclose(fp);

    //---------------------------------
    // モデル再生成
    //---------------------------------
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
                    instances.push_back({m_iModelHdl[0], VGet(worldX, worldY, worldZ)});
                }

                //---------------------------------
                // 階段
                //---------------------------------
                if (map[y][z][x] == TILE_STAIRS)
                {
                    instances.push_back({ m_iModelHdl[2], VGet(worldX, worldY, worldZ),  m_rotMap[y][z][x] });
                }

                //---------------------------------
                // 床
                //---------------------------------
                if (map[y][z][x] == TILE_FLOOR2)
                {
                    instances.push_back({ m_iModelHdl[1], VGet(worldX, worldY, worldZ) });
                }

               //---------------------------------
               // 床
               //---------------------------------
                if (map[y][z][x] == TILE_BRIDGE)
                {
                    instances.push_back({ m_iModelHdl[3], VGet(worldX, worldY, worldZ) });
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


