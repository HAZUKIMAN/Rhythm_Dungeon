#include "Cat.h"
#include <math.h>

#include "../../Data.h"
#include "../../lib/Input/Input.h"
#include "../Anime/Anime.h"
#include "../common.h"
#include "../../lib/math/MyMatrix.h"

//	定義関連------------------------------
static const float MOVE_SPEED = 0.25f;		// 移動速度
static const float ROT_SPEED = 0.1f;		// 回転速度
static const float JUMP_POWER = 5.0f;		// ジャンプ力
static const float GRAVITY = 0.01f;			// 重力
static const float RADIUS = 5.0f;			// 当たり判定半径
static const float ANIME_SPEED = 1.0f;	// アニメスピード

static const char CAT_MODEL_PATH[]	 = { "Data/Character/Cat/cat.mv1" };
static const char PUTNO_MODEL_PATH[] = { "Data/object/put/Put_No.mv1" };
static const char PUTOK_MODEL_PATH[] = { "Data/object/put/Put_Ok.mv1" };

//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CCat::CCat()
{
	Init();

	m_iPutModel[0] = -1;
	m_iPutModel[1] = -1;
}


//-------------------------------
//		デストラクタ
//-------------------------------
CCat::~CCat()
{
	// 本来は必要ないけど、念のため
	Fin();

	DetachAnim(m_iModelHdl);

	if (m_iPutModel[0] != -1)
	{
		MV1DeleteModel(m_iPutModel[0]);
		m_iPutModel[0] = -1;
	}

	if (m_iPutModel[1] != -1)
	{
		MV1DeleteModel(m_iPutModel[1]);
		m_iPutModel[1] = -1;
	}
}


//-------------------------------
//		初期化
//-------------------------------
void CCat::Init()
{
	m_state = CAT_STATE_NORMAL;
	m_moveMode = MOVE_GROUND;
	m_radius = RADIUS;
	m_isActive = true;

	m_iPutModel[0] = MV1LoadModel(PUTNO_MODEL_PATH);
	m_iPutModel[1] = MV1LoadModel(PUTOK_MODEL_PATH);
}


//-------------------------------
//		データロード
//-------------------------------
void CCat::Load()
{
	VECTOR size = VGet(0.02f, 0.02f, 0.02f);
	VECTOR Size = VGet(0.05f, 0.05f, 0.05f);

	int hndl= MV1LoadModel(CAT_MODEL_PATH);

	MV1SetScale(hndl, size);

	MV1SetScale(m_iPutModel[0], Size);
	MV1SetScale(m_iPutModel[1], Size);
	CObject::Load(hndl);

	RequestLoop(CAT_STATE_NORMAL, ANIME_SPEED, m_iModelHdl);
	m_state = CAT_STATE_NORMAL;
}


//-------------------------------
//		毎フレーム呼ぶ処理
//-------------------------------
void CCat::Step(MapEditor& map)
{
	if (!m_isActive)return;

	//---------------------------------
	// 前フレーム位置保存
	//---------------------------------
	VECTOR oldPos =m_vPosition;

	//---------------------------------
	// 通常行動
	//---------------------------------
	NormalExec(map);

	//---------------------------------
	// 移動
	//---------------------------------
	Move();

	//---------------------------------
	// 移動量
	//---------------------------------
	VECTOR diff =VSub(m_vPosition,oldPos);

	float move =VSize(diff);

	//---------------------------------
	// 歩き
	//---------------------------------
	if (move > 0.1f)
	{
		if (m_state !=
			CAT_STATE_WALK)
		{
			RequestLoop(
				CAT_STATE_WALK,
				ANIME_SPEED,
				m_iModelHdl);

			m_state =
				CAT_STATE_WALK;
		}
	}
	else
	{
		//---------------------------------
		// 待機
		//---------------------------------
		if (m_state !=
			CAT_STATE_NORMAL)
		{
			RequestLoop(
				CAT_STATE_NORMAL,
				ANIME_SPEED,
				m_iModelHdl);

			m_state =
				CAT_STATE_NORMAL;
		}
	}

	//---------------------------------
	// 移動処理
	//---------------------------------
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
	//	重力処理
	m_speed.y -= GRAVITY;
	
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
	if (map.GetMap(
		footY,
		mapZ,
		mapX) == TILE_FLOOR2)
	{
		return true;
	}
	if (map.GetMap(
		footY,
		mapZ,
		mapX) == TILE_BRIDGE)
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
	float x = placeX * TILE_SIZE + TILE_SIZE/2;
	float y = TILE_SIZE / 2;//placeY * TILE_SIZE + TILE_SIZE/2;
	float z = placeZ * TILE_SIZE + TILE_SIZE/2;

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
		if (map.GetMap(placeY - 1, placeZ, placeX) == TILE_FLOOR2)
		{
			canPlace = true;
		}
		if (map.GetMap(placeY - 1, placeZ, placeX) == TILE_BRIDGE)
		{
			canPlace = true;
		}
	}

	VECTOR vec = VGet(x, y, z);

	if (canPlace)
	{
		MV1DrawModel(m_iPutModel[1]);
		MV1SetPosition(m_iPutModel[1], vec);
	}
	else
	{
		MV1DrawModel(m_iPutModel[0]);
		MV1SetPosition(m_iPutModel[0], vec);
	}

}


