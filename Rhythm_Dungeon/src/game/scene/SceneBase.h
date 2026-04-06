#pragma once

//	シーン基本クラス
class CSceneBase {
public:

	//		デストラクタ
	virtual ~CSceneBase() {}
	//		初期化
	virtual void Init() = 0;
	//		データロード
	virtual void Load() = 0;
	//		実行処理
	virtual int Step() = 0;
	//		描画
	virtual void Draw() = 0;
	//		終了処理
	virtual void Fin() = 0;
};




