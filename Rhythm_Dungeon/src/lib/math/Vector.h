#pragma once
#include "DxLib.h"


//ベクトル計算クラス
class CVector
{
public:

	//加算
	static VECTOR	Add(VECTOR vec1, VECTOR vec2);

	//減算( vec1 - vec2 )
	static VECTOR	Subtract(VECTOR vec1, VECTOR vec2);

	//乗算
	static VECTOR	Multiply(VECTOR vec1, VECTOR vec2);

	//ベクトルのスカラー倍
	static VECTOR	Scale(VECTOR vec1, float scale);

	//初期化
	static VECTOR	Identity();

	//ゼロ判定
	static bool		IsZero(VECTOR vec);

	//値セット
	static VECTOR	Set(float x, float y, float z);

	//ベクトルの長さの二乗
	static float	LengthSquare(VECTOR vec);

	//ベクトルの長さ
	static float	Length(VECTOR vec);

	//ベクトルの長さ
	static float	Length(VECTOR vec1, VECTOR vec2);

	//ベクトルの正規化
	static VECTOR	Normalize(VECTOR vec);

	//内積
	static float	VectorDot(VECTOR vec1, VECTOR vec2);

	//外積
	static VECTOR	VectorCross(VECTOR vec1, VECTOR vec2);

	//X軸回転
	// vec		:	回転させるベクトル
	// radX		:	X軸回転させる角度(ラジアン角)
	static VECTOR	RotateX(VECTOR vec, float radX);
	//Y軸回転
	// vec		:	回転させるベクトル
	// radZ		:	Y軸回転させる角度(ラジアン角)
	static VECTOR	RotateY(VECTOR vec, float radY);
	//Z軸回転
	// vec		:	回転させるベクトル
	// radZ		:	Z軸回転させる角度(ラジアン角)
	static VECTOR	RotateZ(VECTOR vec, float radZ);

	//行列を使ったベクトルの変換
	static VECTOR	Transform(VECTOR vec, MATRIX mat);

	//2点からY軸の角度を取得
	// vec1,vec2	:	角度を求める2点
	// @return		:	Y軸の角度
	static float	GetAngleY(VECTOR vec1, VECTOR vec2);
	//2点からZ軸の角度を取得
	// vec1,vec2	:	角度を求める2点
	// @return		:	Z軸の角度
	static float	GetAngleZ(VECTOR vec1, VECTOR vec2);

	//移動ベクトルと法線ベクトルから跳ね返りを取得
	//(壁などとの衝突用)
	//	vSpe	:	反射する物体の移動速度
	//	norm	:	ぶつかる物体の法線
	//	ew		:	跳ね返り係数
	//	mew		:	摩擦係数
	static VECTOR	Reflect(VECTOR spd, VECTOR norm, float ew, float mew);

	//二つの物体から跳ね返りを取得
	//	pos1,pos2		:	二つの物体の位置
	//	spd1,spd2		:	二つの物体の移動速度(反射後の速度に書き換えられる)
	//	size1,size2	:	二つの物体のサイズ
	//	eObj			:	二つの物体の跳ね返り係数
	//	mewObj			:	二つの物体の摩擦係数
	static void	Reflect(VECTOR pos1, VECTOR pos2, VECTOR* spd1, VECTOR* spd2,
		float size1, float size2, float eObj, float mewObj);
};