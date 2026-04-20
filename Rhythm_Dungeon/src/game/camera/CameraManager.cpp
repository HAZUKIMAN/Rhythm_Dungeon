#include "CameraManager.h"
#include <DxLib.h>
#include "PlayCamera.h"
#include "DebugCamera.h"
#include "CameraEditor.h"

//	定義関連------------------------------
// 基本となる視点・注視点・アップベクトル
static const VECTOR DEFAULT_EYE_POS = { 0.0f, 10.0f, -20.0f };
static const VECTOR DEFAULT_FOCUS_POS = { 0.0f, 0.0f, 0.0f };
static const VECTOR DEFAULT_UP_VEC = { 0.0f, 1.0f, 0.0f };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CCameraManager::CCameraManager() : m_eCurrentCameraID(CAMERA_ID_PLAY)
{
	// ひとまず初期はゲーム中カメラを
	// enumの順番通りにpush
	m_cam.push_back(new CPlayCamera);
	m_cam.push_back(new CDebugCamera);
	m_cam.push_back(new CEditorCamera);
}


//-------------------------------
//		デストラクタ
//-------------------------------
CCameraManager::~CCameraManager()
{
	for (CCameraBase* cam : m_cam) 
	{
		delete cam;
	}
	m_cam.clear();
}


//-------------------------------
//		初期化
//-------------------------------
void CCameraManager::Init()
{
	for (CCameraBase* cam : m_cam)
	{
		cam->Init(DEFAULT_EYE_POS, DEFAULT_FOCUS_POS, DEFAULT_UP_VEC);
	}
}


//-------------------------------
//		終了処理
//-------------------------------
void CCameraManager::Fin()
{
	for (CCameraBase* cam : m_cam)
	{
		cam->Fin();
	}
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CCameraManager::Step(CPlayer& player)
{
	// カメラのタイプに合わせて、更新処理を変更する
	m_cam[m_eCurrentCameraID]->Step(player);
}


//-------------------------------
//		更新処理
//-------------------------------
void CCameraManager::Update()
{
	// カメラのタイプに合わせて、描画処理を変更する
	m_cam[m_eCurrentCameraID]->Update();
}


//-------------------------------
//		描画処理
//-------------------------------
void CCameraManager::Draw()
{
	// カメラのタイプに合わせて、描画処理を変更する
	m_cam[m_eCurrentCameraID]->Draw();
}


//-------------------------------
//		描画処理
//-------------------------------
void CCameraManager::ChangeCamera(tagCAMERA_ID ID)
{
	m_eCurrentCameraID = ID;
	switch (m_eCurrentCameraID)
	{
	case CAMERA_ID_DEBUG:
	{
		VECTOR pos = m_cam[CAMERA_ID_PLAY]->GetPosition();
		VECTOR focus = m_cam[CAMERA_ID_PLAY]->GetFocus();
		VECTOR up = m_cam[CAMERA_ID_PLAY]->GetUpvec();
		m_cam[CAMERA_ID_DEBUG]->SetPosition(pos);
		m_cam[CAMERA_ID_DEBUG]->SetPosition(focus);
		m_cam[CAMERA_ID_DEBUG]->SetPosition(up);
	}
		break;
	}
}



//-------------------------------
//		カメラのニア・ファー設定
//-------------------------------
void CCameraManager::SetNearFar(float fNear, float fFar)
{
	SetCameraNearFar(fNear, fFar);
}

