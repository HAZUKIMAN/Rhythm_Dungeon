#pragma once
#include "SceneBase.h"
#include "../camera/CameraManager.h"
#include "../human/Human.h"
#include "../field/BackGroundManager.h"
#include "../map/MapEditor.h"
#include "../object/Object.h"
#include "../cat/Cat.h"
#include "../field/InstalledItem.h"
#include "../field/Goal.h"
#include "../field/Block.h"
#include "../enemy/Enemy.h"

//タイトルクラス
//	@memo	:	継承後も基本はpublicは増やさない
class CSceneGame : public CSceneBase{
private:

	CHuman m_human;							// 人間処理
	CCameraManager m_cameraManager;			// カメラマネージャ
	CCat m_cat;								//猫（操作キャラ）
	std::vector < CInstalledItem*> m_institem;//運べる用のオブジェクト
	std::vector<CBlock*> m_blocks;			// 設置ブロック
	CBackGroundManager m_backgroundManager;	// 背景
	std::vector<CEnemy*> m_enemy;			//エネミー
	MapEditor m_mapedit;					//マップエディター
	ObjectEditor m_objEditor;				//オブジェクトエディター

	CGoal m_goal;							//ゴール
public:
	CSceneGame();
	~CSceneGame();

	//初期化
	void Init();
	//データロード
	void Load();
	//実行処理
	//	@戻り値	:	次のシーン(-1なら変更なし)
	int  Step();
	//描画
	void Draw();
	//終了処理
	void Fin();

private:
	//ゲーム本体の処理
	void Calc();

	void Reset();

	enum ObjectType {
		OBJ_NONE,
		OBJ_HUMAN,
		OBJ_CAT,
		OBJ_ITEM,
		OBJ_GOAL,
		OBJ_PUT_BOX,
		OBJ_SETBLOCK,
		OBJ_ENEMY,
	};

	enum tagMove
	{
		NONE,
		CARRY,
		PUT,
	};

	enum tagDirection
	{
		ROTATION_RIGHT,
		ROTATION_DOWN,
		ROTATION_LEFT,
		ROTATION_UP,
	};

	tagMove move_box;

};




