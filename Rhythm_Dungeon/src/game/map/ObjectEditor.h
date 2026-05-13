#pragma once
#include <DxLib.h>
#include <vector>

class ObjectEditor
{

public:

	//コンストラクタ
	ObjectEditor();
	//デストラクタ
	~ObjectEditor();

	// 初期化
	void Init();
	// データロード
	void Load();
	// 実行処理
	int  Step();
	// 描画
	void Draw();
	// 更新処理
	void Update();
	// 終了処理
	void Fin();

	struct Object {
		int x, z, y;
		int type;
		float rotY;   // 向き
	};

	std::vector<Object> objects;

	enum ObjectType {
		OBJ_NONE,
		OBJ_PLAYER,
		OBJ_ENEMY,
		OBJ_ITEM,
		OBJ_GOAL,
		OBJ_PUT_BOX,
	};

	ObjectType objstate;

	void AddObject(int x, int y, int z, int type);	//設置処理
	void RemoveObject(int x, int y, int z);			//削除処理

	bool IsObjectAt(int x, int y, int z);			//置く前にチェック

private:
	//置く位置を見やすくするもの
	void DrawSelectedTile();

	//マップのセーブ
	void SaveMap(const char* filename);
	//マップの読み込み
	void LoadMap(const char* filename);

	void DrawObjects();
	//マウスの当たり判定の取得
	bool GetMouseHitPosition(VECTOR* outPos);
	//グリッドの取得
	bool GetGridPos(VECTOR hitPos, int* _x, int* _z);

	int m_currentY;
public:
	//場所の取得
	const std::vector<Object>& GetObjects() const;
};