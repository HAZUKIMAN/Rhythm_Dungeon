#include "Fade.h"
#include <DxLib.h>
#include "../../game/common.h"

CFade*	CFade::m_instance = nullptr;

//---------------------------------
//		インスタンス取得
//---------------------------------
CFade*	CFade::GetInstance(void)
{
	Alloc();
	return m_instance;
}


//---------------------------------
//		メモリ確保
//---------------------------------
void	CFade::Alloc(void)
{
	if (m_instance == nullptr)
	{
		m_instance = new CFade;
		m_instance->Reset();
	}
}


//---------------------------------
//		メモリ解放
//---------------------------------
void	CFade::Release(void)
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}


//---------------------------------
//		コンストラクタ
//---------------------------------
CFade::CFade(void) : m_cnt(0.0f), m_spd(0.0f), m_fade(FadeState::FADE_NON),
						m_windowX(WINDOW_WIDTH), m_windowY(WINDOW_HEIGHT)
{
}


//---------------------------------
//		デストラクタ
//---------------------------------
CFade::~CFade(void)
{
}


//---------------------------------
//		フェードリクエスト
//---------------------------------
void	CFade::RequestFade(float speed, bool isFadeIn)
{
	m_spd = speed;
	m_fade = isFadeIn ? FadeState::FADE_IN : FadeState::FADE_OUT;
	m_cnt = isFadeIn ? 255.0f : 0.0f;
}


//---------------------------------
//		更新処理
//---------------------------------
void	CFade::Update(void)
{
	switch (m_fade)
	{
	case FadeState::FADE_IN:
		m_cnt -= m_spd;
		if (m_cnt < 0.0f) m_cnt = 0.0f;
		break;
	case FadeState::FADE_OUT:
		m_cnt += m_spd;
		if (m_cnt > 255.0f) m_cnt = 255.0f;
		break;
	}
}


//---------------------------------
//		描画処理
//---------------------------------
void	CFade::Draw(void)
{
	if (m_fade == FadeState::FADE_NON) return;

	

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)m_cnt);
	DrawBox(0, 0, m_windowX, m_windowY, GetColor(0, 0, 0), TRUE);
	DrawString(1000, 16, "フェイド中", GetColor(255, 0, 0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


//---------------------------------
//		終了判定
//---------------------------------
bool	CFade::IsEnd(void)
{
	bool	isRet = true;

	switch (m_fade)
	{
	case FadeState::FADE_IN:
		if (m_cnt > 0.0f) isRet = false;
		break;
	case FadeState::FADE_OUT:
		if (m_cnt < 255.0f) isRet = false;
		break;
	}

	return isRet;
}

