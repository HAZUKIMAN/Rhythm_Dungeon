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
	};


	enum TileType {
		TILE_NONE = 0,
		TILE_FLOOR = 1,
		TILE_WALL = 2
	};


	// 移動モード
	enum MoveMode
	{
		MOVE_GROUND,	// 地面
		MOVE_WALL,		// 壁
		MOVE_CEILING	// 天井（必要なら）
	};

	MoveMode m_moveMode;
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

private:
	// 移動計算結果を反映
	void Move();
	// 待機･移動中処理
	void NormalExec(MapEditor& map);
	// 壁チェック
	bool CheckWall(MapEditor& map);
};