#include "PlayCamera.h"
#include "../player/Player.h"
#include <math.h>

//	定義関連------------------------------
static const float CAMERA_LENGTH = 70.0f;		// 注視点から視点までの距離
static const float CAMERA_OFFSET_Y = 30.0f;		// 視点の高さ
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CPlayCamera::CPlayCamera()
{
}


//-------------------------------
//		デストラクタ
//-------------------------------
CPlayCamera::~CPlayCamera()
{
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CPlayCamera::Step(CPlayer& player)
{
	// プレイヤーが原点にいて、Y軸回転が0度である事を前提考える
	float rot = player.GetRot().y;
	VECTOR focus = player.GetPos();
	// プレイヤーの回転角度にあわせて上記方向ベクトルを回転させる
	VECTOR dir;
	dir.x = sinf(rot) * CAMERA_LENGTH;
	dir.z = cosf(rot) * CAMERA_LENGTH;
	// 視点の高さは固定
	dir.y = CAMERA_OFFSET_Y;

	//ためし
	//focus = {0, 0, 0};

	// 注視点(プレイヤー)の位置から計算結果の距離を移動させれば
	// カメラの視点になる
	m_pos = VAdd(focus, dir);

	// 注視点はそのままカメラの位置
	m_focus = focus;
	m_focus.y += 30.0f;
}

