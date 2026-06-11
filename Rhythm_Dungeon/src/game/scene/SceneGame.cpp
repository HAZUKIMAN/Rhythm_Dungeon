#include <DxLib.h>
#include "SceneGame.h"
#include "../Common.h"
#include "../collision/CollisionManager.h"
#include "../../lib/Input/Input.h"
#include "../../Data.h"
#include "../../lib/sound/SoundManager.h"
#include "../effect/effekseer.h"
#include "../../lib/sound/effectData/effectData.h"


static const float HIGHT_GRID = 2.5f;	// 移動速度

//-------------------------------
//		コンストラクタ
//-------------------------------
CSceneGame::CSceneGame()
{
	move_box		= NONE;
	m_carryState	= PUT_NONE;
	m_clearSelect	= CLEAR_NEXT;
	m_editMode		= EDIT_MAP;
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
	memset(&m_player_startPos, 0, sizeof(VECTOR));

	// カメラ初期化
	m_cameraManager.Init();
	//ニア・ファー
	m_cameraManager.SetNearFar(5.0f, 5000.0f);
	// 人間初期化
	m_human.Init();
	//// 猫初期化
	m_cat.Init();
	// 背景初期化
	m_backgroundManager.Init();
	//ゴールの初期化
	m_goal.Init();
	//マップエディターの初期化
	m_mapedit.Init();
	m_objEditor.Init();

	//--------------------
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

	Set();

	// カメラ更新処理
	m_cameraManager.Step(m_cat, m_isGoal);

	CSoundManager::Stop(CSoundManager::SOUNDID_CLEAR_BGM);
	CSoundManager::Play(CSoundManager::SOUNDID_GAME_BGM, DX_PLAYTYPE_LOOP);
}


//-------------------------------
//		実行処理
//-------------------------------
int CSceneGame::Step()
{
	int ret = -1;

	//---------------------------------
	// リセット
	//---------------------------------
	if (Input::Key::Push(KEY_INPUT_R) || Input::Controller::Push(XINPUT_BUTTON_Y))
	{
		Reset();
	}

	//---------------------------------
	// クリアメニュー
	//---------------------------------
	if (m_isGoal)
	{
		//---------------------------------
		// 上選択
		//---------------------------------
		if (Input::Key::Push(KEY_INPUT_W) || Input::Controller::Push(XINPUT_BUTTON_DPAD_UP))
		{
			m_clearSelect = CLEAR_NEXT;
		}

		//---------------------------------
		// 下選択
		//---------------------------------
		if (Input::Key::Push(KEY_INPUT_S) || Input::Controller::Push(XINPUT_BUTTON_DPAD_DOWN))
		{
			m_clearSelect = CLEAR_SELECT;
		}

		//---------------------------------
		// 決定
		//---------------------------------
		if (Input::Key::Push(KEY_INPUT_RETURN) || Input::Controller::Push(XINPUT_BUTTON_B))
		{
			CSoundManager::Stop(CSoundManager::SOUNDID_GAME_BGM);

			//---------------------------------
			// 次ステージ
			//---------------------------------
			if (m_clearSelect ==CLEAR_NEXT)
			{
				int stage = Data::GetInstance()->GetSelectStage();
				int maxStage = Data::GetInstance()->GetStageCount();

				//---------------------------------
				// 最終ステージ
				//---------------------------------
				if (stage >= maxStage - 1)
				{
					return SCENEID_SELECT;
				}

				//---------------------------------
				// 次ステージ
				//---------------------------------
				Data::GetInstance()->NextStage();

				CSoundManager::Play(CSoundManager::SOUNDID_SE_ENTER, DX_PLAYTYPE_BACK);
				return SCENEID_GAME;
			}

			//---------------------------------
			// セレクトへ戻る
			//---------------------------------
			if (m_clearSelect == CLEAR_SELECT)
			{
				CSoundManager::StopAll();
				CSoundManager::Play(CSoundManager::SOUNDID_SE_ENTER, DX_PLAYTYPE_BACK);
				return SCENEID_SELECT;
			}
		}

		// 各種更新	
		m_human.Update();
		// 猫の更新
		m_cat.Update();

		//---------------------------------
		// クリア中は停止
		//---------------------------------
		return -1;
	}

	//---------------------------------
	// ゴール判定
	//---------------------------------
	if (CCollisionManager::CheckHithumanToGoal(m_human, m_goal) && !m_isGoal)
	{
		CSoundManager::Stop(CSoundManager::SOUNDID_GAME_BGM);
		CSoundManager::Play(CSoundManager::SOUNDID_CLEAR_BGM, DX_PLAYTYPE_LOOP);

		//humanのクリア処理
		m_human.Clear();
		//猫のクリア処理
		m_cat.Clear();

		m_isGoal = true;
	}

	// カメラ更新処理
	m_cameraManager.Step(m_cat, m_isGoal);
	m_cameraManager.Update();

	//---------------------------------
	// 通常更新
	//---------------------------------
	Calc();


	return ret;
}

