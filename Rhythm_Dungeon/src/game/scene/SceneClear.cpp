#include <DxLib.h>
#include "SceneClear.h"
#include "../Common.h"
#include "../../lib/Input/Input.h"

static const char FILE_NAME[] = "data/graph/clear.png";


//-------------------------------
//		初期化
//-------------------------------
void CSceneClear::Init()
{
	m_hndl = -1;
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneClear::Load()
{
	if (m_hndl == -1)
		m_hndl = LoadGraph(FILE_NAME);
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneClear::Step()
{
	int ret = -1;

	if (Input::Key::Push(KEY_INPUT_V))
		ret = SCENEID_TITLE;

	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneClear::Draw()
{
	DrawGraph(0, 0, m_hndl, TRUE);
	DrawString(1000, 16, "gameclear", GetColor(255, 0, 0));
	DrawString(1000, 30, "vでタイトルへ", GetColor(255, 0, 0));
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneClear::Fin()
{
	if (m_hndl != -1)
	{
		DeleteGraph(m_hndl);
		m_hndl = -1;
	}
}