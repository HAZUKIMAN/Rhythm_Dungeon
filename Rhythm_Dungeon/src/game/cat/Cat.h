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
	bool m_canUseStairs;
	VECTOR m_recpos;
	int m_iPutModel[2];
	bool m_isStairs;
	float m_stairTargetY;
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
		ROTATION_RIGHT,
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

	// 壁の種類
	enum WallType
	{
		WALL_NONE, // 壁なし
		WALL_X,    // 左右の壁
		WALL_Z     // 前後の壁
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
	//リスポーン地点の取得
	VECTOR SetRespawn(VECTOR res) { return m_recpos = res; }
	//猫のクリア処理
	void Clear();

private:
	// 待機･移動中処理
	void NormalExec(MapEditor& map);
	// 移動計算結果を反映
	void Move();
	// 床チェック
	bool CheckGround(MapEditor& map);

};
