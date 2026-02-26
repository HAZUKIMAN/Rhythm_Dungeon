#include <DxLib.h>
#include "SceneGameover.h"
#include "../Common.h"

static const char FILE_NAME[] = "data/graph/gameover.png";


//-------------------------------
//		初期化
//-------------------------------
void CSceneGameover::Init()
{
	m_hndl = -1;
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneGameover::Load()
{
	if (m_hndl == -1)
		m_hndl = LoadGraph(FILE_NAME);
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneGameover::Step()
{
	int ret = -1;
	if (CheckHitKey(KEY_INPUT_V))
		ret = SCENEID_TITLE;
	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneGameover::Draw()
{
	DrawGraph(0, 0, m_hndl, TRUE);
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneGameover::Fin()
{
	if (m_hndl != -1)
	{
		DeleteGraph(m_hndl);
		m_hndl = -1;
	}
}