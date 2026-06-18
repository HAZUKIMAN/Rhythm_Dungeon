#include "Goal.h"

// コンストラクタ・デストラクタ
CGoal::CGoal(){}
CGoal::~CGoal(){
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	m_iModelHdl = -1;
}

// 初期化
void CGoal::Init() {
	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetupCollInfo(m_iModelHdl);
	m_isHitFlag = false;
}

//データのロード
void CGoal::Load(int hndl)
{
	VECTOR size = VGet(0.5f, 0.5f, 0.5f);

	m_iModelHdl = hndl;
	CObject::Load(m_iModelHdl,size);
	MV1SetupCollInfo(m_iModelHdl);
}

// 描画
void CGoal::Draw() {
	// モデルへ反映
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1DrawModel(m_iModelHdl);
}