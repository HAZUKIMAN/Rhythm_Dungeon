#pragma once
#include <DxLib.h>

constexpr int SCORE_HNDL_NUM = 10;
constexpr int COMBO_HNDL_NUM = 10;

// スコア管理クラス
class CScore {

private:

	// スコア
	static int m_score;
	// 前フレームのスコア
	int m_prevScore;
	// スコア表示用の数値
	int m_memScore;

	// コンボ数
	static int m_combo;

	// スコア表示用の数字
	int m_scoreHndl[SCORE_HNDL_NUM];

	// コンボ表示用の数字
	int m_comboHndl[COMBO_HNDL_NUM];

public:

	void Init();
	void Load();
	void Step(bool trg);
	void Exit();
	void Draw();

public:
	// スコアの追加
	// 引数scoreは追加する値
	static void AddScore(int score);

	// スコアの減算
	// 引数scoreは減算する値
	static void SubScore(int score);

	// コンボ数の追加
	// 一回ずつ加算
	static void AddCombo();

	// コンボ数のリセット
	static void ResetCombo();

	inline int GetScore(){ return m_memScore; }
};

