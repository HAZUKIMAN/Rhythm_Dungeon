#pragma once
#include "DxLib.h"

class CObject {
public:
	// デストラクタ
	virtual ~CObject();

protected:

	VECTOR	m_vPosition;	//座標
	VECTOR	m_vRotation;	//回転値
	VECTOR	m_vSpeed;		//移動速度

	int		m_iModelHdl;	//モデルハンドル
	bool	m_isActive;		//アクティブフラグ

protected:

public:

	//	初期化
	virtual void Init();
	//	ロード
	virtual void Load(int hndl);
	// 更新したデータを反映させる
	virtual void Update();
	//	描画
	virtual void Draw();
	//	終了処理
	virtual void Fin();

	//==============================
	//		ゲッタ・セッタ
	//==============================
	//	座標
	inline VECTOR GetPos() { return m_vPosition; }
	//角度
	inline VECTOR GetRot() { return m_vRotation; }

	virtual void SetPos(VECTOR pos);
	//void SetPos(VECTOR pos);
	// 
	//	アクティブフラグ
	inline bool IsActiveFlag() { return m_isActive; }
	//inline void SetActiveFlag(bool isActive) { m_isActive = isActive; }

};