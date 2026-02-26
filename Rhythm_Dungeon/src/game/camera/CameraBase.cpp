#include "CameraBase.h"

static const VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };

//-------------------------------
//		コンストラクタ
//-------------------------------
CCameraBase::CCameraBase() : m_pos(VECTOR_ZERO), m_focus(VECTOR_ZERO), m_up(VECTOR_ZERO)
{
	// ひとまず初期化をしておく
}


//-------------------------------
//		デストラクタ
//-------------------------------
CCameraBase::~CCameraBase()
{
}


//-------------------------------
//		初期化
//-------------------------------
void CCameraBase::Init(VECTOR pos, VECTOR focus, VECTOR up)
{
	m_pos = pos;
	m_focus = focus;
	m_up = up;
}


//-------------------------------
//		終了処理
//-------------------------------
void CCameraBase::Fin()
{
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CCameraBase::Step(CPlayer& player)
{
}


//-------------------------------
//		更新処理
//-------------------------------
void CCameraBase::Update()
{
	// DxLibの方にカメラを更新
	SetCameraPositionAndTargetAndUpVec(m_pos, m_focus, m_up);
}


//-------------------------------
//		描画
//-------------------------------
void CCameraBase::Draw()
{
}
