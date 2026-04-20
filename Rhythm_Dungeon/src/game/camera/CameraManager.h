#pragma once
#include "CameraBase.h"
#include <vector>

// プレイヤー情報取得のため
class CPlayer;

class CCameraManager
{
public:

	// カメラのタイプ
	enum tagCAMERA_ID
	{
		CAMERA_ID_PLAY,		// ゲーム中のカメラ
		CAMERA_ID_DEBUG,	// デバッグ用のカメラ(今後作成)
		CAMERA_ID_EDITOR,	// エディター中のカメラ

		CAMERA_ID_NUM
	};

private:

	std::vector<CCameraBase*> m_cam;		// 各種カメラ
	tagCAMERA_ID m_eCurrentCameraID;	// 現在のカメラID

public:
	// コンストラクタ・デストラクタ
	CCameraManager();
	~CCameraManager();

	// 初期化
	void Init();
	// 終了処理
	void Fin();
	// 更新処理
	void Step(CPlayer& player);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

	//	取得・設定関連===============
	// カメラタイプ変更
	void ChangeCamera(tagCAMERA_ID ID);
	// 現在のカメラタイプ取得
	tagCAMERA_ID GetCameraID(){ return m_eCurrentCameraID; }
	// カメラのニア・ファー設定
	void SetNearFar(float fNear, float fFar);
};