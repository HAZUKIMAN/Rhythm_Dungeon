#include <DxLib.h>
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "../../lib/Fade/Fade.h"



//-------------------------------
//		コンストラクタ
//-------------------------------
CSceneManager::CSceneManager() : m_base(nullptr), m_state(STATEID_INIT), m_next(SCENEID_TITLE)
{
	// 関数ポインタに適切な関数を設定
	m_step[STATEID_INIT] = &CSceneManager::Init;
	m_step[STATEID_LOAD] = &CSceneManager::Load;
	m_step[STATEID_STEP] = &CSceneManager::Step;
	m_step[STATEID_FIN] = &CSceneManager::Fin;
	
	CreateScene();	// 最初のシーン作成
}


//-------------------------------
//		デストラクタ
//-------------------------------
CSceneManager::~CSceneManager()
{
	if (m_base != nullptr)
		delete m_base;
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneManager::Loop()
{
	int ret = -1;


	// 各種処理の呼び出し
	(this->*m_step[m_state])();
	
	// フェードの更新
	CFade::GetInstance()->Update();

	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneManager::Draw()
{
	// 基本的にm_baseはnullにならないが、心配ならnullチェックを
	m_base->Draw();
	CFade::GetInstance()->Draw(); //必ずフェードは最後にする
}


//-------------------------------
//		初期化
//-------------------------------
void CSceneManager::Init()
{
	m_base->Init();
	m_state = STATEID_LOAD;
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneManager::Load()
{
	m_base->Load();
	m_state = STATEID_STEP;
	CFade::GetInstance()->RequestFade(4.0f, true);
}


//-------------------------------
//		実行処理
//-------------------------------
void CSceneManager::Step()
{
	if (!CFade::GetInstance()->IsEnd()) return;

	m_next = m_base->Step();
	if (m_next != -1)
	{
		m_state = STATEID_FIN;
		CFade::GetInstance()->RequestFade(4.0f, false);
	}
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneManager::Fin()
{
	if (!CFade::GetInstance()->IsEnd()) return;

	m_base->Fin();
	CreateScene();	// 次のシーンを作成
	m_state = STATEID_INIT;
}


//-------------------------------
//		次のシーンを作成
//-------------------------------
void CSceneManager::CreateScene()
{
	// 古いシーン削除
	if (m_base != nullptr)
		delete m_base;

	// 新しいシーン作成
	switch (m_next)
	{
	case SCENEID_TITLE:
		m_base = new CSceneTitle;
		break;
	case SCENEID_GAME:
		m_base = new CSceneGame;
		break;
	case SCENEID_GAMEOVER:
		m_base = new CSceneGameover;
		break;
	case SCENEID_CLEAR:
		m_base = new CSceneClear;
		break;
	default:
		// ここはエラー対策の処理を入れてもOK
		break;
	}

	m_next = -1;
}