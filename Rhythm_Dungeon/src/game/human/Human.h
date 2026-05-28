#pragma once
#include "../object/Actor.h"
#include <vector>
#include "../field/Block.h"


class CHuman : public CActor
{
private:

	enum taghumanState {
		HUMAN_STATE_NORMAL,	// 待機
		HUMAN_STATE_RUN,		// 歩き中
		HUMAN_STATE_HIT,		// ジャンプ中

		HUMAN_STATE_NUM
	};

	enum tagDirection
	{
		ROTATION_RIGHT,
		ROTATION_DOWN,
		ROTATION_LEFT,
		ROTATION_UP,
	};

	tagDirection direction;


private:

	taghumanState m_state;	// 人間の状態


	bool m_isMoving;
	VECTOR m_targetPos;
	int m_coolTime;
	int m_moveX;
	int m_moveZ;
	VECTOR m_recpos;

public:
	// コンストラクタ・デストラクタ
	CHuman();
	~CHuman() override;

	// 初期化
	void Init() override;

	// データロード
	void Load();
	// 毎フレーム呼ぶ処理
	void Step();
	// 描画処理
	void Draw() override;
	//プレイヤーの向かせる角度の取得
	void SetDirect(int dir);
	//リスポーン地点の取得
	VECTOR SetRespawn(VECTOR res) { return m_recpos = res; }
	//方向の確保
	int  GetDirect() { return direction; }
private:
	// 移動角度処理
	void Direction();
	// 移動計算結果を反映
	void Move();

public:
	// 待機･移動中処理
	void NormalExec(std::vector<CBlock*>& blocks);
};