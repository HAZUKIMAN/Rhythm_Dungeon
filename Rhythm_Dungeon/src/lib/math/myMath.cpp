#include "myMath.h"
#define _USE_MATH_DEFINES
#include <math.h>

MATRIX MyMath::m_matrix[(int)MyMath::Matrix_Index::Matrix_Max_Num];

//２から１への進行方向と移動量：向かう場所XY1  動かしたいものXY2  XとYのどちらを出すか  増加量
float MyMath::GetDirection(float x1, float y1, float x2, float y2, char moji, float s)
{
	float x = x1 - x2;
	float y = y1 - y2;

	float n = x * x + y * y;

	n = sqrtf(n);

	if (moji == 'x')
	{
		return x / n * s;
	}
	else if (moji == 'y')
	{
		return y / n * s;
	}
	return 0;
}

//1から2への角度(ラジアン)：開始地点XY　終了地点XY
float MyMath::GetAngle(float Ax, float Ay, float Bx, float By)
{
	float num = 0.0f;
	num = atan2f(Ay - By, Ax - Bx);

	return num;
}

//ベクトルの内積を求めて正か負を返す：向かせる場所XY　向かせたいものXY
bool MyMath::GetInnerProduct(float Ax, float Ay, float Bx, float By)
{
	float num = 0.0f;

	num = Ax * Bx + Ay * By;
	if (num > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//度をラジアンに変換（引数：度）
float MyMath::DegToRad(float degree)
{
	return degree * (float)M_PI / 180.0f;
}

//長さの差を返す
float MyMath::GetDefLen_f(float pos_A, float pos_B) { return fabsf(pos_A - pos_B); }

//≒
bool MyMath::NearyEqual(float num1, float num2, float error_num)
{
	if (fabsf(num1 - num2) < error_num)
		return true;

	return false;
}

//桁数を返す
int MyMath::GetNumLen(int num)
{
	int len = 0;

	if (num == 0)
		return 1;
	else
		while (num != 0)
		{
			num /= 10;
			len++;
		}

	return len;
}

//2点間の距離
float MyMath::GetDistance(VECTOR vec1, VECTOR vec2) {
	VECTOR distance_vec = { vec1.x - vec2.x,vec1.y - vec2.y ,vec1.z - vec2.z };

	return VSize(distance_vec);
}

//連立方程式
VECTOR MyMath::Simultaneous_Equations(float a1, float b1, float a2, float b2)
{
	VECTOR Answer = { 0 };

	Answer.x = (b2 - b1) / (a1 - a2);
	Answer.y = a1 * Answer.x + b1;

	return Answer;
}

//引数で受け取ったベクトルを指定の長さにして返す
VECTOR MyMath::GetVector(VECTOR vec, float speed)
{
	VECTOR vResult = { 0 };
	if (VSize(vec) != 0) {
		vResult = VNorm(vec);
		vResult = VScale(vResult, speed);
	}
	return vResult;
}

//現在地から目的地まで指定の速さで動くベクトル
VECTOR MyMath::GetVector(VECTOR vec_here, VECTOR vec_dis, float speed)
{
	//現在地から目的地のベクトル
	VECTOR sub_vec = VSub(vec_dis, vec_here);

	//正規化
	VECTOR nor_vec = VNorm(sub_vec);

	//取得するベクトルの大きさ
	VECTOR mul_vec = VScale(nor_vec, speed);

	return mul_vec;
}

//外積
float MyMath::VCross2D(VECTOR vec1, VECTOR vec2)
{
	return (vec1.x * vec2.y - vec1.y * vec2.x);
}

//ベクトルのなす角
float MyMath::AngleOf2Vector(VECTOR vec1, VECTOR vec2, bool degree_switch)
{
	//ベクトルの長さ
	double len1 = VSize(vec1);
	double len2 = VSize(vec2);

	//cosθ
	double cos_sita = VDot(vec1, vec2) / (len1 * len2);

	//θ
	double sita = 0.0;
	if (cos_sita <= 1)
		sita = acos(cos_sita);

	//角度
	if (degree_switch)
		sita = sita * 180.0 / DX_PI;

	return (float)sita;
}

//回転行列
void MyMath::RotaMatrix(VECTOR& v_pos, VECTOR v_ref_pos, VECTOR v_rota)
{
	//原点に戻す行列
	m_matrix[(int)Matrix_Index::Trans_Origin] = MGetTranslate(VGet(-v_ref_pos.x, -v_ref_pos.y, -v_ref_pos.z));

	//回転行列
	m_matrix[(int)Matrix_Index::Rotation_X] = MGetRotX(v_rota.x);
	m_matrix[(int)Matrix_Index::Rotation_Y] = MGetRotY(v_rota.y);
	m_matrix[(int)Matrix_Index::Rotation_Z] = MGetRotZ(v_rota.z);

	//元に戻す行列
	m_matrix[(int)Matrix_Index::Trans_Back] = MGetTranslate(v_ref_pos);

	//原点に戻す
	v_pos = VTransform(v_pos, m_matrix[(int)Matrix_Index::Trans_Origin]);
	//回転
	v_pos = VTransform(v_pos, m_matrix[(int)Matrix_Index::Rotation_X]);
	v_pos = VTransform(v_pos, m_matrix[(int)Matrix_Index::Rotation_Y]);
	v_pos = VTransform(v_pos, m_matrix[(int)Matrix_Index::Rotation_Z]);
	//元に戻す
	v_pos = VTransform(v_pos, m_matrix[(int)Matrix_Index::Trans_Back]);
}

//移動行列
void MyMath::MoveMatrix(VECTOR& v_pos, VECTOR v_move)
{
	//平均移動行列
	m_matrix[(int)Matrix_Index::Trans_Move] = MGetTranslate(v_move);

	//平均移動
	v_pos = VTransform(v_pos, m_matrix[(int)Matrix_Index::Trans_Move]);
}

//三点からなる面の法線ベクトルを返す
VECTOR MyMath::GetNormalVec(VECTOR pos1, VECTOR pos2, VECTOR pos3)
{
	return VNorm(VCross(VSub(pos2, pos1), VSub(pos3, pos2)));
}

//=====================================================================

//度をラジアンに変換（引数：度）
float MyMath::ChangeDegToRad(float degree)
{
	return degree * (float)M_PI / 180.0f;
}

//ラジアンを度に変換（引数：ラジアン）
float MyMath::ChangeRadToDeg(float radian)
{
	return radian * 180.0f / (float)M_PI;
}

//指定の数にあわせる：変更したい変数, 指定の数, 増加量
bool MyMath::MatchSpecifiedNum(float& fChangeNum, float fSpecified, float fIncrease)
{
	//指定の変数より大きい
	if (fChangeNum > fSpecified)
	{
		fChangeNum -= fIncrease;
	}

	//指定の変数より小さい
	else if (fChangeNum < fSpecified)
	{
		fChangeNum += fIncrease;
	}

	//指定の数を越えた
	if (fChangeNum >= fSpecified - fIncrease && fChangeNum <= fSpecified + fIncrease)
	{
		//変数を固定
		fChangeNum = fSpecified;

		return true;
	}

	return false;
}

//回転が一周(180度)をこえると変数をリセット
void MyMath::RotReset(VECTOR& vRot)
{
	if (vRot.y >= ChangeDegToRad(180.0f))
	{
		vRot.y = vRot.y - DX_PI_F * 2.0f;
	}
	else if (vRot.y <= ChangeDegToRad(-180.0f))
	{
		vRot.y = 2.0f * DX_PI_F + vRot.y;
	}
}
//回転が一周(180度)をこえると変数をリセット
void MyMath::RotReset(float& fRot)
{
	if (fRot >= ChangeDegToRad(180.0f))
	{
		fRot = fRot - DX_PI_F * 2.0f;
	}
	else if (fRot <= ChangeDegToRad(-180.0f))
	{
		fRot = 2.0f * DX_PI_F + fRot;
	}
}