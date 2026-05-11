#include "Vector.h"
#include <math.h>

//加算
VECTOR	CVector::Add(VECTOR vec1, VECTOR vec2)
{
	VECTOR	ret;
	ret.x = vec1.x + vec2.x;
	ret.y = vec1.y + vec2.y;
	ret.z = vec1.z + vec2.z;

	return	ret;
}

//減算
VECTOR	CVector::Subtract(VECTOR vec1, VECTOR vec2)
{
	VECTOR	ret;
	ret.x = vec1.x - vec2.x;
	ret.y = vec1.y - vec2.y;
	ret.z = vec1.z - vec2.z;

	return	ret;
}

//乗算
VECTOR	CVector::Multiply(VECTOR vec1, VECTOR vec2)
{
	VECTOR	ret;
	ret.x = vec1.x * vec2.x;
	ret.y = vec1.y * vec2.y;
	ret.z = vec1.z * vec2.z;

	return	ret;
}

//ベクトルのスカラー倍
VECTOR	CVector::Scale(VECTOR vec1, float scale)
{
	VECTOR	ret;
	ret.x = vec1.x * scale;
	ret.y = vec1.y * scale;
	ret.z = vec1.z * scale;

	return	ret;
}

//初期化
VECTOR	CVector::Identity(void)
{
	VECTOR	ret = { 0.0f, 0.0f, 0.0f };

	return	ret;
}

//ゼロ判定
bool	CVector::IsZero(VECTOR vec)
{
	return	vec.x == 0.0f && vec.y == 0 && vec.z == 0 ? true : false;
}

//値セット
VECTOR	CVector::Set(float x, float y, float z)
{
	VECTOR	ret = { x, y, z };

	return	ret;
}

//ベクトルの長さの二乗
float	CVector::LengthSquare(VECTOR vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

//ベクトルの長さ
float	CVector::Length(VECTOR vec)
{
	return sqrtf(LengthSquare(vec));
}

//ベクトルの長さ
float	CVector::Length(VECTOR vec1, VECTOR vec2)
{
	return Length(Subtract(vec1, vec2));
}

//ベクトルの正規化
VECTOR	CVector::Normalize(VECTOR vec)
{
	// DXライブラリそのまま使用
	return VNorm(vec);
}

//内積
float	CVector::VectorDot(VECTOR vec1, VECTOR vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

//外積
VECTOR	CVector::VectorCross(VECTOR vec1, VECTOR vec2)
{
	VECTOR ret;
	ret.x = vec1.y * vec2.z + vec1.z * vec2.y;
	ret.y = vec1.x * vec2.z + vec1.z * vec2.x;
	ret.z = vec1.x * vec2.y + vec1.y * vec2.x;

	return ret;
}

//X軸回転
VECTOR	CVector::RotateX(VECTOR vec, float radX)
{
	VECTOR	ret;
	ret.x = vec.x;
	ret.y = vec.y * cosf(radX) - vec.z * sinf(radX);
	ret.z = vec.y * sinf(radX) + vec.z * cosf(radX);

	return	ret;
}

//Y軸回転
VECTOR	CVector::RotateY(VECTOR vec, float radY)
{
	VECTOR	ret;
	ret.x = vec.x * cosf(radY) + vec.z * sinf(radY);
	ret.y = vec.y;
	ret.z = vec.z * cosf(radY) - vec.x * sinf(radY);

	return	ret;
}

//Z軸回転
VECTOR	CVector::RotateZ(VECTOR vec, float radZ)
{
	VECTOR	ret;
	ret.x = vec.x * cosf(radZ) - vec.y * sinf(radZ);
	ret.y = vec.x * sinf(radZ) + vec.y * cosf(radZ);
	ret.z = vec.z;

	return	ret;
}

//行列を使ったベクトルの変換
VECTOR	CVector::Transform(VECTOR vec, MATRIX mat)
{
	VECTOR ret;
	ret.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0];
	ret.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1];
	ret.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2];

	return ret;
}

//2点からY軸の角度を取得
float	CVector::GetAngleY(VECTOR vec1, VECTOR vec2)
{
	return atan2f(vec1.x - vec2.x, vec1.z - vec2.z);
}

//2点からZ軸の角度を取得
float	CVector::GetAngleZ(VECTOR vec1, VECTOR vec2)
{
	return atan2f(vec1.y - vec2.y, vec1.x - vec2.x);
}

