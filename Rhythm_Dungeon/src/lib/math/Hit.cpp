#include <math.h>
#include "Hit.h"


//---------------------------------
//		“_‚Æ‹éŒ`‚Ì“–‚½‚è”»’è
//---------------------------------
bool	CHit::CheckDotToSquare(VECTOR vDotPos, VECTOR vSquarePos, VECTOR vSize)
{
	bool	isRet = false;

	if ((vDotPos.x >= vSquarePos.x - vSize.x) && (vDotPos.x <= vSquarePos.x + vSize.x)
		&& (vDotPos.y >= vSquarePos.y - vSize.y) && (vDotPos.y <= vSquarePos.y + vSize.y))
	{
		isRet = true;
	}

	return	isRet;
}


//---------------------------------
//		‹éŒ`“¯Žm‚Ì“–‚½‚è”»’è
//---------------------------------
bool	CHit::CheckSquareToSquare(VECTOR vPos1, VECTOR vPos2, VECTOR vSize1, VECTOR vSize2)
{
	bool	isRet = false;

	if (((vPos1.x - vSize1.x) <= (vPos2.x + vSize2.x))
		&& ((vPos1.x + vSize1.x) >= (vPos2.x - vSize2.x))
		&& ((vPos1.y - vSize1.y) <= (vPos2.y + vSize2.y))
		&& ((vPos1.y + vSize1.y) >= (vPos2.y - vSize2.y)))
	{
		isRet = true;
	}

	return	isRet;
}


//---------------------------------
//		‰~“¯Žm‚Ì“–‚½‚è”»’è
//---------------------------------
bool	CHit::CheckCircleToCircle(VECTOR vPos1, VECTOR vPos2, float fRadius1, float fRadius2)
{
	bool	isRet = false;
	float	x, y, rad;

	x = (vPos1.x - vPos2.x) * (vPos1.x - vPos2.x);
	y = (vPos1.y - vPos2.y) * (vPos1.y - vPos2.y);
	rad = (fRadius1 + fRadius2) * (fRadius1 + fRadius2);

	if (x + y <= rad) isRet = true;

	return	isRet;
}



//---------------------------------
//		‹…“¯Žm‚Ì“–‚½‚è”»’è
//---------------------------------
bool	CHit::CheckSphereToSphere(VECTOR vPos1, VECTOR vPos2, float fRadius1, float fRadius2, float *pOutLen)
{
	VECTOR	vVec;
	bool	isRet = false;
	float	fRad1;
	float	fRad2;

	vVec = VSub(vPos1, vPos2);
	fRad1 = vVec.x * vVec.x + vVec.y * vVec.y + vVec.z * vVec.z;
	fRad2 = (fRadius1 + fRadius2) * (fRadius1 + fRadius2);

	if (fRad1 <= fRad2)
	{
		isRet = true;
		if (pOutLen != NULL)
		{
			*pOutLen = sqrtf(fRad2) - sqrtf(fRad1);
		}
	}
	else if (pOutLen != NULL) *pOutLen = 0;

	return	isRet;
}


//---------------------------------
//		” “¯Žm‚Ì“–‚½‚è”»’è(AABB)
//---------------------------------
bool	CHit::CheckBoxToBox(VECTOR vPos1, VECTOR vPos2, VECTOR vRadius1, VECTOR vRadius2, VECTOR *pOutLen)
{
	bool	isRet = false;

	// ‚Ü‚¸XYÀ•W‚Åƒqƒbƒg‚µ‚Ä‚¢‚é‚©Šm”F
	if (CHit::CheckSquareToSquare(vPos1, vPos2, vRadius1, vRadius2))
	{
		// ZÀ•W‚Å‚àƒqƒbƒg‚µ‚Ä‚¢‚é‚æ‚¤‚Å‚ ‚ê‚ÎAƒqƒbƒgŠm’è
		if ((vPos1.z + vRadius1.z >= vPos2.z - vRadius2.z) && (vPos2.z + vRadius2.z >= vPos1.z - vRadius2.z))
		{
			isRet = true;
			if (pOutLen != NULL)
			{
				// 2‚Â‚Ì•¨‘Ì‚Ì‹——£‚ð‚Æ‚é
				*pOutLen = VSub(vPos2, vPos1);
				if (pOutLen->x < 0.f) pOutLen->x *= -1.f;
				if (pOutLen->y < 0.f) pOutLen->y *= -1.f;
				if (pOutLen->z < 0.f) pOutLen->z *= -1.f;

				// ” ‚ÌƒTƒCƒY‚ÆˆÊ’uŠÖŒW‚©‚ç‚ß‚èž‚Ý‹ï‡‚ð’²‚×‚é
				pOutLen->x = vRadius1.x + vRadius2.x - pOutLen->x;
				if (pOutLen->x < 0.f) pOutLen->x = 0.f;
				pOutLen->y = vRadius1.y + vRadius2.y - pOutLen->y;
				if (pOutLen->y < 0.f) pOutLen->y = 0.f;
				pOutLen->z = vRadius1.z + vRadius2.z - pOutLen->z;
				if (pOutLen->z < 0.f) pOutLen->z = 0.f;
			}
		}
	}

	return isRet;
}
