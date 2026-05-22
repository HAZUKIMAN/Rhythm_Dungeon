#include "Cat.h"
#include <math.h>

#include "../../Data.h"
#include "../../lib/Input/Input.h"
#include "../Anime/Anime.h"
#include "../common.h"
#include "../../lib/math/MyMatrix.h"

//	定義関連------------------------------
static const float MOVE_SPEED = 1.0f;		// 移動速度
static const float ROT_SPEED = 0.1f;		// 回転速度
static const float JUMP_POWER = 5.0f;		// ジャンプ力
static const float GRAVITY = 0.01f;			// 重力
static const float RADIUS = 5.0f;			// 当たり判定半径

static const char CAT_MODEL_PATH[] = { "Data/Character/Cat/cat.mv1" };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CCat::CCat()
{
	Init();
}


//-------------------------------
//		デストラクタ
//-------------------------------
CCat::~CCat()
{
	// 本来は必要ないけど、念のため
	Fin();
}


//-------------------------------
//		初期化
//-------------------------------
void CCat::Init()
{
	m_state = CAT_STATE_NORMAL;
	m_moveMode = MOVE_GROUND;
	m_vRotation.y = DX_PI_F;
	m_radius = RADIUS;
	m_isActive = true;
}


//-------------------------------
//		データロード
//-------------------------------
void CCat::Load()
{
	VECTOR size = VGet(0.01f, 0.01f, 0.01f);

	int hndl= MV1LoadModel(CAT_MODEL_PATH);

	MV1SetScale(hndl, size);
	CObject::Load(hndl);

}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CCat::Step(MapEditor& map)
{
	if (!m_isActive)return;


	// 状態に合わせて行動変化
	switch (m_state)
	{
	case CAT_STATE_NORMAL:
		NormalExec(map);
		break;
	case CAT_STATE_JUMP:
		
		break;
	}

	Move();
}


//-------------------------------
//		描画処理
//-------------------------------
void CCat::Draw()
{
	if (!m_isActive)return;
	CObject::Draw();

	DrawFormatString(100,600,RED,"ねこのY座標：%f",m_vPosition.y);

	DrawFormatString(500,100,WHITE,"MoveMode = %d",m_moveMode);

	//---------------------------------
	// モード表示
	//---------------------------------
	if (m_moveMode == MOVE_GROUND)
	{
		DrawString(100, 100, "GROUND", GREEN);
	}
	else
	{
		DrawString(100, 100, "WALL", RED);
	}

#ifdef MY_DEBUG
	DrawSphere3D(m_vPos, RADIUS, 16, GetColor(0, 0, 255), GetColor(0, 0, 0), FALSE);
#endif
}


//-------------------------------
//		移動計算結果を反映
//-------------------------------
void CCat::Move()
{
	// 重力切替
	switch (m_moveMode)
	{
	case MOVE_GROUND:
		//	重力処理
		m_speed.y -= GRAVITY;
		break;

	case MOVE_WALL_X:
		//	重力処理
		m_speed.x += GRAVITY;
		break;

	case MOVE_WALL_Z:
		//	重力処理
		m_speed.z += GRAVITY;
		break;
	}

	// 移動速度加算
	m_vPosition = VAdd(m_vPosition, m_speed);

}


//-------------------------------
//		待機･移動中処理
//-------------------------------
void CCat::NormalExec(MapEditor& map)
{
	Operation(map);
}


