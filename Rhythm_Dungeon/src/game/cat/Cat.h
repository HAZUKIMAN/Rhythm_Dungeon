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

	enum TileType {
		TILE_NONE  = 0,
		TILE_FLOOR = 1,
		TILE_WALL  = 2
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
	void Step();
	// 描画処理
	void Draw() override;

	void PlaceBlock(MapEditor& map);

private:
	// 移動計算結果を反映
	void Move();
	// 待機･移動中処理
	void NormalExec();

};