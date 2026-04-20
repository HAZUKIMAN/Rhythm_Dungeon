#pragma once

class CEnemyManager;
class CShotManager;
class CPlayer;

class CCollisionManager
{
public:
	// “G‚Æ’e‚Ì“–‚½‚è”»’è
	// @return : “|‚µ‚½“G‚Ì”
	static int CheckHitShotToEnemy(CEnemyManager &enemyManager,
									CShotManager &shotManager);
	// “G‚ÆƒvƒŒƒCƒ„[‚Ì“–‚½‚è”»’è
	static void CheckHitPlayerToEnemy(CPlayer& player,
										CEnemyManager& enemyManager);
};

