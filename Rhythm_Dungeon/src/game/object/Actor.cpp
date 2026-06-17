#include "Actor.h"
#include "../../lib/object/Object.h"
#include "../common.h"


//-------------------------------
//		コンストラクタ
//-------------------------------
CActor::CActor()
    : m_speed(VGet(0.0f, 0.0f, 0.0f))
    , m_radius(2.5f)
    , m_hitCoolTime(HIT_TIME)
    , m_shadowHdl(-1)
{
}

//-------------------------------
//		デストラクタ
//-------------------------------
CActor::~CActor()
{
    DetachAnim(m_iModelHdl);

    if (m_shadowHdl != -1)
    {
        MV1DeleteModel(m_shadowHdl);
        m_shadowHdl = -1;
    }
}

//-------------------------------
//		初期化
//-------------------------------
void CActor::Init()
{
    CObject::Init();

    //---------------------------------
    // ドロップシャドウ生成
    //---------------------------------
    m_shadowHdl = MV1LoadModel("Data/object/shadow/shadow.mv1");

    MV1SetScale(m_shadowHdl, VGet(0.03f, 0.03f, 0.03f));
    MV1SetMaterialDifColor(m_shadowHdl, 0, GetColorF(0, 0, 0, 0.5f));
    MV1SetMaterialSpcColor(m_shadowHdl, 0, GetColorF(0, 0, 0, 0.0f));
}


//------------------------------
//		ゲームメイン更新
//------------------------------
void CActor::Draw()
{
    //---------------------------------
    // ドロップシャドウ更新
    //---------------------------------
    if (m_shadowHdl != -1)
    {
        VECTOR shadowPos = m_vPosition;
        shadowPos.y = m_vPosition.y - 2.5f; // 地面固定

        MV1SetPosition(m_shadowHdl, shadowPos);

        // 高さで影を薄くする
        float alpha = 1.0f - (m_vPosition.y / 20.0f);
        if (alpha < 0.2f) alpha = 0.2f;
        if (alpha > 0.8f) alpha = 0.8f;

        MV1SetMaterialDifColor(m_shadowHdl, 0, GetColorF(0, 0, 0, alpha));

        MV1DrawModel(m_shadowHdl);
    }
}


//------------------------------
//		ゲームメイン更新
//------------------------------
void CActor::Update()
{
    //---------------------------------
    // クールタイム減少
    //---------------------------------
    if (m_hitCoolTime > 0)
    {
        m_hitCoolTime--;
    }

    //---------------------------------
    // モデル位置（足元補正）
    //---------------------------------
    VECTOR model_vec = VGet( m_vPosition.x, m_vPosition.y - 2.5f, m_vPosition.z);

    AnimeUpdate(m_iModelHdl);
    MV1SetRotationXYZ(m_iModelHdl, m_vRotation);
    MV1SetPosition(m_iModelHdl, model_vec);

}

//-------------------------------
//		当たり判定中心取得
//-------------------------------
VECTOR CActor::GetCenter()
{
    VECTOR pos = m_vPosition;
    pos.y += m_radius;

    return pos;
}

//-------------------------------
//		押し戻し
//-------------------------------
void CActor::AddPos(VECTOR Hit)
{
    if (Hit.x == 0.0f &&
        Hit.y == 0.0f &&
        Hit.z == 0.0f) return;

    m_vPosition = VAdd(m_vPosition, Hit);

    m_speed.y = 0;

    MV1SetPosition(m_iModelHdl, m_vPosition);
}

