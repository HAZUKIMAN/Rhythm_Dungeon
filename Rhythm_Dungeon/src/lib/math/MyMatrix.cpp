#include "MyMatrix.h"
#include <math.h>

MATRIX MyMatrix::GetldentityMatrix()
{
	MATRIX result = { 0 };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				result.m[i][j] = 1;
			}
			else
			{
				result.m[i][j] = 0;
			}
		}
	}
	return result;
}

//•½sˆÚ“®
MATRIX MyMatrix::GetTranslateMatrix(float x, float y, float z)
{
	MATRIX result = GetldentityMatrix();
	result.m[0][3] = x;
	result.m[1][3] = y;
	result.m[2][3] = z;

	return result;
}

//Šgk‚Ìs—ñ‚ÌŽæ“¾
MATRIX MyMatrix::GetScaleMatrix(float x, float y, float z)
{
	MATRIX result = GetldentityMatrix();
	result.m[0][0] = x;
	result.m[1][1] = y;
	result.m[2][2] = z;

	return result;
}
//X‰ñ“]s—ñ‚ÌŽæ“¾
MATRIX MyMatrix::GetPitchMatrix(float rot)
{
	MATRIX result = GetldentityMatrix();
	result.m[1][1] = cosf(rot);
	result.m[1][2] = -sinf(rot);
	result.m[2][1] = sinf(rot);
	result.m[2][2] = cosf(rot);

	return result;
}
//Y‰ñ“]s—ñ‚ÌŽæ“¾
MATRIX MyMatrix::GetYawMatrix(float rot)
{
	MATRIX result = GetldentityMatrix();
	result.m[0][0] = cosf(rot);
	result.m[0][2] = sinf(rot);
	result.m[2][0] = -sinf(rot);
	result.m[2][2] = cosf(rot);

	return result;
}
//Z‰ñ“]s—ñ‚ÌŽæ“¾
MATRIX MyMatrix::GetRollMatrix(float rot)
{
	MATRIX result = GetldentityMatrix();
	result.m[0][0] = cosf(rot);
	result.m[0][1] = -sinf(rot);
	result.m[1][0] = sinf(rot);
	result.m[1][1] = cosf(rot);

	return result;
}

MATRIX MyMatrix::MatAdd(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = matA.m[i][j] + matB.m[i][j];
		}
	}
	return result;
}

MATRIX MyMatrix::MatScale(MATRIX mat, float scale)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = mat.m[i][j] * scale;
		}
	}
	return result;
}

MATRIX MyMatrix::MatMult(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += matA.m[i][k] + matB.m[k][j];
			}
		}
	}
	return result;
}

VECTOR MyMatrix::MatTransform(MATRIX matA, VECTOR vec)
{
	float work[4] = { 0 };
	float result_buf[4] = { 0 };
	VECTOR result_vec;
	work[0] = vec.x;
	work[1] = vec.y;
	work[2] = vec.z;
	work[3] = 1;

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			result_buf[i] += (matA.m[i][k] * work[k]);
		}
	}

	result_vec.x = result_buf[0];
	result_vec.y = result_buf[1];
	result_vec.z = result_buf[2];

	return result_vec;
}

MATRIX MyMatrix::MatTranspose(MATRIX mat)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] += mat.m[i][j];
		}
	}
	return result;
}