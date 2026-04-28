#pragma once
#include "../object/Actor.h"


class CPlayer : public CActor
{
public:
	enum tagPlayerState {
		PLAYER_STATE_NORMAL,	// 待機
		PLAYER_STATE_RUN,		// 歩き中
		PLAYER_STATE_JUMP,		// ジャンプ中

		PLAYER_STATE_NUM
	};

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

private:
	// 移動計算結果を反映
	void Move();
	// 待機･移動中処理
	void NormalExec();
	// ジャンプ中処理
	void JumpExec();
};