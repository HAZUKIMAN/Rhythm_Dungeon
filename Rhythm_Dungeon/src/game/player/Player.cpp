#include "Player.h"
#include <math.h>

#include "../../Data.h"
#include "../../lib/Input/Input.h"
#include "../Anime/Anime.h"
#include "../common.h"

//	定義関連------------------------------
static const float MOVE_SPEED = 0.1f;		// 移動速度
static const float ROT_SPEED = 0.03f;		// 回転速度
static const float JUMP_POWER = 5.0f;		// ジャンプ力
static const float GRAVITY = 0.01f;			// 重力
static const float RADIUS = 5.0f;			// 当たり判定半径

static const char PLAYER_MODEL_PATH[] = { "Data/Character/Player/Player.mv1" };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CPlayer::CPlayer()
{
	m_state = PLAYER_STATE_NORMAL;
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
	// 自動移動
	//---------------------------------
	float speed = MOVE_SPEED;

	VECTOR move;

	// 向いている方向へ前進
	move.x = -sinf(m_vRotation.y) * speed;
	move.z = -cosf(m_vRotation.y) * speed;
	move.y = 0.0f;

	// 移動
	m_vPosition = VAdd(m_vPosition, move);
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
