#pragma once
#include "../../lib/object/Object.h"

class CBlock : public CObject {
private:

public:

	// コンストラクタ・デストラクタ
	CBlock();
	~CBlock();

	// 初期化
	void Init();
	// データロード
	void Load(int hndl);
	//更新処理
	void Update();
	// 描画
	void Draw();

	inline int FieldHndl(){ return m_iModelHdl; };

	//フィールドの位置の取得
	inline bool SetActive(bool _act) { m_isActive = _act; }
};