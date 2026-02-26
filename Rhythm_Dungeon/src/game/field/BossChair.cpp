#include "BossChair.h"

// コンストラクタ・デストラクタ
CStageFrame::CStageFrame(){}
CStageFrame::~CStageFrame(){
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	m_iModelHdl = -1;
}

// 初期化
void CStageFrame::Init() {
	m_vPosition = VGet(0.0f, -200.0f, 1560.0f);
	m_iModelHdl = MV1LoadModel("Data/AddData/Stage/BossChair.mv1");
	//Data/object/field/field.x
	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, VGet(0.05f, 0.05f, 0.05f));
	MV1SetupCollInfo(m_iModelHdl);
	m_isHitFlag = false;
}

// 描画
void CStageFrame::Draw() {
	MV1DrawModel(m_iModelHdl);
	//MV1SetOpacityRate(m_iModelHdl, 0.8f);		//透過
}

//------------------------------
//		当たり判定
//------------------------------
VECTOR	CStageFrame::HitCheck(VECTOR start, VECTOR end)
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
