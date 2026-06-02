#include "CollisionManager.h"
//#include "../object/enemy/EnemyManager.h"
#include "../human/Human.h"
#include "../../lib/math/hit.h"

using namespace std;
constexpr int SET_TIME = 150;

int CCollisionManager::m_time = SET_TIME;

CCollisionManager::CCollisionManager()
{
}


//-----------------------------------
// ゴールと人間の当たり判定
//-----------------------------------
bool CCollisionManager::CheckHithumanToGoal(CHuman& human,
    CGoal& goal)
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
void CCollisionManager::CheckHithumanToEnemy(CHuman& human,
    CEnemy* enemy)
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
        human.HitCalc();
    }
}

//人間と配置可能なオブジェクトの計算
void CCollisionManager::HitHumanToInst(CHuman& human,
    VECTOR inst_vec)
{
    VECTOR humanPos = human.GetCenter();
    float humanRadius = 2.5;

    // 座標と半径を取得
    VECTOR InstPos = inst_vec;
    float InstRadius = 2.5;

    if (m_time >= 0)return;

    // 球と球の当たり判定
    if (CHit::CheckSphereToSphere(humanPos, InstPos, humanRadius, InstRadius))
    {
        int state = human.GetDirect();

        m_time = SET_TIME;

        switch (state)
        {
        case 0:
            human.SetDirect(1);
            break;

        case 1:
            human.SetDirect(2);
            break;

        case 2:
            human.SetDirect(3);
            break;

        case 3:
            human.SetDirect(0);
            break;
        }
    }
}

//--------------------------------------
// エネミー配置可能オブジェクトとの判定
//--------------------------------------
void CCollisionManager::HitEnemyToInst(
    std::vector<CEnemy*>& enemies,
    VECTOR inst_vec)
{
    //--------------------------------------
    // Enemy全員
    //--------------------------------------
    for (auto& enemy : enemies)
    {
        if (enemy == nullptr)
            continue;

       //---------------------------------
       // クールタイム中
       //---------------------------------
        if (enemy->GetHitTime() > 0)
        {
            continue;
        }

        //--------------------------------------
        // Enemy情報
        //--------------------------------------
        VECTOR enemyPos = enemy->GetCenter();

        float enemyRadius = 2.5f;

        //--------------------------------------
        // 設置物情報
        //--------------------------------------
        VECTOR instPos =inst_vec;

        float instRadius = 2.5f;

        //--------------------------------------
        // 球判定
        //--------------------------------------
        if (CHit::CheckSphereToSphere(
            enemyPos,
            instPos,
            enemyRadius,
            instRadius))
        {
            int state = enemy->GetDirect();

            //--------------------------------------
            // 向き変更
            //--------------------------------------
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

        //---------------------------------
        // クールタイム開始
        //---------------------------------
            enemy->SetHitTime(HIT_TIME);
        }

    }
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

        if (tile == TILE_FLOOR)
        {
            //----------------------------------
            // 床の上面
            //----------------------------------
            float floorTop =
                (footY + 1) * TILE_SIZE;

            //----------------------------------
            // 床にめり込んでいる
            //----------------------------------
            if (footPos < floorTop)
            {
                result.y =
                    floorTop - footPos;
            }
        }
    }

    //--------------------------------------
    // 壁判定
    //--------------------------------------
    for (int z = -1; z <= 1; z++)
    {
        for (int x = -1; x <= 1; x++)
        {
            //----------------------------------
            // チェック座標
            //----------------------------------
            int checkX = mapX + x;
            int checkY = footY; // ←重要
            int checkZ = mapZ + z;

            //----------------------------------
            // 範囲外
            //----------------------------------
            if (checkX < 0 ||
                checkX >= MAP_W ||
                checkY < 0 ||
                checkY >= MAP_Y ||
                checkZ < 0 ||
                checkZ >= MAP_H)
            {
                continue;
            }

            //----------------------------------
            // 壁以外無視
            //----------------------------------
            if (map.GetMap(
                checkY,
                checkZ,
                checkX) != TILE_WALL)
            {
                continue;
            }

            //----------------------------------
            // 壁ワールド座標
            //----------------------------------
            float worldX =
                (checkX + 0.5f)
                * TILE_SIZE;

            float worldY =
                (checkY + 0.5f)
                * TILE_SIZE;

            float worldZ =
                (checkZ + 0.5f)
                * TILE_SIZE;

            VECTOR blockPos =
                VGet(
                    worldX,
                    worldY,
                    worldZ);

            //----------------------------------
            // 壁半径
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
                VECTOR dir = VSub( center, blockPos);

                //----------------------------------
                // XZだけ
                //----------------------------------
                dir.y = 0.0f;

                float len = VSize(dir);

                //----------------------------------
                // 0除算防止
                //----------------------------------
                if (len <
                    0.0001f)
                {
                    continue;
                }

                //----------------------------------
                // 正規化
                //----------------------------------
                dir =
                    VNorm(dir);

                //----------------------------------
                // 押し戻し
                //----------------------------------
                result.x +=
                    dir.x * hitLen;

                result.z +=
                    dir.z * hitLen;
            }
        }
    }

    return result;
}

