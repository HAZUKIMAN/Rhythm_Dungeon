#pragma once


//--------------------------------------
//		フェードイン・アウトクラス
//--------------------------------------
class CFade {
public:
	enum class FadeState{
		FADE_NON,	// フェード無し
		FADE_IN,	// フェードイン
		FADE_OUT	// フェードアウト
	};
private:
	static	CFade*	m_instance;	// インスタンス

	float		m_cnt;		// フェードのアルファ値
	float		m_spd;		// フェード速度
	FadeState	m_fade;		// フェードの状態
	int			m_windowX;	// ウィンドウサイズ　横
	int			m_windowY;	// ウィンドウサイズ　縦

public:
	// シングルトン-------------------------------
	//		インスタンス取得
	static CFade*	GetInstance(void);

	//		メモリ確保
	static void		Alloc(void);
	//		メモリ解放
	static void		Release(void);
	//--------------------------------------------


	// フェード関連------------------------------
	// フェード削除
	void	Reset(void) { m_fade = FadeState::FADE_NON; }
	// フェードリクエスト
	//@speed		:	フェードの速度　0～255
	//@isFadeIn	:	true=フェードイン false=フェードアウト
	void	RequestFade(float	speed, bool isFadeIn);
	// 更新処理
	void	Update(void);
	// 描画処理
	void	Draw(void);
	// 終了判定
	//@return	:	true=終了　else=まだ終了ではない
	bool	IsEnd(void);
	// 現在のフェードの状態取得
	FadeState	GetState(void) { return m_fade; }
	// ウィンドウサイズ設定
	void	SetWindowSize(int x, int y) { m_windowX = x; m_windowY = y; }
	//--------------------------------------------


private:
	// コンストラクタ・デストラクタ
	CFade();
	~CFade();

	// コピーコンストラクタ・代入演算子
	CFade(const CFade &other);
	CFade& operator= (const CFade& other);
};

