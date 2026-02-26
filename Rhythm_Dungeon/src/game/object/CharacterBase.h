#pragma once
#include <DxLib.h>

class CCharacterBase {
public:
	// コンストラクタ・デストラクタ
	CCharacterBase();
	virtual ~CCharacterBase();

protected:

	VECTOR	m_vPosition;	//座標
	VECTOR	m_vRotation;	//回転値
	VECTOR	m_vSpeed;		//移動速度

	int		m_iModelHdl;	//モデルハンドル
	float	m_jumpPow;		//重力
	float	m_animCnt;		//アニメの再生カウント
	int		m_HitCount;		//クールタイム
	int		m_animIndex;	//アニメインデックス
	bool	m_isActive;		//アクティブフラグ

protected:

public:

	//	初期化
	virtual void Init();
	//	ロード
	virtual void Load(int hndl);
	// 更新したデータを反映させる
	virtual void Update();
	//	描画
	virtual void Draw();
	//	終了処理
	virtual void Exit();

	//==============================
	//		ゲッタ・セッタ
	//==============================
	//	座標
	VECTOR GetPosition() { return m_vPosition; }
	void   SetPosition(VECTOR pos) { m_vPosition = pos; }
	//	アクティブフラグ
	bool   IsActiveFlag() { return m_isActive; }
	void   SetActiveFlag(bool isActive) { m_isActive = isActive; }

public:

	typedef struct {
		float m_fEndFrm;		//アニメ全再生時間
		float m_fFrm;			//アニメ再生時間
		float m_fSpd;			//アニメ再生速度
		int   m_iHndl;			//アニメハンドル
		int   m_iID;			//再生中のアニメハンドル
		int   m_iState;			//アニメ状態
	}ANIMEDATA;

private:

	enum AnimeState
	{
		ANIMSTATE_NORMAL,
		ANIMSTATE_LOOP,
		ANIMSTATE_ENDLOOP
	};

	ANIMEDATA	   m_sAnimeData;	//アニメ再生関連データ

public:

	//アニメリクエスト
		//iAnimID		:	アタッチするアニメ番号
		//iAnimSpd		:	アニメ再生速度
		//iAnimSrcHndl	:	アニメーションを使用する場合持っているハンドル(-1ならばこのモデルアニメ使用）
		//NameCheck		:	別モデルのアニメを使用する場合,モデルのフレーム名が一致しない場合
		//				    アニメをアタッチするか
		//void	RequestAnim(int iAnimID);
	virtual void	Request(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl = -1, bool NameCheck = false);
	//ループアニメリクエスト
	//@memo			:	アニメが最終フレームになったら最初に戻ります。
	virtual void	RequestLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl = -1, bool NameCheck = false);
	//ループアニメリクエスト
	//@memo			:	アニメが最終フレームになったらそのまま。
	virtual void	RequestEndLoop(int iAnimID, float iAnimSpd, int hndl, int iAnimSrcHndl = -1, bool NameCheck = false);

	//アニメの更新
	virtual void	AnimeUpdate(int hndl);
	//アニメのデタッチ
	virtual void	DetachAnim(int hndl);

};
