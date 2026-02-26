#include <DxLib.h>
#include "Frame.h"
#include "../../game/common.h"


double CFrameRate::m_frameRate;			
int CFrameRate:: m_count;				
int CFrameRate::m_updateFrameRateTime;	
int CFrameRate::m_currentTime;			
int CFrameRate::m_lastTime;				



//-------------------------------
//		フレームレート初期化
//-------------------------------
void CFrameRate::Init()
{
	// データを全て初期化しておく
	m_count = 0;
	m_frameRate = 0.0;
	m_currentTime = GetNowCount();
	m_lastTime = m_currentTime;	// とりあえず同じ値を格納
	m_updateFrameRateTime = m_currentTime;
}


//-------------------------------
//		フレームを進めていいか判断
//-------------------------------
bool CFrameRate::CheckNext()
{
	// 現在の時間を取得する
	m_currentTime = GetNowCount();
	// 前回の時間から今回までの差分を計算
	int difTime = m_currentTime - m_lastTime;

	// 前回の時間から指定したフレーム分経過しているかチェック
	if (difTime >= FRAME_RATE_MILLI_SECOND) return true;
	else return false;
}


//-------------------------------
//		フレームレート計算
//-------------------------------
void CFrameRate::Calc()
{
	// 前回時間を更新
	m_lastTime = m_currentTime;
}


//-------------------------------
//		FPS表示
//-------------------------------
void CFrameRate::PrintFps()
{
	// 1フレーム進んだと判断
	m_count++;
	// 前回の表示時間から今回までの差分を計算
	int difTime = m_currentTime - m_updateFrameRateTime;

	// 1秒が経過するまで経過はこれ以降実行しない
	if (difTime >= 1000)
	{
		float frameCnt = (float)(m_count * 1000);
		// フレームレートを計算
		m_frameRate = frameCnt / difTime;
		// カウント初期化
		m_count = 0;
		// フレームレート更新時間を最新に
		m_updateFrameRateTime = m_currentTime;
	}

	// FPSは常に表示を続ける
	DrawFormatString(16, 16, GetColor(255, 0, 0), "FPS:%.2lf", m_frameRate);
}