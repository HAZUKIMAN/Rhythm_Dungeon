
#include"ObjectEditor.h"
#include "../../lib/Input/Input.h"
#include "../common.h"

//コンストラクタ
ObjectEditor::ObjectEditor()
{
    Init();
}
//デストラクタ
ObjectEditor::~ObjectEditor()
{

}

// 初期化
void ObjectEditor::Init()
{
}

// データロード
void ObjectEditor::Load()
{
    LoadMap("object.dat");
}
// 実行処理
int  ObjectEditor::Step()
{
    //データの保存・読み取り
    if (Input::Key::Push(KEY_INPUT_P)) {
        SaveMap("object.dat");
    }
    if (Input::Key::Push(KEY_INPUT_L)) {
      LoadMap("object.dat");
    }

    if (Input::Key::Push(KEY_INPUT_Q)) {

        //現在のオブジェクトの内容を表示
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
            objstate = OBJ_NONE;
            break;
        }
    }

    return 0;
}


//更新処理
void ObjectEditor::Update()
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

        int gx, gz;

        if (GetGridPos(hitPos, &gx, &gz)) {

            if (Input::Key::Keep(KEY_INPUT_O)) { // Oキーでオブジェクトモード

                AddObject(gx, gz, objstate);//今はもう一人のプレイヤーが出るようにしている
            }
            if (Input::Key::Keep(KEY_INPUT_U)) {  // Uキーでオブジェクト削除モード

                RemoveObject(gx, gz);
            }

        }
    }
}

// 描画
void ObjectEditor::Draw()
{
    DrawObjects();

    //現在のオブジェクトの内容を表示
    switch (objstate)
    {
    case OBJ_NONE:
        DrawString(1300, 200, "現在は何も表示されません", PINKU);
        break;
    case OBJ_PLAYER:
        DrawString(1300, 200, "プレイヤーの初期位置", PINKU);
        break;
    case OBJ_ENEMY:
        DrawString(1300, 200, "エネミーの初期位置", PINKU);
        break;
    case OBJ_ITEM:
        DrawString(1300, 200, "アイテムの初期位置", PINKU);
        break;
    case OBJ_GOAL:
        DrawString(1300, 200, "ゴールの位置", PINKU);
        break;
    }
}

// 終了処理
void ObjectEditor::Fin()
{

}


//マップのセーブ
void ObjectEditor::SaveMap(const char* filename)
{
   /* FILE* fp;

    fopen_s(&fp, filename, "wb");

    if (fp == NULL) return;

    int objCount = objects.size();
    fwrite(&objCount, sizeof(int), 1, fp);

    for (auto& obj : objects) {
        fwrite(&obj.x, sizeof(int), 1, fp);
        fwrite(&obj.z, sizeof(int), 1, fp);
        fwrite(&obj.type, sizeof(int), 1, fp);
        fwrite(&obj.rotY, sizeof(float), 1, fp);
    }

    fclose(fp);*/

    FILE* fp;
    fopen_s(&fp, filename, "wb");
    if (!fp) return;

    int objCount = (int)objects.size();
    fwrite(&objCount, sizeof(int), 1, fp);

    for (auto& obj : objects) {
        fwrite(&obj, sizeof(Object), 1, fp);
    }

    fclose(fp);

    printf("保存完了: %d個\n", objCount);

}

//マップの読み込み
void ObjectEditor::LoadMap(const char* filename)
{
   /* FILE* fp;

    fopen_s(&fp, filename, "rb");

    if (fp == NULL) return;

    objects.clear();

    int objCount;
    fread(&objCount, sizeof(int), 1, fp);

    for (int i = 0; i < objCount; i++) {
        Object obj;

        fread(&obj.x, sizeof(int), 1, fp);
        fread(&obj.z, sizeof(int), 1, fp);
        fread(&obj.type, sizeof(int), 1, fp);
        fread(&obj.rotY, sizeof(float), 1, fp);

        objects.push_back(obj);
    }

    fclose(fp);*/

    FILE* fp;

    fopen_s(&fp, filename, "rb");
    if (!fp) {
        printf("ファイル開けない\n");
        return;
    }

    objects.clear();

    int objCount = 0;
    fread(&objCount, sizeof(int), 1, fp);

    printf("読み込み数: %d\n", objCount);

    for (int i = 0; i < objCount; i++) {
        Object obj;
        fread(&obj, sizeof(Object), 1, fp);
        objects.push_back(obj);
    }


    fclose(fp);

    //BuildInstances();
}


//----------------------------
//     チェック(1マス1個)
//----------------------------
bool ObjectEditor::IsObjectAt(int x, int z)
{
    for (auto& obj : objects) {
        if (obj.x == x && obj.z == z) return true;
    }
    return false;
}


//----------------------------
//     設置処理
//----------------------------
void ObjectEditor::AddObject(int x, int z, int type)
{
    Object obj;
    obj.x = x;
    obj.z = z;
    obj.type = type;
    obj.rotY = 0.0f;

    objects.push_back(obj);
}


//---------------------------------
//      削除処理
//---------------------------------
void ObjectEditor::RemoveObject(int x, int z)
{
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i].x == x && objects[i].z == z) {
            objects.erase(objects.begin() + i);
            return;
        }
    }
}


//--------------------------------
//       アイテムの描写
//--------------------------------
void ObjectEditor::DrawObjects()
{
    for (auto& obj : objects) {

        float worldX = (obj.x + 0.5f) * TILE_SIZE;
        float worldZ = (obj.z + 0.5f) * TILE_SIZE;

        VECTOR pos = VGet(worldX, 5.0f, worldZ);//浮かせるので5.0fをいれています

        switch (obj.type) {

        case OBJ_PLAYER:
            DrawSphere3D(pos, 1.5f, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), TRUE);
            break;

        case OBJ_ENEMY:
            DrawSphere3D(pos, 1.5f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
            break;

        case OBJ_ITEM:
            DrawSphere3D(pos, 1.0f, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            break;
        case OBJ_GOAL:
            DrawSphere3D(pos, 1.0f, 16, GetColor(0, 255, 255), GetColor(0, 255, 255), TRUE);
            break;
        }
    }
}


//他のcppで使えますよ(コピーを防ぐ)
const std::vector<ObjectEditor::Object>& ObjectEditor::GetObjects() const
{
    return objects;
}


//-----------------------------------
//      マウスの当たり判定の取得
//-----------------------------------
bool ObjectEditor::GetMouseHitPosition(VECTOR* outPos)
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
    nearPos = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 0.0f));
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
bool ObjectEditor::GetGridPos(VECTOR hitPos, int* pos_x, int* pos_z)
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

