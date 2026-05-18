#include "Block.h"
#include "../common.h"

// コンストラクタ・デストラクタ
CBlock::CBlock(){}

CBlock::~CBlock(){
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	m_iModelHdl = -1;
}

// 初期化
void CBlock::Init() {
	m_vPosition = VGet(0.0f, 0.0f, 0.0f);
	m_iModelHdl = MV1LoadModel("Data/object/stage/Wall.mv1");

	VECTOR size = VGet(0.03f, 0.03f, 0.03f);

	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, size);
	MV1SetupCollInfo(m_iModelHdl);
	m_isActive = true;
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
	DrawSphere3D(m_vPosition, 1.0f, 16, YELLOW, YELLOW, TRUE);
}
