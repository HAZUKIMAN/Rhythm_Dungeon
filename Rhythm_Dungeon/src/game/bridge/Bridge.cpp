#include "Bridge.h"

// コンストラクタ・デストラクタ
CBridge::CBridge() {}

CBridge::~CBridge() {
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	m_iModelHdl = -1;
}

// 初期化
void CBridge::Init() {
	m_iModelHdl = MV1LoadModel("Data/object/bridge/bridge.mv1");
	VECTOR size = VGet(0.08f, 0.08f, 0.08f);
	memset(&m_vRotation, 0, sizeof(VECTOR));
	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, size);
	MV1SetupCollInfo(m_iModelHdl);
	m_isHitFlag = false;

}
//更新処理
void CBridge::Update()
{
	VECTOR vec = VGet(0.0f, m_vRotation.y, 0.0f);
	// モデルへ反映
	MV1SetRotationXYZ(m_iModelHdl, vec);
	MV1SetPosition(m_iModelHdl, m_vPosition);
}

// 描画
void CBridge::Draw() {

	MV1DrawModel(m_iModelHdl);
}