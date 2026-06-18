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
static const float GRAVITY		= 0.02f;	// 重力
static const float RADIUS		=  2.5f;	// 当たり判定半径
static const int   MAXTIME		=  5.0f;	// クールタイム
static const float ANIME_SPEED	=  1.0f;	// アニメスピード
static const float MOVE_HIGHT	=  30.0f;	// 動かす高さ
static const float MIN_HIGHT	= -20.0f;	// リスポーン位置に戻す
//----------------------------------------
//using namespace std;

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
	DetachAnim(m_iModelHdl);
	// 本来は必要ないけど、念のため
	Fin();
}


//-------------------------------
//		初期化
//-------------------------------
void CHuman::Init()
{
	m_isActive = true;
	m_isMoving = false;
	m_isStairs = false;
	m_stairTargetY = 0.0f;
	m_targetPos = m_vPosition;
	m_coolTime = MAXTIME;
	CActor::Init();

	m_radius = RADIUS;

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
void CHuman::Load(int hndl)
{
	VECTOR model_size = VGet(0.02f, 0.02f, 0.02f);

	m_iModelHdl = hndl;
	CObject::Load(m_iModelHdl, model_size);

	RequestLoop(HUMAN_STATE_RUN, ANIME_SPEED, m_iModelHdl);
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
	CActor::Draw();
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
void CHuman::NormalExec(vector<CBlock*>& blocks, vector<CInstalledItem*>& institem, MapEditor& map, float cat_state)
{
	if (m_vPosition.y <= MOVE_HIGHT)
	{
		//---------------------------------
		// 移動中
		//---------------------------------
		if (m_isMoving)
		{
			float addspeed = 0.0f;

			if (Input::Controller::Keep(
				XINPUT_BUTTON_RIGHT_SHOULDER))
			{
				addspeed = 0.2f;
				m_coolTime = 0.0f;
			}

			//---------------------------------
			// 目標方向
			//---------------------------------
			VECTOR dir =VSub(m_targetPos,m_vPosition);

			//---------------------------------
			// 階段じゃないなら
			// Y移動しない
			//---------------------------------
			if (!m_isStairs)
			{
				dir.y = 0.0f;
			}

			//---------------------------------
			// 距離(XZだけ)
			//---------------------------------
			float dist =sqrtf(dir.x * dir.x +dir.z * dir.z);

			//---------------------------------
			// 階段中はYをゆっくり補間
			//---------------------------------
			if (m_isStairs)
			{
				float diffY = m_stairTargetY - m_vPosition.y;

				// 少しずつ近づける（ゆっくり）
				m_vPosition.y += diffY * 0.08f;

				// 微妙なガタ防止
				if (fabs(diffY) < 0.05f)
				{
					m_vPosition.y = m_stairTargetY;
				}
			}

			//---------------------------------
			// 到着
			//---------------------------------
			if (dist <MOVE_SPEED +addspeed)
			{
				m_vPosition.x =m_targetPos.x;
				m_vPosition.z =m_targetPos.z;

				//---------------------------------
				// 階段ならYも合わせる
				//---------------------------------
				if (m_isStairs)
				{
					m_vPosition.y = m_stairTargetY;
				}

				m_isMoving = false;
				m_isStairs = false;
			}
			else
			{
				//---------------------------------
				// 正規化
				//---------------------------------
				dir = VNorm(dir);

				//---------------------------------
				// 移動
				//---------------------------------
				dir =VScale(dir,MOVE_SPEED +addspeed);

				m_vPosition =VAdd(m_vPosition,dir);
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
			// 移動先の高さ取得
			//---------------------------------
			int targetY = 0;

			// 上から下へ探す
			for (int y = MAP_Y - 1; y >= 0;y--)
			{
				int tile = map.GetMap(y, nextZ, nextX);

				//---------------------------------
				// 床がある
				//---------------------------------
				if (tile == TILE_FLOOR ||
					tile == TILE_FLOOR2||
					tile == TILE_BRIDGE||
					tile == TILE_STAIRS )
				{
					targetY = y + 1;
					break;
				}
			}

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
					hitBlock = true;
					break;
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
			// 階段判定
			//---------------------------------

			bool isStairs = false;

			// 今の高さを基準
			float stairs_targetY = m_vPosition.y;

			// 今いる高さ
			int currentY = (int)floor(m_vPosition.y / TILE_SIZE);

			// 目の前のタイル
			int frontTile = map.GetMap(currentY+1, nextZ, nextX);

			// 階段なら上方向
			if (frontTile == TILE_STAIRS)
			{
				isStairs = true;

				//---------------------------------
				// 1段上へ
				//---------------------------------
				stairs_targetY =m_vPosition.y + TILE_SIZE;
			}

			// ワールド座標
			float worldX =(nextX + 0.5f) * TILE_SIZE;
			float worldZ =(nextZ + 0.5f) * TILE_SIZE;

			// 目標位置
			m_targetPos =VGet(worldX,stairs_targetY,worldZ);
			m_stairTargetY =stairs_targetY;
			m_isStairs =isStairs;

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

	m_isMoving = false;
	m_isStairs = false;
	m_coolTime = MAXTIME;
	m_stairTargetY = 0.0f;

	//リセット関連
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