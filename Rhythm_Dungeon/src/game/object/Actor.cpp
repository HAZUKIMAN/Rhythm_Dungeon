#include "Actor.h"
#include "Object.h"

//-------------------------------
//		コンストラクタ
//-------------------------------
CActor::CActor() : m_speed(VGet(0.0f, 0.0f, 0.0f)), m_radius(0.0f)
{
}


//-------------------------------
//		デストラクタ
//-------------------------------
CActor::~CActor()
{
}


//-------------------------------
//		初期化
//-------------------------------
void CActor::Init()
{
	CObject::Init();
	m_radius = 0.0f;
}


//-------------------------------
//		当たり判定後の処理
//-------------------------------
void CActor::HitCalc()
{
	// とりあえずはフラグを消すだけ
	m_isActive = false;
}


//-------------------------------
//		当たり判定に使う中心の座標取得
//-------------------------------
VECTOR CActor::GetCenter()
{
	VECTOR pos = m_vPosition;
	pos.y += m_radius;
	return pos;
}

void  CActor::AddPos(VECTOR Hit)
{
	// 押し戻しが無ければ終了
	if (Hit.x == 0.0f &&
		Hit.y == 0.0f &&
		Hit.z == 0.0f) return;

	// 現在位置に押し戻しを加算
	m_vPosition = VAdd(m_vPosition, Hit);

	// 床に埋まらないよう固定
	//m_speed.y = 0.0f;

	// モデルへ反映
	MV1SetPosition(m_iModelHdl, m_vPosition);
}

