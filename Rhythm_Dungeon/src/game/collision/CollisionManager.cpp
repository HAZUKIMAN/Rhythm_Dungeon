#include "CollisionManager.h"
//#include "../object/enemy/EnemyManager.h"
#include "../human/Human.h"
#include "../../lib/math/hit.h"
#include "../../lib/sound/SoundManager.h"

using namespace std;
constexpr int SET_TIME = 150;

int CCollisionManager::m_time = SET_TIME;

CCollisionManager::CCollisionManager()
{
}


//-----------------------------------
// ゴールと人間の当たり判定
//-----------------------------------
bool CCollisionManager::CheckHithumanToGoal(CHuman& human, CGoal& goal)
{
    VECTOR humanPos = human.GetCenter();
    float humanRadius = human.GetRadius();

    VECTOR vec = { goal.GetPos().x,goal.GetPos().y + 1.0f ,goal.GetPos().z };
    // 座標と半径を取得
    VECTOR goalPos = vec;
    float goalRadius = 2.0f;
    // 球と球の当たり判定
    if (CHit::CheckSphereToSphere(humanPos, goalPos, humanRadius, goalRadius))
    {
        return true;
    }
    return false;
}

//-----------------------------------
// エネミーと人間の当たり判定
//-----------------------------------
void CCollisionManager::CheckHithumanToEnemy(CHuman& human, CEnemy* enemy)
{
    VECTOR humanPos = human.GetCenter();
    float humanRadius = human.GetRadius();

    VECTOR vec = { enemy->GetPos().x,enemy->GetPos().y + 1.0f ,enemy->GetPos().z };
    // 座標と半径を取得
    VECTOR enemyPos = vec;
    float enemyRadius = 1.0f;

    // 球と球の当たり判定
    if (CHit::CheckSphereToSphere(humanPos, enemyPos, humanRadius, enemyRadius))
    {
        CSoundManager::Play(CSoundManager::SOUNDID_SE_HIT, DX_PLAYTYPE_BACK);
        human.HitCalc();
    }
}


//--------------------------------------
// マップとの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitMap(VECTOR center,float radius, MapEditor& map)
{
    //--------------------------------------
    // 押し戻し結果
    //--------------------------------------
    VECTOR result = VGet(0, 0, 0);

    //--------------------------------------
    // ワールド座標 → マップ座標
    //--------------------------------------
    int mapX = (int)floor(center.x / TILE_SIZE);
    int mapZ = (int)floor(center.z / TILE_SIZE);

    //--------------------------------------
    // 足元座標
    //--------------------------------------
    float footPos = center.y - radius;

    //--------------------------------------
    // 足元Y
    //--------------------------------------
    int footY = (int)floor(footPos / TILE_SIZE);

    //--------------------------------------
    // 範囲外防止
    //--------------------------------------
    if (mapX < 0 || mapX >= MAP_W ||
        mapZ < 0 || mapZ >= MAP_H)
    {
        return result;
    }

    //--------------------------------------
    // 床判定
    //--------------------------------------
    if (footY >= 0 && footY < MAP_Y)
    {
        //----------------------------------
        // 床があるか
        //----------------------------------
        int tile = map.GetMap(footY,mapZ,mapX);

        if (tile == TILE_FLOOR || tile == TILE_FLOOR2 || tile == TILE_BRIDGE|| tile == TILE_STAIRS)
        {
            //----------------------------------
            // 床の上面
            //----------------------------------
            float floorTop = (footY + 1) * TILE_SIZE;

            //----------------------------------
            // 床にめり込んでいる
            //----------------------------------
            if (footPos < floorTop)
            {
                result.y = floorTop - footPos;
            }
        }
    }

    return result;
}

//--------------------------------------------
// 猫の座標をマップ座標にして計算を行う
//--------------------------------------------
VECTOR CCollisionManager::HitCatToMap(VECTOR center, float radius, MapEditor& map)
{
    //--------------------------------------
   // 押し戻し結果
   //--------------------------------------
    VECTOR result = VGet(0, 0, 0);

    //--------------------------------------
    // ワールド座標 → マップ座標
    //--------------------------------------
    int mapX = (int)floor(center.x / TILE_SIZE);
    int mapZ = (int)floor(center.z / TILE_SIZE);

    //--------------------------------------
    // 足元座標
    //--------------------------------------
    float footPos = center.y - radius;

    //--------------------------------------
    // 足元Y
    //--------------------------------------
    int footY = (int)floor(footPos / TILE_SIZE);

    //--------------------------------------
    // 範囲外防止
    //--------------------------------------
    if (mapX < 0 || mapX >= MAP_W ||
        mapZ < 0 || mapZ >= MAP_H)
    {
        return result;
    }

    //--------------------------------------
    // 床判定
    //--------------------------------------
    if (footY >= 0 &&
        footY < MAP_Y)
    {
        //----------------------------------
        // 床があるか
        //----------------------------------
        int tile = map.GetMap(
            footY,
            mapZ,
            mapX);

        if (tile == TILE_FLOOR || tile == TILE_FLOOR2 || tile == TILE_BRIDGE || tile == TILE_STAIRS)
        {
            //----------------------------------
            // 床の上面
            //----------------------------------
            float floorTop = (footY + 1) * TILE_SIZE;

            //----------------------------------
            // 床にめり込んでいる
            //----------------------------------
            if (footPos < floorTop)
            {
                result.y = floorTop - footPos;
            }
        }
    }

    return result;
}

//--------------------------------------
// エネミーとオブジェクトの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitEnemyToObject(std::vector<CEnemy*>& enemies,ObjectEditor& object)
{

    VECTOR result = VGet(0, 0, 0);

    const auto& objs = object.GetObjects();

    //--------------------------------------
    // Enemy全員
    //--------------------------------------
    for (auto& enemy : enemies)
    {
        if (enemy == nullptr) continue;

       //---------------------------------
       // クールタイム中
       //---------------------------------
        if (enemy->GetHitTime() > 0)
        {
            continue;
        }

        //--------------------------------------
        // Object全員
        //--------------------------------------
        for (const auto& obj : objs)
        {
            if (obj.type != OBJ_ITEM)
                continue;

            //--------------------------------------
            // ワールド座標
            //--------------------------------------
            float worldX = (obj.x + 0.5f) * TILE_SIZE;
            float worldY = (obj.y + 0.5f) * TILE_SIZE;
            float worldZ = (obj.z + 0.5f) * TILE_SIZE;

            VECTOR objPos = VGet(worldX, worldY, worldZ);

            //--------------------------------------
            // 距離
            //--------------------------------------
            VECTOR diff = VSub(enemy->GetCenter(), objPos);

            diff.y = 0.0f;
            float dist = VSize(diff);

            //--------------------------------------
            // 近づいたら方向変更
            //--------------------------------------
            if (dist < 10.0f)
            {
                int state = enemy->GetDirect();

              switch (state)
              {
                case 0:
                    enemy->SetDirect(1);
                    break;

                case 1:
                    enemy->SetDirect(2);
                    break;

                case 2:
                    enemy->SetDirect(3);
                    break;

                case 3:
                    enemy->SetDirect(0);
                    break;
              }
                
                //--------------------------------------
                // 少し押し戻す
                //--------------------------------------
                if (dist > 0.001f)
                {
                    diff = VNorm(diff);

                    result = VScale(diff, 0.5f);
                }

                //---------------------------------
                // クールタイム開始
                //---------------------------------
                enemy->SetHitTime(HIT_TIME);
            }
        }
    }

    return result;
}