//----------------------------
// ブロック設置
//----------------------------
void CCat::PlaceBlock(ObjectEditor& objEditor)
{
	int mapX = (int)(m_vPosition.x / TILE_SIZE);
	int mapY = (int)floor(m_vPosition.y / TILE_SIZE);
	int mapZ = (int)(m_vPosition.z / TILE_SIZE);

	float rot = m_vRotation.y;

	int dirX = (int)roundf(-sinf(rot));
	int dirZ = (int)roundf(-cosf(rot));

	int placeX = mapX + dirX;
	int placeZ = mapZ + dirZ;

	if (placeX < 0 || placeZ < 0)
		return;

	if (placeX >= MAP_W || placeZ >= MAP_H)
		return;

	//---------------------------------
	// 既にオブジェクトがあるなら置かない
	//---------------------------------
	if (objEditor.IsObjectAt(placeX, mapY, placeZ))
		return;

	objEditor.AddObject(placeX, mapY, placeZ, OBJ_PUT_BOX);
}
//------------------------------
// 床チェック
//------------------------------
bool CCat::CheckGround(MapEditor& map)
{
	//---------------------------------
	// 猫の足元を見る
	//---------------------------------
	int mapX =(int)floor(m_vPosition.x / TILE_SIZE);
	int mapY =(int)floor(m_vPosition.y / TILE_SIZE);
	int mapZ =(int)floor(m_vPosition.z / TILE_SIZE);

	//---------------------------------
	// 足元
	//---------------------------------
	int footY = mapY - 1;

	//---------------------------------
	// 範囲外防止
	//---------------------------------
	if (mapX < 0 || mapX >= MAP_W	|| 
		footY < 0 || footY >= MAP_Y ||
		mapZ < 0 || mapZ >= MAP_H)
	{
		return false;
	}

	//---------------------------------
	// 床があるか
	//---------------------------------
	if (map.GetMap(
		footY,
		mapZ,
		mapX) == TILE_FLOOR)
	{
		return true;
	}

	return false;
}
//------------------------------
// 壁チェック
//------------------------------
CCat::WallType CCat::CheckWall(MapEditor& map)
{
	float checkRadius = 5.5f;

	//---------------------------------
	// チェック位置
	//---------------------------------
	VECTOR rightPos =
		VGet(m_vPosition.x + checkRadius, m_vPosition.y, m_vPosition.z
		);

	VECTOR leftPos =
		VGet(m_vPosition.x - checkRadius, m_vPosition.y, m_vPosition.z);

	VECTOR frontPos =
		VGet(m_vPosition.x, m_vPosition.y, m_vPosition.z + checkRadius);

	VECTOR backPos =
		VGet(m_vPosition.x, m_vPosition.y, m_vPosition.z - checkRadius);

	//---------------------------------
	// X方向の壁
	//---------------------------------
	auto CheckPos = [&](VECTOR pos)
		{
			int mapX =(int)floor(pos.x / TILE_SIZE);
			int mapY =(int)floor(pos.y / TILE_SIZE);
			int mapZ =(int)floor(pos.z / TILE_SIZE);

			//---------------------------------
			// 範囲外
			//---------------------------------
			if (mapX < 0 ||mapX >= MAP_W ||
				mapY < 0 ||mapY >= MAP_Y ||
				mapZ < 0 ||mapZ >= MAP_H)
			{
				return false;
			}

			return map.GetMap(mapY,mapZ,mapX) == TILE_WALL;
		};

	//---------------------------------
	// 左右壁 → X壁
	//---------------------------------
	if (CheckPos(rightPos) || CheckPos(leftPos))
	{
		return WALL_X;
	}

	//---------------------------------
	// 前後壁 → Z壁
	//---------------------------------
	if (CheckPos(frontPos) || CheckPos(backPos))
	{
		return WALL_Z;
	}

	return WALL_NONE;
}
//------------------------------
// ブロック設置位置表示
//------------------------------
void CCat::DrawPlaceBlockPreview(MapEditor & map)
{
	//---------------------------------
	// 今いるマス
	//---------------------------------
	int mapX = (int)floor(m_vPosition.x / TILE_SIZE);
	int mapY = (int)floor(m_vPosition.y / TILE_SIZE);
	int mapZ = (int)floor(m_vPosition.z / TILE_SIZE);

	//---------------------------------
	// 向いている方向
	//---------------------------------
	float rot = m_vRotation.y;

	int dirX = (int)roundf(-sinf(rot));
	int dirZ = (int)roundf(-cosf(rot));

	//---------------------------------
	// 設置予定位置
	//---------------------------------
	int placeX = mapX + dirX;
	int placeY = mapY;
	int placeZ = mapZ + dirZ;

	//---------------------------------
	// 範囲外なら終了
	//---------------------------------
	if (placeX < 0 || placeX >= MAP_W ||
		placeY < 0 || placeY >= MAP_Y ||
		placeZ < 0 || placeZ >= MAP_H)
	{
		return;
	}

	//---------------------------------
	// ワールド座標
	//---------------------------------
	float x0 = placeX * TILE_SIZE;
	float y0 = placeY * TILE_SIZE;
	float z0 = placeZ * TILE_SIZE;

	float x1 = x0 + TILE_SIZE;
	float y1 = y0 + TILE_SIZE;
	float z1 = z0 + TILE_SIZE;

	//---------------------------------
	// 下の床を確認
	//---------------------------------
	bool canPlace = false;

	// 1つ下に床があるか
	if (placeY > 0)
	{
		if (map.GetMap(placeY - 1, placeZ, placeX) == TILE_FLOOR)
		{
			canPlace = true;
		}
	}

	//---------------------------------
	// 色変更
	//---------------------------------
	int color;

	if (canPlace)
	{
		// 置ける
		color = BLUE;
	}
	else
	{
		// 置けない
		color = RED;
	}

	//---------------------------------
	// 半透明表示
	//---------------------------------
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawCube3D(VGet(x0, y0, z0),VGet(x1, y1, z1),color,color,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//---------------------------------
	// 枠表示
	//---------------------------------
	DrawCube3D(VGet(x0, y0, z0),VGet(x1, y1, z1),
			color,color,FALSE);

}
//操作関係処理
//------------------------------
void CCat::Operation(MapEditor& map)
{
	//---------------------------------
// 元位置保存
//---------------------------------
	VECTOR oldPos = m_vPosition;

	//---------------------------------
	// 入力値
	//---------------------------------
	float forward = 0.0f;
	float side = 0.0f;

	//---------------------------------
	// 回転（地上のみ）
	//---------------------------------
	if (m_moveMode == MOVE_GROUND)
	{
		if (Input::Key::Keep(KEY_INPUT_D))
		{
			m_vRotation.y += ROT_SPEED;
		}

		if (Input::Key::Keep(KEY_INPUT_A))
		{
			m_vRotation.y -= ROT_SPEED;
		}
	}

	//---------------------------------
	// 前後入力
	//---------------------------------
	if (Input::Key::Keep(KEY_INPUT_W))
	{
		forward = -MOVE_SPEED;
	}

	if (Input::Key::Keep(KEY_INPUT_S))
	{
		forward = MOVE_SPEED;
	}

	//---------------------------------
	// 左右入力（壁用）
	//---------------------------------
	if (Input::Key::Keep(KEY_INPUT_D))
	{
		side = MOVE_SPEED;
	}

	if (Input::Key::Keep(KEY_INPUT_A))
	{
		side = -MOVE_SPEED;
	}

	//---------------------------------
	// 向きベクトル
	//---------------------------------
	float sinY = sinf(m_vRotation.y);
	float cosY = cosf(m_vRotation.y);

	//---------------------------------
	// 壁検知
	//---------------------------------
	WallType wall = CheckWall(map);

	//---------------------------------
	// モード切替
	//---------------------------------
	if (wall == WALL_NONE)
	{
		m_moveMode = MOVE_GROUND;
	}
	else if (wall == WALL_X)
	{
		m_moveMode = MOVE_WALL_X;
	}
	else if (wall == WALL_Z)
	{
		m_moveMode = MOVE_WALL_Z;
	}

	//---------------------------------
	// 地面移動
	//---------------------------------
	if (m_moveMode == MOVE_GROUND)
	{
		VECTOR move;
		m_vRotation.x = 0.0f;
		m_vRotation.z = 0.0f;

		move.x = sinY * forward;
		move.y = 0.0f;
		move.z = cosY * forward;

		m_vPosition = VAdd(m_vPosition, move);
	}

	//---------------------------------
	// X方向の壁
	//---------------------------------
	else if (m_moveMode == MOVE_WALL_X)
	{

		m_vRotation.z = DX_PI_F / 2;
		m_vRotation.x = 0.0f;
		
		// 上下移動
		m_vPosition.y -= forward;

		// 左右移動
		m_vPosition.z -= side;

		// 壁に吸着
		int wallX = (int)floor(m_vPosition.x / TILE_SIZE);
		float wallPosX =(wallX + 0.5f)* TILE_SIZE;

		// 壁面固定
		m_vPosition.x = wallPosX - 2.5f;
	}

	//---------------------------------
	// Z方向の壁
	//---------------------------------
	else if (m_moveMode == MOVE_WALL_Z)
	{
		m_vRotation.z = 0.0f;
		m_vRotation.x = DX_PI_F/2;

		// 上下移動
		m_vPosition.y -= forward;

		// 左右移動
		m_vPosition.x += side;

		// 壁に吸着
		int wallZ =(int)floor(m_vPosition.z / TILE_SIZE);
		float wallPosZ =(wallZ + 0.5f)* TILE_SIZE;

		// 壁面固定
		m_vPosition.z =wallPosZ - 2.5f;
	}

	// 地面モードのみ
	if (m_moveMode == MOVE_GROUND)
	{
		if (!CheckGround(map))
		{
			m_vPosition = oldPos;
		}
	}
}

