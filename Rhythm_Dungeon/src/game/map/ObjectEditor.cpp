
#include"ObjectEditor.h"
#include "../../lib/Input/Input.h"
#include "../common.h"

//---------------------------------
// コンストラクタ
//---------------------------------
ObjectEditor::ObjectEditor()
{
    Init();
}

//---------------------------------
// デストラクタ
//---------------------------------
ObjectEditor::~ObjectEditor()
{
}

//---------------------------------
// 初期化
//---------------------------------
void ObjectEditor::Init()
{
    //---------------------------------
    // 現在の選択オブジェクト
    //---------------------------------
    objstate = OBJ_PLAYER;

    //---------------------------------
    // 現在の高さ
    //---------------------------------
    m_currentY = 0;

    //---------------------------------
    // オブジェクト削除
    //---------------------------------
    objects.clear();
}

//---------------------------------
// データロード
//---------------------------------
void ObjectEditor::Load()
{
    LoadMap("object.dat");
}

//---------------------------------
// 実行処理
//---------------------------------
int ObjectEditor::Step()
{
    //---------------------------------
    // 保存
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_P))
    {
        SaveMap("object.dat");
    }

    //---------------------------------
    // 読み込み
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_L))
    {
        LoadMap("object.dat");
    }

    //---------------------------------
    // 高さ上昇
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_UP))
    {
        m_currentY++;
    }

    //---------------------------------
    // 高さ下降
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_DOWN))
    {
        m_currentY--;

        if (m_currentY < 0)
        {
            m_currentY = 0;
        }
    }

    //---------------------------------
    // オブジェクト変更
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_1))
    {
        switch (objstate)
        {
        case OBJ_NONE:
            objstate = OBJ_PLAYER;
            break;

        case OBJ_PLAYER:
            objstate = OBJ_ENEMY;
            break;

        case OBJ_ENEMY:
            objstate = OBJ_ITEM;
            break;

        case OBJ_ITEM:
            objstate = OBJ_GOAL;
            break;

        case OBJ_GOAL:
            objstate = OBJ_PUT_BOX;
            break;

        case OBJ_PUT_BOX:
            objstate = OBJ_NONE;
            break;
        }
    }

    return 0;
}

//---------------------------------
// 更新処理
//---------------------------------
void ObjectEditor::Update()
{
    VECTOR hitPos;

    //---------------------------------
    // マウス位置取得
    //---------------------------------
    if (GetMouseHitPosition(&hitPos))
    {
        int gx;
        int gz;

        //---------------------------------
        // ワールド→グリッド
        //---------------------------------
        if (GetGridPos(hitPos, &gx, &gz))
        {
            //---------------------------------
            // 設置
            //---------------------------------
            if (Input::Key::Push(KEY_INPUT_O))
            {
                AddObject(gx, m_currentY, gz, objstate);
            }

            //---------------------------------
            // 削除
            //---------------------------------
            if (Input::Key::Push(KEY_INPUT_U))
            {
                RemoveObject(gx, m_currentY, gz);
            }
        }
    }
}

//---------------------------------
// 描画
//---------------------------------
void ObjectEditor::Draw()
{

    

    //---------------------------------
    // オブジェクト描画
    //---------------------------------
    DrawObjects();

    //---------------------------------
    // 現在高さ表示
    //---------------------------------
    DrawFormatString(1300,100,WHITE,"現在の高さ : %d",m_currentY);

    //
    DrawString(800, 10, "INPUT_KEY_1でリスポーン地点を変換", YELLOW);

    //---------------------------------
    // 現在オブジェクト表示
    //---------------------------------
    switch (objstate)
    {
    case OBJ_NONE:
        DrawString(1300, 200,"OBJ_NONE", WHITE);
        break;

    case OBJ_PLAYER:
        DrawString(1300, 200,"OBJ_PLAYER", BLUE);
        break;

    case OBJ_ENEMY:
        DrawString(1300, 200,"OBJ_ENEMY", RED);
        break;

    case OBJ_ITEM:
        DrawString(1300, 200,"OBJ_ITEM", GREEN);
        break;

    case OBJ_GOAL:
        DrawString(1300, 200,"OBJ_GOAL", YELLOW);
        break;

    case OBJ_PUT_BOX:
        DrawString(1300, 200,"OBJ_PUT_BOX", PINKU);
        break;
    }
}

