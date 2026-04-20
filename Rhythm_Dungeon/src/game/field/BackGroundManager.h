#pragma once
#include "Sky.h"
#include "Field.h"
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
	
	//当たり判定処理
	//@center :当たり判定をする相手の座標
	//@radius :相手を球として判定するのでその半径
	//@return :押し戻す方向&距離
	VECTOR	HitCheck(VECTOR start, VECTOR end);

private:

	CSky   c_sky;		//空
	CField c_fired;		//フィールド
};
