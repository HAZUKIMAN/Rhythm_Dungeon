#include <DxLib.h>
#include "SceneTitle.h"
#include "../Common.h"

static const char FILE_NAME[] = "data/graph/title.png";


//-------------------------------
//		初期化
//-------------------------------
void CSceneTitle::Init()
{
	m_hndl = -1;
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneTitle::Load()
{
	if (m_hndl == -1)
		m_hndl = LoadGraph(FILE_NAME);
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneTitle::Step()
{
	int ret = -1;
	if (CheckHitKey(KEY_INPUT_Z))
		ret = SCENEID_GAME;
	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneTitle::Draw()
{
	DrawGraph(0, 0, m_hndl, TRUE);
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneTitle::Fin()
{
	if (m_hndl != -1)
	{
		DeleteGraph(m_hndl);
		m_hndl = -1;
	}
}