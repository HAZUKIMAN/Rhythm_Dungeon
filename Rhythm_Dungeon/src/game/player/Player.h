#pragma once
#include "../object/Actor.h"


class CPlayer : public CActor
{
private:

	enum tagPlayerState {
		PLAYER_STATE_NORMAL,	// 待機
		PLAYER_STATE_RUN,		// 歩き中
		PLAYER_STATE_JUMP,		// ジャンプ中

		PLAYER_STATE_NUM
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
	tagPlayerState m_state;	// プレイヤーの状態

public:
	// コンストラクタ・デストラクタ
	CPlayer();
	~CPlayer() override;

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
	// 待機･移動中処理
	void NormalExec();
};