#include <DxLib.h>
#include "SceneSelect.h"
#include "../Common.h"
#include "../../lib/Input/Input.h"
#include "../../Data.h"
#include "../../lib/sound/SoundManager.h"



static const char FILE_NAME[] = "Data/StageSelect/StageSelectBase.png";


//-------------------------------
//		初期化
//-------------------------------
void CSceneSelect::Init()
{
	//---------------------------------
	// 選択カーソル初期化
	//---------------------------------
	m_selectStage = 0;
	m_hndl = -1;
	m_stageMax = 9;

}


//-------------------------------
//		データロード
//-------------------------------
void CSceneSelect::Load()
{
	if (m_hndl == -1)
		m_hndl = LoadGraph(FILE_NAME);

	CSoundManager::Play(CSoundManager::SOUNDID_SELECT_BGM, DX_PLAYTYPE_LOOP);
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneSelect::Step()
{
	int ret = -1;
	if (Input::Key::Push(KEY_INPUT_V)|| Input::Controller::Push(XINPUT_BUTTON_Y))ret = SCENEID_TITLE;
	ret = StageSelectUpdate();

	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneSelect::Draw()
{
	//---------------------------------
	// 中央位置
	//---------------------------------
	int centerX = WINDOW_WIDTH_HALF;
	int centerY = WINDOW_HEIGHT_HALF;

	DrawGraph(0, 0, m_hndl, TRUE);

	//---------------------------------
	// ステージ表示
	//---------------------------------
	for (int i = 0;i < m_stageMax;i++)

	{
		// 選択位置との差
		int diff = i - m_selectStage;

		// 横位置
		int x = centerX+ diff * 250;
		int y = centerY;

		// 選択中は大きく
		int size =(diff == 0)? 220: 140;
		int color =(diff == 0)? YELLOW: GRAY;

		// 枠
		DrawBox(
			x - size / 2,y - size / 2,
			x + size / 2,y + size / 2, color ,true);

		DrawBox(
			x - size / 2 ,y - size / 2,
			x + size / 2, y + size / 2, WHITE ,false);

		// テキスト
		char str[32];

		sprintf_s(str,"STAGE %d",i + 1);
		DrawString(x - 40,y,str,BLACK);
	}

	DrawString(500,650,"← → Select  Enter Start",WHITE);

	DrawString(560, 100, "STAGE SELECT", WHITE);
	DrawString(1000, 30, "vでタイトルへ", RED);
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneSelect::Fin()
{
	if (m_hndl != -1)
	{
		DeleteGraph(m_hndl);
		m_hndl = -1;
	}
}

int CSceneSelect::StageSelectUpdate()
{

	int ret = -1;

	//---------------------------------
	// 左
	//---------------------------------
	if (Input::Key::Push(KEY_INPUT_LEFT) ||Input::Controller::Push(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_selectStage--;

		CSoundManager::Play(CSoundManager::SOUNDID_SE_BACK, DX_PLAYTYPE_BACK);

		if (m_selectStage < 0)
		{
			m_selectStage =
				m_stageMax - 1;
		}
	}

	//---------------------------------
	// 右
	//---------------------------------
	if (Input::Key::Push(KEY_INPUT_RIGHT) || Input::Controller::Push(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_selectStage++;

		CSoundManager::Play(CSoundManager::SOUNDID_SE_BACK, DX_PLAYTYPE_BACK);

		if (m_selectStage >=
			m_stageMax)
		{
			m_selectStage = 0;
		}
	}

	//---------------------------------
	// 決定
	//---------------------------------
	if (Input::Key::Push(KEY_INPUT_RETURN) || Input::Controller::Push(XINPUT_BUTTON_B))
	{
		//---------------------------------
		// 選択ステージ保存
		//---------------------------------
		Data::GetInstance()->SetSelectStage(m_selectStage);

		CSoundManager::Stop(CSoundManager::SOUNDID_SELECT_BGM);
		CSoundManager::Play(CSoundManager::SOUNDID_SE_ENTER, DX_PLAYTYPE_BACK);

		ret =SCENEID_GAME;
	}

	return ret;
}