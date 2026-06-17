#include"Cat.h"
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
static const float ANIME_SPEED = 1.0f;		// アニメスピード
static const float MIN_HIGHT = -20.0f;		// リスポーン位置に戻す

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

	CActor::Init();

	m_radius = RADIUS;
	m_isActive = true;
	m_isStairs = false;
	m_stairTargetY = 0.0f;

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

	int hndl = MV1LoadModel(CAT_MODEL_PATH);

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

	if (m_isActive == false)
	{
		m_vPosition = VGet(m_recpos.x, m_recpos.y, m_recpos.z);
		m_isActive = true;
	}

	if (!m_isActive)return;
	//---------------------------------
	// 前フレーム位置保存
	//---------------------------------
	VECTOR oldPos = m_vPosition;

	//---------------------------------
	// 重力
	//---------------------------------
	m_speed.y -= GRAVITY;
	
	NormalExec(map);

	//---------------------------------
	// 移動量
	//---------------------------------
	VECTOR diff = VSub(m_vPosition, oldPos);
	float move = VSize(diff);

	if (m_vPosition.y <= MIN_HIGHT)
	{
		m_isActive = false;
	}
	//---------------------------------
	// 歩き
	//---------------------------------
	if (move > 0.1f)
	{
		if (m_state !=CAT_STATE_WALK)
		{
			RequestLoop(CAT_STATE_WALK,ANIME_SPEED,m_iModelHdl);

			m_state =CAT_STATE_WALK;
		}
	}
	else
	{
		//---------------------------------
		// 待機
		//---------------------------------
		if (m_state != CAT_STATE_NORMAL)
		{
			RequestLoop(CAT_STATE_NORMAL,ANIME_SPEED,m_iModelHdl);

			m_state = CAT_STATE_NORMAL;
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
	CActor::Draw();
	CObject::Draw();

	DrawFormatString(100, 600, RED, "ねこのY座標：%f", m_vPosition.y);


#ifdef MY_DEBUG
	DrawSphere3D(m_vPos, RADIUS, 16, GetColor(0, 0, 255), GetColor(0, 0, 0), FALSE);
#endif
}


//-------------------------------
//		移動計算結果を反映
//-------------------------------
void CCat::Move()
{
	// 移動速度加算
	m_vPosition = VAdd(m_vPosition, m_speed);
}


//-------------------------------
//		待機･移動中処理
//-------------------------------
void CCat::NormalExec(MapEditor& map)
{
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

	Input::Controller::LStickIncline(stickX, stickY);

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
		float targetRot = atan2f(-moveX, -moveZ);

		//---------------------------------
		// 角度差
		//---------------------------------
		float diff = targetRot - m_vRotation.y;

		//---------------------------------
		// -PI～PI補正
		//---------------------------------
		while (diff > DX_PI_F)
		{
			diff -= DX_PI_F * 2.0f;
		}

		while (diff < -DX_PI_F)
		{
			diff += DX_PI_F * 2.0f;
		}

		//---------------------------------
		// 回転
		//---------------------------------
		float rotSpeed = 0.2f;

		m_vRotation.y += diff * rotSpeed;

		//---------------------------------
		// 斜め速度補正
		//---------------------------------
		float len = sqrtf(moveX * moveX + moveZ * moveZ);

		if (len > 0.0f)
		{
			moveX /= len;
			moveZ /= len;
		}

		//---------------------------------
		// 地面移動
		//---------------------------------
		m_vPosition.x += moveX * MOVE_SPEED;
		m_vPosition.z += moveZ * MOVE_SPEED;

		//---------------------------------
		// 階段チェック
		//---------------------------------
		m_isStairs = false;

		// 現在位置
		int mapX =(int)floor(m_vPosition.x / TILE_SIZE);
		int mapZ =(int)floor(m_vPosition.z / TILE_SIZE);
		// 今いる高さ
		int currentY = (int)floor(m_vPosition.y / TILE_SIZE);

		//---------------------------------
		// 前方向
		//---------------------------------
		int dirX = 0;
		int dirZ = 0;

		if (fabs(moveX) > fabs(moveZ))
		{
			dirX =(moveX > 0.0f) ? 1 : -1;
		}
		else
		{
			dirZ =(moveZ > 0.0f) ? 1 : -1;
		}

		// 前マス
		int nextX = mapX + dirX;
		int nextZ = mapZ + dirZ;


		//---------------------------------
		// 1段上を見る
		//---------------------------------
		int frontTile = map.GetMap(currentY, nextZ, nextX);

		//---------------------------------
		// 階段
		//--------------------------------
		if (frontTile == TILE_STAIRS)
		{
			m_isStairs = true;

			//---------------------------------
			// 階段の高さへ
			//---------------------------------
			m_stairTargetY = (currentY + 1) * TILE_SIZE;
		}

		//---------------------------------
		// 階段Y補間
		//---------------------------------
		if (m_isStairs)
		{
			float diffY = m_stairTargetY - m_vPosition.y;

			// ゆっくり登る
			m_vPosition.y += diffY * 0.15f;

			// ガタ防止
			if (fabs(diffY) < 0.05f)
			{
				m_vPosition.y =m_stairTargetY;
			}
		}
	}
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
	int mapX = (int)floor(m_vPosition.x / TILE_SIZE);
	int mapY = (int)floor(m_vPosition.y / TILE_SIZE);
	int mapZ = (int)floor(m_vPosition.z / TILE_SIZE);

	//---------------------------------
	// 足元
	//---------------------------------
	int footY = mapY - 1;

	//---------------------------------
	// 範囲外防止
	//---------------------------------
	if (mapX < 0 || mapX >= MAP_W ||
		footY < 0 || footY >= MAP_Y ||
		mapZ < 0 || mapZ >= MAP_H)
	{
		return false;
	}

	//---------------------------------
	// 床があるか
	//---------------------------------
	if (map.GetMap(footY,mapZ,mapX) == TILE_FLOOR)
	{
		return true;
	}
	if (map.GetMap(footY,mapZ,mapX) == TILE_FLOOR2)
	{
		return true;
	}
	if (map.GetMap(footY,mapZ,mapX) == TILE_BRIDGE)
	{
		return true;
	}
	if (map.GetMap(footY,mapZ,mapX) == TILE_STAIRS)
	{
		return true;
	}

	return false;
}


//------------------------------
// ブロック設置位置表示
//------------------------------
void CCat::DrawPlaceBlockPreview(MapEditor& map)
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
	float x = placeX * TILE_SIZE + TILE_SIZE / 2;
	float y = m_vPosition.y;//placeY * TILE_SIZE + TILE_SIZE/2;
	float z = placeZ * TILE_SIZE + TILE_SIZE / 2;

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

	VECTOR vec = VGet(x, y - TILE_SIZE/2, z);

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
	//front
	//---------------------------------
	// 左F
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

//猫のクリア処理
void CCat::Clear()
{
	RequestLoop(CAT_STATE_DANCE, ANIME_SPEED, m_iModelHdl);
	m_state = CAT_STATE_DANCE;
}
