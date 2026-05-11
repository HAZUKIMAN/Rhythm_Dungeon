#pragma once
#include <DxLib.h>

class MyMath
{
private:

	enum class Matrix_Index
	{
		Trans_Move,			//平行移動

		Trans_Origin,		//原点に戻す
		Rotation_X,			//X軸回転
		Rotation_Y,			//Y軸回転
		Rotation_Z,			//Z軸回転
		Trans_Back,			//元の座標に戻す

		Scale,				//大きさを変える

		Matrix_Max_Num
	};

	static MATRIX m_matrix[(int)Matrix_Index::Matrix_Max_Num];		//移動、回転のための行列

public:

	//２から１への進行方向と移動量：向かう場所XY1  動かしたいものXY2  XとYのどちらを出すか  増加量
	static float GetDirection(float x1, float y1, float x2, float y2, char moji, float s);

	//1から2への角度(ラジアン)：向かせる場所XY　向かせたいものXY
	static float GetAngle(float Ax, float Ay, float Bx, float By);

	//ベクトルの内積を求めて正か負を返す：向かせる場所XY　向かせたいものXY
	static bool GetInnerProduct(float Ax, float Ay, float Bx, float By);

	//度をラジアンに変換（引数：度）
	static float DegToRad(float degree);

	//長さの差を返す
	static float GetDefLen_f(float pos_A, float pos_B);

	//≒
	static bool NearyEqual(float num1, float num2, float error_num = 0.1f);

	//桁数を返す
	static int GetNumLen(int num);

	//2点間の距離
	static float GetDistance(VECTOR vec1, VECTOR vec2);

	//連立方程式
	static VECTOR Simultaneous_Equations(float a1, float b1, float a2, float b2);

	//度をラジアンに変換（引数：度）
	static float ChangeDegToRad(float degree);
	//ラジアンを度に変換（引数：ラジアン）
	static float ChangeRadToDeg(float radian);

	//指定の数にあわせる：変更したい変数, 指定の数, 増加量
	static bool MatchSpecifiedNum(float& fChangeNum, float fSpecified, float fIncrease);

	//回転が一周(180度)をこえると変数をリセット
	static void RotReset(VECTOR& vRot);
	//回転が一周(180度)をこえると変数をリセット
	static void RotReset(float& fRot);

	//ベクトル============================================================
	//外積2D
	static float VCross2D(VECTOR vec1, VECTOR vec2);

	//引数で受け取ったベクトルを指定の長さにして返す
	//大きさが０なら(0,0,0)のベクトルを返す
	static VECTOR GetVector(VECTOR vec, float speed = 1.0f);

	//現在地から目的地まで指定の速さで動くベクトル
	static VECTOR GetVector(VECTOR vec_here, VECTOR vec_dis, float speed = 1.0f);

	//ベクトルのなす角
	static float AngleOf2Vector(VECTOR vec1, VECTOR vec2, bool degree_switch = false);

	//行列==================================================================
	//引数：回転前のポジション、回転の基準点、回転角度(ラジアン)
	//回転行列
	static void RotaMatrix(VECTOR& v_pos, VECTOR v_ref_pos, VECTOR v_rota);

	//移動行列
	static void MoveMatrix(VECTOR& v_pos, VECTOR v_move);

	//======================================================================

	//三点からなる面の法線ベクトルを返す
	//ベクトルの方向は時計回り
	static VECTOR GetNormalVec(VECTOR pos1, VECTOR pos2, VECTOR pos3);
};