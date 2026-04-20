#pragma once
#include <DxLib.h>

class MapEditor
{
public:

	//コンストラクタ
	MapEditor();
	//デストラクタ
	~MapEditor();

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

};