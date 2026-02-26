#include <DxLib.h>
#include "SceneGame.h"
#include "../Common.h"
#include "../collision/CollisionManager.h"
#include "../system/SoundManager.h"

static const int CLEAR_NUMBER = 15;		// クリアに必要な撃破数


//-------------------------------
//		コンストラクタ
//-------------------------------
CSceneGame::CSceneGame()
{
}


//-------------------------------
//		デストラクタ
//-------------------------------
CSceneGame::~CSceneGame()
{
}


//-------------------------------
//		初期化
//-------------------------------
void CSceneGame::Init()
{
	// カメラ初期化
	m_cameraManager.Init();
	m_cameraManager.SetNearFar(5.0f, 5000.0f);

	// プレイヤー初期化
	m_player.Init();
	//// 敵初期化
	//m_enemyManager.Init();
	//// ショット初期化
	//m_shotManager.Init();
	// 背景初期化
	//m_backgroundManager.Init();

	m_destroyCnt = 0;
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneGame::Load()
{
	//m_backgroundManager.Load();
	m_player.Load();
	/*m_enemyManager.Load();
	m_shotManager.Load();*/

	//// 各種更新
	m_player.Update();
	//m_enemyManager.Update();
	//m_shotManager.Update();
	//m_backgroundManager.Update();
	//m_cameraManager.Step();
	m_cameraManager.Update();
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneGame::Step()
{
	int ret = -1;
	Calc();

	if (!m_player.IsActiveFlag())
		ret = SCENEID_GAMEOVER;
	else if (m_destroyCnt >= CLEAR_NUMBER)
		ret = SCENEID_CLEAR;
	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneGame::Draw()
{
	//m_backgroundManager.Draw();
	m_cameraManager.Draw();
	m_player.Draw();
	//m_enemyManager.Draw();
	//m_shotManager.Draw();*/

	DrawFormatString(32, 32, GetColor(255, 0, 0), "クリアまであと%d体", CLEAR_NUMBER - m_destroyCnt);
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneGame::Fin()
{
	m_cameraManager.Fin();
	m_player.Fin();
	/*m_enemyManager.Fin();
	m_shotManager.Fin();
	m_backgroundManager.Fin();*/
}


//-------------------------------
//		ゲーム本体の処理
//-------------------------------
void CSceneGame::Calc()
{
	if (m_cameraManager.GetCameraID() == CCameraManager::CAMERA_ID_PLAY)
	{
		// プレイヤー更新処理
		m_player.Step();
		// 敵更新
		//m_enemyManager.Step();
		// ショット更新
		//m_shotManager.Step();
		// 背景更新
		//m_backgroundManager.Step();
		// 当たり判定処理
		/*m_destroyCnt += CCollisionManager::CheckHitShotToEnemy(m_enemyManager, m_shotManager);
		CCollisionManager::CheckHitPlayerToEnemy(m_player, m_enemyManager);*/

		// 各種更新
		m_player.Update();
		/*m_enemyManager.Update();
		m_shotManager.Update();
		m_backgroundManager.Update();*/
	}

	// カメラ切り替え処理
	if (CheckHitKey(KEY_INPUT_C))
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_DEBUG);
	else if (CheckHitKey(KEY_INPUT_V))
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_PLAY);
	// カメラ更新処理
	m_cameraManager.Step(m_player);
	m_cameraManager.Update();
}