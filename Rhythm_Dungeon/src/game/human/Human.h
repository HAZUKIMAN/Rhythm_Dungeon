#pragma once
#include "../object/Actor.h"
#include <vector>
#include "../field/Block.h"
#include "../field/InstalledItem.h"
#include "../Anime/Anime.h"
#include "../common.h"
#include "../../lib/sound/effectData/effectData.h"
#include "../effect/effekseer.h"
#include "../map/MapEditor.h"

using namespace std;

class CHuman : public CActor
{
private:

	enum taghumanState {
		HUMAN_STATE_NORMAL,	// 待機
		HUMAN_STATE_RUN,	// 歩き中
		HUMAN_STATE_HIT,	// ジャンプ中

		HUMAN_STATE_NUM
	};

	enum tagDirection
	{
		ROTATION_RIGHT,
		ROTATION_DOWN,
		ROTATION_LEFT,
		ROTATION_UP,
	};

	enum TileType {
		TILE_NONE = 0,
		TILE_FLOOR = 1,
		TILE_STAIRS = 2,
		TILE_FLOOR2 = 3,
		TILE_BRIDGE = 4,
	};

	tagDirection direction;

	enum tagMove
	{
		NONE,
		CARRY,
		PUT,
	};

private:

	taghumanState m_state;	// 人間の状態

	bool m_isMoving;
	int m_coolTime;
	int m_moveX;
	int m_moveZ;
	float m_setrot;
	bool m_isStairs;
	float m_stairTargetY;
	VECTOR m_recpos;
	VECTOR m_targetPos;

public:
	// コンストラクタ・デストラクタ
	CHuman();
	~CHuman() override;

	// 初期化
	void Init() override;
	// データロード
	void Load(int hndl);
	// 毎フレーム呼ぶ処理
	void Step();
	// 描画処理
	void Draw() override;
	//プレイヤーの向かせる角度の取得
	void SetDirect(int dir);
	//リスポーン地点の取得
	VECTOR SetRespawn(VECTOR res) { return m_recpos = res; }
	//角度を保存
	float Setrot(float rot) { return m_setrot = rot; }
	//リセット用
	void Reset();
	// 当たり判定後の処理
	void HitCalc();
	//方向の確保
	int  GetDirect() { return direction; }
	//クリアしたとき
	void Clear();

private:
	// 移動角度処理
	void Direction();
	// 移動計算結果を反映
	void Move();

public:
	// 待機･移動中処理
	void NormalExec(vector<CBlock*>& blocks, vector<CInstalledItem*>& institem, MapEditor& map, float cat_state);
};