void CSceneGame::Set()
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

			//拡大率
			VECTOR effect_scale = VGet(1.0f, 1.0f, 1.0f);
			//呼び出すエフェクトのID
			int effectId = CEffectData::GetId(EFFECT_HIT_ENEMY);
			//コインの位置にエフェクトを呼び出す
			CEffekseerCtrl::Request(effectId, vec, false);
			//エフェクトの拡大・縮小
			CEffekseerCtrl::SetScale(effectId, effect_scale);

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
				m_startDer = 0.0f;
			}
			else if (rotDeg >= 45 && rotDeg < 135)
			{
				m_human.SetDirect(2); // LEFT
				m_human.Setrot(DX_PI_F / 2);
				m_startDer = DX_PI_F / 2;
			}
			else if (rotDeg >= 135 && rotDeg < 225)
			{
				m_human.SetDirect(3); // UP
				m_human.Setrot(DX_PI_F);
				m_startDer = DX_PI_F;
			}
			else
			{
				m_human.SetDirect(0); // RIGHT
				m_human.Setrot(-DX_PI_F / 2);
				m_startDer = -DX_PI_F / 2;
			}

		}

		if (obj.type == OBJ_CAT)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y = (obj.y + 0.5f) * TILE_SIZE;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			m_cat.SetPos(VGet(worldpos_x, worldpos_y, worldpos_z));

			//猫の初期位置を保存
			m_player_startPos = VGet(worldpos_x, worldpos_y, worldpos_z);

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
			float worldpos_x =(obj.x + 0.5f) * TILE_SIZE;
			float worldpos_y =(obj.y + 0.5f) * TILE_SIZE;
			float worldpos_z =(obj.z + 0.5f) * TILE_SIZE;

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
	//空の描画
	m_backgroundManager.Draw();
	//マップの描画
	m_mapedit.Draw();
	//カメラの描画
	m_cameraManager.Draw();
	//humanの描画
	m_human.Draw();	
	//猫の描画
	m_cat.Draw();
	//猫の目の前の描画
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

	//ゴールの描画
	m_goal.Draw();

	//-------------------------------------
	// エディターモードの描画
	//-------------------------------------
	if (m_cameraManager.GetCameraID() == CCameraManager::CAMERA_ID_EDITOR)
	{
		m_objEditor.Draw();
		DrawString(1220, 300, "TABで切り替え", WHITE);

		if (m_editMode == EDIT_MAP)
		{
			DrawString(1220, 320, "MAP EDIT MODE", GREEN);
		}
		else
		{
			DrawString(1220, 320, "OBJECT EDIT MODE", YELLOW);
		}
	}

	//ゴールした時のクリア後の選択描画
	if (m_isGoal)
	{
		DrawBox(200,150,600,450,GetColor(0, 0, 0),TRUE);
		DrawString(300,180,"STAGE CLEAR!",YELLOW);

		//---------------------------------
		// 次ステージ
		//---------------------------------
		DrawString(280,260,m_clearSelect== CLEAR_NEXT? "> NEXT STAGE": "NEXT STAGE",WHITE);

		//---------------------------------
		// セレクト
		//---------------------------------
		DrawString(280,320,m_clearSelect== CLEAR_SELECT? "> SELECT": "SELECT",WHITE);
	}

	//カメラの切り替え表示
	DrawFormatString(1200,20,WHITE,"デバックカメラ切り替え処理:Key C \nエディターカメラ切り替え処理:Key B\nプレイカメラへの切り替え:key V");
	DrawFormatString(700, 100, RED, "人間のＸ軸：%f\n 人間のY軸：%f\n人間のZ軸：%f", m_human.GetPos().x, m_human.GetPos().y, m_human.GetPos().z);
}


