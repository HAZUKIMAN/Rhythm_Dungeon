#pragma once
#include <DxLib.h>
#include "CameraBase.h"

class CPlayCamera : public CCameraBase
{

public:
	// コンストラクタ・デストラクタ
	CPlayCamera();
	~CPlayCamera() override;

	// 毎フレーム呼ぶ処理
	void Step(CPlayer& player) override;
};