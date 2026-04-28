#pragma once
#include <DxLib.h>

class CAnime
{
public:

	//アニメーション構造体
	typedef struct {
		float m_fEndFrm;		//アニメ全再生時間
		float m_fFrm;			//アニメ再生時間
		float m_fSpd;			//アニメ再生速度
		int   m_iHndl;			//アニメハンドル
		int   m_iID;			//再生中のアニメハンドル
		int   m_iState;			//アニメ状態
	}ANIMEDATA;

private:

	//アニメステート
	enum AnimeState
	{
		ANIMSTATE_NORMAL,
		ANIMSTATE_LOOP,
		ANIMSTATE_ENDLOOP
	};

	ANIMEDATA m_sAnimeData;	//アニメ再生関連データ

public:

	//アニメリクエスト
	//iAnimID		:	アタッチするアニメ番号
	//iAnimSpd		:	アニメ再生速度
	//iAnimSrcHndl	:	アニメーションを使用する場合持っているハンドル(-1ならばこのモデルアニメ使用）
	//NameCheck		:	別モデルのアニメを使用する場合,モデルのフレーム名が一致しない場合
	//				    アニメをアタッチするか
	//void	RequestAnim(int iAnimID);
	 void	Request(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl = -1, bool NameCheck = false);
	//ループアニメリクエスト
	//@memo			:	アニメが最終フレームになったら最初に戻ります。
	 void	RequestLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl = -1, bool NameCheck = false);
	//ループアニメリクエスト
	//@memo			:	アニメが最終フレームになったらそのまま。
	 void	RequestEndLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl = -1, bool NameCheck = false);

	//アニメの更新
	 void	AnimeUpdate(int hndl);
	//アニメのデタッチ
	 void	DetachAnim(int hndl);

};


