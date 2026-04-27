#pragma once
#include <DxLib.h>
#include "CameraBase.h"

class CEditorCamera : public CCameraBase
{

public:
	// コンストラクタ・デストラクタ
	CEditorCamera();
	~CEditorCamera() override;

	void Step(CPlayer& player);

	// 注視点の上下移動
	void MoveUpDown();
	// 注視点の前後移動
	void MoveFrontBack();
	// 注視点の左右移動
	void MoveLeftRight();
	// 注視点の左右回転
	void RotateLeftRight();

};
