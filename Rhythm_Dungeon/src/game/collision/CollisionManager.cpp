#include "CollisionManager.h"
//#include "../object/enemy/EnemyManager.h"
#include "../player/Player.h"
#include "../../lib/math/hit.h"

using namespace std;
constexpr int SET_TIME = 20;

int CCollisionManager::m_time = SET_TIME;

CCollisionManager::CCollisionManager()
{
}

//---------------------------------
//		敵とプレイヤーの当たり判定
//---------------------------------
void CCollisionManager::CheckHitPlayerToBlock(CPlayer& player,
    CInstalledItem& inst)
{
    m_time--;

	VECTOR playerPos = player.GetCenter();
	float playerRadius = player.GetRadius();

    VECTOR vec = { inst.GetPos().x,inst.GetPos().y + 1.0f ,inst.GetPos().z };
	// 座標と半径を取得
	VECTOR instPos = vec;
	float instRadius = 2.0f;

    if (m_time >= 0)return;

	// 球と球の当たり判定
	if (CHit::CheckSphereToSphere(playerPos, instPos, playerRadius, instRadius))
	{
        //プレイヤーの処理
        int  state = player.GetDirect();
        m_time = SET_TIME;

        switch (state)
        {
        case 0:
            player.SetDirect(1);
            break;
        case 1:
            player.SetDirect(2);
            break;
        case 2:
            player.SetDirect(3);
            break;
        case 3:
            player.SetDirect(4);
            break;
        }
	}
}

// ゴールとプレイヤーの当たり判定
bool CCollisionManager::CheckHitPlayerToGoal(CPlayer& player,
    CGoal& goal)
{
    VECTOR playerPos = player.GetCenter();
    float playerRadius = player.GetRadius();

    VECTOR vec = { goal.GetPos().x,goal.GetPos().y + 1.0f ,goal.GetPos().z };
    // 座標と半径を取得
    VECTOR goalPos = vec;
    float goalRadius = 2.0f;
    // 球と球の当たり判定
    if (CHit::CheckSphereToSphere(playerPos, goalPos, playerRadius, goalRadius))
    {
        return true;
    }
    return false;
}

//フィールドとの当たり判定処理
	//@center :当たり判定をする相手の座標
	//@radius :相手を球として判定するのでその半径
	//@return :押し戻す方向&距離
//---------------------------------------------------------
// オブジェクトとの当たり判定
//---------------------------------------------------------
VECTOR CCollisionManager::HitPlayerToObject(
    VECTOR center,
    float radius,
    ObjectEditor& object,
    CPlayer& player
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

            //プレイヤーの処理
            int  state = player.GetDirect();

            switch (state)
            {
            case 0:
                player.SetDirect(1);
                break;
            case 1:
                player.SetDirect(2);
                break;
            case 2:
                player.SetDirect(3);
                break;
            case 3:
                player.SetDirect(4);
                break;
            }
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
     int mapY = (int)(center.y / TILE_SIZE);

     //--------------------------------------
     //   床の当たり判定
     //---------------------------------------
     if (map.GetMap(mapY,mapZ, mapX) == 1)
     {
         result.y = 2.5f;
     }

     //--------------------------------------
     // 周囲チェック
     //--------------------------------------
     for (int z = -1; z <= 1; z++)
     {
         for (int x = -1; x <= 1; x++)
         {
             int checkX = mapX + x;
             int checkZ = mapZ + z;
             int checkY = mapY + z;

             //----------------------------------
             // 範囲外防止
             //----------------------------------
             if (checkX < 0 || checkZ < 0)
                 continue;

             if (checkX >= MAP_W ||
                 checkZ >= MAP_H)
                 continue;

             //----------------------------------
             // 壁じゃなければスキップ
             //----------------------------------
             if (map.GetMap(checkY,checkZ, checkX) != 2)
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

                 //----------------------------------
                 // 押し出し方向
                 //----------------------------------
                 VECTOR dir;

                 dir.x = dx / dist;
                 dir.y = 0;
                 dir.z = dz / dist;

                 //----------------------------------
                 // 押し戻し量
                 //----------------------------------
                 float push = hitRange - dist;

                 //----------------------------------
                 // 加算
                 //----------------------------------
                 result = VAdd(result, VScale(dir, push));
             }

            
         }
     }

     return result;
}

//猫とオブジェクトの計算
VECTOR CCollisionManager::HitCatToObject(VECTOR center,
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