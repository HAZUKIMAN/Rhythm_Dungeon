#pragma once
#include "DxLib.h"


//===========================================
//		ベクトル計算クラス
//===========================================


class CVector
{
public:

	//		加算
	static VECTOR	Add( VECTOR vec1, VECTOR vec2 );

	//		減算( vec1 - vec2 )
	static VECTOR	Subtract( VECTOR vec1, VECTOR vec2 );

	//		ベクトルのスカラー倍
	static VECTOR	Scale(VECTOR vec1, float scale);

	//		内積
	static float	VectorDot(VECTOR vec1, VECTOR vec2);

	//		外積
	static VECTOR	VectorCross(VECTOR vec1, VECTOR vec2);

	//		ベクトルの長さの二乗
	static float	LengthSquare( VECTOR vec );

	//		ベクトルの長さ
	static float	Length( VECTOR vec );

	//		2点間のベクトルの距離
	static float	Distance( VECTOR vec1, VECTOR vec2 );

	//		ベクトルの正規化
	static VECTOR	Normalize( VECTOR );

	//		初期化
	static VECTOR	Identity();

	//		ゼロ判定
	static bool		IsZero( VECTOR vec );

	//		値セット
	static VECTOR	Set( float x, float y, float z );
};