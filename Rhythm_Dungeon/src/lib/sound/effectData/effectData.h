#pragma once


enum tagEffectKind	//エフェクトの種類
{
	EFFECT_HIT_ENEMY,		//敵に当たった時エフェクト
	EFFECT_CAT_WALK,		//猫の歩いている時のエフェクト
	EFFECT_HUMAN_RESET,		//humanの復活エフェクト
	EFFECT_GOAL,			//クリア時の空を飛ぶエフェクト

	EFFECT_NUM,				//エフェクトの数
};

class CEffectData
{
private:

	static int m_id[EFFECT_NUM];			//エフェクトのID
public:
	//コンストラクタ
	CEffectData();

	//初期化
	static void Init();

	//ロード
	static void Load();

	//エフェクトのIDを取得
	static int GetId(tagEffectKind _kind) { return m_id[_kind]; }

};

