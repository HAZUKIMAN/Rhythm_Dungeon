#pragma once
#include "../cat/Cat.h"
#include <vector>
#include "DxLib.h"
#include "../map/ObjectEditor.h"
#include "../map/MapEditor.h"

class CPlayer;
class CObjectEditor;
//--------------------------------------
// オブジェクト種類
//--------------------------------------
enum ObjectType
{
	OBJ_PLAYER,
	OBJ_ENEMY,
	OBJ_ITEM,
	OBJ_BLOCK
};

//--------------------------------------
// オブジェクト情報
//--------------------------------------
struct Object
{
	int x;
	int z;
	int type;
};

class CCollisionManager
{
public:
	//// 敵と弾の当たり判定
	//// @return : 倒した敵の数
	//static int CheckHitShotToEnemy(CEnemyManager &enemyManager,
	//								CShotManager &shotManager);
	// 猫とプレイヤーの当たり判定
	static void CheckHitPlayerToCat(CPlayer& player,
		CCat& cat);

	
	static VECTOR HitObject(VECTOR center,
		float radius,
		ObjectEditor&object
	);


	//プレイヤーの座標をマップ座標にして計算を行う
	static VECTOR HitMap(
		VECTOR center,
		float radius,
		MapEditor& map
	);

};

