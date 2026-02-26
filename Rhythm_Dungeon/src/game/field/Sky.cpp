#include"Sky.h"

// コンストラクタ・デストラクタ
CSky::CSky() {}
CSky::~CSky() {}


// 初期化
void CSky::Init() {
	m_vPosition = VGet(0.0f, 0.0f, 0.0f);
	m_Rot= VGet(0.0f, 0.0f, 0.0f);
	m_iModelHdl = MV1LoadModel("data/object/sky/Sky1.mv1");//"data/object/sky/sky.mv1"
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, VGet(8.8f, 8.8f, 8.8f));
}

// 描画
void CSky::Draw() {
	m_Rot.y+= 0.0001f;
	MV1SetRotationXYZ(m_iModelHdl, m_Rot);
	MV1DrawModel(m_iModelHdl);
}