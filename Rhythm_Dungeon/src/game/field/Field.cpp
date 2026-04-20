#include "Field.h"

// コンストラクタ・デストラクタ
CField::CField(){}
CField::~CField(){
	MV1TerminateCollInfo(m_iModelHdl);
	MV1DeleteModel(m_iModelHdl);
	m_iModelHdl = -1;
}

// 初期化
void CField::Init() {
	m_vPosition = VGet(0.0f, 300.0f, 0.0f);
	m_iModelHdl = MV1LoadModel("Data/object/NewBossStage/UpStage.mv1");

	//コリジョン情報
	MV1SetPosition(m_iModelHdl, m_vPosition);
	MV1SetScale(m_iModelHdl, VGet(0.4f, 0.2f, 0.4f));
	MV1SetupCollInfo(m_iModelHdl);
	m_isHitFlag = false;
	
}

// 描画
void CField::Draw() {

	MV1DrawModel(m_iModelHdl);
}

//------------------------------
//		当たり判定
//------------------------------
VECTOR	CField::HitCheck(VECTOR start, VECTOR end)
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
