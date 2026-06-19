#pragma once
#include "SceneBase.h"
#include "../camera/CameraManager.h"
#include "../human/Human.h"
#include "../field/BackGroundManager.h"
#include "../map/MapEditor.h"
#include "../../lib/object/Object.h"
#include "../cat/Cat.h"
#include "../field/InstalledItem.h"
#include "../field/Goal.h"
#include "../field/Block.h"
#include "../enemy/Enemy.h"
#include "../bridge/Bridge.h"

using namespace std;

//タイトルクラス
//	@memo	:	継承後も基本はpublicは増やさない
class CSceneGame : public CSceneBase{

private:

	enum STATE_MODEL
	{
		MODEL_HUMAN,
		MODEL_CAT,
		MODEL_ENEMY,
		MODEL_INSTITEM,
		MODEL_BRIDGE,
		MODEL_GOAL,
		MODEL_BLOCKS,

		MODEL_NUM
	};


	VECTOR	m_human_startPos;
	float	m_startDer;
	int		m_iModelHdl[MODEL_NUM];	//モデルハンドル

private:

	CHuman m_human;								// 人間処理
	CCameraManager m_cameraManager;				// カメラマネージャ
	CCat m_cat;									// 猫（操作キャラ）
	vector < CInstalledItem*> m_institem;		// 運べる用のオブジェクト
	vector<CBlock*> m_blocks;					// 設置ブロック
	CBackGroundManager m_backgroundManager;		// 背景
	vector<CEnemy*> m_enemy;					// エネミー
	vector<CBridge*> m_bridge;					// 橋
	MapEditor m_mapedit;						// マップエディター
	ObjectEditor m_objEditor;					// オブジェクトエディター
	CGoal m_goal;								// ゴール
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
	void Set();
	void Reset();
	void CatCrry();
	void CatCrryToBridge();

	//クリアしたかどうか
	enum ClearMenu
	{
		CLEAR_NONE,
		CLEAR_NEXT,
		CLEAR_SELECT
	};

	bool m_isGoal;
	int m_clearSelect;


	enum ObjectType {
		OBJ_NONE,
		OBJ_HUMAN,
		OBJ_CAT,
		OBJ_ITEM,
		OBJ_GOAL,
		OBJ_PUT_BOX,
		OBJ_SETBLOCK,
		OBJ_ENEMY,
		OBJ_BRIDGE,
	};

	enum tagMove
	{
		NONE,
		CARRY,
		PUT,
	};

	enum EditMode
	{
		EDIT_MAP,
		EDIT_OBJECT
	};

	EditMode m_editMode;

	enum tagDirection
	{
		ROTATION_RIGHT,
		ROTATION_DOWN,
		ROTATION_LEFT,
		ROTATION_UP,
	};

	enum CarryState
	{
		PUT_NONE,
		CARRY_ITEM,
		CARRY_BRIDGE
	};

	enum TileType {
		TILE_NONE = 0,
		TILE_FLOOR = 1,
		TILE_STAIRS = 2,
		TILE_FLOOR2 = 3,
		TILE_BRIDGE = 4,
	};

	CarryState m_carryState;


	tagMove move_box;

	CInstalledItem* m_carryItem = nullptr;
	CBridge* m_carryBridge = nullptr;
};




