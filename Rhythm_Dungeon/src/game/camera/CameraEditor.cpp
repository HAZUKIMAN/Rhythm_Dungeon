#include "CameraEditor.h"
#include "../../lib/Input/Input.h"
#include <corecrt_math.h>

//	定義関連------------------------------
static const float CAMERA_LENGTH = 60.0f;		// 注視点から視点までの距離
static const float CAMERA_OFFSET_Y = 30.0f;		// 視点の高さ
//----------------------------------------
//	定義関連------------------------------
#define DEG_TO_RAD(a)	(a * DX_PI_F / 180.0f)
static const float FOCUS_Y_MAX = 40.0f;			// 注視点の上移動限界
static const float FOCUS_Y_MIN = 0.0f;			// 注視点の下移動限界
static const float FOCUS_Y_SPEED = 1.0f;		// 注視点の上下移動速度
static const float CAMERA_SPEED = 1.0f;			// カメラの前後左右移動速度
//----------------------------------------

//-------------------------------
//		コンストラクタ
//-------------------------------
CEditorCamera::CEditorCamera()
{
	m_pos = VGet(0, 10, -20);
}


//-------------------------------
//		デストラクタ
//-------------------------------
CEditorCamera::~CEditorCamera()
{
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CEditorCamera::Step(CPlayer& player)
{
	// カメラ上下移動
	MoveUpDown();
	// カメラ前後左右移動
	MoveFrontBack();
	MoveLeftRight();
	// カメラの回転
	RotateLeftRight();
}

//-------------------------------
//		注視点の上下移動
//-------------------------------
void CEditorCamera::MoveUpDown()
{
	if (Input::Key::Push(KEY_INPUT_UP))
	{
		m_focus.y = m_focus.y + FOCUS_Y_SPEED > FOCUS_Y_MAX ? FOCUS_Y_MAX : m_focus.y + FOCUS_Y_SPEED;
	}
	else if (Input::Key::Push(KEY_INPUT_DOWN))
	{
		m_focus.y = m_focus.y - FOCUS_Y_SPEED < FOCUS_Y_MIN ? FOCUS_Y_MIN : m_focus.y - FOCUS_Y_SPEED;
	}
}



//-------------------------------
//		注視点の前後移動
//-------------------------------
void CEditorCamera::MoveFrontBack()
{
	VECTOR dir;
	// 前進
	if (Input::Key::Push(KEY_INPUT_W))
	{
		// 視点・注視点から方向ベクトルを取得
		dir = VSub(m_focus, m_pos);
		dir.y = 0.0f;
		// 一度正規化して、移動速度に合わせる
		dir = VNorm(dir);
		dir = VScale(dir, CAMERA_SPEED);
		// 現在の視点･注視点に加算
		m_pos = VAdd(m_pos, dir);
		m_focus = VAdd(m_focus, dir);
	}
	else if (Input::Key::Push(KEY_INPUT_S))
	{
		// 視点・注視点から方向ベクトルを取得
		dir = VSub(m_pos, m_focus);
		dir.y = 0.0f;
		// 一度正規化して、移動速度に合わせる
		dir = VNorm(dir);
		dir = VScale(dir, CAMERA_SPEED);
		// 現在の視点･注視点に加算
		m_pos = VAdd(m_pos, dir);
		m_focus = VAdd(m_focus, dir);
	}
}




//-------------------------------
//		注視点の左右移動
//-------------------------------
void CEditorCamera::MoveLeftRight()
{
	VECTOR vec1, vec2, cross;
	// 前進
	if (Input::Key::Push(KEY_INPUT_A))
	{
		// 視点・注視点から2つの方向ベクトルを取得
		vec1 = VSub(m_pos, m_focus);
		vec2 = vec1;
		vec2.y -= 1.0f;
		// 外積を求めて移動速度に合わせる
		cross = VCross(vec1, vec2);
		cross = VNorm(cross);
		cross = VScale(cross, CAMERA_SPEED);
		// 現在の視点･注視点に加算
		m_pos = VAdd(m_pos, cross);
		m_focus = VAdd(m_focus, cross);
	}
	else if (Input::Key::Push(KEY_INPUT_D))
	{
		// 視点・注視点から2つの方向ベクトルを取得
		vec1 = VSub(m_pos, m_focus);
		vec2 = vec1;
		vec2.y -= 1.0f;
		// 外積を求めて移動速度に合わせる
		cross = VCross(vec2, vec1);
		cross = VNorm(cross);
		cross = VScale(cross, CAMERA_SPEED);
		// 現在の視点･注視点に加算
		m_pos = VAdd(m_pos, cross);
		m_focus = VAdd(m_focus, cross);
	}
}




//-------------------------------
//		注視点の左右移動
//-------------------------------
void CEditorCamera::RotateLeftRight()
{
	float fDir = 0.0f;
	// 入力したキーに応じて回転方向を変える
	if (Input::Key::Push(KEY_INPUT_RIGHT))
	{
		fDir = -1.0f;
	}
	else if (Input::Key::Push(KEY_INPUT_LEFT))
	{
		fDir = 1.0f;
	}

	if (fDir != 0.0f)
	{
		// 視点･注視点のベクトルから回転角度を求める
		VECTOR vVec = VSub(m_focus, m_pos);
		vVec.y = 0.0f;
		float fRot = atan2f(vVec.z, vVec.x);

		// 求めた回転角度に+1or-1度回転させる
		fRot += DEG_TO_RAD(1.0f) * fDir;

		// 元々の視点から注視点までの距離を使い、ベクトルを回転
		float fLength = VSize(vVec);
		vVec.x = cosf(fRot) * fLength;
		vVec.z = sinf(fRot) * fLength;
		// 注視点の位置を視点+求めたベクトルにする
		m_focus.x = m_pos.x + vVec.x;
		m_focus.z = m_pos.z + vVec.z;
	}
}