#include "Vector.h"
#include <math.h>

//---------------------------------
//		加算
//---------------------------------
VECTOR	CVector::Add( VECTOR vec1, VECTOR vec2 )
{
	VECTOR	ret;
	ret.x	=	vec1.x + vec2.x;
	ret.y	=	vec1.y + vec2.y;
	ret.z	=	vec1.z + vec2.z;

	return	ret;
}


//---------------------------------
//		減算
//---------------------------------
VECTOR	CVector::Subtract( VECTOR vec1, VECTOR vec2 )
{
	VECTOR	ret;
	ret.x	=	vec1.x - vec2.x;
	ret.y	=	vec1.y - vec2.y;
	ret.z	=	vec1.z - vec2.z;

	return	ret;
}


//---------------------------------
//		ベクトルのスカラー倍
//---------------------------------
VECTOR	CVector::Scale( VECTOR vec1, float scale )
{
	VECTOR	ret;
	ret.x	=	vec1.x * scale;
	ret.y	=	vec1.y * scale;
	ret.z	=	vec1.z * scale;

	return	ret;
}


//---------------------------------
//		内積
//---------------------------------
float	CVector::VectorDot(VECTOR vec1, VECTOR vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}


//---------------------------------
//		外積
//---------------------------------
VECTOR	CVector::VectorCross(VECTOR vec1, VECTOR vec2)
{
	VECTOR ret;
	ret.x = vec1.y * vec2.z - vec1.z * vec2.y;
	ret.y = vec1.z * vec2.x - vec1.x * vec2.z;
	ret.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return ret;
}


//---------------------------------
//		ベクトルの長さの二乗
//---------------------------------
float	CVector::LengthSquare(VECTOR vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}


//---------------------------------
//		ベクトルの長さ
//---------------------------------
float	CVector::Length(VECTOR vec)
{
	return sqrtf(LengthSquare(vec));
}


//---------------------------------
//		2点間のベクトルの距離
//---------------------------------
float	CVector::Distance(VECTOR vec1, VECTOR vec2)
{
	return Length(Subtract(vec1, vec2));
}


//---------------------------------
//		ベクトルの正規化
//---------------------------------
VECTOR	CVector::Normalize(VECTOR vec)
{
	VECTOR vRet;
	float size = Length(vec);
	vRet.x = vec.x / size;
	vRet.y = vec.y / size;
	vRet.z = vec.z / size;
	return vRet;
}


//---------------------------------
//		初期化
//---------------------------------
VECTOR	CVector::Identity( void )
{
	VECTOR	ret	=	{ 0.f, 0.f, 0.f };

	return	ret;
}


//---------------------------------
//		ゼロ判定
//---------------------------------
bool	CVector::IsZero( VECTOR vec )
{
	return	vec.x == 0.f && vec.y == 0 && vec.z == 0 ? true : false ;
}


//---------------------------------
//		値セット
//---------------------------------
VECTOR	CVector::Set( float x, float y, float z )
{
	VECTOR	ret	=	{ x, y, z };

	return	ret;
}