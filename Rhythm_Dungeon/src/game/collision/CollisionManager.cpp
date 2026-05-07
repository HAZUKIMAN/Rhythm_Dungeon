#include "CollisionManager.h"
//#include "../object/enemy/EnemyManager.h"
#include "../player/Player.h"
#include "../../lib/math/hit.h"

using namespace std;
constexpr float TILE_SIZE = 5.0f;
constexpr int MAP_W = 50;
constexpr int MAP_H = 50;
//---------------------------------
//		敵と弾の当たり判定
//---------------------------------
//int	CCollisionManager::CheckHitShotToEnemy(CEnemyManager &enemyManager,
//												CShotManager &shotManager)
//{
//	int ret = 0;
//	list<CActor*> enemyList = enemyManager.GetEnemyList();
//	list<CActor*> shotList = shotManager.GetPlayerShotList();
//
//	for (CActor* shot : shotList)
//	{
//		// 座標と半径を取得
//		VECTOR shotPos = shot->GetCenter();
//		float shotRadius = shot->GetRadius();
//
//		for (CActor* enemy : enemyList)
//		{
//			// 座標と半径を取得
//			VECTOR enemyPos = enemy->GetCenter();
//			float enemyRadius = enemy->GetRadius();
//
//			// 球と球の当たり判定
//			if(CHit::CheckSphereToSphere(shotPos, enemyPos, shotRadius, enemyRadius))
//			{
//				// ここまでくれば当たった！！
//				shot->HitCalc();
//				enemy->HitCalc();
//				ret++;
//			}
//		}
//	}
//
//	return ret;
//}


//---------------------------------
//		敵とプレイヤーの当たり判定
//---------------------------------
void	CCollisionManager::CheckHitPlayerToCat(CPlayer& player,
	CCat& cat)
{
	VECTOR playerPos = player.GetCenter();
	float playerRadius = player.GetRadius();

	// 座標と半径を取得
	VECTOR catPos = cat.GetCenter();
	float catRadius = cat.GetRadius();

	// 球と球の当たり判定
	if (CHit::CheckSphereToSphere(playerPos, catPos, playerRadius, catRadius))
	{
		// ここまでくれば当たった！！
		/*player.HitCalc();
		cat.HitCalc();*/
	}
	
}

//フィールドとの当たり判定処理
	//@center :当たり判定をする相手の座標
	//@radius :相手を球として判定するのでその半径
	//@return :押し戻す方向&距離
//---------------------------------------------------------
// オブジェクトとの当たり判定
//---------------------------------------------------------
VECTOR CCollisionManager::HitObject(
    VECTOR center,
    float radius,
    ObjectEditor& object
)
{
    VECTOR result = VGet(0, 0, 0);

    //--------------------------------------
   // ObjectEditorから取得
   //--------------------------------------
    const auto& objs = object.GetObjects();
    //------------------------------------------
    // 全オブジェクトをチェック
    //------------------------------------------
    for (const auto& obj : objs)
    {
        //--------------------------------------
        // 壁だけ当たり判定
        //--------------------------------------
        if (obj.type != OBJ_BLOCK)
            continue;
        //--------------------------------------
        // マップ座標 → ワールド座標
        //--------------------------------------
        float worldX = (obj.x + 0.5f) * TILE_SIZE;
        float worldZ = (obj.z + 0.5f) * TILE_SIZE;

        VECTOR objPos = VGet(worldX, 0, worldZ);
        //--------------------------------------
        // プレイヤー → オブジェクト
        //--------------------------------------
        float dx = center.x - objPos.x;
        float dz = center.z - objPos.z;
        //--------------------------------------
        // 距離
        //--------------------------------------
        float distSq = dx * dx + dz * dz;
        //--------------------------------------
        // 当たり判定半径
        //--------------------------------------
        float hitRange = radius + 2.5f;
        //--------------------------------------
        // 当たった
        //--------------------------------------
        if (distSq < hitRange * hitRange)
        {
            float dist = sqrtf(distSq);
            //----------------------------------
            // 0除算防止
            //----------------------------------
            if (dist <= 0.0001f)
                continue;
            //----------------------------------
            // 押し戻し方向
            //----------------------------------
            VECTOR dir;
            dir.x = dx / dist;
            dir.y = 0.0f;
            dir.z = dz / dist;
            //----------------------------------
            // めり込み量
            //----------------------------------
            float push = hitRange - dist;
            //----------------------------------
            // 押し戻し
            //----------------------------------
            result = VAdd(result, VScale(dir, push));
        }
    }

    return result;
}

//フィールドとの当たり判定処理
    //@center :当たり判定をする相手の座標
    //@radius :相手を球として判定するのでその半径
    //@return :押し戻す方向&距離
 VECTOR CCollisionManager::HitMap(VECTOR center,
    float radius,
    MapEditor& map)
{
     VECTOR result = VGet(0, 0, 0);

     //--------------------------------------
     // ワールド座標 → マップ座標
     //--------------------------------------
     int mapX = (int)(center.x / TILE_SIZE);
     int mapZ = (int)(center.z / TILE_SIZE);

     //--------------------------------------
     // 周囲チェック
     //--------------------------------------
     for (int z = -1; z <= 1; z++)
     {
         for (int x = -1; x <= 1; x++)
         {
             int checkX = mapX + x;
             int checkZ = mapZ + z;

             //----------------------------------
             // 範囲外防止
             //----------------------------------
             if (checkX < 0 || checkZ < 0)
                 continue;

             if (checkX >= MAP_W ||
                 checkZ >= MAP_H)
                 continue;

             //----------------------------------
             // 床じゃなければスキップ
             //----------------------------------
             if (map.GetMap(checkZ, checkX) != 1)
                 continue;

             //----------------------------------
             // タイル中心座標
             //----------------------------------
             float worldX = (checkX + 0.5f) * TILE_SIZE;
             float worldZ = (checkZ + 0.5f) * TILE_SIZE;

             //----------------------------------
             // プレイヤーとの差
             //----------------------------------
             float dx = center.x - worldX;
             float dz = center.z - worldZ;

             //----------------------------------
             // 距離
             //----------------------------------
             float distSq = dx * dx + dz * dz;

             //----------------------------------
             // 当たり半径
             //----------------------------------
             float hitRange = radius + (TILE_SIZE * 0.5f);

             //----------------------------------
             // 当たった
             //----------------------------------
             if (distSq < hitRange * hitRange)
             {
                 float dist = sqrtf(distSq);

                 if (dist <= 0.0001f)
                     continue;

                 VECTOR dir;

                 dir.x = dx / dist;
                 dir.y = 0;
                 dir.z = dz / dist;

                 float push = hitRange - dist;

                 result = VAdd(result, VScale(dir, push));
             }
         }
     }

     return result;
}