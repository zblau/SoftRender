#include "stdafx.h"
#include "MatrixTransformer.h"

#define PI 3.1415926

MatrixTransformer::MatrixTransformer()
{
}


MatrixTransformer::~MatrixTransformer()
{
}

float *MatrixTransformer::MatrixMultiple(float matrix[4][4], float coord[4])
{
	float *fResult = new float[4];
	fResult[0] = matrix[0][0] * coord[0]
			   + matrix[0][1] * coord[1]
			   + matrix[0][2] * coord[2]
			   + matrix[0][3] * coord[3];
	
	fResult[1] = matrix[1][0] * coord[0]
			   + matrix[1][1] * coord[1]
			   + matrix[1][2] * coord[2]
			   + matrix[1][3] * coord[3];

	fResult[2] = matrix[2][0] * coord[0]
			   + matrix[2][1] * coord[1]
			   + matrix[2][2] * coord[2]
			   + matrix[2][3] * coord[3];

	fResult[3] = matrix[3][0] * coord[0]
			   + matrix[3][1] * coord[1]
			   + matrix[3][2] * coord[2]
			   + matrix[3][3] * coord[3];
	return fResult;
}

float *MatrixTransformer::MatrixRotate(float coord[4], float fAngle, Axis axis)
{
	switch (axis)
	{
	case X:
		return MatrixRotateX(coord, fAngle);
	case Y:
		return MatrixRotateY(coord, fAngle);
	case Z:
		return MatrixRotateZ(coord, fAngle);
	default:
		ASSERT(true);
	}

	return NULL;
}

float *MatrixTransformer::MatrixRotateX(float coord[4], float fAngle)
{
	double dRadian = fAngle * PI / 180;
	float matrix[4][4] = 
	{
		1.0f,	0.0f,			0.0f,			0.0f,
		0.0f,	cos(dRadian),	-sin(dRadian),	0.0f,
		0.0f,	sin(dRadian),	cos(dRadian),	0.0f,
		0.0f,	0.0f,			0.0f,			1.0f
	};
	return MatrixMultiple(matrix, coord);
}

float *MatrixTransformer::MatrixRotateY(float coord[4], float fAngle)
{
	double dRadian = fAngle * PI / 180;
	float matrix[4][4] =
	{
		cos(dRadian),	0.0f,	sin(dRadian),	0.0f,
		0.0f,			1.0f,	0.0f,			0.0f,
		-sin(dRadian),	0.0f,	cos(dRadian),	0.0f,
		0.0f,			0.0f,	0.0f,			1.0f
	};
	return MatrixMultiple(matrix, coord);
}

float *MatrixTransformer::MatrixRotateZ(float coord[4], float fAngle)
{
	double dRadian = fAngle * PI / 180;
	float matrix[4][4] =
	{
		cos(dRadian),	-sin(dRadian),	0.0f,	0.0f,
		sin(dRadian),	cos(dRadian),	0.0f,	0.0f,
		0.0f,			0.0f,			1.0f,	0.0f,
		0.0f,			0.0f,			0.0f,	1.0f
	};
	return MatrixMultiple(matrix, coord);
}

float *MatrixTransformer::MatrixMove(float coord[4], float fX, float fY, float fZ)
{
	float *arrRes = new float[4];
	float matrix[4][4] =
	{
		1.0f,	0.0f,	0.0f,	fX,
		0.0f,	1.0f,	0.0f,	fY,
		0.0f,	0.0f,	1.0f,	fZ,
		0.0f,	0.0f,	0.0f,	1.0f
	};
	return MatrixMultiple(matrix, coord);
}

float *MatrixTransformer::MatrixScale(float coord[4], float fX, float fY, float fZ)
{
	float *arrRes = new float[4];
	float matrix[4][4] =
	{
		fX,		0.0f,	0.0f,	0.0f,
		0.0f,	fY,		0.0f,	0.0f,
		0.0f,	0.0f,	fZ,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};
	return MatrixMultiple(matrix, coord);
}
