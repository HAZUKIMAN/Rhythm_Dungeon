#pragma once

// フレームレート管理用クラス
class CFrameRate {
private:
	static double m_frameRate;			// フレームレート(画面表示用)
	static int m_count;					// フレーム数カウント(画面表示用)
	static int m_updateFrameRateTime;	// フレームレート更新時間(画面表示用)
	static int m_currentTime;			// 現在のPC時間
	static int m_lastTime;				// 1フレーム前のPC時間

public:
	// フレームレート初期化
	static void Init();
	// フレームを進めていいか判断
	static bool CheckNext();
	// フレームレート計算
	static void Calc();
	// FPS表示
	static void PrintFps();

};




