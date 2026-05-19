#include "Player.h"
#include <math.h>

#include "../../Data.h"
#include "../../lib/Input/Input.h"
#include "../Anime/Anime.h"
#include "../common.h"

//	定義関連------------------------------
static const float MOVE_SPEED	= 0.5f;		// 移動速度
static const float ROT_SPEED	= 0.03f;	// 回転速度
static const float JUMP_POWER	= 5.0f;		// ジャンプ力
static const float GRAVITY		= 0.01f;	// 重力
static const float RADIUS		= 2.5f;		// 当たり判定半径
static const float MAXTIME		= 20.0f;	// クールタイム

static const char PLAYER_MODEL_PATH[] = { "Data/Character/Player/Player.mv1" };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CPlayer::CPlayer()
{
	m_state   = PLAYER_STATE_NORMAL;
	direction = ROTATION_RIGHT;
}


//-------------------------------
//		デストラクタ
//-------------------------------
CPlayer::~CPlayer()
{
	// 本来は必要ないけど、念のため
	Fin();
}


//-------------------------------
//		初期化
//-------------------------------
void CPlayer::Init()
{
	m_state = PLAYER_STATE_NORMAL;
	m_vRotation.y = -DX_PI_F / 2;
	m_radius = RADIUS;
	m_isActive = true;
	m_isMoving = false;
	m_targetPos = m_vPosition;
	m_moveX = 0;
	m_moveZ = 0;
	m_coolTime = MAXTIME;
}


//-------------------------------
//		データロード
//-------------------------------
void CPlayer::Load()
{
	VECTOR size = VGet(0.015f, 0.015f, 0.015f);
	int hndl= MV1LoadModel(PLAYER_MODEL_PATH);

	MV1SetScale(hndl, size);
	CObject::Load(hndl);

}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CPlayer::Step()
{
	if (!m_isActive)return;

	// 状態に合わせて行動変化
	switch (m_state)
	{
	case PLAYER_STATE_NORMAL:
		break;
	case PLAYER_STATE_JUMP:
		break;
	}

	Direction();
	NormalExec();
	Move();
}


//-------------------------------
//		描画処理
//-------------------------------
void CPlayer::Draw()
{
	if (!m_isActive)return;
	CObject::Draw();
	DrawFormatString(100, 500, RED, "プレイヤーY角度：%f", m_vRotation.y);

#ifdef MY_DEBUG
	DrawSphere3D(m_vPos, RADIUS, 16, GetColor(0, 0, 255), GetColor(0, 0, 0), FALSE);
#endif
}


//-------------------------------
//		移動計算結果を反映
//-------------------------------
void CPlayer::Move()
{
	//	重力処理
	m_speed.y -= GRAVITY;
	// 移動速度加算
	m_vPosition = VAdd(m_vPosition, m_speed);

}


//-------------------------------
//		待機･移動中処理
//-------------------------------
void CPlayer::NormalExec()
{
	//---------------------------------
	// 移動中
	//---------------------------------
	if (m_isMoving)
	{
		VECTOR dir = VSub(m_targetPos,m_vPosition);
		float dist = VSize(dir);
		//---------------------------------
		// 到着
		//---------------------------------
		if (dist < MOVE_SPEED)
		{
			m_vPosition = m_targetPos;
			m_isMoving =false;
		}
		else
		{
			// 正規化
			dir = VNorm(dir);
			// 少しずつ移動
			dir =VScale(dir,MOVE_SPEED);

			m_vPosition =VAdd(m_vPosition,dir);
		}
		return;
	}

	m_coolTime--;

//---------------------------------
// 移動開始
//---------------------------------
	if (m_coolTime <= 0)
	{
		m_coolTime = MAXTIME;

		// 現在マス取得
		int mapX =(int)floor(m_vPosition.x /TILE_SIZE);
		int mapZ =(int)floor(m_vPosition.z /TILE_SIZE);

		// 向いている方向
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
		// 次マス
		mapX += dirX;
		mapZ += dirZ;

		// 中心位置
		float worldX =(mapX + 0.5f)* TILE_SIZE;
		float worldZ =(mapZ + 0.5f)* TILE_SIZE;

		// 目標地点
		m_targetPos =VGet(worldX,m_vPosition.y,worldZ);

		// 移動開始
		m_isMoving = true;
	}
}


//----------------------------
//		移動角度処理
//----------------------------
void CPlayer::Direction()
{
	switch (direction)
	{
	case ROTATION_RIGHT:	//右を向いている
		m_vRotation.y = -DX_PI_F / 2;
		if (Input::Key::Push(KEY_INPUT_H))direction = ROTATION_DOWN;

		break;
	case ROTATION_DOWN:		//下を向いている

		m_vRotation.y = 0;
		if (Input::Key::Push(KEY_INPUT_H))direction = ROTATION_LEFT;

		break;

	case ROTATION_LEFT:		//左を向いている

		m_vRotation.y = DX_PI_F / 2;

		if (Input::Key::Push(KEY_INPUT_H))direction = ROTATION_UP;

		break;
	case ROTATION_UP:		//上を向いている

		m_vRotation.y = DX_PI_F;
		if (Input::Key::Push(KEY_INPUT_H))direction = ROTATION_RIGHT;

		break;
	}
}


void CPlayer::SetDirect(int dir)
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