//---------------------------------
// 終了処理
//---------------------------------
void ObjectEditor::Fin()
{
}

//---------------------------------
// 保存
//---------------------------------
void ObjectEditor::SaveMap(const char* filename)
{
    FILE* fp;

    fopen_s(&fp, filename, "wb");

    if (!fp)return;

    //---------------------------------
    // 個数保存
    //---------------------------------
    int objCount = (int)objects.size();

    fwrite(&objCount, sizeof(int), 1, fp);

    //---------------------------------
    // 本体保存
    //---------------------------------
    for (auto& obj : objects)
    {
        fwrite( &obj, sizeof(Object), 1, fp);
    }

    fclose(fp);

    printf("保存完了 : %d個\n",
        objCount);
}

//---------------------------------
// 読み込み
//---------------------------------
void ObjectEditor::LoadMap(
    const char* filename)
{
    FILE* fp;

    fopen_s(&fp, filename, "rb");

    if (!fp)
    {
        printf("読み込み失敗\n");
        return;
    }

    //---------------------------------
    // 一旦削除
    //---------------------------------
    objects.clear();

    //---------------------------------
    // 個数取得
    //---------------------------------
    int objCount = 0;

    fread( &objCount, sizeof(int), 1, fp);

    //---------------------------------
    // 本体読み込み
    //---------------------------------
    for (int i = 0; i < objCount;i++)
    {
        Object obj;

        fread( &obj, sizeof(Object), 1, fp);

        objects.push_back(obj);
    }

    fclose(fp);

    printf("読み込み完了 : %d個\n",objCount);
}

//---------------------------------
// 存在チェック
//---------------------------------
bool ObjectEditor::IsObjectAt(int x,int y,int z)
{
    for (auto& obj : objects)
    {
        if (obj.x == x &&
            obj.y == y &&
            obj.z == z)
        {
            return true;
        }
    }

    return false;
}

//---------------------------------
// 設置
//---------------------------------
void ObjectEditor::AddObject(int x, int y, int z, int type)
{
    //---------------------------------
    // 重複防止
    //---------------------------------
    if (IsObjectAt(x, y, z))
        return;

    Object obj;

    obj.x = x;
    obj.y = y;
    obj.z = z;

    obj.type = type;

    obj.rotY = 0.0f;

    objects.push_back(obj);
}

//---------------------------------
// 削除
//---------------------------------
void ObjectEditor::RemoveObject(int x, int y ,int z)
{
    for (int i = 0;i < objects.size();i++)
    {
        if (objects[i].x == x &&
            objects[i].y == y &&
            objects[i].z == z)
        {
            objects.erase(objects.begin() + i);

            return;
        }
    }
}

//---------------------------------
// 描画
//---------------------------------
void ObjectEditor::DrawObjects()
{

    DrawSelectedTile();

    for (auto& obj : objects)
    {
        //---------------------------------
        // グリッド→ワールド
        //---------------------------------
        float worldX =
            (obj.x + 0.5f)
            * TILE_SIZE;

        float worldY = (obj.y * TILE_SIZE);

        float worldZ = (obj.z + 0.5f)* TILE_SIZE;

        VECTOR pos = VGet(worldX, worldY, worldZ);

        //---------------------------------
        // 種類別描画
        //---------------------------------
        switch (obj.type)
        {
        case OBJ_PLAYER:
            DrawSphere3D(pos, 1.5f, 16,BLUE,BLUE, TRUE);
            break;

        case OBJ_ENEMY:
            DrawSphere3D(pos,1.5f,16,RED,RED,TRUE);
            break;

        case OBJ_ITEM:
            DrawSphere3D(pos, 1.0f, 16, GREEN, GREEN,TRUE);
            break;

        case OBJ_GOAL:
            DrawSphere3D(pos, 1.0f, 16, YELLOW, YELLOW, TRUE);
            break;

        case OBJ_PUT_BOX:
            DrawSphere3D(pos, 1.0f, 16, PINKU, PINKU, TRUE);
            break;
        }
    }
}

