#pragma once
#include "../Object/Object.h"

class CMainStage : public CObject {
public:

	enum tagStage
	{
		STAGE1,
		STAGE2,
	};

	tagStage m_stage;

	int m_Timer;
	int m_num;


	int m_iStageHndl;

	bool m_isHitFlag;	// ヒットしているかどうかを判断(通常はいらない)

	// コンストラクタ・デストラクタ
	CMainStage();
	~CMainStage();

	// 初期化
	void Init();

	// 描画
	void Draw();

	//当たり判定処理
	//@center :当たり判定をする相手の座標
	//@radius :相手を球として判定するのでその半径
	//@return :押し戻す方向&距離
	//VECTOR HitCheck(VECTOR center, float radius);

	VECTOR	HitCheck(VECTOR start, VECTOR end);

	inline int FieldHndl(){ return m_iModelHdl; };



	inline VECTOR BornLinePos1() { return MV1GetFramePosition(m_iModelHdl, 1);} //ステージ上の 1レーンハンドルの取得
	inline VECTOR BornLinePos2() { return MV1GetFramePosition(m_iModelHdl, 4);} //ステージ上の 2レーンハンドルの取得
	inline VECTOR BornLinePos3() { return MV1GetFramePosition(m_iModelHdl, 7);} //ステージ上の 3レーンハンドルの取得
	inline VECTOR BornLinePos4() { return MV1GetFramePosition(m_iModelHdl,10);} //ステージ上の 4レーンハンドルの取得
	
};