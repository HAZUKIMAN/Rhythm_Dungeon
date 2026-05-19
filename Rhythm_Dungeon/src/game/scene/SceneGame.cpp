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
	////ブロックの初期化
	//m_block.Init();
	//運べるアイテムの初期化
	m_institem.Init();
	// 背景初期化
	m_backgroundManager.Init();
	//ゴール
	m_goal.Init();

	m_mapedit.Init();
	m_objEditor.Init();

	m_blocks.clear();
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneGame::Load()
{
	m_player.Load();
	m_cat.Load();
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
	
	////ゴールとプレイヤーの判定
	//if (CCollisionManager::CheckHitPlayerToGoal(m_player, m_goal))
	//{
	//	ret = SCENEID_GAMEOVER;
	//}

	if (Input::Key::Push(KEY_INPUT_R))
	{
		auto& objs = m_objEditor.GetObjects();

		printf("size = %d\n", (int)objs.size());

		for (const auto& obj : objs) {


			if (obj.type == OBJ_PLAYER)
			{
				float gridSize = 5.0f;
				float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
				float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

				m_player.SetPos(VGet(worldpos_x, gridSize, worldpos_z));
			}
			if (obj.type == OBJ_ENEMY)
			{
				float gridSize = 5.0f;
				float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
				float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

				m_cat.SetPos(VGet(worldpos_x, gridSize, worldpos_z));
			}
			if (obj.type == OBJ_ITEM)
			{
				float gridSize = 5.0f;

				float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
				float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

				VECTOR vec = VGet(worldpos_x, gridSize, worldpos_z);
				m_institem.SetPos(vec);
				
			}
			if (obj.type == OBJ_GOAL)
			{
				float gridSize = 5.0f;
				float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
				float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

				VECTOR vec = VGet(worldpos_x, gridSize, worldpos_z);
				m_goal.SetPos(vec);
			}
			//---------------------------------
			// ブロック生成
			//---------------------------------
			if (obj.type == OBJ_SETBLOCK)
			{
				float gridSize = 5.0f;

				float worldpos_x =(obj.x + 0.5f)* TILE_SIZE;

				float worldpos_z =(obj.z + 0.5f)* TILE_SIZE;
				VECTOR pos =VGet(worldpos_x,gridSize,worldpos_z);

				//---------------------------------
				// 新しいブロック作成
				//---------------------------------
				CBlock* block = new CBlock;

				block->Init();
				block->SetPos(pos);

				m_blocks.push_back(block);
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

	m_mapedit.Draw();
	m_objEditor.Draw();

	m_cameraManager.Draw();
	m_player.Draw();
	m_cat.Draw();
	m_cat.DrawPlaceBlockPreview(m_mapedit);
	m_institem.Draw();
	//ブロックの初期化
	for (auto& block : m_blocks)
	{
		block->Draw();
	}

	m_goal.Draw();		//ゴール


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
		m_cat.Step(m_mapedit);
		
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
			VECTOR vec = VGet( m_cat.GetPos().x, m_cat.GetPos().y, m_cat.GetPos().z );
			m_institem.SetPos(vec);

			if (Input::Key::Push(KEY_INPUT_G))
			{
				m_cat.PlaceBlock(m_objEditor);

				auto& objs = m_objEditor.GetObjects();

				for (const auto& obj : objs) {

					if (obj.type == OBJ_PUT_BOX)
					{
						float gridSize = 3.0f;

						float worldX = (obj.x + 0.5f) * TILE_SIZE;
						float worldY = (obj.y + 0.5f) * TILE_SIZE;
						float worldZ = (obj.z + 0.5f) * TILE_SIZE;

						m_institem.SetPos(VGet(worldX, worldY, worldZ));//2.5fはマスの真ん中に持っていくよう

						m_objEditor.RemoveObject(obj.x, obj.y, obj.z);
					}
					
				}

				move_box = NONE;
			}
		}
	
		//プレイヤーと設置ブロック
		m_player.AddPos(CCollisionManager::HitPlayerToBlock(m_player,m_blocks));

		//  プレイヤーと床と壁との当たり判定
		m_player.AddPos(CCollisionManager::HitMap(m_player.GetCenter(), m_player.GetRadius(), m_mapedit));

		// オブジェクト一覧とプレイヤーの当たり判定
		m_player.AddPos(CCollisionManager::HitCatToObject(m_player, m_objEditor));

		//  猫と床と壁との当たり判定
		VECTOR vec = VGet(m_cat.GetCenter().x, m_cat.GetCenter().y +2.0, m_cat.GetCenter().z);

		m_cat.AddPos(CCollisionManager::HitMap(m_cat.GetCenter(),m_cat.GetRadius(), m_mapedit));


		// 各種更新
		m_player.Update();
		// 猫の更新
		m_cat.Update();
		//アイテムの更新処理
		m_institem.Update();
		//ブロックの設置
		for (auto& block : m_blocks)
		{
			block->Update();
		}
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