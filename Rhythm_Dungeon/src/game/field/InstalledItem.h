#pragma once
#include "../../lib/object/Object.h"

class CInstalledItem : public CObject {
private:
	
public:

	bool m_isHitFlag;	// ヒットしているかどうかを判断(通常はいらない)

	// コンストラクタ・デストラクタ
	CInstalledItem();
	~CInstalledItem();

	// 初期化
	void Init();
	// データロード
	void Load(int hndl);
	//更新処理
	void Update();
	// 描画
	void Draw();

	//ブロックの生存フラグ取得
	inline bool GetActive() { return m_isActive; }
	inline int FieldHndl(){ return m_iModelHdl; };

	//フィールドの位置の取得
	inline bool SetActive(bool _act) { m_isActive = _act; }
};