//移動ベクトルと法線ベクトルから跳ね返りを取得
VECTOR	CVector::Reflect(VECTOR spd, VECTOR norm, float ew, float mew)
{
	VECTOR	ret;
	VECTOR	vp;			// 壁に垂直な速度
	VECTOR	vr;			// 壁に水平な速度
	float	vpl;		// 壁に垂直な方向の速度の大きさ
	float	vrl2;		// 壁に垂直な方向の速度の大きさの二乗
	float	f;

	vpl = CVector::VectorDot(spd, norm);
	vp = CVector::Scale(norm, vpl);
	vr = CVector::Subtract(spd, vp);
	vrl2 = CVector::LengthSquare(vr);
	f = mew * (1.0f * ew) * vpl;
	f *= f;

	if (vrl2 > f)
	{
		f = f / vrl2;
		f = 1.0f - sqrtf(f);
		// 誤差でマイナスになった場合の処理
		if (f < 0.0f) f = 0.0f;
		vr = CVector::Scale(vr, f);
	}
	else
	{
		vr = CVector::Identity();
	}

	vp = CVector::Scale(vp, -ew);
	ret = CVector::Add(vp, vr);

	return ret;
}

//二つの物体から跳ね返りを取得
void CVector::Reflect(VECTOR pos1, VECTOR pos2, VECTOR* spd1, VECTOR* spd2,
	float size1, float size2, float eObj, float mewObj)
{
	VECTOR	v, v21, vp1, vr, vr1, vp2, vr2, dvr, jrMax, jr, jp;
	float	f, jrl, jrlMax2, dvrl, jpl;

	// 物体1の中心から衝突への方向ベクトル取得
	v = CVector::Subtract(pos2, pos1);
	f = 1.0f / (size1 + size2) * size1;
	v = CVector::Scale(v, f);

	// 物体1から見た物体2の速度差取得
	v21 = CVector::Subtract(*spd2, *spd1);
	jpl = (1.0f + eObj) * CVector::VectorDot(v21, v) / (2.f * size1);

	// 物体にかかる横の力積の大きさ / 質量取得
	jrl = jpl * mewObj;

	// 物体1の速度を分解
	vp1 = CVector::Scale(v, CVector::VectorDot(*spd1, v) / (size1 * size1));
	vr1 = CVector::Subtract(*spd1, vp1);
	// 物体2の速度を分解
	vp2 = CVector::Scale(v, CVector::VectorDot(*spd2, v) / (size2 * size2));
	vr2 = CVector::Subtract(*spd2, vp2);

	// vr1とvr2の差分から、「物体1にかかる横の力積ベクトル / 質量」と
	// 「物体の摩擦をなくす力積の大きさ / 質量」の二乗を取得
	dvr = CVector::Subtract(vr2, vr1);
	f = 1.0f / (size1 + size2) * size1;
	jrMax = CVector::Scale(dvr, f);
	jrlMax2 = CVector::LengthSquare(jrMax);

	// 摩擦力が全て働く場合
	if (jrl * jrl <= jrlMax2)
	{
		dvrl = CVector::Length(dvr);
		// 0割りの可能性があるはず
		if (dvrl == 0.0f) jr = CVector::Identity();
		else jr = CVector::Scale(dvr, jrl / dvrl);
		// 物体にかかる横の力積ベクトルをそれぞれに加算
		vr1 = CVector::Add(vr1, jr);
		vr2 = CVector::Subtract(vr2, jr);
	}
	// 摩擦力が途中で消えてしまい、ボールの水平速度が平均になる場合
	else
	{
		vr = CVector::Add(vr1, vr2);
		f = 1.0f / (size1 + size2) * size1;
		// サイズに合わせて反射
		vr1 = CVector::Scale(vr, f);
		vr2 = CVector::Scale(vr, 1.0f - f);
	}
	// 物体のにかける縦の力積ベクトル / 質量
	jp = CVector::Scale(v, jpl / size1);
	vp1 = CVector::Add(vp1, jp);
	vp2 = CVector::Subtract(vp1, jp);

	// 計算結果を速度に渡す
	*spd1 = CVector::Add(vp1, vr1);
	*spd2 = CVector::Add(vp2, vr2);
}