//-------------------------------
//		終了処理
//-------------------------------
void CSceneGame::Fin()
{
	// カメラの終了処理	
	m_cameraManager.Fin();
	// humanの終了処理	
	m_human.Fin();
	// 猫の終了処理	
	m_cat.Fin();
	// 空の終了処理	
	m_backgroundManager.Fin();
	// マップエディターの終了処理	
	m_mapedit.Fin();
	// オブジェクトエディターの終了処理	
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

		//猫が物を運ぶ処理全般
		CatCrry();
		CatCrryToBridge();
		
		// 待機･移動中処理 
		m_human.NormalExec(m_blocks, m_institem, move_box);

		for (auto& enemy : m_enemy) { enemy->NormalExec(m_blocks,m_institem, move_box); }

		//  人間と床と壁との当たり判定
		m_human.AddPos(CCollisionManager::HitMap(m_human.GetCenter(), m_human.GetRadius(), m_mapedit));

		//エネミーとオブジェクトの当たり判定
		CCollisionManager::HitEnemyToObject(m_enemy, m_objEditor);

		//  猫と床と壁との当たり判定
		VECTOR vec = VGet(m_cat.GetCenter().x, m_cat.GetCenter().y + 2.0, m_cat.GetCenter().z);

		//猫とマップの当たり判定
		m_cat.AddPos(CCollisionManager::HitMap(m_cat.GetCenter(), m_cat.GetRadius(), m_mapedit));

		//エネミーとマップの当たり判定
		for (auto& enemy : m_enemy)
		{
			VECTOR hit =CCollisionManager::HitMap(enemy->GetCenter(),enemy->GetRadius(),m_mapedit);
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

	//エディター時の切り替え処理
	if (m_cameraManager.GetCameraID() == CCameraManager::CAMERA_ID_EDITOR)
	{
		//---------------------------------
		// 編集モード切り替え
		//---------------------------------
		if (Input::Key::Push(KEY_INPUT_TAB))
		{
			if (m_editMode ==EDIT_MAP)
			{
				m_editMode = EDIT_OBJECT;
			}
			else
			{
				m_editMode = EDIT_MAP;
			}
		}

		if (m_editMode == EDIT_OBJECT)
		{
			m_objEditor.Step();
			m_objEditor.Update();

		}
		if (m_editMode == EDIT_MAP)
		{
			m_mapedit.Step(m_objEditor);
			m_mapedit.Update();
		}
	}

	//デバックカメラ切り替え処理
	if (Input::Key::Push(KEY_INPUT_C))
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_DEBUG);
	//エディターカメラ切り替え処理
	if (Input::Key::Push(KEY_INPUT_B))
	{
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_EDITOR);
		CSoundManager::StopAll();
	}
	//プレイカメラへの切り替え
	else if (Input::Key::Push(KEY_INPUT_V))
		m_cameraManager.ChangeCamera(CCameraManager::CAMERA_ID_PLAY);

}


