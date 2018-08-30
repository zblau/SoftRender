#pragma once
class Rasterize
{
public:
	Rasterize();
	~Rasterize();

	static int **PickLinePixel(int firstX, int firstY, int firstZ, int secX, int secY, int secZ, int &nArrayLength);
	static void DrawLine(HDC hdc, int firstPoint[2], int secPoint[2]);
	static void DrawHollowRectangle(HDC hdc, int firstPoint[2], int secPoint[2], int thirdPoint[2]);
	static bool **DrawSolidRectangle(int firstPoint[3], int secPoint[3], int thirdPoint[3], Color firstColor, Color secColor, Color thirdColor, OneData arrOneData[2000][1000]);
	static int *GetPixelCoordFromLine(int nXCoord, int nYCoord, bool &bSwitchXY, bool &bNegtiveSlope);
};
