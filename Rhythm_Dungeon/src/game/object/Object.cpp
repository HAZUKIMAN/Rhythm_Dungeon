#include "Object.h"


#define WAITTIME (60)

static constexpr int  ENEMY__NUM(50);

// デストラクタ
CObject::~CObject() {
	Fin();
}
//------------------------------
//		初期化
//------------------------------
void CObject::Init()
{
	memset(&m_vPosition, 0, sizeof(VECTOR));
	memset(&m_vRotation, 0, sizeof(VECTOR));
	memset(&m_vSpeed, 0, sizeof(VECTOR));

	m_iModelHdl = -1;
	m_isActive = true;
}
//------------------------------
//		ロード
//------------------------------
void CObject::Load(int hndl)
{
}
//------------------------------
//		ゲームメイン更新
//------------------------------
void CObject::Update()
{
}
//------------------------------
//		描画
//------------------------------
void CObject::Draw()
{
	if (!m_isActive) return;

	if (m_iModelHdl != -1)
	{
		MV1DrawModel(m_iModelHdl);
	}

}
//------------------------------
//		終了処理
//------------------------------
void CObject::Fin()
{
	if (m_iModelHdl != -1)
	{
		MV1DeleteModel(m_iModelHdl);
		m_iModelHdl = -1;
	}
}
