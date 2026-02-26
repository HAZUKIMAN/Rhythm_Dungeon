#include "MainStage.h"

// コンストラクタ・デストラクタ
CMainStage::CMainStage(){}

CMainStage::~CMainStage(){
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
}

// 初期化
void CMainStage::Init() {
	m_vPosition = VGet(0.0f, -170.0f, 600.0f);
	//m_iModelHdl = MV1LoadModel("Data/object/NewBossStage/BossFloorStage.mv1");
	m_iModelHdl = MV1LoadModel("Data/AddData/Stage/LenStage.mv1");
	m_iStageHndl = MV1LoadModel("Data/AddData/Stage/LenStage1.mv1");
	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	//コリジョン情報
	MV1SetPosition(m_iStageHndl, m_vPosition);

	//サイズ変更
	MV1SetScale(m_iModelHdl, VGet(0.05f, 0.05f, 0.05f));
	MV1SetScale(m_iStageHndl, VGet(0.05f, 0.05f, 0.05f));

	MV1SetupCollInfo(m_iModelHdl);
	MV1SetupCollInfo(m_iStageHndl);


	//m_iModelHdl = -1;
	m_Timer = 30;//120にしたいので
	m_num = 0;

	m_stage = STAGE1;

	//m_isHitFlag = false;
}

// 描画
void CMainStage::Draw() {

	if (m_Timer > 0)m_Timer -= 1;

	if (m_Timer == 0)
	{
		m_num += 1;

		if (m_num == 1)
		{
			m_stage = STAGE2; m_Timer = 30;
		}
		if (m_num == 2)
		{
			m_stage = STAGE1; m_num = 0; m_Timer = 30;
		}
	}

	if(m_stage  == STAGE1)MV1DrawModel(m_iModelHdl);
	if (m_stage == STAGE2)MV1DrawModel(m_iStageHndl);
}
//------------------------------
//		当たり判定
//------------------------------

VECTOR	CMainStage::HitCheck(VECTOR start, VECTOR end)
{
	VECTOR	out = end;

	MV1_COLL_RESULT_POLY	res;	// 当たり判定結果格納構造体
	res = MV1CollCheck_Line(m_iModelHdl, -1, start, end);	// 当たり判定結果格納

	// 障害物があった場合
	if (res.HitFlag == 1)
	{
		// 障害物とあたった場所へ移動
		out = res.HitPosition;
		// 上方向へ押し出す距離を計算し、加算する
		VECTOR v = VSub(end, res.HitPosition);
		float f = VSize(v);
		out.y += f;
	}

	return out;
}

