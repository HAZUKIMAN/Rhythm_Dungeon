#pragma once
#include <DxLib.h>

class MyMatrix
{
public:

	static MATRIX GetldentityMatrix();
	//•½sˆÚ“®
	static MATRIX GetTranslateMatrix(float x, float y, float z);
	//Šgk‚Ìs—ñ‚Ìæ“¾
	static MATRIX GetScaleMatrix(float x, float y, float z);
	//X‰ñ“]s—ñ‚Ìæ“¾
	static MATRIX GetPitchMatrix(float rot);
	//Y‰ñ“]s—ñ‚Ìæ“¾
	static MATRIX GetYawMatrix(float rot);
	//Z‰ñ“]s—ñ‚Ìæ“¾
	static MATRIX GetRollMatrix(float rot);

	static MATRIX MatAdd(MATRIX matA, MATRIX matB);

	static MATRIX MatScale(MATRIX mat, float scale);

	static MATRIX MatMult(MATRIX matA, MATRIX matB);

	static VECTOR MatTransform(MATRIX matA, VECTOR vec);

	static MATRIX MatTranspose(MATRIX mat);


};