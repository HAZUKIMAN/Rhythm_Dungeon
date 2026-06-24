#include "Block.h"
#include "../common.h"

// コンストラクタ・デストラクタ
CBlock::CBlock(){}

CBlock::~CBlock(){
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	
}

// 初期化
void CBlock::Init() {
	CObject::Init();
	m_vPosition = VGet(0.0f, 0.0f, 0.0f);

	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	m_isActive = true;
}

//データのロード
void CBlock::Load(int hndl)
{
	VECTOR model_size = VGet(0.03f, 0.03f, 0.03f);

	CObject::Load(hndl, model_size);

	MV1SetupCollInfo(m_iModelHdl);
}

//更新処理
void CBlock::Update()
{
	// モデルへ反映
	MV1SetPosition(m_iModelHdl, m_vPosition);
}

// 描画
void CBlock::Draw() {
	MV1DrawModel(m_iModelHdl);
}