//---------------------------------
// オブジェクト取得
//---------------------------------
const std::vector<ObjectEditor::Object>&ObjectEditor::GetObjects() const
{
    return objects;
}

//---------------------------------
// マウスレイ取得
//---------------------------------
bool ObjectEditor::GetMouseHitPosition(VECTOR* outPos)
{
    int mouseX;
    int mouseY;

    GetMousePoint(&mouseX,&mouseY);

    //---------------------------------
    // スクリーン→ワールド
    //---------------------------------
    VECTOR nearPos = ConvScreenPosToWorldPos(VGet((float)mouseX,(float)mouseY,0.0f));
    VECTOR farPos = ConvScreenPosToWorldPos(VGet((float)mouseX,(float)mouseY,1.0f));

    //---------------------------------
    // レイ方向
    //---------------------------------
    VECTOR dir = VSub(farPos, nearPos);

    //---------------------------------
    // 平行対策
    //---------------------------------
    if (fabs(dir.y) < 0.0001f) return false;

    //---------------------------------
    // Y高さとの交点
    //---------------------------------
    float targetY = m_currentY * TILE_SIZE;

    float t = (targetY - nearPos.y)/ dir.y;

    if (t < 0.0f) return false;

    //---------------------------------
    // 交点
    //---------------------------------
    *outPos = VAdd(nearPos,VScale(dir, t));

    return true;
}

//---------------------------------
// グリッド取得
//---------------------------------
bool ObjectEditor::GetGridPos(VECTOR hitPos,int* pos_x,int* pos_z)
{
    *pos_x =(int)floor(hitPos.x / TILE_SIZE);

    *pos_z =(int)floor(hitPos.z / TILE_SIZE);

    //---------------------------------
    // 範囲外
    //---------------------------------
    if (*pos_x < 0 ||*pos_x >= MAP_W ||
        *pos_z < 0 ||*pos_z >= MAP_H)
    {
        return false;
    }
    return true;
}

//置く位置を見やすくするもの
void ObjectEditor::DrawSelectedTile()
{
    //---------------------------------
    // グリッド線描画
    //---------------------------------
    for (int z = 0; z <= MAP_H; z++)
    {
        DrawLine3D(
            VGet(0, 0.1f, z * TILE_SIZE),
            VGet(MAP_W * TILE_SIZE, 0.1f, z * TILE_SIZE),
            GRAY
        );
    }

    for (int x = 0; x <= MAP_W; x++)
    {
        DrawLine3D(
            VGet(x * TILE_SIZE, 0.1f, 0),
            VGet(x * TILE_SIZE, 0.1f, MAP_H * TILE_SIZE),
            GRAY
        );
    }

    //---------------------------------
    // 設置予定位置表示
    //---------------------------------
    VECTOR hitPos;

    if (GetMouseHitPosition(&hitPos))
    {
        int gx, gz;

        if (GetGridPos(hitPos, &gx, &gz))
        {
            //---------------------------------
            // マス座標
            //---------------------------------
            float x0 = gx * TILE_SIZE;
            float z0 = gz * TILE_SIZE;

            float x1 = x0 + TILE_SIZE;
            float z1 = z0 + TILE_SIZE;

            //---------------------------------
            // 高さ
            //---------------------------------
            float y0 = m_currentY * TILE_SIZE;
            float y1 = y0 + TILE_SIZE;

            //---------------------------------
            // 半透明表示
            //---------------------------------
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
            DrawCube3D( VGet(x0, y0, z0), VGet(x1, y1, z1), GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            //---------------------------------
            // 枠表示
            //---------------------------------
            DrawCube3D( VGet(x0, y0, z0), VGet(x1, y1, z1), RED, RED, FALSE);
        }
    }
}

