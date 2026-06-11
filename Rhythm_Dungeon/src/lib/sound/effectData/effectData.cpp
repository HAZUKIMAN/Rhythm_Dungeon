#include"effectData.h"
#include "../../../game/effect/effekseer.h"


int CEffectData::m_id[EFFECT_NUM];

//コンストラクタ
CEffectData::CEffectData()
{
	Init();
}

//初期化
void CEffectData::Init()
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		m_id[i] = -1;
	}
}

//ロード
void CEffectData::Load()
{
	// ロードするエフェクトを管理----------
	const char* EFFECT_PATH[EFFECT_NUM] = {
		"Data/effect/ef_Hit_enemy.efkefc",
		"Data/effect/ef_cat_walk_dash.efkefc",
		"Data/effect/ef_human_spawn.efkefc",//ef.efk
		"Data/effect/ef_clear_jump.efkefc"
	};

	//-------------------------------------

	//エフェクトのロード
	for (int effect_i = 0; effect_i < EFFECT_NUM; effect_i++)
	{
		m_id[effect_i] = CEffekseerCtrl::LoadData(EFFECT_PATH[effect_i]);
	}
}


