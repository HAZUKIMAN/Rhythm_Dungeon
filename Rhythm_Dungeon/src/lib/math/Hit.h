#pragma once

#include <DxLib.h>


class CHit
{
public:

	//***************************************
	//		2D用関数
	//***************************************

	//		点と矩形の当たり判定
	//	vDotPos		:	点の位置
	//	vSquarePos	:	矩形の中心位置	
	//	vSize		:	矩形の縦、横幅(半径)
	static bool	CheckDotToSquare(VECTOR vDotPos, VECTOR vSquarePos, VECTOR vSize);

	//		矩形同士の当たり判定
	//	vPos1, vPos2	:	判定する2つの物体の中心位置	
	//	vSize1, vSize2	:	判定する2つの物体の縦、横幅(半径)
	static bool	CheckSquareToSquare(VECTOR vPos1, VECTOR vPos2, VECTOR vSize1, VECTOR vSize2);

	//		円同士の当たり判定
	//	vPos1, vPos2		:	判定する2つの物体の中心位置
	//	fRadius1, fRadius2	:	判定する2つの物体の半径
	static bool	CheckCircleToCircle(VECTOR vPos1, VECTOR vPos2, float fRadius1, float fRadius2);

	//***************************************
	//		3D用関数
	//***************************************

	//		球同士の当たり判定
	//	vPos1, vPos2		:	判定する2つの物体の中心位置
	//	fRadius1, fRadius2	:	判定する2つの物体の半径
	//	pOutLen				:	めり込んだ距離
	static bool	CheckSphereToSphere(VECTOR vPos1, VECTOR vPos2, float fRadius1, float fRadius2, float *pOutLen = NULL);

	//		箱同士の当たり判定(AABB)
	//	vPos1, vPos2		:	判定する2つの物体の中心位置
	//	vRadius1, vRadius2	:	判定する2つの物体の半径
	//	pOutLen				:	めり込んだ距離
	static bool	CheckBoxToBox(VECTOR vPos1, VECTOR vPos2, VECTOR vRadius1, VECTOR vRadius2, VECTOR *pOutLen = NULL);

};
