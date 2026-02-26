#include "CollisionManager.h"
//#include "../object/enemy/EnemyManager.h"
//#include "../object/shot/ShotManager.h"
//#include "../object/player/Player.h"
#include "../../lib/math/hit.h"

using namespace std;

////---------------------------------
////		“G‚Æ’e‚Ì“–‚½‚è”»’è
////---------------------------------
//int	CCollisionManager::CheckHitShotToEnemy(CEnemyManager &enemyManager,
//												CShotManager &shotManager)
//{
//	int ret = 0;
//	list<CActor*> enemyList = enemyManager.GetEnemyList();
//	list<CActor*> shotList = shotManager.GetPlayerShotList();
//
//	for (CActor* shot : shotList)
//	{
//		// À•W‚Æ”¼Œa‚ðŽæ“¾
//		VECTOR shotPos = shot->GetCenter();
//		float shotRadius = shot->GetRadius();
//
//		for (CActor* enemy : enemyList)
//		{
//			// À•W‚Æ”¼Œa‚ðŽæ“¾
//			VECTOR enemyPos = enemy->GetCenter();
//			float enemyRadius = enemy->GetRadius();
//
//			// ‹…‚Æ‹…‚Ì“–‚½‚è”»’è
//			if(CHit::CheckSphereToSphere(shotPos, enemyPos, shotRadius, enemyRadius))
//			{
//				// ‚±‚±‚Ü‚Å‚­‚ê‚Î“–‚½‚Á‚½II
//				shot->HitCalc();
//				enemy->HitCalc();
//				ret++;
//			}
//		}
//	}
//
//	return ret;
//}
//
//
////---------------------------------
////		“G‚Æ’e‚Ì“–‚½‚è”»’è
////---------------------------------
//void	CCollisionManager::CheckHitPlayerToEnemy(CPlayer& player,
//	CEnemyManager& enemyManager)
//{
//	list<CActor*> enemyList = enemyManager.GetEnemyList();
//
//	VECTOR playerPos = player.GetCenter();
//	float playerRadius = player.GetRadius();
//
//	for (CActor* enemy : enemyList)
//	{
//		// À•W‚Æ”¼Œa‚ðŽæ“¾
//		VECTOR enemyPos = enemy->GetCenter();
//		float enemyRadius = enemy->GetRadius();
//
//		// ‹…‚Æ‹…‚Ì“–‚½‚è”»’è
//		if (CHit::CheckSphereToSphere(playerPos, enemyPos, playerRadius, enemyRadius))
//		{
//			// ‚±‚±‚Ü‚Å‚­‚ê‚Î“–‚½‚Á‚½II
//			player.HitCalc();
//			enemy->HitCalc();
//		}
//	}
//}