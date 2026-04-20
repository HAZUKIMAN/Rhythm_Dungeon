#pragma once
// フレームレート管理用クラス
class CBackGroundManager {
private:
	
public:
	//コンストラクタ
	CBackGroundManager();
	//デストラクタ
	~CBackGroundManager();

private:
	// 初期化
	void Init();
	// データロード
	void Load();
	// 実行処理
	void Step();
	// 終了処理
	void Fin();
	// 次のシーンを作成
	void CreateScene();
};
