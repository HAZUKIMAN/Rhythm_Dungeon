#pragma once
#include "../object/Actor.h"
#include <vector>
#include "../field/Block.h"
#include "../field/InstalledItem.h"

class CEnemy : public CActor
{
private:

	enum tagEnemyState {
		ENEMY_STATE_NORMAL,	// 待機
		ENEMY_STATE_RUN,		// 歩き中
		ENEMY_STATE_HIT,		// ジャンプ中

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

	tagEnemyState m_state;	// 人間の状態


	bool m_isMoving;
	VECTOR m_targetPos;
	int m_coolTime;
	int m_moveX;
	int m_moveZ;

public:
	// コンストラクタ・デストラクタ
	CEnemy();
	~CEnemy() override;

	// 初期化
	void Init() override;

	// データロード
	void Load();
	// 毎フレーム呼ぶ処理
	void Step();
	// 描画処理
	void Draw() override;

	void SetDirect(int dir);

	int  GetDirect() { return direction; }
private:
	// 移動角度処理
	void Direction();
	// 移動計算結果を反映
	void Move();

public:
	// 待機･移動中処理
	void NormalExec(const std::vector<CBlock*>& blocks);
};
