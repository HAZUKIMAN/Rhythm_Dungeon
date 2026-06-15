#include <math.h>
#include "../../Data.h"
#include "../Anime/Anime.h"
#include "../common.h"
#include"Enemy.h"
#include "../../lib/Input/Input.h"

//	定義関連------------------------------
static const float MOVE_SPEED = 0.1f;	// 移動速度
static const float ROT_SPEED = 0.03f;	// 回転速度
static const float JUMP_POWER = 5.0f;	// ジャンプ力
static const float GRAVITY = 0.01f;		// 重力
static const float RADIUS = 2.5f;		// 当たり判定半径
static const float MAXTIME = 10.0f;		// クールタイム
static const float ANIME_SPEED = 1.0f;	// アニメスピード
static const float MOVE_HIGHT = 0.0f;	// 動く高さ

static const char HUMAN_MODEL_PATH[] = { "Data/Character/ScondBoss/enemy.mv1" };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CEnemy::CEnemy()
{
	
}


//-------------------------------
//		デストラクタ
//-------------------------------
CEnemy::~CEnemy()
{
	// 本来は必要ないけど、念のため
	Fin();
	DetachAnim(m_iModelHdl);
}


//-------------------------------
//		初期化
//-------------------------------
void CEnemy::Init()
{
	m_isActive = true;
	m_isMoving = false;
	m_targetPos = m_vPosition;
	m_radius = 2.5f;
	m_moveX = 0;
	m_moveZ = 0;
	m_coolTime = MAXTIME;

    m_state = ENEMY_STATE_NORMAL;
    direction = ROTATION_RIGHT;
}


