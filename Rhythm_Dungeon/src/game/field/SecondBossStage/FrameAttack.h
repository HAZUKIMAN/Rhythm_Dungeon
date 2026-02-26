#pragma once

#include <DxLib.h>
#include "../../Object/Object.h"

class CSecondBossFrame : public CObject {
public:

	bool m_isHitFlag;	// ヒットしているかどうかを判断(通常はいらない)
	// コンストラクタ・デストラクタ
	CSecondBossFrame();
	~CSecondBossFrame();

	// 初期化
	void Init();

	// 描画
	void Draw();

	//ポジションを入れる
	void SetPos(VECTOR _pvec);
	
	inline int FieldHndl() { return m_iModelHdl; };
	
};
