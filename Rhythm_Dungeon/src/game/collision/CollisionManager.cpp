#include "CollisionManager.h"
//#include "../object/enemy/EnemyManager.h"
#include "../player/Player.h"
#include "../../lib/math/hit.h"

using namespace std;
constexpr int SET_TIME = 50;

int CCollisionManager::m_time = SET_TIME;

CCollisionManager::CCollisionManager()
{
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
    // 足元座標
    //--------------------------------------
    float footPos = center.y - radius;

    //--------------------------------------
    // 足元のマップY
    //--------------------------------------
    int footY = (int)floor(footPos / TILE_SIZE);

    //--------------------------------------
    // 床判定
    //--------------------------------------
    if (footY >= 0)
    {
        //----------------------------------
        // 床があるか
        //----------------------------------
        if (map.GetMap(footY, mapZ, mapX)
            == TILE_FLOOR)
        {
            // 床の上面
            float floorTop = (footY + 1) * TILE_SIZE;

            // 足が床に埋まっている
            if (footPos <= floorTop)
            {
                // 上へ押し戻す
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
            if (map.GetMap(checkY, checkZ, checkX)
                != TILE_WALL)
            {
                continue;
            }

            //----------------------------------
            // ブロック座標
            //----------------------------------
            float worldX =(checkX + 0.5f) * TILE_SIZE;
            float worldY =(checkY + 0.5f) * TILE_SIZE;
            float worldZ =(checkZ + 0.5f) * TILE_SIZE;

            VECTOR blockPos =VGet(worldX, worldY, worldZ);

            //----------------------------------
            // ブロック半径
            //----------------------------------
            float blockRadius =
                TILE_SIZE * 0.5f;

            //----------------------------------
            // めり込み量
            //----------------------------------
            float hitLen = 0.0f;

            //----------------------------------
            // 球同士判定
            //----------------------------------
            if (CHit::CheckSphereToSphere(center,blockPos,radius,blockRadius,&hitLen))
            {
                // 押し戻し方向
                VECTOR dir =
                    VSub(center, blockPos);
                // 長さ
                float len = VSize(dir);
                // 0除算防止
                if (len <= 0.0001f)
                {
                    continue;
                }

                // 正規化
                dir = VNorm(dir);
                dir.y = 0.0f;

                // XZ方向だけ押し戻す
                result.x += dir.x * hitLen;
                result.z += dir.z * hitLen;
            }
        }
    }

    return result;
}

//--------------------------------------
// Playerと設置ブロックの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitPlayerToBlock(
    CPlayer& player,
    std::vector<CBlock*>& blocks)
{
    //--------------------------------------
    // クールタイム
    //--------------------------------------
    m_time--;

    //--------------------------------------
    // Player情報
    //--------------------------------------
    VECTOR playerPos =
        player.GetCenter();

    float playerRadius =
        player.GetRadius();

    //--------------------------------------
    // 押し戻し結果
    //--------------------------------------
    VECTOR result =
        VGet(0, 0, 0);

    //--------------------------------------
    // 一番近い当たり
    //--------------------------------------
    float nearestHit =
        999999.0f;

    CBlock* hitBlock =
        nullptr;

    //--------------------------------------
    // 全ブロック判定
    //--------------------------------------
    for (auto block : blocks)
    {
        if (block == nullptr)
            continue;

        VECTOR blockPos =
            block->GetPos();

        float blockRadius =
            2.5f;

        float hitLen =
            0.0f;

        //--------------------------------------
        // 球判定
        //--------------------------------------
        if (!CHit::CheckSphereToSphere(
            playerPos,
            blockPos,
            playerRadius,
            blockRadius,
            &hitLen))
        {
            continue;
        }

        //--------------------------------------
        // 一番近いブロック
        //--------------------------------------
        float dist =
            VSize(
                VSub(
                    playerPos,
                    blockPos));

        if (dist < nearestHit)
        {
            nearestHit =
                dist;

            hitBlock =
                block;
        }
    }

    //--------------------------------------
    // 当たっていない
    //--------------------------------------
    if (hitBlock == nullptr)
    {
        return result;
    }

    //--------------------------------------
    // クールタイム中
    //--------------------------------------
    if (m_time <= 0)
    {
        int state =
            player.GetDirect();

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
            player.SetDirect(0);
            break;
        }

        m_time = SET_TIME;
    }

    //--------------------------------------
    // 押し戻し
    //--------------------------------------
    VECTOR dir =
        VSub(
            playerPos,
            hitBlock->GetPos());

    float len =
        VSize(dir);

    if (len > 0.0001f)
    {
        dir = VNorm(dir);
        dir.y = 0.0f;

        result =VScale(dir,1.0f);
    }

    result.y = 0.0f;
    result.z = 0.0f;
    return result;
}



//--------------------------------------
// Playerとオブジェクトの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitCatToObject(
    CPlayer& player,ObjectEditor& object)
{
    m_time--;
    float radius = player.GetRadius();
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
        if (obj.type != OBJ_ITEM)
            continue;

        if (m_time >= 0)continue;

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
        if (CHit::CheckSphereToSphere(player.GetCenter(), objPos, radius, blockRadius, &hitLen))
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

            // 押し戻し方向
            VECTOR dir = VSub(player.GetCenter(), objPos);
            // 長さ
            float len = VSize(dir);
            // 0除算防止
            if (len <= 0.0001f)
                continue;

            // 正規化
            dir = VNorm(dir);
            dir.y = 0.0f;

            // 押し戻し
            result = VAdd(result, VScale(dir, hitLen));
        }
    }

    return result;
}