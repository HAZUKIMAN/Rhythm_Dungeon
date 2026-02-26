#pragma once
#include <DxLib.h>

// プレイヤー情報取得のため
class CPlayer;

class CCameraBase
{
protected:
	VECTOR m_pos;		// 視点
	VECTOR m_focus;		// 注視点
	VECTOR m_up;		// カメラの上方向

public:
	// コンストラクタ・デストラクタ
	CCameraBase();
	virtual ~CCameraBase();

	// 初期化
	//	@vPos		:	視点
	//	@vFocus		:	注視点
	//	@vUp		:	上方向
	virtual void Init(VECTOR vPos, VECTOR vFocus, VECTOR vUp);

	// 終了処理
	virtual void Fin();

	// 毎フレーム呼ぶ処理
	virtual void Step(CPlayer& player);
	// 更新処理
	virtual void Update();
	// 描画
	virtual void Draw();

	//	取得・設定関連===============
	// 視点取得
	VECTOR GetPosition() { return m_pos; }
	void SetPosition(VECTOR& pos) { m_pos = pos; }
	// 注視点取得
	VECTOR GetFocus() { return m_focus; }
	void SetFocus(VECTOR& focus) { m_focus = focus; }
	// アップベクトル取得
	VECTOR GetUpvec() { return m_up; }
	void SetUpvec(VECTOR& up) { m_up = up; }
};