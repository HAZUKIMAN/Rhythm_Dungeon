
#include"ObjectEditor.h"
#include "../../lib/Input/Input.h"
#include "../common.h"
#include "../../Data.h"

//static const char TEST_MAP_PATH[] = { "Data/_dat/object.dat" };


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
    // 現在の選択オブジェクト
    objstate = OBJ_HUMAN;
    // 現在の高さ
    m_currentY = 0;
    // 現在の角度
    m_currentRotY = 0.0f;
    // オブジェクト削除
    objects.clear();
}

//---------------------------------
// データロード
//---------------------------------
void ObjectEditor::Load()
{
    
}

//---------------------------------
// 実行処理
//---------------------------------
int ObjectEditor::Step()
{
   

    //---------------------------------
    // 高さ上昇
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_Q))
    {
        m_currentY++;
    }

    //---------------------------------
    // 高さ下降
    //---------------------------------
    if (Input::Key::Push(KEY_INPUT_E))
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
            objstate = OBJ_HUMAN;
            break;

        case OBJ_HUMAN:
            objstate = OBJ_ENEMY;
            break;
        case OBJ_ENEMY:
            objstate = OBJ_CAT;
            break;
        case OBJ_CAT:
            objstate = OBJ_ITEM;
            break;

        case OBJ_ITEM:
            objstate = OBJ_GOAL;
            break;

        case OBJ_GOAL:
            objstate = OBJ_SETBLOCK;
            break;

      /*  case OBJ_PUT_BOX:
            objstate = OBJ_SETBLOCK;
            break;*/
        case OBJ_SETBLOCK:
            objstate = OBJ_BRIDGE;
            break;

        case OBJ_BRIDGE:
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

            // 設置
            if (Input::Key::Push(KEY_INPUT_O))
            {
                AddObject(gx, m_currentY, gz, objstate);
            }

            // 削除
            if (Input::Key::Push(KEY_INPUT_U))
            {
                RemoveObject(gx, m_currentY, gz);
                if (m_currentY >= MAP_Y)
                {
                    m_currentY = MAP_Y - 1;
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

    //---------------------------------
    // 現在の選択オブジェクト表示
    //---------------------------------
    DrawBox(1200, 180, 1550, 320, GetColor(30, 30, 30), TRUE);
    DrawBox(1200, 180, 1550, 320, WHITE, FALSE);

    DrawFormatString( 1220, 200, WHITE,"現在のオブジェクト");

    switch (objstate)
    {
    case OBJ_NONE:
        DrawFormatString(1220, 240, WHITE, "NONE");
        break;

    case OBJ_HUMAN:
        DrawFormatString(1220, 240, BLUE, "HUMAN");
        break;

    case OBJ_ENEMY:
        DrawFormatString(1220, 240, RED, "ENEMY");
        break;

    case OBJ_CAT:
        DrawFormatString(1220, 240, YELLOW, "CAT");
        break;

    case OBJ_ITEM:
        DrawFormatString(1220, 240, GREEN, "ITEM");
        break;

    case OBJ_GOAL:
        DrawFormatString(1220, 240, PINKU, "GOAL");
        break;

    case OBJ_SETBLOCK:
        DrawFormatString(1220, 240, PURPLE, "BLOCK");
        break;

    case OBJ_BRIDGE:
        DrawFormatString(1220, 240, ORANGE, "ORANGE");
        break;
    }

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

    DrawFormatString( 1220, 280, WHITE, "向き : %s",dirText);

    //---------------------------------
    // 操作説明
    //---------------------------------
    DrawBox(20, 600, 450, 850, GetColor(20, 20, 20), TRUE);
    DrawBox(20, 600, 450, 850, WHITE, FALSE);

    DrawString(40, 620, "[1] オブジェクト変更", WHITE);
    DrawString(40, 650, "[↑↓] 高さ変更", WHITE);
    DrawString(40, 680, "[8][9] 回転", WHITE);
    DrawString(40, 710, "[O] 設置", WHITE);
    DrawString(40, 740, "[U] 削除", WHITE);
    DrawString(40, 770, "[P] 保存", WHITE);
    DrawString(40, 800, "[L] 読み込み", WHITE);
}

//---------------------------------
// 終了処理
//---------------------------------
void ObjectEditor::Fin()
{

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

    // 回転保存
    obj.rotY = m_currentRotY;

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
        float worldX = (obj.x + 0.5f)* TILE_SIZE;

        float worldY = (obj.y * TILE_SIZE);

        float worldZ = (obj.z + 0.5f)* TILE_SIZE;

        VECTOR pos = VGet(worldX, worldY, worldZ);

        //---------------------------------
        // 種類別描画
        //---------------------------------
        switch (obj.type)
        {
        case OBJ_HUMAN:
            DrawSphere3D(pos, 1.5f, 16,BLUE,BLUE, TRUE);
            break;

        case OBJ_CAT:
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

        case OBJ_SETBLOCK:
            DrawSphere3D(pos, 1.0f, 16, PURPLE, PURPLE, TRUE);
            break;

        case OBJ_ENEMY:
            DrawSphere3D(pos, 1.0f, 16, BLACK, BLACK, TRUE);
            break;

        case OBJ_BRIDGE:
            DrawSphere3D(pos, 1.0f, 16, ORANGE, ORANGE, TRUE);
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
        DrawLine3D(VGet(0, 0.1f, z * TILE_SIZE), VGet(MAP_W * TILE_SIZE, 0.1f, z * TILE_SIZE),GRAY);
    }

    for (int x = 0; x <= MAP_W; x++)
    {
        DrawLine3D( VGet(x * TILE_SIZE, 0.1f, 0), VGet(x * TILE_SIZE, 0.1f, MAP_H * TILE_SIZE), GRAY);
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
            float y0 = m_currentY * TILE_SIZE ;
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

            //---------------------------------
           // 向き表示
           //---------------------------------
            VECTOR center = VGet((x0 + x1) * 0.5f, y1 + 0.5f, (z0 + z1) * 0.5f);
            VECTOR dir;

            dir.x = -sinf(m_currentRotY) * TILE_SIZE * 0.5f;
            dir.y = 0.0f;
            dir.z = -cosf(m_currentRotY) * TILE_SIZE * 0.5f;

            VECTOR end = VAdd(center, dir);

            DrawLine3D(center, end, RED);
        }
    }
}

void ObjectEditor::AddLoadedObject(
    const Object& obj)
{
    objects.push_back(obj);
}

void ObjectEditor::Clear()
{
    objects.clear();
}

