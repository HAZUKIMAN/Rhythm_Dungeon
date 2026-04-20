#pragma once
#include "DxLib.h"

#include "../Object/Object.h"

#define NUMBER (2)
//#define FLAG_NUM	(5)

class CSky : public CObject {
private:
	VECTOR m_Rot;
public:
	// コンストラクタ・デストラクタ
	CSky();
	~CSky();

	// 初期化
	void Init();
	// 描画
	void Draw();
};
