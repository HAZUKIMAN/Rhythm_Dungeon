#pragma once
#include <DxLib.h>
#include <vector>
#include "ObjectEditor.h"
#include "../common.h"





class MapEditor
{
private:

	enum TileType {
		TILE_NONE  = 0,
		TILE_FLOOR = 1,
		TILE_WALL  = 2
	};

public:

	

	TileType map[MAP_H][MAP_W];

	int m_iModelHdl;		//ステージのモデルハンドル
	int m_iModelHdl_Wall;	//ステージ用モデルの壁

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
	bool GetGridPos(VECTOR hitPos, int* _x, int* _z);
	//選択しているマスを赤く表示する
	void DrawSelectedTile();
	//
	int GetMap(int z, int x)const;
	// マップ設定
	void SetMap(int z, int x, int value);

	void BuildInstances();					//インスタンスの生成用

private:

	//インスタンス用の構造体
	struct Instance {
		int m_iModelHdl;
		/*int m_iModelHdl_Wall;*/
		VECTOR m_vPosition;
		VECTOR m_vRotation;
		VECTOR m_vScale;
	};
	//インスタンス確保
	std::vector<Instance> instances;

	

	

	//グリッドは位置情報の管理のしやすさとマウスを使用したレイキャストを使用したかったため（岡松対策）
	//バイナリを使用した理由データの軽量化テキストよりバイナリの方が軽くて高速：セーブとか使用したかったのでコンパクトの方がよかった。
};