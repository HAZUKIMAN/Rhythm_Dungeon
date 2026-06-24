#include "PlayCamera.h"
#include "../human/Human.h"
#include <math.h>
#include "../../lib/Input/Input.h"

//-----------定義関連-----------------------
static const float CAMERA_LENGTH = 40.0f;		// 注視点から視点までの距離
static const float CAMERA_OFFSET_Y = 30.0f;		// 視点の高さ
//------------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CPlayCamera::CPlayCamera()
{
	m_cameraRotY = -DX_PI_F / 4.0f;
	m_cameraRotX = 3.0f;

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
void CPlayCamera::Step(CCat& cat)
{
 
	//---------------------------------
	// プレイヤー位置
	//---------------------------------
	VECTOR focus = cat.GetPos();

	//---------------------------------
	// 少し上を見る
	//---------------------------------
	focus.y += 5.0f;

	//---------------------------------
	// 右スティックカメラ
	//---------------------------------
	ControlCan(focus);

}


void CPlayCamera::ControlCan(VECTOR targetPos)
{
	//---------------------------------
	// 右スティック取得
	//---------------------------------
	float stickX =0.0f;
	float stickY =0.0f;

	Input::Controller::RStickIncline(stickX,stickY);

	//---------------------------------
	// 左右回転のみ
	//---------------------------------
	float rotSpeed = 0.5f;

	m_cameraRotY = -DX_PI_F / 4.0f;		//そこまでカメラを動かしたくないので少しだけ
	m_cameraRotY += stickX * rotSpeed;

	//---------------------------------
	// 円状位置
	//---------------------------------
	float x = sinf(m_cameraRotY);
	float z = cosf(m_cameraRotY);

	//---------------------------------
	// カメラ位置
	//---------------------------------
	m_pos = VGet(
			targetPos.x - x* CAMERA_LENGTH,
			targetPos.y + CAMERA_OFFSET_Y, // ←固定高さ
			targetPos.z - z* CAMERA_LENGTH);

	//---------------------------------
	// 注視点
	//---------------------------------
	m_focus = targetPos;

	m_focus.y += 5.0f;

	//---------------------------------
	// カメラ設定
	//---------------------------------
	SetCameraPositionAndTarget_UpVecY(m_pos,m_focus);
}

