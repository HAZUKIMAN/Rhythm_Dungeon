#pragma once
#include "SceneBase.h"
#include "../camera/CameraManager.h"
#include "../player/Player.h"
//#include "../object/shot/ShotManager.h"
#include "../field/BackGroundManager.h"
#include "../map/MapEditor.h"
#include "../object/Object.h"
#include "../cat/Cat.h"
#include "../field/InstalledItem.h"

//タイトルクラス
//	@memo	:	継承後も基本はpublicは増やさない
class CSceneGame : public CSceneBase{
private:

	CPlayer m_player;						// プレイヤー処理
	CCameraManager m_cameraManager;			// カメラマネージャ
	CCat m_cat;								//猫（操作キャラ）
	CInstalledItem m_institem;				//運べる用のオブジェクト
	//CShotManager m_shotManager;			// ショット管理
	CBackGroundManager m_backgroundManager;	// 背景
	MapEditor m_mapeditor;					//マップエディター
	ObjectEditor m_objEditor;

public:
	CSceneGame();
	~CSceneGame();

	//初期化
	void Init();
	//データロード
	void Load();
	//実行処理
	//	@戻り値	:	次のシーン(-1なら変更なし)
	int Step();
	//描画
	void Draw();
	//終了処理
	void Fin();

private:
	//ゲーム本体の処理
	void Calc();

	enum ObjectType {
		OBJ_NONE,
		OBJ_PLAYER,
		OBJ_ENEMY,
		OBJ_ITEM,
		OBJ_GOAL,
	};
};