//-------------------------------
//		データロード
//-------------------------------
void CEnemy::Load()
{
	VECTOR size = VGet(0.015f, 0.015f, 0.015f);
	int hndl = MV1LoadModel(HUMAN_MODEL_PATH);

	MV1SetScale(hndl, size);
	CObject::Load(hndl);

	RequestLoop(ENEMY_STATE_RUN, ANIME_SPEED, m_iModelHdl);
	m_state = ENEMY_STATE_RUN;
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CEnemy::Step()
{
	if (!m_isActive)return;

	// 状態に合わせて行動変化
	switch (m_state)
	{
	case ENEMY_STATE_NORMAL:
		break;
	case ENEMY_STATE_RUN:
		break;
	case ENEMY_STATE_HIT:
		break;
	}

	Direction();
	//NormalExec();
	Move();
}


//-------------------------------
//		描画処理
//-------------------------------
void CEnemy::Draw()
{
	if (!m_isActive)return;
	CObject::Draw();

	//DrawFormatString(100, 500, RED, "人間Y角度：%f", m_vRotation.y);

#ifdef MY_DEBUG
	DrawSphere3D(m_vPos, RADIUS, 16, GetColor(0, 0, 255), GetColor(0, 0, 0), FALSE);
#endif
}


//-------------------------------
//		移動計算結果を反映
//-------------------------------
void CEnemy::Move()
{
	//	重力処理
	m_speed.y -= GRAVITY;
	// 移動速度加算
	m_vPosition = VAdd(m_vPosition, m_speed);

}


void CEnemy::NormalExec(const std::vector<CBlock*>& blocks, std::vector<CInstalledItem*>& institem, float cat_state)
{
	if (m_vPosition.y >= MOVE_HIGHT)
	{
		//---------------------------------
		 // 移動中
		 //---------------------------------
		if (m_isMoving)
		{
			float addspeed = 0.5f;

			VECTOR dir = VSub(m_targetPos, m_vPosition);


			if (Input::Controller::Keep(XINPUT_BUTTON_RIGHT_SHOULDER))//早送りしたいのはenemyとhumanのみなので直に書いています
			{
				addspeed = 0.8f;
				m_coolTime = 0.0f;
			}

			// Y無視
			dir.y = 0.0f;

			float dist = VSize(dir);

			// 到着
			if (dist < MOVE_SPEED + addspeed)
			{
				m_vPosition = m_targetPos;
				m_isMoving = false;
			}
			else
			{
				// 正規化
				dir = VNorm(dir);
				// 少しずつ移動
				dir = VScale(dir, MOVE_SPEED + addspeed);
				m_vPosition = VAdd(m_vPosition, dir);
			}

			return;
		}

		//---------------------------------
		// クールタイム
		//---------------------------------
		m_coolTime--;

		//---------------------------------
		// 次の移動開始
		//---------------------------------
		if (m_coolTime <= 0)
		{
			m_coolTime = MAXTIME;

			//---------------------------------
			// 現在マス取得
			//---------------------------------
			int mapX = (int)floor(m_vPosition.x / TILE_SIZE);
			int mapZ = (int)floor(m_vPosition.z / TILE_SIZE);

			//---------------------------------
			// 向いている方向
			//---------------------------------
			int dirX = 0;
			int dirZ = 0;

			switch (direction)
			{
			case ROTATION_RIGHT:
				dirX = 1;
				dirZ = 0;
				break;

			case ROTATION_LEFT:
				dirX = -1;
				dirZ = 0;
				break;

			case ROTATION_UP:
				dirX = 0;
				dirZ = 1;
				break;

			case ROTATION_DOWN:
				dirX = 0;
				dirZ = -1;
				break;
			}

			//---------------------------------
			// 次マス
			//---------------------------------
			int nextX = mapX + dirX;
			int nextZ = mapZ + dirZ;

			//---------------------------------
			// ブロックチェック
			//---------------------------------
			bool hitBlock = false;

			for (auto block : blocks)
			{
				if (block == nullptr)continue;
				int blockX = (int)floor(block->GetPos().x / TILE_SIZE);
				int blockZ = (int)floor(block->GetPos().z / TILE_SIZE);

				//---------------------------------
				// 次マスにある
				//---------------------------------
				if (blockX == nextX && blockZ == nextZ)
				{
					hitBlock = true;break;
				}
			}

			if (cat_state != CARRY)
			{
				for (auto block : institem)
				{
					if (block == nullptr)continue;
					int blockX = (int)floor(block->GetPos().x / TILE_SIZE);
					int blockZ = (int)floor(block->GetPos().z / TILE_SIZE);

					//---------------------------------
					// 次マスにある
					//---------------------------------
					if (blockX == nextX && blockZ == nextZ)
					{
						hitBlock = true;break;
					}
				}
			}


			//---------------------------------
			// ブロックに当たる
			//---------------------------------
			if (hitBlock)
			{
				switch (direction)
				{
				case ROTATION_RIGHT:
					direction = ROTATION_DOWN;
					break;

				case ROTATION_DOWN:
					direction = ROTATION_LEFT;
					break;

				case ROTATION_LEFT:
					direction = ROTATION_UP;
					break;

				case ROTATION_UP:
					direction = ROTATION_RIGHT;
					break;
				}

				return;
			}
			//---------------------------------
			// 中心位置
			//---------------------------------
			float worldX = (nextX + 0.5f) * TILE_SIZE;
			float worldZ = (nextZ + 0.5f) * TILE_SIZE;
			//---------------------------------
			// 目標地点
			//---------------------------------
			m_targetPos = VGet(worldX, m_vPosition.y, worldZ);
			//---------------------------------
			// 移動開始
			//---------------------------------
			m_isMoving = true;
		}
	}
}


//----------------------------
//		移動角度処理
//----------------------------
void CEnemy::Direction()
{
	switch (direction)
	{
	case ROTATION_RIGHT:	//右を向いている
		m_vRotation.y = -DX_PI_F / 2;
		break;

	case ROTATION_DOWN:		//下を向いている
		m_vRotation.y = DX_PI_F;
		break;

	case ROTATION_LEFT:		//左を向いている
		m_vRotation.y = DX_PI_F / 2;
		break;

	case ROTATION_UP:		//上を向いている
		m_vRotation.y = 0;
		break;
	}
}


void CEnemy::SetDirect(int dir)
{
	switch (dir)
	{
	case 0:
		direction = ROTATION_RIGHT;
		break;
	case 1:
		direction = ROTATION_DOWN;
		break;
	case 2:
		direction = ROTATION_LEFT;
		break;
	case 3:
		direction = ROTATION_DOWN;
		break;
	default:
		break;
	}
}