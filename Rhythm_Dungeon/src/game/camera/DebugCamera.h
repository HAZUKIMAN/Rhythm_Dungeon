#pragma once
#include <DxLib.h>
#include "CameraBase.h"

// プレイヤー情報取得のため
class CPlayer;
// エネミー情報取得のため
class CEnemyManager;

class CDebugCamera : public CCameraBase
{
public:
	// コンストラクタ・デストラクタ
	CDebugCamera();
	~CDebugCamera() override;

	// 毎フレーム呼ぶ処理
	void Step(CPlayer& player) override;
	// 描画
	void Draw() override;

private:
	// 注視点の上下移動
	void MoveUpDown();
	// 注視点の前後移動
	void MoveFrontBack();
	// 注視点の左右移動
	void MoveLeftRight();
	// 注視点の左右回転
	void RotateLeftRight();
};