//--------------------------------------
// Humanとオブジェクトの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitHumanToObject(
    CHuman& human,
    ObjectEditor& object)
{
    VECTOR result = VGet(0, 0, 0);

    const auto& objs =
        object.GetObjects();

    //--------------------------------------
    // Object全検索
    //--------------------------------------
    for (const auto& obj : objs)
    {
        //--------------------------------------
        // ITEM以外無視
        //--------------------------------------
        if (obj.type != OBJ_ITEM)
            continue;

        //--------------------------------------
        // ワールド座標
        //--------------------------------------
        float worldX =
            (obj.x + 0.5f) * TILE_SIZE;

        float worldY =
            (obj.y + 0.5f) * TILE_SIZE;

        float worldZ =
            (obj.z + 0.5f) * TILE_SIZE;

        VECTOR objPos =
            VGet(worldX, worldY, worldZ);

        //--------------------------------------
        // Humanとの距離
        //--------------------------------------
        VECTOR diff =
            VSub(human.GetCenter(), objPos);

        diff.y = 0.0f;

        float dist =VSize(diff);

        //--------------------------------------
        // 十分近い
        //--------------------------------------
        if (dist < TILE_SIZE)
        {
            int state =
                human.GetDirect();

            //--------------------------------------
            // 向き変更
            //--------------------------------------
            switch (state)
            {
            case 0:
                human.SetDirect(1);
                break;

            case 1:
                human.SetDirect(2);
                break;

            case 2:
                human.SetDirect(3);
                break;

            case 3:
                human.SetDirect(0);
                break;
            }

            //--------------------------------------
            // 少し押し戻す
            //--------------------------------------
            if (dist > 0.001f)
            {
                diff = VNorm(diff);

                result =
                    VScale(diff, 1.0f);
            }

            //--------------------------------------
            // 1回反応したら終了
            //--------------------------------------
            break;
        }
    }

    return result;
}

//--------------------------------------
// エネミーとオブジェクトの当たり判定
//--------------------------------------
VECTOR CCollisionManager::HitEnemyToObject(
    std::vector<CEnemy*>& enemies,ObjectEditor& object)
{
    m_time--;

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

            if (m_time >= 0)
                continue;

            //--------------------------------------
            // ワールド座標
            //--------------------------------------
            float worldX =
                (obj.x + 0.5f) * TILE_SIZE;

            float worldY =
                (obj.y + 0.5f) * TILE_SIZE;

            float worldZ =
                (obj.z + 0.5f) * TILE_SIZE;

            VECTOR objPos =
                VGet(worldX, worldY, worldZ);

            //--------------------------------------
            // 距離
            //--------------------------------------
            VECTOR diff =
                VSub(enemy->GetCenter(), objPos);

            diff.y = 0.0f;

            float dist = VSize(diff);

            //--------------------------------------
            // 近づいたら方向変更
            //--------------------------------------
            if (dist < 10.0f)
            {
                int state = enemy->GetDirect();

                m_time = SET_TIME;

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

                    result =
                        VScale(diff, 0.5f);
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
