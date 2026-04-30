
#include"ObjectEditor.h"
#include "../../lib/Input/Input.h"

constexpr int MAP_W = 50;
constexpr int MAP_H = 50;
constexpr float TILE_SIZE = 5.0f;

//コンストラクタ
ObjectEditor::ObjectEditor()
{

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
}
// 実行処理
int  ObjectEditor::Step()
{

    return 0;
}

// 描画
void ObjectEditor::Draw()
{
    DrawObjects();
}

// 終了処理
void ObjectEditor::Fin()
{

}


//マップのセーブ
void ObjectEditor::SaveMap(const char* filename)
{
    FILE* fp;

    fopen_s(&fp, filename, "wb");

    if (fp == NULL) return;

    int objCount = objects.size();
    fwrite(&objCount, sizeof(int), 1, fp);

    for (auto& obj : objects) {
        fwrite(&obj, sizeof(Object), 1, fp);

        fclose(fp);
    };

}
//マップの読み込み
void ObjectEditor::LoadMap(const char* filename)
{
    FILE* fp;

    fopen_s(&fp, filename, "rb");

    if (fp == NULL) return;

    objects.clear();

    int objCount;
    fread(&objCount, sizeof(int), 1, fp);

    for (int i = 0; i < objCount; i++) {
        Object obj;
        fread(&obj, sizeof(Object), 1, fp);
        objects.push_back(obj);
    }
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

        VECTOR pos = VGet(worldX, 0, worldZ);

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
        }
    }
}
