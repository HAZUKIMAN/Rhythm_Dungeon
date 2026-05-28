#include "InstalledItem.h"

// コンストラクタ・デストラクタ
CInstalledItem::CInstalledItem() {}

CInstalledItem::~CInstalledItem(){
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	m_iModelHdl = -1;
}

// 初期化
void CInstalledItem::Init() {
	m_iModelHdl = MV1LoadModel("Data/object/stage/Wall.mv1");
	VECTOR size = VGet(0.03f, 0.03f, 0.03f);

	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, size);
	MV1SetupCollInfo(m_iModelHdl);
	m_isHitFlag = false;
	
}
//更新処理
void CInstalledItem::Update()
{
	// モデルへ反映
	MV1SetPosition(m_iModelHdl, m_vPosition);
}

// 描画
void CInstalledItem::Draw() {

	MV1DrawModel(m_iModelHdl);
}