#include"Anime.h"

//RequestLoop(PLAYER_STATE_WALK, 0.5f, m_iModelHdl); 
//m_eState = PLAYER_STATE_WALK;
//DetachAnim(m_iModelHdl);


//---------------------------
//		アニメリクエスト
//----------------------------
//iAnimID		:	アタッチするアニメ番号
//iAnimSpd		:	アニメ再生速度
//iAnimSrcHndl	:	アニメーションを使用する場合持っているハンドル(-1ならばこのモデルアニメ使用）
//NameCheck		:	別モデルのアニメを使用する場合,モデルのフレーム名が一致しない場合
//				    アニメをアタッチするか
void CAnime::Request(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl, bool NameCheck) {

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
void CAnime::RequestLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl, bool NameCheck) {
	Request(iAnimID, iAnimSpd, hndl, iAnimSrcHndl, NameCheck);
	//ここだけループに変える
	m_sAnimeData.m_iState = ANIMSTATE_LOOP;
}

//----------------------------
//	ループアニメリクエスト
//-----------------------------
//@memo	: アニメが最終フレームになったらそのまま。
void CAnime::RequestEndLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl, bool NameCheckl) {
	if (m_sAnimeData.m_iID == iAnimID)return;

	Request(iAnimID, iAnimSpd, hndl, iAnimSrcHndl, NameCheckl);
	//ここだけループに変える
	m_sAnimeData.m_iState = ANIMSTATE_ENDLOOP;
}

//-------------------------------
//		アニメの更新
//-------------------------------
void  CAnime::AnimeUpdate(int hndl)
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

//------------------------------
//		アニメのデタッチ
//------------------------------
void  CAnime::DetachAnim(int hndl) {
	//アニメの消去
	if (m_sAnimeData.m_iHndl != -1)
	{
		MV1DetachAnim(hndl, m_sAnimeData.m_iHndl);
		m_sAnimeData.m_iHndl = -1;
	}
}