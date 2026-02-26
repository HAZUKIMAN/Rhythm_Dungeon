#pragma once
#include "DxLib.h"

//===========================================
//		サウンド操作クラス
//===========================================
class CSoundManager
{
public:
	enum tagSoundID{
		SOUNDID_BGM,		// ゲーム中のBGM
		SOUNDID_SE_PLSHOT,	// SE プレイヤーショット音
		SOUNDID_SE_EXPLORE,	// SE 爆発音

		SOUNDID_NUM
	};
private:
	static int m_iHndl[SOUNDID_NUM];	// サウンドハンドル

public:
	CSoundManager();
	~CSoundManager();

	//		初期化
	static void	Init();
	//		終了処理
	static void	Fin();

	//		全データ読み込み
	//	@return : true = 全ロード成功 false = 一つ以上読み込み失敗
	static bool	LoadAllData();

	//***************************************
	//		取得関係
	//***************************************
	//		再生時間取得(ミリ秒)
	//	@memo	:	圧縮形式の再生データは不可らしい
	static LONGLONG	GetSoundTime(tagSoundID iID);
	//		再生総時間取得(ミリ秒)
	static LONGLONG	GetSoundAllTime(tagSoundID iID);
	//		音楽再生判定
	//	@[iID]	:	データ読み込みした際のID番号
	//	@return	:	true = 再生中, false = 停止中
	static bool	IsPlay(tagSoundID iID);


	//***************************************
	//		設定関係
	//***************************************
	//		再生開始時間設定
	//	@[ms]	:	再生開始時間(ミリ秒)
	//	@memo	:	Play()前に設定し、Play()では途中から再生にしてください
	//				使用時は必ず停止してからにしましょう
	static void	SetStartFrame(tagSoundID iID, int ms);
	//		ボリューム設定
	//	@[fVol]	:	ボリューム(0.0～1.0)
	static void	SetVolume(tagSoundID iID, float fVol);


	//***************************************
	//		音楽再生関係
	//***************************************

	//		音楽再生
	//	@[iID]		:	データ読み込みした際のID番号
	//	@[iType]	:	再生タイプ　DXライブラリと同じです
	//					DX_PLAYTYPE_NORMAL　:　ノーマル再生
	//					DX_PLAYTYPE_BACK　　:　バックグラウンド再生
	//					DX_PLAYTYPE_LOOP　　:　ループ再生
	//	@[isStart]	:	true = 最初から再生, false = 途中から再生
	//	@return	:	0 = 成功, -1 = エラー
	static int	Play(tagSoundID iID, int iType = DX_PLAYTYPE_BACK, bool isStart = true);

	//		音楽停止
	//	@[iID]	:	データ読み込みした際のID番号
	//	@return	:	0 = 成功, -1 = エラー
	static int	Stop(tagSoundID iID);
	//		全音楽停止
	static void	StopAll(void);
};
