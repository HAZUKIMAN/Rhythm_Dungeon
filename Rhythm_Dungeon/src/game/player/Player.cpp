#include "Player.h"
#include <math.h>

#include "../../Data.h"
#include "../../lib/Input/Input.h"

//	定義関連------------------------------
static const float MOVE_SPEED = 1.0f;		// 移動速度
static const float ROT_SPEED = 0.03f;		// 回転速度
static const float JUMP_POWER = 5.0f;		// ジャンプ力
static const float GRAVITY = 0.1f;			// 重力
static const float RADIUS = 5.0f;			// 当たり判定半径

static const char PLAYER_MODEL_PATH[] = { "Data/AddData/Chara/Player1.mv1" };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CPlayer::CPlayer()
{
	m_state = PLAYER_STATE_NORMAL;
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
	m_vRotation.y = DX_PI_F;
	m_radius = RADIUS;
}


//-------------------------------
//		データロード
//-------------------------------
void CPlayer::Load()
{
	int hndl= MV1LoadModel(PLAYER_MODEL_PATH);
	CObject::Load(hndl);
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CPlayer::Step()
{
	Data::GetInstance()->AddScore(1);
	// 状態に合わせて行動変化
	switch (m_state)
	{
	case PLAYER_STATE_NORMAL:
		NormalExec();
		break;
	case PLAYER_STATE_JUMP:
		JumpExec();
		break;
	}

	// 以下共通処理-----------------------------
	// Zキーで弾を発射
	if (Input::Key::Push(KEY_INPUT_Z))
	{
		// プレイヤーの体から出るように座標を上げる
		VECTOR pos = m_vPosition;
		pos.y += 5.0f;
		// 速度はプレイヤーと同じ方法で移動方向を決める
		VECTOR spd;
		const float SHOT_SPEED = 5.0f;
		spd.x = sinf(m_vRotation.y) * -SHOT_SPEED;
		spd.z = cosf(m_vRotation.y) * -SHOT_SPEED;
		spd.y = 0.0f;
	}

	Move();
}


//-------------------------------
//		描画処理
//-------------------------------
void CPlayer::Draw()
{
	CObject::Draw();
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
	if (m_vPosition.y < 0.0f)
	{
		m_vPosition.y = 0.0f;
		m_speed.y = 0.0f;
		m_state = PLAYER_STATE_NORMAL;
	}
}


//-------------------------------
//		待機･移動中処理
//-------------------------------
void CPlayer::NormalExec()
{
	float speed = 0.0f;
	if (Input::Key::Push(KEY_INPUT_UP))
	{
		speed = MOVE_SPEED;
	}
	if (Input::Key::Push(KEY_INPUT_DOWN))
	{
		speed = -MOVE_SPEED;
	}
	if (Input::Key::Push(KEY_INPUT_RIGHT))
	{
		m_vRotation.y += ROT_SPEED;
	}
	if (Input::Key::Push(KEY_INPUT_LEFT))
	{
		m_vRotation.y -= ROT_SPEED;
	}

	// 移動したようであれば、移動用計算を行う
	if (speed != 0.0f)
	{
		m_speed.x = sinf(m_vRotation.y) * -speed;
		m_speed.z = cosf(m_vRotation.y) * -speed;
	}
	else
	{
		m_speed.x = m_speed.z = 0.0f;
	}

	// ジャンプ処理
	if (Input::Key::Push(KEY_INPUT_SPACE))
	{
		m_speed.y = JUMP_POWER;
		m_state = PLAYER_STATE_JUMP;
	}
}


//-------------------------------
//		ジャンプ中処理
//-------------------------------
void CPlayer::JumpExec()
{
	float speed = 0.0f;
	if (Input::Key::Push(KEY_INPUT_UP))
	{
		speed = MOVE_SPEED;
	}
	if (Input::Key::Push(KEY_INPUT_DOWN))
	{
		speed = -MOVE_SPEED;
	}
	if (Input::Key::Push(KEY_INPUT_RIGHT))
	{
		m_vRotation.y += ROT_SPEED;
	}
	if (Input::Key::Push(KEY_INPUT_LEFT))
	{
		m_vRotation.y -= ROT_SPEED;
	}

	// 移動したようであれば、移動用計算を行う
	if (speed != 0.0f)
	{
		m_speed.x = sinf(m_vRotation.y) * -speed;
		m_speed.z = cosf(m_vRotation.y) * -speed;
	}
	else
	{
		m_speed.x = m_speed.z = 0.0f;
	}
}