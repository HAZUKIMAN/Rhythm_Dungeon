#pragma once
#include "../Common.h"

class CSceneBase;

// フレームレート管理用クラス
class CSceneManager {
private:
	// 各シーンの流れ
	// 基本的にどのシーンもこの流れだけで動くようにする
	// さらに流れを増やしたい場合は、各シーンのStep内で増やすこと
	enum tagState {
		STATEID_INIT,
		STATEID_LOAD,
		STATEID_STEP,
		STATEID_FIN,

		STATEID_NUM
	};

	CSceneBase* m_base;								// 各シーンを格納
	int	m_state;									// 現在の状態
	int m_next;										// 次に移動するシーン
	void (CSceneManager::* m_step[STATEID_NUM])();	// シーンを関数ポインタで管理するためのもの

public:
	//		コンストラクタ
	CSceneManager();
	//		デストラクタ
	~CSceneManager();

	//		実行処理
	//	@戻り値	:	0=そのまま　1=ゲーム終了
	int Loop();
	//		描画
	void Draw();

private:
	//		初期化
	void Init();
	//		データロード
	void Load();
	//		実行処理
	void Step();
	//		終了処理
	void Fin();

	//		次のシーンを作成
	void CreateScene();
};





