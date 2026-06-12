#pragma once
#include "../cat/Cat.h"
#include <vector>
#include "DxLib.h"
#include "../map/ObjectEditor.h"
#include "../map/MapEditor.h"
#include "../field/Goal.h"
#include "../field/Block.h"
#include "../enemy/Enemy.h"

class CHuman;
class CObjectEditor;
//--------------------------------------
// オブジェクト種類
//--------------------------------------

class CCollisionManager
{
private:

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

	enum TileType {
		TILE_NONE = 0,
		TILE_FLOOR = 1,
		TILE_STAIRS = 2,
		TILE_FLOOR2 = 3,
		TILE_BRIDGE = 4,
	};

public:

	static int m_time;

	CCollisionManager();

	// ゴールと人間の当たり判定
	static bool CheckHithumanToGoal(CHuman& human,CGoal& inst);

	//人間とエネミーの当たり判定
	static void CheckHithumanToEnemy(CHuman& human,CEnemy* enemy);

	// エネミーとオブジェクトの当たり判定
	static VECTOR HitEnemyToObject(std::vector<CEnemy*>& enemies, ObjectEditor& object);

	//マップ座標にして計算を行う
	static VECTOR HitMap( VECTOR center, float radius, MapEditor& map);
	//猫の座標をマップ座標にして計算を行う
	static VECTOR HitCatToMap(VECTOR center, float radius, MapEditor& map);

};

