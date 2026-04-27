#pragma once
#include <DxLib.h>

class MapEditor
{
public:

	int m_iModelHdl;		//ステージのモデルハンドル

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
	// 更新処理
	void Update();
	// 終了処理
	void Fin();


	//マップのセーブ
	void SaveMap(const char* filename);
	//マップの読み込み
	void LoadMap(const char* filename);
	//マウスの当たり判定の取得
	bool GetMouseHitPosition(VECTOR* outPos);
	//グリッドの取得
	bool GetGridPos(VECTOR hitPos, int* gx, int* gz);

private:

	struct RemoveMouse
	{
		float mouseX;
		float mouseY;
	};
	//ここに変数を宣言
};