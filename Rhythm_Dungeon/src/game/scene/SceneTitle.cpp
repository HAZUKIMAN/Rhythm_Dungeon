#include <DxLib.h>
#include "SceneTitle.h"
#include "../Common.h"
#include "../../lib/Input/Input.h"
#include "../../lib/sound/SoundManager.h"

constexpr char BASE_HNDL[] = "Data/Title/Title02.png";
constexpr char HUMAN_HNDL[] = "Data/Title/TitleCharacter.png";
constexpr char NAME_HNDL[] = "Data/Title/TitleName.png";

//-------------------------------
//		初期化
//-------------------------------
void CSceneTitle::Init()
{
	for(int init=0; init< HNDLNUM; init++)
	m_hndl[init] = -1;
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneTitle::Load()
{
	if (m_hndl[HNDL_BASE] == -1)
		m_hndl[HNDL_BASE] = LoadGraph(BASE_HNDL);

	if (m_hndl[HNDL_HUMAN] == -1)
		m_hndl[HNDL_HUMAN] = LoadGraph(HUMAN_HNDL);

	if (m_hndl[HNDL_NAME] == -1)
		m_hndl[HNDL_NAME] = LoadGraph(NAME_HNDL);

	CSoundManager::Play(CSoundManager::SOUNDID_TITLE_BGM, DX_PLAYTYPE_LOOP);
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneTitle::Step()
{
	int ret = -1;

	if (Input::Key::Push(KEY_INPUT_Z) || Input::Controller::Push(XINPUT_BUTTON_B)) 
	{ ret = SCENEID_SELECT; 
	CSoundManager::Stop(CSoundManager::SOUNDID_TITLE_BGM);
	CSoundManager::Play(CSoundManager::SOUNDID_SE_ENTER, DX_PLAYTYPE_BACK);
	}

	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneTitle::Draw()
{
	/*DrawGraph(WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF, m_hndl[HNDL_BASE], TRUE);
	DrawGraph(WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF, m_hndl[HNDL_HUMAN], TRUE);
	DrawGraph(WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF, m_hndl[HNDL_NAME], TRUE);*/

	DrawRotaGraph(WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF, 1 , 0 ,m_hndl[HNDL_BASE], TRUE);
	DrawRotaGraph(WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF, 1, 0,m_hndl[HNDL_HUMAN], TRUE,TRUE);
	DrawRotaGraph(WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF, 1, 0,m_hndl[HNDL_NAME], TRUE);

	//DrawString(1000, 16 , "Title" , GetColor(255, 0, 0));
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneTitle::Fin()
{
	for (int fin = 0; fin < HNDLNUM; fin++)
	{
		if (m_hndl[fin] != -1)
		{
			DeleteGraph(m_hndl[fin]);
			m_hndl[fin] = -1;
		}
	}
}