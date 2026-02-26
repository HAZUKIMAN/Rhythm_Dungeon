#include "Score.h"
#include "../Common.h"

int CScore::m_combo;
int CScore::m_score;

const char* FILE_PATH[]{
	// スコアの数字ハンドル
	"Data/number/number16x32_01.png",
	// コンボ数の数字ハンドル
	"Data/number/number12x24_04.png",
};



// 初期化
void CScore::Init() {
	// 数値の初期化
	m_combo = 0;
	m_score = 0;
	m_prevScore = 0;
	m_memScore = 0;

	// ハンドルの初期化
	memset(m_scoreHndl, -1, sizeof(m_scoreHndl));
	memset(m_comboHndl, -1, sizeof(m_comboHndl));
}

// 読み込み
void CScore::Load() {
	if (m_scoreHndl[0] == -1) {
		LoadDivGraph(FILE_PATH[0], 10, 10, 1, SCORE_SIZE_X, SCORE_SIZE_Y, m_scoreHndl);
	}
	if (m_comboHndl[0] == -1) {
		LoadDivGraph(FILE_PATH[1], 10, 10, 1, COMBO_SIZE_X, COMBO_SIZE_Y, m_comboHndl);
	}
	
}

// 更新
void CScore::Step(bool trg) {

	if (trg != true)
	{
		int sco = COUNTUP_NUM;

		if (m_combo >= 10)
		{
			sco = sco * 2;
		}
		if (m_combo >= 20)
		{
			sco = sco * 2;
		}
		if (m_combo >= 30)
		{
			sco = sco * 2;
		}

		// 前のスコアと今のスコアが異なれば表示用スコアを加減する
		if (m_score > m_prevScore) {
			m_memScore += sco;
			if (m_memScore > m_score) {
				m_memScore = m_score;
			}
		}
		else if (m_score < m_prevScore) {
			m_memScore -= sco;
			if (m_memScore < m_score) {
				m_memScore = m_score;
			}
		}
		else {
			m_prevScore = m_score;
			m_memScore = m_score;
		}

		// 表示用スコア数値が元のスコアに追いついたら
		if (m_memScore == m_score) {
			m_prevScore = m_score;
		}
		else {
			m_prevScore = m_memScore;
		}

		// 小さくなった時の変化
		if (m_score < 0) {
			m_score = 0;
		}
		if (m_combo < 0) {
			m_combo = 0;
		}
	}
}

// 終了時処理
void CScore::Exit() {

	if (m_comboHndl[0] != -1) {
		for (int i = 0; i < COMBO_HNDL_NUM; i++) {
			DeleteGraph(m_comboHndl[i]);
			m_comboHndl[i] = -1;
		}
	}
	if (m_scoreHndl[0] != -1) {
		for (int i = 0; i < SCORE_HNDL_NUM; i++) {
			DeleteGraph(m_scoreHndl[i]);
			m_scoreHndl[i] = -1;
		}
	}
}

// 描画
void CScore::Draw() {

	int score = m_memScore;

	DrawBox(1100, 300, 1350, 330, GetColor(0, 0, 0), TRUE);

	DrawBox(1100, 350, 1350, 380, GetColor(0, 0, 0), TRUE);

	// スコアの表示
	for (int i = 1; i <= SCORE_DIGIT; i++) {
		int num = score % 10;
		int x = WINDOW_WIDTH - ((SCORE_SIZE_X * i)+150);
		DrawFormatString(x, SCORE_HIGH, WHITE, "%d", num);

		//DrawRotaGraph(x, SCORE_HIGH, 1.0f, 0.0f, m_scoreHndl[num], TRUE);

		score = score / 10;
	}

	DrawString((WINDOW_WIDTH - 320), SCORE_HIGH, "score:", WHITE);

	int combo = m_combo;
	// 桁
	int digit = 0;
	// 桁数を計算
	while (combo > 0) {
		if (combo / 10 > 0) digit++;
		combo = combo / 10;
	}
	combo = m_combo;

	// コンボの表示
	for (int i = 0; i <= digit; i++) {
		int num = combo % 10;
		int x = COMBO_X - (COMBO_SIZE_X * i);

		DrawFormatString(x, COMBO_Y, WHITE, "%d", num);

		DrawString((WINDOW_WIDTH - 320), COMBO_Y, "combo:", WHITE);

		//DrawRotaGraph(x, COMBO_Y, 1.0f, 0.0f, m_comboHndl[num], TRUE);

		combo = combo / 10;
	}

}

void CScore::AddScore(int score)
{
	m_score += score;
}

void CScore::SubScore(int score)
{
	m_score -= score;
}

void CScore::AddCombo()
{
	m_combo++;
}

void CScore::ResetCombo()
{
	m_combo = 0;
}


