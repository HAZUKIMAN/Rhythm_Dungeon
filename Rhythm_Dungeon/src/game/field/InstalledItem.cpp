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
	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetupCollInfo(m_iModelHdl);
	m_isHitFlag = false;
	
}

//データのロード
void CInstalledItem::Load(int hndl)
{
	VECTOR model_size = VGet(0.03f, 0.03f, 0.03f);
	
	CObject::Load(hndl,model_size);

	MV1SetupCollInfo(m_iModelHdl);
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