//-------------------------------
// 猫がブロックを運ぶ処理
//-------------------------------
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
				if (Input::Key::Push(KEY_INPUT_J) || Input::Controller::Push(XINPUT_BUTTON_A))
				{
					move_box = CARRY;
					CSoundManager::Play(CSoundManager::SOUNDID_SE_CARRY, DX_PLAYTYPE_BACK);
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
	if (move_box == CARRY && m_carryItem != nullptr)
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
		if (Input::Key::Push(KEY_INPUT_G) || Input::Controller::Push(XINPUT_BUTTON_B))
		{
			CSoundManager::Play(CSoundManager::SOUNDID_SE_PUT, DX_PLAYTYPE_BACK);

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

	}
}


//-------------------------------
// 猫が橋を運ぶ処理
//-------------------------------
void CSceneGame::CatCrryToBridge()
{
	//--------------------------------------
		// 橋を拾う
		//--------------------------------------
	if (m_carryState == NONE)
	{

		//---------------------------------
		// 魚を持っていたら拾えない
		//---------------------------------
		if (move_box == CARRY)
		{
			return;
		}

		for (auto& bridge : m_bridge)
		{
			VECTOR diff =
				VSub(
					m_cat.GetPos(),
					bridge->GetPos());

			//---------------------------------
			// 近くにある
			//---------------------------------
			if (VSize(diff) < 10.0f)
			{
				if (Input::Key::Push(KEY_INPUT_J) || Input::Controller::Push(XINPUT_BUTTON_A))
				{
					CSoundManager::Play(CSoundManager::SOUNDID_SE_CARRY, DX_PLAYTYPE_BACK);
					//---------------------------------
					// 橋のマップ座標取得
					//---------------------------------
					VECTOR pos = bridge->GetPos();

					int x =(int)floor(pos.x /TILE_SIZE);
					int y =(int)floor(pos.y /TILE_SIZE);
					int z =(int)floor(pos.z /TILE_SIZE);

					//---------------------------------
					// 猫の位置
					//---------------------------------
					VECTOR catPos = m_cat.GetPos();

					int catX =(int)floor(catPos.x /TILE_SIZE);
					int catZ =(int)floor(catPos.z /TILE_SIZE);

					//---------------------------------
					// 乗ってるなら拾えない
					//---------------------------------
					if (catX == x &&
						catZ == z)
					{
						continue;
					}

					//---------------------------------
					// 設置済みの床を消す
					//---------------------------------
					if (m_mapedit.GetMap(y,z,x) == TILE_BRIDGE)//
					{
						m_mapedit.SetMap(y,z,x,TILE_NONE);
						m_mapedit.BuildInstances();
					}

					//---------------------------------
					// 持つ
					//---------------------------------
					m_carryBridge =bridge;
					m_carryState =CARRY_BRIDGE;
					return;
				}
			}
		}
	}

	//--------------------------------------
	// 運搬中
	//--------------------------------------
	if (m_carryState == CARRY_BRIDGE)
	{
		//---------------------------------
		// 安全チェック
		//---------------------------------
		if (m_carryBridge ==
			nullptr)
		{
			return;
		}

		//---------------------------------
		// 猫についてくる
		//---------------------------------
		VECTOR pos =m_cat.GetPos();
		pos.y += 5.0f;
		m_carryBridge->SetPos(pos);

		//---------------------------------
		// Gで橋を置く
		//---------------------------------
		if (Input::Key::Push(KEY_INPUT_G) || Input::Controller::Push(XINPUT_BUTTON_B))
		{
			CSoundManager::Play(CSoundManager::SOUNDID_SE_PUT, DX_PLAYTYPE_BACK);

			//---------------------------------
			// 猫座標
			//---------------------------------
			VECTOR catPos =m_cat.GetPos();

			int catX =(int)floor(catPos.x /TILE_SIZE);
			int catY =(int)floor(catPos.y / TILE_SIZE);
			int catZ =(int)floor(catPos.z /TILE_SIZE);

			//---------------------------------
			// 猫の向き
			//---------------------------------
			float rot =m_cat.GetRot().y;

			int dirX =(int)roundf(-sinf(rot));
			int dirZ =(int)roundf(-cosf(rot));

			//---------------------------------
			// 橋位置
			//---------------------------------
			int bridgeX = catX + dirX;

			int bridgeZ = catZ + dirZ;

			//---------------------------------
			// 2マス先
			//---------------------------------
			int landX = catX + dirX * 2;
			int landZ = catZ + dirZ * 2;

			//---------------------------------
			// 範囲外防止
			//---------------------------------
			if (bridgeX < 0 ||
				bridgeX >= MAP_W ||
				bridgeZ < 0 ||
				bridgeZ >= MAP_H)
			{
				return;
			}

			if (landX < 0 ||
				landX >= MAP_W ||
				landZ < 0 ||
				landZ >= MAP_H)
			{
				return;
			}

			//---------------------------------
			// 1マス先は穴のみ
			//---------------------------------
			if (m_mapedit.GetMap(
				catY,
				bridgeZ,
				bridgeX)
				!= TILE_NONE)
			{
				return;
			}

			//---------------------------------
			// 2マス先に床必要
			//---------------------------------
			int tile =
				m_mapedit.GetMap(
					catY,
					landZ,
					landX);

			if (tile != TILE_FLOOR && tile != TILE_FLOOR2)
			{
				return;
			}

			//---------------------------------
			// ワールド座標
			//---------------------------------
			float worldX =(bridgeX + 0.5f)* TILE_SIZE;
			float worldY =catY *TILE_SIZE;
			float worldZ =(bridgeZ + 0.5f)* TILE_SIZE;

			//---------------------------------
			// 配置
			//---------------------------------
			m_carryBridge->SetPos(
				VGet(
					worldX,
					worldY + 2.5f,
					worldZ));

			//---------------------------------
			// 向き
			//---------------------------------
			float rotDeg = rot *180.0f /DX_PI_F;

			while (rotDeg < 0)
			{
				rotDeg += 360.0f;
			}

			rotDeg =fmod(rotDeg,360.0f);

			if (rotDeg >= 315 ||
				rotDeg < 45)
			{
				m_carryBridge->SetRotation(0.0f);
			}
			else if (rotDeg >= 45 &&rotDeg < 135)
			{
				m_carryBridge->SetRotation(DX_PI_F / 2);
			}
			else if (rotDeg >= 135 &&rotDeg < 225)
			{
				m_carryBridge->SetRotation(DX_PI_F);
			}
			else
			{
				m_carryBridge->SetRotation(-DX_PI_F / 2);
			}

			//---------------------------------
			// 床化
			//---------------------------------
			m_mapedit.SetMap(
				catY,
				bridgeZ,
				bridgeX,
				TILE_BRIDGE);//

			m_mapedit.BuildInstances();

			//---------------------------------
			// 手放す
			//---------------------------------
			m_carryBridge =nullptr;
			m_carryState =PUT_NONE;

		}
	}
}


