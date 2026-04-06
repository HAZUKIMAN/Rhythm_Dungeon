#pragma once
#include "SceneBase.h"
#include "../camera/CameraManager.h"
#include "../player/Player.h"
//#include "../object/shot/ShotManager.h"
//#include "../object/enemy/EnemyManager.h"
//#include "../object/background/BackgroundManager.h"

//	タイトルクラス
//	@memo	:	継承後も基本はpublicは増やさない
class CSceneGame : public CSceneBase{
private:

	CPlayer m_player;						// プレイヤー処理
	CCameraManager m_cameraManager;			// カメラマネージャ
	//CEnemyManager m_enemyManager;			// 敵全般処理
	//CShotManager m_shotManager;				// ショット管理
	//CBackgroundManager m_backgroundManager;	// 背景
	

public:
	CSceneGame();
	~CSceneGame();

	//		初期化
	void Init();
	//		データロード
	void Load();
	//		実行処理
	//	@戻り値	:	次のシーン(-1なら変更なし)
	int Step();
	//		描画
	void Draw();
	//		終了処理
	void Fin();

private:
	//		ゲーム本体の処理
	void Calc();
};




