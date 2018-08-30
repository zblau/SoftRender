#pragma once
class MatrixTransformer
{
public:
	MatrixTransformer();
	~MatrixTransformer();

	enum Axis
	{
		X = 0,
		Y,
		Z
	};

	static float *MatrixMultiple(float matrix[4][4], float coord[4]);
	static float *MatrixRotateX(float coord[4], float fAngle);
	static float *MatrixRotateY(float coord[4], float fAngle);
	static float *MatrixRotateZ(float coord[4], float fAngle);
	static float *MatrixRotate(float coord[4], float fAngle, Axis axis);
	static float *MatrixMove(float coord[4], float fX, float fY, float fZ);
	static float *MatrixScale(float coord[4], float fX, float fY, float fZ);
};