//-------------------------------
// 初期位置にリセットさせる処理
//-------------------------------
void CSceneGame::Reset()
{
	//---------------------------------
	// 持ち物解除
	//---------------------------------
	m_carryBridge = nullptr;
	m_carryItem = nullptr;
	m_carryState = PUT_NONE;

	//---------------------------------
	// 現在の敵削除
	//---------------------------------
	for (auto enemy : m_enemy)
	{
		delete enemy;
	}
	m_enemy.clear();

	//---------------------------------
	// 現在の魚削除
	//---------------------------------
	for (auto item : m_institem)
	{
		delete item;
	}
	m_institem.clear();

	//---------------------------------
	// 現在の橋削除
	//---------------------------------
	for (auto bridge : m_bridge)
	{
		delete bridge;
	}
	m_bridge.clear();

	//---------------------------------
	// 現在の設置ブロック削除
	//---------------------------------
	for (auto block : m_blocks)
	{
		delete block;
	}
	m_blocks.clear();

	//---------------------------------
	// マップ再読み込み
	//---------------------------------
	m_mapedit.LoadMap(
		Data::GetInstance()->GetStagePath(),
		m_objEditor);

	//---------------------------------
	// オブジェクト再生成
	//---------------------------------
	auto& objs = m_objEditor.GetObjects();

	for (const auto& obj : objs)
	{
		//---------------------------------
		// ワールド座標変換
		//---------------------------------
		float worldX =
			(obj.x + 0.5f)
			* TILE_SIZE;

		float worldY =
			(obj.y + 0.5f)
			* TILE_SIZE;

		float worldZ =
			(obj.z + 0.5f)
			* TILE_SIZE;

		VECTOR pos =
			VGet(
				worldX,
				worldY,
				worldZ);

		//---------------------------------
		// エネミー
		//---------------------------------
		if (obj.type == OBJ_ENEMY)
		{
			CEnemy* enemy =
				new CEnemy;

			enemy->Init();
			enemy->Load();

			enemy->SetPos(pos);

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

			m_enemy.push_back(
				enemy);
		}

		//---------------------------------
		// 魚
		//---------------------------------
		if (obj.type == OBJ_ITEM)
		{
			CInstalledItem* item =
				new CInstalledItem;

			item->Init();

			item->SetPos(pos);

			m_institem.push_back(
				item);
		}

		//---------------------------------
		// 橋
		//---------------------------------
		if (obj.type == OBJ_BRIDGE)
		{
			CBridge* bridge =
				new CBridge;

			bridge->Init();

			bridge->SetPos(pos);

			m_bridge.push_back(
				bridge);
		}

		//---------------------------------
		// 設置ブロック
		//---------------------------------
		if (obj.type == OBJ_SETBLOCK)
		{
			CBlock* block =
				new CBlock;

			block->Init();
			VECTOR vec = VGet(pos.x,pos.y+ 1.5f,pos.z);
			block->SetPos(vec);

			m_blocks.push_back(
				block);
		}

		//---------------------------------
		// 人間
		//---------------------------------
		if (obj.type == OBJ_HUMAN)
		{
			float worldpos_x = (obj.x + 0.5f) * TILE_SIZE;
			float worldpos_z = (obj.z + 0.5f) * TILE_SIZE;

			float hight = 10.0f;

			VECTOR vec = VGet(worldpos_x, hight, worldpos_z);
			m_human.SetPos(vec);
			m_human.SetRespawn(vec);

			//拡大率
			VECTOR effect_scale = VGet(1.0f, 1.0f, 1.0f);
			//呼び出すエフェクトのID
			int effectId = CEffectData::GetId(EFFECT_HIT_ENEMY);
			//コインの位置にエフェクトを呼び出す
			CEffekseerCtrl::Request(effectId, vec, false);
			//エフェクトの拡大・縮小
			CEffekseerCtrl::SetScale(effectId, effect_scale);

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
				m_startDer = 0.0f;
			}
			else if (rotDeg >= 45 && rotDeg < 135)
			{
				m_human.SetDirect(2); // LEFT
				m_human.Setrot(DX_PI_F / 2);
				m_startDer = DX_PI_F / 2;
			}
			else if (rotDeg >= 135 && rotDeg < 225)
			{
				m_human.SetDirect(3); // UP
				m_human.Setrot(DX_PI_F);
				m_startDer = DX_PI_F;
			}
			else
			{
				m_human.SetDirect(0); // RIGHT
				m_human.Setrot(-DX_PI_F / 2);
				m_startDer = -DX_PI_F / 2;
			}
		}
	}

	//---------------------------------
	// プレイヤー位置
	//---------------------------------
	m_cat.SetPos(
		m_player_startPos);

	
}
