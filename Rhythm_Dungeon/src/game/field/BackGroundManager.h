#pragma once
#include "Sky.h"
#include "Block.h"
// フレームレート管理用クラス
class CBackGroundManager {
private:
	
public:
	//コンストラクタ
	CBackGroundManager();
	//デストラクタ
	~CBackGroundManager();

	// 初期化
	void Init();
	// データロード
	void Load();
	// 実行処理
	int  Step();
	// 描画
	void Draw();
	// 終了処理
	void Fin();
	

private:

	CSky   c_sky;		//空
};
