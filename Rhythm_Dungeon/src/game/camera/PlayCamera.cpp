#include "PlayCamera.h"
#include "../human/Human.h"
#include <math.h>
#include "../../lib/Input/Input.h"

//	定義関連------------------------------
static const float CAMERA_LENGTH = 50.0f;		// 注視点から視点までの距離
static const float CAMERA_OFFSET_Y = 20.0f;		// 視点の高さ
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CPlayCamera::CPlayCamera()
{
	m_cameraRotY = 3.0f;
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
    // カメラ回転
    //---------------------------------
    if (Input::Key::Keep(KEY_INPUT_LEFT))
    {
        m_cameraRotY -= 0.03f;
    }

    if (Input::Key::Keep(KEY_INPUT_RIGHT))
    {
        m_cameraRotY += 0.03f;
    }

    //---------------------------------
    // プレイヤー位置
    //---------------------------------
    VECTOR focus = cat.GetPos();

    //---------------------------------
    // 回転したカメラ位置
    //---------------------------------
    VECTOR dir;

    dir.x = sinf(m_cameraRotY) * CAMERA_LENGTH;
    dir.z = cosf(m_cameraRotY) * CAMERA_LENGTH;
    dir.y = CAMERA_OFFSET_Y;

    //---------------------------------
    // カメラ位置
    //---------------------------------
    m_pos = VAdd(focus, dir);

    //---------------------------------
    // 注視点
    //---------------------------------
    m_focus = focus;
    m_focus.y += 5.0f;
}

