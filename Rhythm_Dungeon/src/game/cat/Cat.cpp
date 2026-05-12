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
static const float GRAVITY = 0.1f;			// 重力
static const float RADIUS = 5.0f;			// 当たり判定半径

static const char CAT_MODEL_PATH[] = { "Data/Character/Cat/cat.mv1" };
//----------------------------------------


//-------------------------------
//		コンストラクタ
//-------------------------------
CCat::CCat()
{
	m_state = PLAYER_STATE_NORMAL;
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
void CCat::Step()
{
	if (!m_isActive)return;

	Data::GetInstance()->AddScore(1);
	// 状態に合わせて行動変化
	switch (m_state)
	{
	case PLAYER_STATE_NORMAL:
		NormalExec();
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
void CCat::NormalExec()
{
	bool Hit = false;

	// キャラクターの回転
	if (Input::Key::Keep(KEY_INPUT_D)) {
		m_vRotation.y += ROT_SPEED;
		Hit = true;
	}
	else if (Input::Key::Keep(KEY_INPUT_A)) {
		m_vRotation.y -= ROT_SPEED;
		Hit = true;
	}

	// キャラクターの移動
	float fSpd = 0.0f;

	if (Input::Key::Keep(KEY_INPUT_W)) {
		fSpd = -MOVE_SPEED;
		Hit = true;
	}
	else if (Input::Key::Keep(KEY_INPUT_S)) {
		fSpd = MOVE_SPEED;
		Hit = true;
	}
	if (Hit)
	{
		MATRIX ANGLE;

		//プレイヤーの移動を三角形で-----------------
		ANGLE = MyMatrix::GetYawMatrix(m_vRotation.y);
		VECTOR v = VGet(0, 0, fSpd);
		VECTOR ves = MyMatrix::MatTransform(ANGLE, v);
		//計算した速度を座標に足し算する
		m_vPosition = VAdd(m_vPosition, ves);

		VECTOR v1 = VGet(10, 10, 0);
		VECTOR Vc = MyMatrix::MatTransform(ANGLE, v1);
	}
}

//---------------------------------
// ブロック設置
//---------------------------------
void CCat::PlaceBlock(ObjectEditor& objEditor)
{
	int mapX = (int)(m_vPosition.x / TILE_SIZE);
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

	objEditor.AddObject(
		placeX,
		placeZ,
		OBJ_PUT_BOX
	);


}
