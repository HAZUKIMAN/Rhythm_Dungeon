#pragma once

#include "../object/Object.h"

class CGoal : public CObject {
public:

	bool m_isHitFlag;	// ヒットしているかどうかを判断(通常はいらない)

	// コンストラクタ・デストラクタ
	CGoal();
	~CGoal();

	// 初期化
	void Init();

	// 描画
	void Draw();

	//当たり判定処理
	//@center :当たり判定をする相手の座標
	//@radius :相手を球として判定するのでその半径
	//@return :押し戻す方向&距離
	//VECTOR HitCheck(VECTOR center, float radius);

	inline int FieldHndl(){ return m_iModelHdl; };
};