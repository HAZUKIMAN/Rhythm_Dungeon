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
	m_carryState = PUT_NONE;
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


	m_bridge.clear();
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

	m_mapedit.Load(m_objEditor);
	m_objEditor.Load();

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
	//	ret = SCENEID_SELECT;
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

		//----------------------------
		//エネミーを生成
		//---------------------------
		if (obj.type == OBJ_ENEMY)
		{
			float worldpos_x =
				(obj.x + 0.5f) * TILE_SIZE;

			float worldpos_y =
				(obj.y + 0.5f) * TILE_SIZE;

			float worldpos_z =
				(obj.z + 0.5f) * TILE_SIZE;

			VECTOR pos =
				VGet(worldpos_x,worldpos_y,worldpos_z);

			//---------------------------------
			// エネミー作成
			//---------------------------------
			CEnemy* enemy = new CEnemy;

			enemy->Init();
			enemy->Load();

			//---------------------------------
			// 位置
			//---------------------------------
			enemy->SetPos(pos);

			//---------------------------------
			// 半径
			//---------------------------------
			enemy->SetRadius(2.5f);

			//---------------------------------
			// rotY → direction変換
			//---------------------------------
			float rotDeg =
				obj.rotY * 180.0f / DX_PI_F;

			//---------------------------------
			// マイナス対策
			//---------------------------------
			while (rotDeg < 0)
			{
				rotDeg += 360.0f;
			}

			rotDeg = fmod(rotDeg, 360.0f);

			//---------------------------------
			// 向き判定
			//---------------------------------
			if (rotDeg >= 315 ||
				rotDeg < 45)
			{
				enemy->SetDirect(1);
				// DOWN
			}
			else if (
				rotDeg >= 45 &&
				rotDeg < 135)
			{
				enemy->SetDirect(2);
				// LEFT
			}
			else if (
				rotDeg >= 135 &&
				rotDeg < 225)
			{
				enemy->SetDirect(3);
				// UP
			}
			else
			{
				enemy->SetDirect(0);
				// RIGHT
			}

			//---------------------------------
			// 登録
			//---------------------------------
			m_enemy.push_back(enemy);
		}


		//---------------------------
		//		橋の生成
		//---------------------------
		if (obj.type == OBJ_BRIDGE)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y = (obj.y + 0.5f) * TILE_SIZE + HIGHT_GRID;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			VECTOR pos = VGet(worldpos_x, worldpos_y, worldpos_z);

			//---------------------------------
			// 新しいブロック作成
			//---------------------------------
			CBridge* bridge = new CBridge;
			bridge->Init();
			bridge->SetPos(pos);

			m_bridge.push_back(bridge);

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
	
	//運べるブロックの描画
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

	//橋の描画
	for (auto& bridge : m_bridge)
	{
		bridge->Draw();
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

	//エネミーの削除
	for (auto& enemy : m_enemy)
	{
		enemy->Fin();
	}

	//橋の削除
	for (auto& bridge : m_bridge)
	{
		bridge->Fin();
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


		//猫物を運ぶ処理全般
		CatCrry();
		CatCrryToBridge();
		
		// 待機･移動中処理 
		m_human.NormalExec(m_blocks, m_institem);

		for (auto& enemy : m_enemy) 
		{ enemy->NormalExec(m_blocks,m_institem); }

		//  人間と床と壁との当たり判定
		m_human.AddPos(CCollisionManager::HitMap(m_human.GetCenter(), m_human.GetRadius(), m_mapedit));

		// オブジェクト一覧と人間の当たり判定
		//m_human.AddPos(CCollisionManager::HitHumanToObject(m_human, m_objEditor));

		CCollisionManager::HitEnemyToObject(m_enemy, m_objEditor);

		//  猫と床と壁との当たり判定
		VECTOR vec = VGet(m_cat.GetCenter().x, m_cat.GetCenter().y + 2.0, m_cat.GetCenter().z);

		//猫とマップの当たり判定
		m_cat.AddPos(CCollisionManager::HitMap(m_cat.GetCenter(), m_cat.GetRadius(), m_mapedit));

		//エネミーとマップの当たり判定
		for (auto& enemy : m_enemy)
		{
			VECTOR hit =
				CCollisionManager::HitMap(enemy->GetCenter(),enemy->GetRadius(),m_mapedit);

			enemy->AddPos(hit);

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

		//ブロックの描画
		for (auto& bridge : m_bridge)
		{
			//アイテムの更新処理
			bridge->Update();
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
		m_mapedit.Step(m_objEditor);
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

void CSceneGame::CatCrry()
{
	//--------------------------------------------
		// 箱を拾う
		//--------------------------------------------
	if (move_box != CARRY)
	{
		for (auto& institem : m_institem)
		{
			//---------------------------------
			// 猫との距離
			//---------------------------------
			VECTOR memo =
				VSub(
					m_cat.GetPos(),
					institem->GetPos());

			//---------------------------------
			// 近い + J
			//---------------------------------
			if (VSize(memo) < 10.0f)
			{
				if (Input::Key::Push(KEY_INPUT_J))
				{
					move_box = CARRY;

					//---------------------------------
					// 持つ箱を記憶
					//---------------------------------
					m_carryItem = institem;

					break;
				}
			}
		}
	}

	//--------------------------------------------
	// 箱を持っている
	//--------------------------------------------
	if (move_box == CARRY &&
		m_carryItem != nullptr)
	{
		//---------------------------------
		// 猫についてくる
		//---------------------------------
		VECTOR carryPos =
			VGet(
				m_cat.GetPos().x,
				m_cat.GetPos().y + 5.0f,
				m_cat.GetPos().z);

		m_carryItem->SetPos(carryPos);

		//---------------------------------
		// Gで置く
		//---------------------------------
		if (Input::Key::Push(KEY_INPUT_G))
		{
			//---------------------------------
			// 設置位置を作る
			//---------------------------------
			m_cat.PlaceBlock(m_objEditor);

			const auto& objs = m_objEditor.GetObjects();

			//---------------------------------
			// PUT_BOX探す
			//---------------------------------
			for (const auto& obj : objs)
			{
				if (obj.type == OBJ_PUT_BOX)
				{
					float worldX = (obj.x + 0.5f) * TILE_SIZE;
					float worldY = (obj.y + 0.5f) * TILE_SIZE + 2.5f;
					float worldZ = (obj.z + 0.5f) * TILE_SIZE;

					//---------------------------------
					// 持ってる箱だけ置く
					//---------------------------------
					m_carryItem->SetPos(
						VGet(
							worldX,
							worldY,
							worldZ));

					//---------------------------------
					// 配置済みとして保存
					//---------------------------------
					m_objEditor.AddObject(
						obj.x,
						obj.y,
						obj.z,
						OBJ_ITEM);

					//---------------------------------
					// 設置ポイント削除
					//---------------------------------
					m_objEditor.RemoveObject(
						obj.x,
						obj.y,
						obj.z);

					break;
				}
			}

			//---------------------------------
			// 持ち解除
			//---------------------------------
			move_box = NONE;
			m_carryItem = nullptr;
		}

		//--------------------------------------------
		// 持ってない時だけ当たり判定
		//--------------------------------------------
		if (move_box == NONE)
		{
			for (auto& institem : m_institem)
			{
				CCollisionManager::HitHumanToInst(
					m_human, institem->GetPos());

				CCollisionManager::HitEnemyToInst(
					m_enemy, institem->GetPos());
			}
		}

	}
}

void CSceneGame::CatCrryToBridge()
{
	//--------------------------------------
// 持っていない
//--------------------------------------
	if (m_carryState == NONE)
	{
		for (auto& bridge : m_bridge)
		{
			VECTOR diff =
				VSub(
					m_cat.GetPos(),
					bridge->GetPos());

			//---------------------------------
			// 近い
			//---------------------------------
			if (VSize(diff) < 10.0f)
			{
				//---------------------------------
				// Jで拾う
				//---------------------------------
				if (Input::Key::Push(KEY_INPUT_J))
				{
					m_carryBridge =
						bridge;

					m_carryState =
						CARRY_BRIDGE;

					return;
				}
			}
		}
	}

	//--------------------------------------
	// 運搬中
	//--------------------------------------
	if (m_carryState ==
		CARRY_BRIDGE)
	{
		//---------------------------------
		// 猫についてくる
		//---------------------------------
		VECTOR pos =
			m_cat.GetPos();

		pos.y += 5.0f;

		m_carryBridge->SetPos(
			pos);

		//---------------------------------
		// Gで置く
		//---------------------------------
		if (Input::Key::Push(
			KEY_INPUT_G))
		{
			VECTOR catPos =
				m_cat.GetPos();

			//---------------------------------
			// グリッド化
			//---------------------------------
			int x =
				(int)floor(
					catPos.x /
					TILE_SIZE);

			int y =
				(int)floor(
					catPos.y /
					TILE_SIZE);

			int z =
				(int)floor(
					catPos.z /
					TILE_SIZE);

			//---------------------------------
			// グリッド中央
			//---------------------------------
			float worldX =
				(x + 0.5f)
				* TILE_SIZE;

			float worldY =
				y *
				TILE_SIZE;

			float worldZ =
				(z + 0.5f)
				* TILE_SIZE;

			//---------------------------------
			// 設置
			//---------------------------------
			m_carryBridge->SetPos(
				VGet(
					worldX,
					worldY+2.5f,
					worldZ));

			float rotDeg = m_cat.GetRot().y * 180.0f / DX_PI_F;;

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
				// DOWN
				m_carryBridge->SetRotation(0.0f);
			}
			else if (rotDeg >= 45 && rotDeg < 135)
			{
				// LEFT
				m_carryBridge->SetRotation(DX_PI_F / 2);
			}
			else if (rotDeg >= 135 && rotDeg < 225)
			{
				// UP
				m_carryBridge->SetRotation(DX_PI_F);
			}
			else
			{
				// RIGHT
				m_carryBridge->SetRotation(-DX_PI_F / 2);
			}

			//---------------------------------
			// 持ち解除
			//---------------------------------
			m_carryBridge =
				nullptr;

			m_carryState =
				PUT_NONE;
		}
	}
}