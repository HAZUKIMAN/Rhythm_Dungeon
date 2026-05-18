#pragma once
#include "../object/Actor.h"
#include "../field/InstalledItem.h"
#include "../map/MapEditor.h"


class CCat : public CActor
{
public:

	enum tagCatState {
		PLAYER_STATE_NORMAL,	// 待機
		PLAYER_STATE_RUN,		// 歩き中
		PLAYER_STATE_JUMP,		// ジャンプ中

		PLAYER_STATE_NUM
	};

private:

	tagCatState m_state;	// プレイヤーの状態

	enum ObjectType {
		OBJ_NONE,
		OBJ_PLAYER,
		OBJ_ENEMY,
		OBJ_ITEM,
		OBJ_GOAL,
		OBJ_PUT_BOX,
		OBJ_SETBLOCK
	};


	enum TileType {
		TILE_NONE = 0,
		TILE_FLOOR = 1,
		TILE_WALL = 2
	};

	//現在の床面(動くと起用のもの)
	enum MoveMode
	{
		MOVE_GROUND,   // 地面
		MOVE_WALL_X,   // X壁
		MOVE_WALL_Z,   // Z壁
		MOVE_CEILING   // 天井
	};

	MoveMode m_moveMode;

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

private:
	// 移動計算結果を反映
	void Move();
	// 待機･移動中処理
	void NormalExec(MapEditor& map);
	//操作関係処理
	void Operation(MapEditor& map);
	// 床チェック
	bool CheckGround(MapEditor& map);
	// 壁チェック
	WallType CheckWall(MapEditor& map);

};