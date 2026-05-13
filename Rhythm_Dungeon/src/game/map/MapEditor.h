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

	//---------------------------------
// インスタンス
//---------------------------------
	struct InstanceData
	{
		int m_iModelHdl;
		VECTOR m_vPosition;
	};

public:

	//---------------------------------
	// 3Dマップ
	//---------------------------------
	TileType map[MAP_Y][MAP_H][MAP_W];

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

	void BuildInstances();

    bool GetMouseHitPosition(VECTOR* outPos);
    bool GetGridPos(VECTOR hitPos, int* pos_x, int* pos_z);

    void SaveMap(const char* filename);
    void LoadMap(const char* filename);

    int GetMap(int y, int z, int x) const;

    void SetMap(int y, int z, int x, int value);

private:

    //---------------------------------
    // インスタンス
    //---------------------------------
    std::vector<InstanceData> instances;

    //---------------------------------
    // 現在の高さ
    //---------------------------------
    int m_currentY;
};
	//グリッドは位置情報の管理のしやすさとマウスを使用したレイキャストを使用したかったため（岡松対策）
	//バイナリを使用した理由データの軽量化テキストよりバイナリの方が軽くて高速：セーブとか使用したかったのでコンパクトの方がよかった。