#include <DxLib.h>
#include "SceneGame.h"
#include "../Common.h"
#include "../collision/CollisionManager.h"
#include "../system/SoundManager.h"
#include "../../lib/Input/Input.h"


//-------------------------------
//		コンストラクタ
//-------------------------------
CSceneGame::CSceneGame()
{
	move_box = NONE;
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
	//ゴール
	m_goal.Init();

	m_mapedit.Init();
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
	m_mapedit.Load();
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

	////ゴールとプレイヤーの判定
	//if (CCollisionManager::CheckHitPlayerToGoal(m_player, m_goal))
	//{
	//	ret = SCENEID_GAMEOVER;
	//}
		

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
			if (obj.type == OBJ_GOAL)
			{
				float gridSize = 5.0f;
				float worldpos_x = obj.x * gridSize + 2.5f;
				float worldpos_z = obj.z * gridSize + 2.5f;

				VECTOR vec = VGet(worldpos_x, 5.0f, worldpos_z);
				m_goal.SetPos(vec);
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
	
	m_goal.Draw();		//ゴール
	m_mapedit.Draw();
	m_objEditor.Draw();

	//m_enemyManager.Draw();
	//m_shotManager.Draw();*/

	//カメラの切り替え表示
	DrawFormatString(1200,20,WHITE,"デバックカメラ切り替え処理:Key C \nエディターカメラ切り替え処理:Key B\nプレイカメラへの切り替え:key V");

	DrawFormatString(700, 100, RED, "プレイヤーのＸ軸：%f\n プレイヤーのY軸：%f\nプレイヤーのZ軸：%f", m_player.GetPos().x, m_player.GetPos().y, m_player.GetPos().z);
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
	m_mapedit.Fin();
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
		
		//--------------------------------------------
		//		猫がボックスを運ぶかどうかの判定
		//--------------------------------------------
		VECTOR Memo = VGet(0, 0, 0);
		//1回差をとります//正規化//アークタンジェント
		Memo = VSub(m_cat.GetPos(), m_institem.GetPos());
		if (VSize(Memo) > 5.0f) {
			if (Input::Key::Keep(KEY_INPUT_J))
			{
				move_box = CARRY;
			}
		}
		if (move_box == CARRY)
		{
			
		}
		
		if (Input::Key::Push(KEY_INPUT_G))
			m_cat.PlaceBlock(m_mapedit);


		//ボックスとプレイヤーの当たり判定
		CCollisionManager::CheckHitPlayerToBlock(m_player, m_institem);

		//-------------------------------------
		//  プレイヤーと床と壁との当たり判定
		//-------------------------------------
		m_player.AddPos(CCollisionManager::HitMap(
			m_player.GetCenter(),
			2.0f,
			m_mapedit
		));

		//-------------------------------------
		//  猫と床と壁との当たり判定
		//-------------------------------------
		m_cat.AddPos(CCollisionManager::HitMap(
			m_cat.GetCenter(),
			2.0f,
			m_mapedit
		));

		
		////------------------------------------------------
		////	マップにあるオブジェクトとプレーヤーの床判定
		////------------------------------------------------
		//VECTOR push = CCollisionManager::HitPlayerToObject(
		//	m_player.GetPos(),
		//	2.0f,
		//	m_objEditor,
		//	m_player
		//);
		//m_player.AddPos(push);

		////------------------------------------------------
		////		マップにあるオブジェクトと猫の床判定
		////------------------------------------------------
		//VECTOR cat_push = CCollisionManager::HitCatToObject(
		//	m_cat.GetPos(),
		//	2.0f,
		//	m_objEditor
		//);
		//m_cat.AddPos(cat_push);

		// 各種更新
		m_player.Update();
		// 猫の更新
		m_cat.Update();
		//アイテムの更新処理
		m_institem.Update();
	}

	if (m_cameraManager.GetCameraID() == CCameraManager::CAMERA_ID_EDITOR)
	{
		m_mapedit.Step();
		m_mapedit.Update();
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