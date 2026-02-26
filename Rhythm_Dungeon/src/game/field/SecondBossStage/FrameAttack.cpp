#include "FrameAttack.h"

// コンストラクタ・デストラクタ
CSecondBossFrame::CSecondBossFrame() {}

CSecondBossFrame::~CSecondBossFrame() {
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	m_iModelHdl = -1;
}

// 初期化
void CSecondBossFrame::Init() {

	m_vPosition = VGet(0.0f, -150.0f, 0.0f);

	m_iModelHdl = MV1LoadModel("Data/object/SecondBoss/AttackFrame.mv1");

	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, VGet(1.4f, 1.0f, 1.3f));
	MV1SetupCollInfo(m_iModelHdl);

	m_isHitFlag = false;
}

// 描画
void CSecondBossFrame::Draw() {


	MV1DrawModel(m_iModelHdl);

	MV1SetPosition(m_iModelHdl, m_vPosition);

}

//ポジションを入れる
void CSecondBossFrame::SetPos(VECTOR _pvec)
{
	m_vPosition = _pvec;
	m_vPosition.y = -150.0f;

}