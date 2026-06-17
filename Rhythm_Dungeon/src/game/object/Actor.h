#pragma once
#include "../../lib/object/Object.h"
#include <DxLib.h>
#include"../Anime/Anime.h"

// ゲーム内で実際に使う3Dキャラ
class CActor : public CObject, public CAnime
{
protected:
	VECTOR	m_speed;		// 移動速度
	float	m_radius;		// 半径(今回は球なのでfloat)
	int		m_hitCoolTime;	// クールタイム
	int		m_shadowHdl;	//影のハンドル

public:

	// コンストラクタ・デストラクタ
	CActor();
	virtual ~CActor();

	// 初期化
	virtual void Init();

	// 描画
	virtual void Draw();

	// 更新したデータを反映させる
	virtual void Update();
	
	//===============================
	//		取得・設定関連
	//===============================
	// 
	// 半径
	virtual inline float GetRadius(void){ return m_radius; }
	virtual inline void SetRadius(float rad) { m_radius = rad; }

	// 速度
	virtual inline VECTOR GetSpeed(void) { return m_speed; }
	virtual inline void SetSpeed(const VECTOR& speed) { m_speed = speed; }

	virtual void AddPos(VECTOR Hit);

	virtual inline int GetHitTime() { return m_hitCoolTime; }
	virtual inline int SetHitTime(int time) { return m_hitCoolTime = time; }


	// 当たり判定に使う中心の座標
	virtual VECTOR GetCenter();
};