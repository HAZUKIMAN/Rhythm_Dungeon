#include <DxLib.h>
#include "SceneGame.h"
#include "../Common.h"
#include "../collision/CollisionManager.h"
#include "../system/SoundManager.h"
#include "../../lib/Input/Input.h"


constexpr float TILE_SIZE = 5.0f;

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
	//// 猫初期化
	m_cat.Init();
	//// ショット初期化
	//m_shotManager.Init();
	//運べるアイテムの初期化
	m_institem.Init();
	// 背景初期化
	m_backgroundManager.Init();

	m_mapeditor.Init();
	m_objEditor.Init();
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneGame::Load()
{
	//m_backgroundManager.Load();
	m_player.Load();
	m_cat.Load();
	//m_shotManager.Load();

	m_mapeditor.Load();
	m_objEditor.Load();


}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneGame::Step()
{
	int ret = -1;
	Calc();
	//if (!m_player.IsActiveFlag())ret = SCENEID_GAMEOVER;
	//else if (m_destroyCnt >= CLEAR_NUMBER)
	//	ret = SCENEID_CLEAR;

	if (Input::Key::Push(KEY_INPUT_Z))
		ret = SCENEID_GAMEOVER;


	if (Input::Key::Push(KEY_INPUT_R))
	{
		auto& objs = m_objEditor.GetObjects();

		printf("size = %d\n", (int)objs.size());

		/*enum ObjectType {
			OBJ_NONE,
			OBJ_PLAYER,
			OBJ_ENEMY,
			OBJ_ITEM,
			OBJ_GOAL,
		};*/
		for (const auto& obj : objs) {
			if (obj.type == OBJ_PLAYER)
			{
				float gridSize = 5.0f;

				m_player.SetPos(VGet(obj.x * gridSize + 2.5f , 10.0f,obj.z * gridSize + 2.5f));//2.5fはマスの真ん中に持っていくよう
			}
			if (obj.type == OBJ_ENEMY)
			{
				float gridSize = 5.0f;

				m_cat.SetPos(VGet(obj.x * gridSize + 2.5f, 10.0f, obj.z * gridSize + 2.5f));//2.5fはマスの真ん中に持っていくよう
			}
			if (obj.type == OBJ_ITEM)
			{
				float gridSize = 5.0f;
				float worldpos_x = obj.x * gridSize + 2.5f;
				float worldpos_z = obj.z * gridSize + 2.5f;

				VECTOR vec = VGet(worldpos_x, 5.0f, worldpos_z);
				m_institem.SetPos(vec);
				
			}
		}
	}

	return ret;
}


//-------------------------------
//		描画
//-------------------------------
void CSceneGame::Draw()
{
	m_backgroundManager.Draw();
	m_cameraManager.Draw();
	m_player.Draw();
	m_cat.Draw();
	m_institem.Draw();
	m_mapeditor.Draw();
	m_objEditor.Draw();
	//m_enemyManager.Draw();
	//m_shotManager.Draw();*/

	//カメラの切り替え表示
	DrawFormatString(1200,20,WHITE,"デバックカメラ切り替え処理:Key C \nエディターカメラ切り替え処理:Key B\nプレイカメラへの切り替え:key V");

	DrawFormatString(700, 100, RED, "プレイヤーのＸ軸：%f", m_player.GetPos().x);
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneGame::Fin()
{
	m_cameraManager.Fin();
	m_player.Fin();
	m_cat.Fin();
	m_backgroundManager.Fin();
	m_mapeditor.Fin();
	m_objEditor.Fin();
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
		// 猫の更新処理
		m_cat.Step();
		// ショット更新
		//m_shotManager.Step();
		// 背景更新
		//m_backgroundManager.Step();
		// 当たり判定処理
		/*m_destroyCnt += CCollisionManager::CheckHitShotToEnemy(m_enemyManager, m_shotManager);*/

		m_collision.CheckHitPlayerToCat(m_player, m_cat);

		m_player.AddPos(CCollisionManager::HitMap(
			m_player.GetCenter(),
			1.0f,
			m_mapeditor
		));

		//ステージとプレーヤーの床判定
		VECTOR push = m_collision.HitObject(
			m_player.GetPos(),
			1.0f,
			m_objEditor
		);
		m_player.AddPos(push);


		// 各種更新
		m_player.Update();
		// 猫の更新
		m_cat.Update();
		//アイテムの更新処理
		m_institem.Update();
		/*m_enemyManager.Update();
		m_shotManager.Update();
		m_backgroundManager.Update();*/

	}

	if (m_cameraManager.GetCameraID() == CCameraManager::CAMERA_ID_EDITOR)
	{
		m_mapeditor.Step();
		m_mapeditor.Update();
		m_objEditor.Step();
		m_objEditor.Update();
	}
	

	//デバックカメラ切り替え処理
	if (Input::Key::Push(KEY_INPUT_C))
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_DEBUG);
	//エディターカメラ切り替え処理
	if (Input::Key::Push(KEY_INPUT_B))
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_EDITOR);
	//プレイカメラへの切り替え
	else if (Input::Key::Push(KEY_INPUT_V))
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_PLAY);



	// カメラ更新処理
	m_cameraManager.Step(m_player);
	m_cameraManager.Update();


}