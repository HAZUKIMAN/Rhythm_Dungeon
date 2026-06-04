#pragma once

#include "../Object/Object.h"

class CBridge : public CObject {
private:

public:

	bool m_isHitFlag;	// ヒットしているかどうかを判断(通常はいらない)

	// コンストラクタ・デストラクタ
	CBridge();
	~CBridge();

	// 初期化
	void Init();
	//更新処理
	void Update();
	// 描画
	void Draw();

	//ブロックの生存フラグ取得
	inline bool GetActive() { return m_isActive; }
	inline int FieldHndl() { return m_iModelHdl; }

	//フィールドの位置の取得
	inline bool SetActive(bool _act) { m_isActive = _act; }
};