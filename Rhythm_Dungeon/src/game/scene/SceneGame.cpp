#include <DxLib.h>
#include "SceneGame.h"
#include "../Common.h"
#include "../collision/CollisionManager.h"
#include "../system/SoundManager.h"
#include "../../lib/Input/Input.h"

static const float HIGHT_GRID = 2.5f;	// 移動速度

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

	// 人間初期化
	m_human.Init();
	//// 猫初期化
	m_cat.Init();
	// 背景初期化
	m_backgroundManager.Init();
	//ゴール
	m_goal.Init();

	m_mapedit.Init();
	m_objEditor.Init();



	m_institem.clear();
	m_blocks.clear();
	m_enemy.clear();
}


//-------------------------------
//		データロード
//-------------------------------
void CSceneGame::Load()
{
	m_human.Load();
	m_cat.Load();

	char stageFile[64];

	/*sprintf_s(
		stageFile,
		"stage%d.dat",
		g_SelectStage + 1
	);*/

	m_mapedit.Load(stageFile);
	m_objEditor.Load(stageFile);

	Reset();
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneGame::Step()
{
	int ret = -1;
	Calc();
	
	////ゴールと人間の判定
	//if (CCollisionManager::CheckHithumanToGoal(m_human, m_goal))
	//{
	//	ret = SCENEID_TITLE;
	//}

	if (Input::Key::Push(KEY_INPUT_R))
	{
		Reset();
	}

	return ret;
}

void CSceneGame::Reset()
{
	auto& objs = m_objEditor.GetObjects();

	printf("size = %d\n", (int)objs.size());

	for (const auto& obj : objs) {


		if (obj.type == OBJ_HUMAN)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			float hight = 10.0f;

			VECTOR vec = VGet(worldpos_x, hight, worldpos_z);
			m_human.SetPos(vec);
			m_human.SetRespawn(vec);
			//---------------------------------
			// rotY → direction変換
			//---------------------------------
			float rotDeg = obj.rotY * 180.0f / DX_PI_F;

			// マイナス対策
			while (rotDeg < 0)
			{
				rotDeg += 360.0f;
			}

			rotDeg = fmod(rotDeg, 360.0f);

			//---------------------------------
			// 向き判定
			//---------------------------------
			if (rotDeg >= 315 || rotDeg < 45)
			{
				m_human.SetDirect(1); // DOWN
				m_human.Setrot(0.0f);
			}
			else if (rotDeg >= 45 && rotDeg < 135)
			{
				m_human.SetDirect(2); // LEFT
				m_human.Setrot(DX_PI_F / 2);
			}
			else if (rotDeg >= 135 && rotDeg < 225)
			{
				m_human.SetDirect(3); // UP
				m_human.Setrot(DX_PI_F);
			}
			else
			{
				m_human.SetDirect(0); // RIGHT
				m_human.Setrot(-DX_PI_F / 2);
			}
		}

		if (obj.type == OBJ_CAT)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y = (obj.y + 0.5f) * TILE_SIZE;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;
			float hight = 10;

			m_cat.SetPos(VGet(worldpos_x, worldpos_y, worldpos_z));
			m_cat.SetRadius(obj.rotY);
		}
		if (obj.type == OBJ_ITEM)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y = (obj.y + 0.5f) * TILE_SIZE + HIGHT_GRID;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			VECTOR pos = VGet(worldpos_x, worldpos_y, worldpos_z);

			//---------------------------------
			// 新しいブロック作成
			//---------------------------------
			CInstalledItem* inst = new CInstalledItem;
			inst->Init();
			inst->SetPos(pos);

			m_institem.push_back(inst);

		}

		if (obj.type == OBJ_GOAL)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y = (obj.y + 0.5f) * TILE_SIZE;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			VECTOR vec = VGet(worldpos_x, worldpos_y, worldpos_z);
			m_goal.SetPos(vec);
		}
		//---------------------------------
		// ブロック生成
		//---------------------------------
		if (obj.type == OBJ_SETBLOCK)
		{
			
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y = (obj.y + 0.5f) * TILE_SIZE + HIGHT_GRID;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			VECTOR pos = VGet(worldpos_x, worldpos_y, worldpos_z);

			//---------------------------------
			// 新しいブロック作成
			//---------------------------------
			CBlock* block = new CBlock;

			block->Init();
			block->SetPos(pos);

			m_blocks.push_back(block);
		}
		//エネミーを生成
		if (obj.type == OBJ_ENEMY)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y = (obj.y + 0.5f) * TILE_SIZE;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			VECTOR pos = VGet(worldpos_x, worldpos_y, worldpos_z);

			//---------------------------------
			// 新しいブロック作成
			//---------------------------------
			CEnemy* enemy = new CEnemy;

			enemy->Init();
			enemy->Load();

			enemy->SetPos(pos);
			enemy->SetRadius(obj.rotY);

			m_enemy.push_back(enemy);

		}

	}
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
	m_human.Draw();
	m_cat.Draw();
	m_cat.DrawPlaceBlockPreview(m_mapedit);
	
	//ブロックの描画
	for (auto& institem : m_institem)
	{
		institem->Draw();
	}

	//エネミーの描画
	for (auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	//ブロックの描画
	for (auto& block : m_blocks)
	{
		block->Draw();
	}

	m_goal.Draw();		//ゴール


	//カメラの切り替え表示
	DrawFormatString(1200,20,WHITE,"デバックカメラ切り替え処理:Key C \nエディターカメラ切り替え処理:Key B\nプレイカメラへの切り替え:key V");

	DrawFormatString(700, 100, RED, "人間のＸ軸：%f\n 人間のY軸：%f\n人間のZ軸：%f", m_human.GetPos().x, m_human.GetPos().y, m_human.GetPos().z);
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneGame::Fin()
{
	m_cameraManager.Fin();
	m_human.Fin();
	m_cat.Fin();
	m_backgroundManager.Fin();
	m_mapedit.Fin();
	m_objEditor.Fin();

	//ブロックの削除
	for (auto& institem : m_institem)
	{
		institem->Fin();
	}

	for (auto& enemy : m_enemy)
	{
		enemy->Fin();
	}
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
		// 人間更新処理
		m_human.Step();

		//ブロックの初期化
		for (auto& enemy : m_enemy)
		{
			enemy->Step();
		}

		// 猫の更新処理
		m_cat.Step(m_mapedit);
		
		//ブロックの描画
		for (auto& institem : m_institem)
		{
		//--------------------------------------------
		//		猫がボックスを運ぶかどうかの判定
		//--------------------------------------------
		VECTOR Memo = VGet(0, 0, 0);
		//1回差をとります//正規化//アークタンジェント
		Memo = VSub(m_cat.GetPos(), institem->GetPos());

		if (VSize(Memo) < 10.0f) 
		{
			if (Input::Key::Keep(KEY_INPUT_J))
			{
				move_box = CARRY;

				auto& objs = m_objEditor.GetObjects();
				for (const auto& obj : objs) {
					if (obj.type == OBJ_ITEM)
					{
						m_objEditor.RemoveObject(obj.x, obj.y, obj.z);
					}
					if (obj.type == OBJ_PUT_BOX)
					{
						float worldX = (obj.x + 0.5f) * TILE_SIZE;
						float worldY = (obj.y + 0.5f) * TILE_SIZE + 2.5;
						float worldZ = (obj.z + 0.5f) * TILE_SIZE;

						m_objEditor.RemoveObject(obj.x, obj.y, obj.z);
					}
				}
			}
		}

		if (move_box == CARRY)
		{
			VECTOR vec = VGet(m_cat.GetPos().x, m_cat.GetPos().y, m_cat.GetPos().z);
			institem->SetPos(vec);

			if (Input::Key::Push(KEY_INPUT_G))
			{
				m_cat.PlaceBlock(m_objEditor);

				auto& objs = m_objEditor.GetObjects();

				for (const auto& obj : objs) {

					if (obj.type == OBJ_PUT_BOX)
					{

						float worldX = (obj.x + 0.5f) * TILE_SIZE;
						float worldY = (obj.y + 0.5f) * TILE_SIZE+2.5;
						float worldZ = (obj.z + 0.5f) * TILE_SIZE;

						institem->SetPos(VGet(worldX, worldY, worldZ));

						//m_objEditor.RemoveObject(obj.x, obj.y, obj.z);
					}

				}

				move_box = NONE;
			}
		}

	}
	

		// 待機･移動中処理
		m_human.NormalExec(m_blocks,m_institem);

		for (auto& enemy : m_enemy)
		{
			enemy->NormalExec(m_blocks);
		}

		//猫が箱を持っていない時
		if (!move_box == CARRY) {
			//ブロックの描画
			for (auto& institem : m_institem)
			{
				CCollisionManager::HitCatToInst(m_human, institem->GetPos());
			}
		}

		//  人間と床と壁との当たり判定
		m_human.AddPos(CCollisionManager::HitMap(m_human.GetCenter(), m_human.GetRadius(), m_mapedit));

		// オブジェクト一覧と人間の当たり判定
		m_human.AddPos(CCollisionManager::HitCatToObject(m_human, m_objEditor));

		//  猫と床と壁との当たり判定
		VECTOR vec = VGet(m_cat.GetCenter().x, m_cat.GetCenter().y +2.0, m_cat.GetCenter().z);

		//猫とマップの当たり判定
		m_cat.AddPos(CCollisionManager::HitMap(m_cat.GetCenter(),m_cat.GetRadius(), m_mapedit));

		//エネミーとマップの当たり判定
		for (auto& enemy : m_enemy)
		{
			enemy->AddPos(CCollisionManager::HitMap(enemy->GetCenter(), enemy->GetRadius(), m_mapedit));

			CCollisionManager::CheckHithumanToEnemy(m_human, enemy);
		}

		// 各種更新
		m_human.Update();

		// 猫の更新
		m_cat.Update();

		//ブロックの描画
		for (auto& institem : m_institem)
		{
			//アイテムの更新処理
			institem->Update();
		}
		//ブロックの設置
		for (auto& block : m_blocks)
		{
			block->Update();
		}

		//エネミーの更新
		for (auto& enemy : m_enemy)
		{
			enemy->Update();
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
	m_cameraManager.Step(m_cat);
	m_cameraManager.Update();


}