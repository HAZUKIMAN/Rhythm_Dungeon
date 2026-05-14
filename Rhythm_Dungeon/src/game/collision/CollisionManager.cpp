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

//-----------------------------------------
//		ブロックとプレイヤーの当たり判定
//-----------------------------------------
void CCollisionManager::CheckHitPlayerToBlock(CPlayer& player,
    CInstalledItem& inst)
{
    m_time--;

	VECTOR playerPos = player.GetCenter();
	float playerRadius = player.GetRadius();

    VECTOR vec = { inst.GetPos().x,inst.GetPos().y + 1.0f ,inst.GetPos().z };
	// 座標と半径を取得
	VECTOR instPos = vec;
	float instRadius = 2.5f;

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


//-----------------------------------
// ゴールとプレイヤーの当たり判定
//-----------------------------------
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
   // //--------------------------------------
   //// ObjectEditorから取得
   ////--------------------------------------
   // const auto& objs = object.GetObjects();
   // //------------------------------------------
   // // 全オブジェクトをチェック
   // //------------------------------------------
   // for (const auto& obj : objs)
   // {
   //     //--------------------------------------
   //     // 壁だけ当たり判定
   //     //--------------------------------------
   //     if (obj.type != OBJ_BLOCK)
   //         continue;
   //     //--------------------------------------
   //     // マップ座標 → ワールド座標
   //     //--------------------------------------
   //     float worldX = (obj.x + 0.5f) * TILE_SIZE;
   //     float worldZ = (obj.z + 0.5f) * TILE_SIZE;

   //     VECTOR objPos = VGet(worldX, 0, worldZ);
   //     //--------------------------------------
   //     // プレイヤー → オブジェクト
   //     //--------------------------------------
   //     float dx = center.x - objPos.x;
   //     float dz = center.z - objPos.z;
   //     //--------------------------------------
   //     // 距離
   //     //--------------------------------------
   //     float distSq = dx * dx + dz * dz;
   //     //--------------------------------------
   //     // 当たり判定半径
   //     //--------------------------------------
   //     float hitRange = radius + 2.5f;
   //     //--------------------------------------
   //     // 当たった
   //     //--------------------------------------
   //     if (distSq < hitRange * hitRange)
   //     {
   //         float dist = sqrtf(distSq);
   //         //----------------------------------
   //         // 0除算防止
   //         //----------------------------------
   //         if (dist <= 0.0001f)
   //             continue;
   //         //----------------------------------
   //         // 押し戻し方向
   //         //----------------------------------
   //         VECTOR dir;
   //         dir.x = dx / dist;
   //         dir.y = 0.0f;
   //         dir.z = dz / dist;
   //         //----------------------------------
   //         // めり込み量
   //         //----------------------------------
   //         float push = hitRange - dist;
   //         //----------------------------------
   //         // 押し戻し
   //         //----------------------------------
   //         result = VAdd(result, VScale(dir, push));

   //         //プレイヤーの処理
   //         int  state = player.GetDirect();

   //         switch (state)
   //         {
   //         case 0:
   //             player.SetDirect(1);
   //             break;
   //         case 1:
   //             player.SetDirect(2);
   //             break;
   //         case 2:
   //             player.SetDirect(3);
   //             break;
   //         case 3:
   //             player.SetDirect(4);
   //             break;
   //         }
   //     }
    //}

    return result;
}

