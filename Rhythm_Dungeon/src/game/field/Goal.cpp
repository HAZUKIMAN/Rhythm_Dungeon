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
	m_iModelHdl = MV1LoadModel("Data/object/Flag/Flag.x");
	VECTOR size = VGet(0.1f, 0.1f, 0.1f);
	//Data/object/field/field.x
	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, size);
	MV1SetupCollInfo(m_iModelHdl);
	m_isHitFlag = false;
}

// 描画
void CGoal::Draw() {
	// モデルへ反映
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1DrawModel(m_iModelHdl);
}