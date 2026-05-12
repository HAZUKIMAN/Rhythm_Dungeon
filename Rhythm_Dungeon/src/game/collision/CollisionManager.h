#pragma once
#include "../cat/Cat.h"
#include <vector>
#include "DxLib.h"
#include "../map/ObjectEditor.h"
#include "../map/MapEditor.h"
#include "../field/Goal.h"

class CPlayer;
class CObjectEditor;
//--------------------------------------
// オブジェクト種類
//--------------------------------------

class CCollisionManager
{
private:

	enum ObjectType
	{
		OBJ_PLAYER,
		OBJ_ENEMY,
		OBJ_ITEM,
		OBJ_BLOCK
	};

public:

	static int m_time;

	CCollisionManager();

	// ブロックとプレイヤーの当たり判定
	static void CheckHitPlayerToBlock(CPlayer& player,
		CInstalledItem& inst);

	// ゴールとプレイヤーの当たり判定
	static bool CheckHitPlayerToGoal(CPlayer& player,
		CGoal& inst);

	
	static VECTOR HitPlayerToObject(VECTOR center,
		float radius,
		ObjectEditor&object,
		CPlayer& player
	);

	//猫とオブジェクトの計算
	static VECTOR HitCatToObject(VECTOR center,
		float radius,
		ObjectEditor& object
	);

	//プレイヤーの座標をマップ座標にして計算を行う
	static VECTOR HitMap(
		VECTOR center,
		float radius,
		MapEditor& map
	);

};

