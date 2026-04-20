#include "CameraEditor.h"
#include "../../lib/Input/Input.h"

//	定義関連------------------------------
static const float CAMERA_LENGTH = 60.0f;		// 注視点から視点までの距離
static const float CAMERA_OFFSET_Y = 30.0f;		// 視点の高さ
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
	VECTOR camPos = VGet(0, 10, -20);
	float angleY = 0;

	if (Input::Key::Keep(KEY_INPUT_W))
		m_pos.z -= 0.5f;
	if (Input::Key::Keep(KEY_INPUT_S))
		m_pos.z += 0.5f;
	if (Input::Key::Keep(KEY_INPUT_D))
		m_pos.x -= 0.5f;
	if (Input::Key::Keep(KEY_INPUT_A))
		m_pos.x += 0.5f;
}