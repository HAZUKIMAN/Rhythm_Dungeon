#include "SoundManager.h"

int CSoundManager::m_iHndl[SOUNDID_NUM];


//---------------------------------
//		コンストラクタ
//---------------------------------
CSoundManager::CSoundManager(void)
{
	for (int i = 0; i < SOUNDID_NUM; i++)
	{
		m_iHndl[i] = -1;
	}
}


//---------------------------------
//		デストラクタ
//---------------------------------
CSoundManager::~CSoundManager(void)
{
	Fin();
}


//---------------------------------
//		初期化
//---------------------------------
void	CSoundManager::Init(void)
{
	for (int i = 0; i < SOUNDID_NUM; i++)
	{
		m_iHndl[i] = -1;
	}
}


//---------------------------------
//		終了処理
//---------------------------------
void	CSoundManager::Fin(void)
{

	for (int i = 0; i < SOUNDID_NUM; i++)
	{
		if (m_iHndl[i] != -1)
		{
			DeleteSoundMem(m_iHndl[i]);
			m_iHndl[i] = -1;
		}
	}
}


//---------------------------------
//		全データ読み込み
//---------------------------------
bool	CSoundManager::LoadAllData()
{
	bool isRet = true;
	const char pFileName[SOUNDID_NUM][128] =
	{
		"data/sound/bgm00.mp3", "data/sound/se_plshot.mp3", "data/sound/se_explore.mp3"
	};

	for (int i = 0; i < SOUNDID_NUM; i++)
	{
		m_iHndl[i] = LoadSoundMem(pFileName[i]);
		// 一つでもロード失敗があればフラグをオフに
		if (m_iHndl[i] == -1) isRet = false;
	}

	return isRet;
}


//---------------------------------
//		再生時間取得(ミリ秒)
//---------------------------------
LONGLONG		CSoundManager::GetSoundTime(tagSoundID iID)
{
	return	GetSoundCurrentTime(m_iHndl[iID]);
}


//---------------------------------
//		再生総時間取得(ミリ秒)
//---------------------------------
LONGLONG		CSoundManager::GetSoundAllTime(tagSoundID iID)
{
	return	GetSoundTotalTime(m_iHndl[iID]);
}


//---------------------------------
//		音楽再生判定
//---------------------------------
bool	CSoundManager::IsPlay(tagSoundID iID)
{
	return CheckSoundMem(m_iHndl[iID]) == 1 ? true : false;
}


//---------------------------------
//		再生開始時間設定
//---------------------------------
void	CSoundManager::SetStartFrame(tagSoundID iID, int ms)
{
	// 指定IDの周波数を取得して設定
	int	iFreq = GetFrequencySoundMem(m_iHndl[iID]) * ms / 1000;
	SetCurrentPositionSoundMem(iFreq, m_iHndl[iID]);
}


//---------------------------------
//		ボリューム設定
//---------------------------------
void	CSoundManager::SetVolume(tagSoundID iID, float fVol)
{
	if (fVol < 0.f || fVol > 1.f) return;
	ChangeVolumeSoundMem((int)(255.f * fVol), m_iHndl[iID]);
}


//---------------------------------
//		音楽再生
//---------------------------------
int		CSoundManager::Play(tagSoundID iID, int iType, bool isStart)
{
	return PlaySoundMem(m_iHndl[iID], iType, isStart);
}


//---------------------------------
//		音楽停止
//---------------------------------
int		CSoundManager::Stop(tagSoundID iID)
{
	return StopSoundMem(m_iHndl[iID]);
}


//---------------------------------
//		全音楽停止
//---------------------------------
void	CSoundManager::StopAll(void)
{
	for (int i = 0; i < SOUNDID_NUM; i++)
		StopSoundMem(m_iHndl[i]);
}