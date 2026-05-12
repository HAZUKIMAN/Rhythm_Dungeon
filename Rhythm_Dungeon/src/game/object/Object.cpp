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
}
//------------------------------
//		ロード
//------------------------------
void CObject::Load(int hndl)
{
	m_iModelHdl = hndl;
}
//------------------------------
//		ゲームメイン更新
//------------------------------
void CObject::Update()
{
	//AnimeUpdate(m_iModelHdl);
	MV1SetRotationXYZ(m_iModelHdl, m_vRotation);
	MV1SetPosition(m_iModelHdl, m_vPosition);
}
//------------------------------
//		描画
//------------------------------
void CObject::Draw()
{
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
} //m_vPosition = pos;

void CObject::SetPos(VECTOR pos)
{
	m_vPosition = pos;
	MV1SetPosition(
		m_iModelHdl,
		m_vPosition
	);
}


