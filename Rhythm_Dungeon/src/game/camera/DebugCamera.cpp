#include "DebugCamera.h"
#include <math.h>
#include "../../lib/Input/Input.h"

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
CDebugCamera::CDebugCamera()
{
}


//-------------------------------
//		デストラクタ
//-------------------------------
CDebugCamera::~CDebugCamera()
{
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CDebugCamera::Step(CPlayer& player)
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
//		描画
//-------------------------------
void CDebugCamera::Draw()
{
	// 注視点に球体を表示
	DrawSphere3D(m_focus, 1.0f, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), FALSE);
	// 文字を表示
	DrawFormatString(16, 32, GetColor(255, 255, 255), "デバッグカメラモード");
	DrawFormatString(16, 48, GetColor(255, 255, 255), "カメラ視点 　X=%.02f,Y=%.02f, Z=%.02f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(16, 64, GetColor(255, 255, 255), "カメラ注視点 X=%.02f,Y=%.02f, Z=%.02f", m_focus.x, m_focus.y, m_focus.z);
}



//-------------------------------
//		注視点の上下移動
//-------------------------------
void CDebugCamera::MoveUpDown()
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
void CDebugCamera::MoveFrontBack()
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
void CDebugCamera::MoveLeftRight()
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
void CDebugCamera::RotateLeftRight()
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
