#include "Human.h"
#include <math.h>
#include "../../Data.h"
#include "../../lib/Input/Input.h"
#include "../Anime/Anime.h"
#include "../common.h"
#include "../../lib/sound/effectData/effectData.h"
#include "../effect/effekseer.h"

//	定義関連------------------------------
static const float MOVE_SPEED	=  0.09f;	// 移動速度
static const float ROT_SPEED	= 0.03f;	// 回転速度
static const float GRAVITY		=0.02f;		// 重力
static const float RADIUS		=  2.5f;	// 当たり判定半径
static const float MAXTIME		=  5.0f;	// クールタイム
static const float ANIME_SPEED	=  1.0f;	// アニメスピード
static const float MOVE_HIGHT	=  10.0f;	// 動かす高さ
static const float MIN_HIGHT	= -20.0f;	// リスポーン位置に戻す

static const char HUMAN_MODEL_PATH[] = { "Data/Character/player/player.mv1" };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CHuman::CHuman()
{
}


//-------------------------------
//		デストラクタ
//-------------------------------
CHuman::~CHuman()
{
	// 本来は必要ないけど、念のため
	Fin();
	DetachAnim(m_iModelHdl);
}


//-------------------------------
//		初期化
//-------------------------------
void CHuman::Init()
{
	m_radius = RADIUS;
	m_isActive = true;
	m_isMoving = false;
	m_targetPos = m_vPosition;
	m_moveX = 0;
	m_moveZ = 0;
	m_coolTime = MAXTIME;

	memset(&m_recpos, 0, sizeof(VECTOR));

	//---------------------------------
	// 初期向き
	//---------------------------------
	direction = ROTATION_UP; // ←好きな向き
	m_vRotation.y = DX_PI_F;
}


//-------------------------------
//		データロード
//-------------------------------
void CHuman::Load()
{
	VECTOR size = VGet(0.017f, 0.017f, 0.017f);
	int hndl= MV1LoadModel(HUMAN_MODEL_PATH);

	MV1SetScale(hndl, size);
	CObject::Load(hndl);

	RequestLoop(HUMAN_STATE_RUN, ANIME_SPEED,m_iModelHdl);
	m_state = HUMAN_STATE_RUN;
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CHuman::Step()
{

	if (!m_isActive)
	{
		VECTOR pos = VGet(m_recpos.x, m_recpos.y, m_recpos.z);

		m_vPosition = pos;
		Reset();

		//拡大率
		VECTOR effect_scale = VGet(0.5f, 0.5f, 0.5f);
		//呼び出すエフェクトのID
		int effectId = CEffectData::GetId(EFFECT_HUMAN_RESET);
		//コインの位置にエフェクトを呼び出す
		CEffekseerCtrl::Request(effectId, VGet(pos.x,pos.y - 5.0f,pos.z), false);
		//エフェクトの拡大・縮小
		CEffekseerCtrl::SetScale(effectId, effect_scale);

		m_isActive = true;
	}

	if (!m_isActive)return;
	
	// 状態に合わせて行動変化
	switch (m_state)
	{
	case HUMAN_STATE_NORMAL:
		break;
	case HUMAN_STATE_RUN:
		break;
	case HUMAN_STATE_HIT:
		break;
	}

	Direction();
	Move();

	if (m_isActive == false)
	{
		m_vPosition = VGet(m_recpos.x, m_recpos.y, m_recpos.z);
		Reset();
		m_isActive = true;
	}


	if (m_vPosition.y <= MIN_HIGHT)
	{
		m_isActive = false;
	}

}


//-------------------------------
//		描画処理
//-------------------------------
void CHuman::Draw()
{
	if (!m_isActive)return;
	CObject::Draw();

#ifdef MY_DEBUG
	DrawSphere3D(m_vPos, RADIUS, 16, GetColor(0, 0, 255), GetColor(0, 0, 0), FALSE);
#endif
}


//-------------------------------
//		移動計算結果を反映
//-------------------------------
void CHuman::Move()
{
	//	重力処理
	m_speed.y -= GRAVITY;
	// 移動速度加算
	m_vPosition = VAdd(m_vPosition, m_speed);

}


//---------------------------------
//		待機･移動中処理
//---------------------------------
void CHuman::NormalExec(std::vector<CBlock*>& blocks, std::vector<CInstalledItem*> institem, float cat_state)
{
	if (m_vPosition.y <= MOVE_HIGHT)
	{
		//---------------------------------
		// 移動中
		//---------------------------------
		if (m_isMoving)
		{
			float addspeed=0.0f;
			VECTOR dir = VSub(m_targetPos, m_vPosition);

			if (Input::Controller::Keep(XINPUT_BUTTON_RIGHT_SHOULDER))//早送りしたいのはenemyとhumanのみなので直に書いています
			{
				addspeed = 0.2f;
				m_coolTime = 0.0f;
			}

			// Y無視
			dir.y = 0.0f;
			float dist = VSize(dir);

			// 到着
			if (dist < MOVE_SPEED+ addspeed)
			{
				m_vPosition = m_targetPos;
				m_isMoving = false;
			}
			else
			{
				// 正規化
				dir = VNorm(dir);
				// 少しずつ移動
				dir = VScale(dir, MOVE_SPEED+ addspeed);
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
			m_targetPos = VGet(worldX, 0.0f, worldZ);
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
void CHuman::Direction()
{
	switch (direction)
	{
	case ROTATION_RIGHT:	//右を向いている
		m_vRotation.y = -DX_PI_F / 2;
		break;

	case ROTATION_DOWN:		//下を向いている
		m_vRotation.y = 0;
		break;

	case ROTATION_LEFT:		//左を向いている
		m_vRotation.y = DX_PI_F / 2;
		break;

	case ROTATION_UP:		//上を向いている
		m_vRotation.y = DX_PI_F;
		break;
	}
}


void CHuman::SetDirect(int dir)
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
		direction = ROTATION_UP; // ←修正
		break;
	}
}


void CHuman::Reset()
{
	
	// 0～360に変換
	float rotDeg = m_setrot * 180.0f / DX_PI_F;

	// マイナス対策
	while (rotDeg < 0)
	{
		rotDeg += 360.0f;
	}

	rotDeg = fmod(rotDeg, 360.0f);

	// 方向判定
	if (rotDeg >= 315 || rotDeg < 45)
	{
		direction = ROTATION_DOWN;
	}
	else if (rotDeg >= 45 && rotDeg < 135)
	{
		direction = ROTATION_LEFT;
	}
	else if (rotDeg >= 135 && rotDeg < 225)
	{
		direction = ROTATION_UP;
	}
	else
	{
		direction = ROTATION_RIGHT;
	}

	m_targetPos = m_vPosition;
}

// 当たり判定後の処理
void CHuman::HitCalc()
{
	m_isActive = false;

	//拡大率
	VECTOR effect_scale = VGet(1.0f, 1.0f, 1.0f);
	//呼び出すエフェクトのID
	int effectId = CEffectData::GetId(EFFECT_HIT_ENEMY);
	//コインの位置にエフェクトを呼び出す
	CEffekseerCtrl::Request(effectId, m_vPosition, false);
	//エフェクトの拡大・縮小
	CEffekseerCtrl::SetScale(effectId, effect_scale);

}

//クリアしたとき
void CHuman::Clear()
{
	RequestLoop(HUMAN_STATE_NORMAL, ANIME_SPEED, m_iModelHdl);
	m_state = HUMAN_STATE_NORMAL;
}