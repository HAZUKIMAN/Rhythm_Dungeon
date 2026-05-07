#pragma once
#include <DxLib.h>
#include <vector>
#include "ObjectEditor.h"


class MapEditor
{
public:

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
	

	inline int GetHndl(){ return m_iModelHdl; };

	int GetMap(int z, int x)const;

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

	void BuildInstances();					//インスタンスの生成用
	
	//当たり判定処理
	//@center :当たり判定をする相手の座標
	//@radius :相手を球として判定するのでその半径
	//@return :押し戻す方向&距離
	VECTOR	HitCheck(VECTOR start, VECTOR end);

	//グリッドは位置情報の管理のしやすさとマウスを使用したレイキャストを使用したかったため（岡松対策）
	//バイナリを使用した理由データの軽量化テキストよりバイナリの方が軽くて高速：セーブとか使用したかったのでコンパクトの方がよかった。
};