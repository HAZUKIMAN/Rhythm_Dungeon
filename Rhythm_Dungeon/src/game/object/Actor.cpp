#include "Actor.h"
#include "../../lib/object/Object.h"
#include "../common.h"


//-------------------------------
//		コンストラクタ
//-------------------------------
CActor::CActor() : m_speed(VGet(0.0f, 0.0f, 0.0f)), m_radius(2.5f), m_hitCoolTime(HIT_TIME)
{

}


//-------------------------------
//		デストラクタ
//-------------------------------
CActor::~CActor()
{
	DetachAnim(m_iModelHdl);
}


//-------------------------------
//		初期化
//-------------------------------
void CActor::Init()
{
	CObject::Init();
}


//------------------------------
//		ゲームメイン更新
//------------------------------
void CActor::Update()
{
	//---------------------------------
	// クールタイム減少
	//---------------------------------
	if (m_hitCoolTime > 0)
	{
		m_hitCoolTime--;
	}

	AnimeUpdate(m_iModelHdl);
	MV1SetRotationXYZ(m_iModelHdl, m_vRotation);
	MV1SetPosition(m_iModelHdl, m_vPosition);
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

	m_speed.y = 0;
	// モデルへ反映
	MV1SetPosition(m_iModelHdl, m_vPosition);
}

