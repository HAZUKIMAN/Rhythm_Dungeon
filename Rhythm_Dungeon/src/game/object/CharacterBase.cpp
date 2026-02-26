#include "CharacterBase.h"

#define WAITTIME (60)

static const int  ENEMY__NUM(50);

// コンストラクタ
CCharacterBase::CCharacterBase() {
}

// デストラクタ
CCharacterBase::~CCharacterBase() {
	if (m_iModelHdl != -1) {
		MV1DeleteModel(m_iModelHdl);
	}
}
//------------------------------
//		初期化
//------------------------------
void CCharacterBase::Init()
{
	memset(&m_vPosition, 0, sizeof(VECTOR));
	memset(&m_vRotation, 0, sizeof(VECTOR));
	memset(&m_vSpeed, 0, sizeof(VECTOR));

	m_jumpPow   = 0.0f;
	m_HitCount  = 0.0f;
	m_animIndex = -1.0f;
	m_iModelHdl = -1.0f;
	m_isActive  = true;
}


//------------------------------
//		ロード
//------------------------------
void CCharacterBase::Load(int hndl)
{
}
//------------------------------
//		ゲームメイン更新
//------------------------------
void CCharacterBase::Update()
{
}

//------------------------------
//		描画
//------------------------------
void CCharacterBase::Draw()
{
	if (!m_isActive) return;

	if (m_iModelHdl != -1)
	{
		MV1DrawModel(m_iModelHdl);
	}
}
//------------------------------
//		終了処理
//------------------------------
void CCharacterBase::Exit()
{
	if (m_iModelHdl != -1)
	{
		MV1DeleteModel(m_iModelHdl);
		m_iModelHdl = -1;
	}
}

//アニメリクエスト
//iAnimID		:	アタッチするアニメ番号
//iAnimSpd		:	アニメ再生速度
//iAnimSrcHndl	:	アニメーションを使用する場合持っているハンドル(-1ならばこのモデルアニメ使用）
//NameCheck		:	別モデルのアニメを使用する場合,モデルのフレーム名が一致しない場合
//				    アニメをアタッチするか
void CCharacterBase::Request(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl, bool NameCheck) {

	if (m_sAnimeData.m_iID == iAnimID)
	{
		return;
	}

	DetachAnim(hndl);

	//アニメ再生＆各種データのセット
	m_sAnimeData.m_iHndl = MV1AttachAnim(hndl, iAnimID, iAnimSrcHndl, NameCheck);
	m_sAnimeData.m_iID = iAnimID;
	m_sAnimeData.m_fSpd = iAnimSpd;
	m_sAnimeData.m_fEndFrm = MV1GetAnimTotalTime(hndl, iAnimID);
	m_sAnimeData.m_fFrm = 0.0f;
	m_sAnimeData.m_iState = ANIMSTATE_NORMAL;
}
//ループアニメリクエスト
//@memo			:	アニメが最終フレームになったら最初に戻ります。
void CCharacterBase::RequestLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl, bool NameCheck) {
	Request(iAnimID, iAnimSpd, hndl, iAnimSrcHndl, NameCheck);
	//ここだけループに変える
	m_sAnimeData.m_iState = ANIMSTATE_LOOP;
}
//ループアニメリクエスト
//@memo	: アニメが最終フレームになったらそのまま。
void CCharacterBase::RequestEndLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl, bool NameCheckl) {
	if (m_sAnimeData.m_iID == iAnimID)return;

	Request(iAnimID, iAnimSpd, hndl, iAnimSrcHndl, NameCheckl);
	//ここだけループに変える
	m_sAnimeData.m_iState = ANIMSTATE_ENDLOOP;
}
//アニメの更新
void  CCharacterBase::AnimeUpdate(int hndl)
{
	//アニメーションの速度更新
	m_sAnimeData.m_fFrm += m_sAnimeData.m_fSpd;

	if (m_sAnimeData.m_fFrm >= m_sAnimeData.m_fEndFrm)
	{
		//最終フレームまで来たら、状態に合わせて動作を変える
		switch (m_sAnimeData.m_iState)
		{
			//通常は終了させる
		case ANIMSTATE_NORMAL:
			DetachAnim(hndl);
			m_sAnimeData.m_iID = -1.0f;
			m_sAnimeData.m_fEndFrm = 0.0f;
			m_sAnimeData.m_fFrm = 0.0f;
			m_sAnimeData.m_fSpd = 0.0f;
			return;
			//ループは最初に戻るだけ
		case ANIMSTATE_LOOP:
			m_sAnimeData.m_fFrm = 0.0f;
			break;
			//エンドループは最後に止める
		case ANIMSTATE_ENDLOOP:
			m_sAnimeData.m_fFrm = m_sAnimeData.m_fEndFrm;
			break;
		}
	}
	//再生時間
	MV1SetAttachAnimTime(hndl, m_sAnimeData.m_iHndl, m_sAnimeData.m_fFrm);
}
//アニメのデタッチ
void  CCharacterBase::DetachAnim(int hndl) {
	//アニメの消去
	if (m_sAnimeData.m_iHndl != -1)
	{
		MV1DetachAnim(hndl, m_sAnimeData.m_iHndl);
		m_sAnimeData.m_iHndl = -1;
	}
}