#pragma once
#include "SceneBase.h"

//	タイトルクラス
class CSceneTitle : public CSceneBase{
private:

	enum TITLE_HNDL
	{
		HNDL_BASE,
		HNDL_HUMAN,
		HNDL_NAME,

		HNDLNUM
	};

	int m_hndl[HNDLNUM];

public:
	// 初期化
	void Init();
	// データロード
	void Load();
	// 実行処理
	//	@戻り値	:	次のシーン(-1なら変更なし)
	int Step();
	// 描画
	void Draw();
	// 終了処理
	void Fin();
};