//操作関係処理
//------------------------------
void CCat::Operation(MapEditor& map)
{
	//---------------------------------
	// 元位置保存
	//---------------------------------
	VECTOR oldPos =m_vPosition;

	//---------------------------------
	// 入力値
	//---------------------------------
	float moveX = 0.0f;
	float moveZ = 0.0f;

	//---------------------------------
	// 左スティック取得
	//---------------------------------
	float stickX = 0.0f;
	float stickY = 0.0f;

	Input::Controller::LStickIncline(stickX,stickY);

	//---------------------------------
	// キーボード入力
	//---------------------------------
	if (Input::Key::Keep(KEY_INPUT_A))
	{
		moveX -= 1.0f;
	}

	if (Input::Key::Keep(KEY_INPUT_D))
	{
		moveX += 1.0f;
	}

	if (Input::Key::Keep(KEY_INPUT_W))
	{
		moveZ += 1.0f;
	}

	if (Input::Key::Keep(KEY_INPUT_S))
	{
		moveZ -= 1.0f;
	}

	//---------------------------------
	// DPad入力
	//---------------------------------
	if (Input::Controller::Keep(XINPUT_BUTTON_DPAD_LEFT))
	{
		moveX -= 1.0f;

	}

	if (Input::Controller::Keep(XINPUT_BUTTON_DPAD_RIGHT))
	{
		moveX += 1.0f;
	}

	if (Input::Controller::Keep(XINPUT_BUTTON_DPAD_UP))
	{
		moveZ += 1.0f;
	}

	if (Input::Controller::Keep(XINPUT_BUTTON_DPAD_DOWN))
	{
		moveZ -= 1.0f;
	}

	//---------------------------------
	// 左スティック優先
	//---------------------------------
	if (fabs(stickX) > 0.1f)
	{
		moveX = stickX;
	}

	if (fabs(stickY) > 0.1f)
	{
		moveZ = stickY;
	}



	//---------------------------------
	// 入力がある
	//---------------------------------
	if (fabs(moveX) > 0.01f ||
		fabs(moveZ) > 0.01f)
	{
		//---------------------------------
		// 目標角度
		//---------------------------------
		float targetRot =atan2f(-moveX,-moveZ);

		//---------------------------------
		// 角度差
		//---------------------------------
		float diff =targetRot - m_vRotation.y;

		//---------------------------------
		// -PI～PI補正
		//---------------------------------
		while (diff > DX_PI_F)
		{
			diff -=DX_PI_F* 2.0f;
		}

		while (diff < -DX_PI_F)
		{
			diff +=DX_PI_F* 2.0f;
		}

		//---------------------------------
		// ヌルっと回転
		//---------------------------------
		float rotSpeed =0.2f;

		m_vRotation.y +=diff* rotSpeed;

		//---------------------------------
		// 斜め速度補正
		//---------------------------------
		float len =sqrtf(moveX * moveX +moveZ * moveZ);

		if (len > 0.0f)
		{
			moveX /= len;
			moveZ /= len;
		}

		//---------------------------------
		// 地面移動
		//---------------------------------
		if (m_moveMode== MOVE_GROUND)
		{
			m_vRotation.x =0.0f;
			m_vRotation.z =0.0f;

			m_vPosition.x +=moveX* MOVE_SPEED;
			m_vPosition.z +=moveZ* MOVE_SPEED;
		}
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
		m_moveMode =MOVE_GROUND;
	}
	else if (wall == WALL_X)
	{
		m_moveMode =MOVE_WALL_X;
	}
	else if (wall == WALL_Z)
	{
		m_moveMode =MOVE_WALL_Z;
	}

	//---------------------------------
	// X方向壁
	//---------------------------------
	if (m_moveMode ==MOVE_WALL_X)
	{
		m_vRotation.z = DX_PI_F / 2;
		m_vRotation.x = 0.0f;
		m_vPosition.y -= moveZ* MOVE_SPEED;

		int wallX = (int)floor(m_vPosition.x/ TILE_SIZE);
		float wallPosX = (wallX + 0.5f)* TILE_SIZE;

		m_vPosition.x = wallPosX- 2.5f;
	}

	//---------------------------------
	// Z方向壁
	//---------------------------------
	else if (m_moveMode ==MOVE_WALL_Z)
	{
		m_vRotation.z = 0.0f;
		m_vRotation.x = DX_PI_F / 2;
		m_vPosition.y -= moveZ* MOVE_SPEED;

		int wallZ = (int)floor(m_vPosition.z/ TILE_SIZE);
		float wallPosZ = (wallZ + 0.5f)* TILE_SIZE;
		m_vPosition.z = wallPosZ- 2.5f;
	}

	//---------------------------------
	// 落下防止
	//---------------------------------
	if (m_moveMode== MOVE_GROUND)
	{
		if (!CheckGround(map))
		{
			m_vPosition = oldPos;
		}
	}
}

//--------------------------------------
// 猫の向き取得
//--------------------------------------
int CCat::GetDirection()
{
	float rot = m_vRotation.y;

	//---------------------------------
	// 右
	//---------------------------------
	if (fabs(rot - (-DX_PI_F / 2)) < 0.1f)
	{
		return ROTATION_RIGHT;
	}

	//---------------------------------
	// 下
	//---------------------------------
	if (fabs(rot - 0.0f) < 0.1f)
	{
		return ROTATION_DOWN;
	}

	//---------------------------------
	// 左
	//---------------------------------
	if (fabs(rot - (DX_PI_F / 2)) < 0.1f)
	{
		return ROTATION_LEFT;
	}

	//---------------------------------
	// 上
	//---------------------------------
	return ROTATION_UP;
}

