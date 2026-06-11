#pragma once
#include "../object/Actor.h"
#include "../field/InstalledItem.h"
#include "../map/MapEditor.h"


class CCat : public CActor
{
public:

	enum tagCatState {
		CAT_STATE_NORMAL,	// 待機
		CAT_STATE_WIDEMOVE,	// 横揺れ
		CAT_STATE_WALK,		// 歩き中
		CAT_STATE_RUN,		// 走り中
		CAT_STATE_EAT,		// 食べる
		CAT_STATE_DANCE,	// 踊る

		CAT_STATE_NUM
	};

private:

	int m_iPutModel[2];

	tagCatState m_state;	// 人間の状態

	enum ObjectType {
		OBJ_NONE,
		OBJ_HUMAN,
		OBJ_CAT,
		OBJ_ITEM,
		OBJ_GOAL,
		OBJ_PUT_BOX,
		OBJ_SETBLOCK
	};

	enum Direction
	{
		ROTATION_RIGHT = 0,
		ROTATION_DOWN,
		ROTATION_LEFT,
		ROTATION_UP
	};

	enum TileType {
		TILE_NONE = 0,
		TILE_FLOOR = 1,
		TILE_STAIRS = 2,
		TILE_FLOOR2 = 3,
		TILE_BRIDGE = 4,
	};

	// 階段の種類
	enum StairsType
	{
		Stairs_NONE, // 階段なし
		Stairs_X,    // 左右の階段
		Stairs_Z     // 前後の階段
	};


public:
	// コンストラクタ・デストラクタ
	CCat();
	~CCat() override;

	// 初期化
	void Init() override;

	// データロード
	void Load();
	// 毎フレーム呼ぶ処理
	void Step(MapEditor& map);
	// 描画処理
	void Draw() override;
	//ブロックの設置
	void PlaceBlock(ObjectEditor& objEditor);
	//物を置ける位置を表示
	void DrawPlaceBlockPreview(MapEditor& map);
	// 猫の向き取得
	int GetDirection();
	//クリアしたとき
	void Clear();
private:

	// 移動計算結果を反映
	void Move();
	// 待機･移動中処理
	void NormalExec(MapEditor& map);
	//操作関係処理
	void Operation(MapEditor& map);
	// 床チェック
	bool CheckGround(MapEditor& map);

};
