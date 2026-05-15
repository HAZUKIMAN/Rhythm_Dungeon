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
	m_state = PLAYER_STATE_NORMAL;
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


	if (Input::Key::Push(KEY_INPUT_M))
	{
		switch (m_moveMode)
		{
		case MOVE_GROUND:
			m_moveMode = MOVE_WALL;
			break;

		case MOVE_WALL:
			m_moveMode = MOVE_GROUND;
			break;
		}
	}

	// 状態に合わせて行動変化
	switch (m_state)
	{
	case PLAYER_STATE_NORMAL:
		NormalExec(map);
		break;
	case PLAYER_STATE_JUMP:
		
		break;
	}

	// 以下共通処理-----------------------------
	// Zキーで弾を発射
	if (Input::Key::Push(KEY_INPUT_Z))
	{
		// プレイヤーの体から出るように座標を上げる
		VECTOR pos = m_vPosition;
		pos.y += 5.0f;
		// 速度はプレイヤーと同じ方法で移動方向を決める
		VECTOR spd;
		const float SHOT_SPEED = 5.0f;
		spd.x = sinf(m_vRotation.y) * -SHOT_SPEED;
		spd.z = cosf(m_vRotation.y) * -SHOT_SPEED;
		spd.y = 0.0f;
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
	//---------------------------------
	// 移動前座標保存
	//---------------------------------
	VECTOR oldPos = m_vPosition;

	bool isMove = false;

	//---------------------------------
	// 左右移動量
	//---------------------------------
	float sideMove = 0.0f;

	//---------------------------------
	// 回転
	//---------------------------------
	if (Input::Key::Keep(KEY_INPUT_D))
	{
		if(m_moveMode == MOVE_GROUND)m_vRotation.y += ROT_SPEED;

		// 壁移動時の左右
		sideMove += MOVE_SPEED;
		isMove = true;
	}

	if (Input::Key::Keep(KEY_INPUT_A))
	{
		if (m_moveMode == MOVE_GROUND)m_vRotation.y -= ROT_SPEED;

		// 壁移動時の左右
		sideMove -= MOVE_SPEED;
		isMove = true;
	}

	//---------------------------------
	// 前後移動
	//---------------------------------
	float fSpd = 0.0f;

	if (Input::Key::Keep(KEY_INPUT_W))
	{
		fSpd = -MOVE_SPEED;
		isMove = true;
	}

	if (Input::Key::Keep(KEY_INPUT_S))
	{
		fSpd = MOVE_SPEED;
		isMove = true;
	}

	//---------------------------------
	// 移動処理
	//---------------------------------
	if (isMove)
	{
		MATRIX ANGLE;

		// Y回転行列
		ANGLE = MyMatrix::GetYawMatrix(m_vRotation.y);

		// 地上移動-
		if (m_moveMode == MOVE_GROUND)
		{
			// 前後移動
			VECTOR forward = VGet(0, 0, fSpd);
			// 回転適用
			VECTOR move = MyMatrix::MatTransform(ANGLE, forward);
			// 移動
			m_vPosition = VAdd(m_vPosition, move);
		}

		// Z回転行列
		ANGLE = MyMatrix::GetYawMatrix(m_vRotation.z);

		//---------------------------------
		// 壁歩きモード
		//---------------------------------
		if (m_moveMode == MOVE_WALL)
		{
			// 上下移動
			VECTOR upMove = VGet(0, -fSpd, 0);
			// 向き反映
			VECTOR move = MyMatrix::MatTransform(ANGLE, upMove);
			// 左右移動
			move.x += sideMove;
			// 移動
			m_vPosition = VAdd(m_vPosition, move);
		}

		//---------------------------------
		// 現在のマス取得
		//---------------------------------
		int mapX = (int)floor(m_vPosition.x / TILE_SIZE);
		int mapY = (int)floor(m_vPosition.y / TILE_SIZE);
		int mapZ = (int)floor(m_vPosition.z / TILE_SIZE);

		// 範囲外なら戻す
		if (mapX < 0 || mapX >= MAP_W ||
			mapY < 0 || mapY >= MAP_Y ||
			mapZ < 0 || mapZ >= MAP_H)
		{
			m_vPosition = oldPos;
			return;
		}
		//---------------------------------
		// 地上モード
		//---------------------------------
		if (m_moveMode == MOVE_GROUND)
		{
			// 足元の床を見る
			int footY = mapY - 1;
			// 床が無いなら戻す
			if (footY < 0 || map.GetMap(footY, mapZ, mapX)!= TILE_FLOOR)
			{
				m_vPosition = oldPos;
			}
		}
		//---------------------------------
		// 壁モード
		//---------------------------------
		if (m_moveMode == MOVE_WALL)
		{
			// 壁が無いなら戻す
			if (map.GetMap(mapY, mapZ, mapX)!= TILE_WALL)
			{
				m_vPosition = oldPos;
			}
		}
	}

	//壁が周囲１マス以内の時
	if (CheckWall(map))
	{
	// モード切替
		if (Input::Key::Push(KEY_INPUT_M))
		{
			if (m_moveMode == MOVE_GROUND)
			{
				// 壁歩き用
				m_vRotation.x = DX_PI_F / 2;
			}
		}
	}
	// 地上用
	else
	{
		m_moveMode = MOVE_GROUND;
		m_vRotation.x = 0.0f;
	}

}


//---------------------------------
// ブロック設置
//---------------------------------
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


//---------------------------------
// 壁チェック
//---------------------------------
bool CCat::CheckWall(MapEditor& map)
{
	//---------------------------------
	// 判定距離
	//---------------------------------
	float checkRadius = TILE_SIZE /** 0.5f*/;

	//---------------------------------
	// チェック位置
	//---------------------------------
	VECTOR checkPos[6];

	// 右
	checkPos[0] = VGet(m_vPosition.x + checkRadius, m_vPosition.y, m_vPosition.z);
	// 左
	checkPos[1] = VGet(m_vPosition.x - checkRadius, m_vPosition.y, m_vPosition.z);
	// 上
	checkPos[2] = VGet(m_vPosition.x, m_vPosition.y + checkRadius, m_vPosition.z);
	// 下
	checkPos[3] = VGet(m_vPosition.x, m_vPosition.y - checkRadius, m_vPosition.z);
	// 前
	checkPos[4] = VGet(m_vPosition.x, m_vPosition.y, m_vPosition.z + checkRadius);
	// 後
	checkPos[5] = VGet(m_vPosition.x, m_vPosition.y, m_vPosition.z - checkRadius);

	// 6方向チェック
	for (int i = 0; i < 6; i++)
	{
		int mapX = (int)floor(checkPos[i].x / TILE_SIZE);
		int mapY = (int)floor(checkPos[i].y / TILE_SIZE);
		int mapZ = (int)floor(checkPos[i].z / TILE_SIZE);

		// 範囲外
		if (mapX < 0 || mapX >= MAP_W ||
			mapY < 0 || mapY >= MAP_Y ||
			mapZ < 0 || mapZ >= MAP_H)
		{
			continue;
		}

		// 壁判定
		if (map.GetMap(mapY, mapZ, mapX) == TILE_WALL)
		{
			return true;
		}
	}

	return false;
}


//---------------------------------
// ブロック設置位置表示
//---------------------------------
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
	DrawCube3D(
		VGet(x0, y0, z0),VGet(x1, y1, z1),
		color,color,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//---------------------------------
	// 枠表示
	//---------------------------------
	DrawCube3D(VGet(x0, y0, z0),VGet(x1, y1, z1),
			color,color,FALSE);

	}

