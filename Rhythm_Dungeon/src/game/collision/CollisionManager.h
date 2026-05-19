#pragma once
#include "../cat/Cat.h"
#include <vector>
#include "DxLib.h"
#include "../map/ObjectEditor.h"
#include "../map/MapEditor.h"
#include "../field/Goal.h"
#include "../field/Block.h"

class CPlayer;
class CObjectEditor;
//--------------------------------------
// オブジェクト種類
//--------------------------------------

class CCollisionManager
{
private:

	enum ObjectType {
		OBJ_NONE,
		OBJ_PLAYER,
		OBJ_ENEMY,
		OBJ_ITEM,
		OBJ_GOAL,
		OBJ_PUT_BOX,
		OBJ_SETBLOCK
	};

	enum TileType {
		TILE_NONE  = 0,
		TILE_FLOOR = 1,
		TILE_WALL  = 2
	};

public:

	static int m_time;

	CCollisionManager();

	// ゴールとプレイヤーの当たり判定
	static bool CheckHitPlayerToGoal(CPlayer& player,
		CGoal& inst);

	// Playerと設置ブロックの当たり判定
	static VECTOR HitPlayerToBlock(CPlayer& player,
		std::vector<CBlock*>& blocks);

	//プレイヤーとオブジェクトの計算
	static VECTOR HitCatToObject(CPlayer& player,
		ObjectEditor& object);

	//プレイヤーの座標をマップ座標にして計算を行う
	static VECTOR HitMap( VECTOR center, float radius, MapEditor& map);

};

