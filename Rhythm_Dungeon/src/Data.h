#pragma once

//　シングルトンの見本でおいているだけなので消します


class Data
{
public:
	// インスタンス取得
	static Data* GetInstance()
	{
		if (m_instance == nullptr) m_instance = new Data;

		return m_instance;
	}
	// インスタンスの削除
	static void DeleteInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

	// スコア操作
	int GetScore() { return m_score; }
	void SetScore(int score) { m_score = score; }
	void AddScore(int score) { m_score += score; }

private:
	Data() { m_score = 100; }
	~Data() {}
	Data(const Data& other);
	Data operator = (const Data& other);

	static Data* m_instance;

	int m_score;	// ゲームのスコア
};
