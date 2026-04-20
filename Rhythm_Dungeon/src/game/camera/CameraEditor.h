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
};