//--------------------------------------
// マップとの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitMap(
    VECTOR center,
    float radius,
    MapEditor& map)
{
    //--------------------------------------
    // 押し戻し結果
    //--------------------------------------
    VECTOR result = VGet(0, 0, 0);

    //--------------------------------------
    // ワールド座標 → マップ座標
    //--------------------------------------
    int mapX = (int)floor(center.x / TILE_SIZE);
    int mapY = (int)floor(center.y / TILE_SIZE);
    int mapZ = (int)floor(center.z / TILE_SIZE);

    //--------------------------------------
    // 範囲外防止
    //--------------------------------------
    if (mapX < 0 || mapX >= MAP_W ||
        mapY < 0 || mapY >= MAP_Y ||
        mapZ < 0 || mapZ >= MAP_H)
    {
        return result;
    }

    //--------------------------------------
    // 足元判定
    //--------------------------------------
    int footY = mapY - 1;

    if (footY >= 0)
    {
        //----------------------------------
        // 床チェック
        //----------------------------------
        if (map.GetMap(footY, mapZ, mapX) == TILE_FLOOR)
        {
            //----------------------------------
            // 床の上面
            //----------------------------------
            float floorTop = (footY + 1) * TILE_SIZE;

            //----------------------------------
            // 足が埋まっている
            //----------------------------------
            float footPos = center.y - radius;

            if (footPos < floorTop)
            {
                //----------------------------------
                // 上方向へ押し戻す
                //----------------------------------
                result.y = floorTop - footPos;
            }
        }
    }

    //--------------------------------------
    // 周囲ブロック判定
    //--------------------------------------
    for (int z = -1; z <= 1; z++)
    {
        for (int x = -1; x <= 1; x++)
        {
            //----------------------------------
            // チェック座標
            //----------------------------------
            int checkX = mapX + x;
            int checkY = mapY;
            int checkZ = mapZ + z;

            //----------------------------------
            // 範囲外防止
            //----------------------------------
            if (checkX < 0 || checkX >= MAP_W ||
                checkY < 0 || checkY >= MAP_Y ||
                checkZ < 0 || checkZ >= MAP_H)
            {
                continue;
            }

            //----------------------------------
            // 壁以外スキップ
            //----------------------------------
            if (map.GetMap(checkY, checkZ, checkX) != TILE_WALL)
            {
                continue;
            }

            //----------------------------------
            // ワールド座標
            //----------------------------------
            float worldX = (checkX + 0.5f) * TILE_SIZE;
            float worldY = (checkY + 0.5f) * TILE_SIZE;
            float worldZ = (checkZ + 0.5f) * TILE_SIZE;

            VECTOR blockPos = VGet(worldX, worldY, worldZ);

            //----------------------------------
            // ブロック半径
            //----------------------------------
            float blockRadius = TILE_SIZE * 0.5f;

            //----------------------------------
            // めり込み量
            //----------------------------------
            float hitLen = 0.0f;

            //----------------------------------
            // 球同士判定
            //----------------------------------
            if (CHit::CheckSphereToSphere(
                center,
                blockPos,
                radius,
                blockRadius,
                &hitLen))
            {
                //----------------------------------
                // 押し戻し方向
                //----------------------------------
                VECTOR dir = VSub(center, blockPos);

                //----------------------------------
                // 長さ
                //----------------------------------
                float len = VSize(dir);

                //----------------------------------
                // 0除算防止
                //----------------------------------
                if (len <= 0.0001f)
                    continue;

                //----------------------------------
                // 正規化
                //----------------------------------
                dir = VNorm(dir);

                //----------------------------------
                // 横方向だけ押し戻す
                //----------------------------------
                dir.y = 0.0f;

                //----------------------------------
                // 押し戻し
                //----------------------------------
                result = VAdd(result, VScale(dir, hitLen));
            }
        }
    }

    return result;
}



//--------------------------------------
// 猫とオブジェクトの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitCatToObject(
    VECTOR center, float radius, ObjectEditor& object)
{
    //--------------------------------------
    // 押し戻し結果
    //--------------------------------------
    VECTOR result = VGet(0, 0, 0);

    //--------------------------------------
    // ObjectEditorから取得
    //--------------------------------------
    const auto& objs = object.GetObjects();

    //--------------------------------------
    // 全オブジェクトチェック
    //--------------------------------------
    for (const auto& obj : objs)
    {
        //--------------------------------------
        // ブロックだけ判定
        //--------------------------------------
        if (obj.type != OBJ_BLOCK)
            continue;

        //--------------------------------------
        // マップ座標 → ワールド座標
        //--------------------------------------
        float worldX = (obj.x + 0.5f) * TILE_SIZE;
        float worldY = (obj.y + 0.5f) * TILE_SIZE;
        float worldZ = (obj.z + 0.5f) * TILE_SIZE;

        VECTOR objPos = VGet(worldX, worldY, worldZ);

        //--------------------------------------
        // ブロック半径
        //--------------------------------------
        float blockRadius = TILE_SIZE * 0.5f;

        //--------------------------------------
        // めり込み量
        //--------------------------------------
        float hitLen = 0.0f;

        //--------------------------------------
        // 球同士の当たり判定
        //--------------------------------------
        if (CHit::CheckSphereToSphere(
            center,
            objPos,
            radius,
            blockRadius,
            &hitLen))
        {
            //----------------------------------
            // 押し戻し方向
            //----------------------------------
            VECTOR dir = VSub(center, objPos);

            //----------------------------------
            // 長さ
            //----------------------------------
            float len = VSize(dir);

            //----------------------------------
            // 0除算防止
            //----------------------------------
            if (len <= 0.0001f)
                continue;

            //----------------------------------
            // 正規化
            //----------------------------------
            dir = VNorm(dir);

            //----------------------------------
            // Y方向は押し戻さない
            //----------------------------------
            dir.y = 0.0f;

            //----------------------------------
            // 押し戻し
            //----------------------------------
            result = VAdd(result, VScale(dir, hitLen));
        }
    }

    return result;
}