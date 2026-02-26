#pragma once
#include "../object/Object.h"
#include <DxLib.h>

//	ゲーム内で実際に使う3Dキャラ
class CActor : public CObject
{
protected:
	VECTOR m_speed;		// 移動速度
	float m_radius;		// 半径(今回は球なのでfloat)

public:
	//	コンストラクタ・デストラクタ
	CActor();
	virtual ~CActor();

	//	初期化
	virtual void Init();
	//	当たり判定後の処理
	virtual void HitCalc();
	
	//===============================
	//		取得・設定関連
	//===============================
	//	半径
	float GetRadius(void){ return m_radius; }
	void SetRadius(float rad) { m_radius = rad; }
	//	速度
	VECTOR GetSpeed(void) { return m_speed; }
	void SetSpeed(const VECTOR& speed) { m_speed = speed; }
	//	当たり判定に使う中心の座標
	virtual VECTOR GetCenter();
};