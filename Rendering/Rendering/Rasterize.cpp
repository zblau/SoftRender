#include "stdafx.h"
#include "Rasterize.h"
#include "gdipluscolor.h"

Rasterize::Rasterize()
{
}


Rasterize::~Rasterize()
{
}

void Rasterize::DrawHollowRectangle(HDC hdc, int firstPoint[2], int secPoint[2], int thirdPoint[2])
{
	DrawLine(hdc, firstPoint,	secPoint);
	DrawLine(hdc, secPoint,		thirdPoint);
	DrawLine(hdc, thirdPoint,	firstPoint);
}

void Rasterize::DrawLine(HDC hdc, int first[2], int sec[2])
{
	int nArrayLength = 0;
	int **arrPixelOne = PickLinePixel(first[0], first[1], sec[0], sec[1], 0, 0, nArrayLength);
	for (int i = 0; i != nArrayLength; ++i)
	{
		SetPixel(hdc, arrPixelOne[i][0], arrPixelOne[i][1], RGB(87, 250, 255));
		delete(arrPixelOne[i]);
	}
	delete(arrPixelOne);
}

int **Rasterize::PickLinePixel(int firstX, int firstY, int firstZ, int secX, int secY, int secZ, int &nArrayLength)
{
	int *firstPoint	= new int[2]{ firstX,	firstY };
	int *secPoint	= new int[2]{ secX,		secY };
	if (secX < firstX)
	{
		int *ForSwitch = firstPoint;
		firstPoint = secPoint;
		secPoint = ForSwitch;
	}

	int nX = secPoint[0] - firstPoint[0];
	int nY = secPoint[1] - firstPoint[1];
	delete(secPoint);

	bool bSwitchXY = false;
	bool bNegtiveSlope = false;
	int *nIncreaseY = GetPixelCoordFromLine(nX, nY, bSwitchXY, bNegtiveSlope);

	int nYLength	= nY >= 0 ? nY : -nY;
	int nLength		= (nX >= nYLength ? nX : nYLength) + 1;
	nArrayLength	= nLength;

	int **arrResult = new int*[nLength];
	if (bSwitchXY)
	{
		if (bNegtiveSlope)
		{
			for (int i = 0; i != nLength; ++i)
			{
				arrResult[i] = new int[3];
				arrResult[i][0] = firstPoint[0] + nIncreaseY[i];
				arrResult[i][1] = firstPoint[1] - i;
			}
		}
		else
		{
			for (int i = 0; i != nLength; ++i)
			{
				arrResult[i] = new int[3];
				arrResult[i][0] = firstPoint[0] + nIncreaseY[i];
				arrResult[i][1] = firstPoint[1] + i;
			}
		}
	}
	else
	{
		if (bNegtiveSlope)
		{
			for (int i = 0; i != nLength; ++i)
			{
				arrResult[i] = new int[3];
				arrResult[i][0] = i + firstPoint[0];
				arrResult[i][1] = firstPoint[1] - nIncreaseY[i];
			}
		}
		else
		{
			for (int i = 0; i != nLength; ++i)
			{
				arrResult[i] = new int[3];
				arrResult[i][0] = i + firstPoint[0];
				arrResult[i][1] = nIncreaseY[i] + firstPoint[1];
			}
		}
	}
	delete(firstPoint);
	delete(nIncreaseY);

	// z buffer
	float fOneStep = (secZ - firstZ) * 1.0f / (nArrayLength - 1);
	for (int i = 0; i != nLength; ++i)
	{
		arrResult[i][2] = firstZ + int(fOneStep * i + 0.5f);
	}
	
	return arrResult;
}

bool **Rasterize::DrawSolidRectangle(int firstPoint[3], int secPoint[3], int thirdPoint[3], Color firstColor, Color secColor, Color thirdColor, OneData arrOneData[2000][1000])
{
	int nMaxY = INT_MIN;
	int nMinY = INT_MAX;

	int nArrayOneLength = 0;
	int **arrPixelOne	= PickLinePixel(firstPoint[0], firstPoint[1], firstPoint[2], secPoint[0], secPoint[1], secPoint[2], nArrayOneLength);
	for (int i = 0; i != nArrayOneLength; ++i)
	{
		if (arrPixelOne[i][1] > nMaxY)
		{
			nMaxY = arrPixelOne[i][1];
		}
		if (arrPixelOne[i][1] < nMinY)
		{
			nMinY = arrPixelOne[i][1];
		}
	}
	
	int nArrayTwoLength = 0;
	int **arrPixelTwo	= PickLinePixel(secPoint[0], secPoint[1], secPoint[2], thirdPoint[0], thirdPoint[1], thirdPoint[2], nArrayTwoLength);
	for (int i = 0; i != nArrayTwoLength; ++i)
	{
		if (arrPixelTwo[i][1] > nMaxY)
		{
			nMaxY = arrPixelTwo[i][1];
		}
		if (arrPixelTwo[i][1] < nMinY)
		{
			nMinY = arrPixelTwo[i][1];
		}
	}

	int nArrayThreeLength	= 0;
	int **arrPixelThree		= PickLinePixel(thirdPoint[0], thirdPoint[1], thirdPoint[2], firstPoint[0], firstPoint[1], firstPoint[2], nArrayThreeLength);
	for (int i = 0; i != nArrayThreeLength; ++i)
	{
		if (arrPixelThree[i][1] > nMaxY)
		{
			nMaxY = arrPixelThree[i][1];
		}
		if (arrPixelThree[i][1] < nMinY)
		{
			nMinY = arrPixelThree[i][1];
		}
	}

	int nBorderArrayLength = nMaxY - nMinY + 1;
	int **arrBorder = new int*[nBorderArrayLength];
	for (int i = 0; i != nBorderArrayLength; ++i)
	{
		// Xmin Z Xmax Z Y
		arrBorder[i] = new int[5]{ INT_MAX, 0, INT_MIN, 0, 0 };
	}

	for (int i = 0; i != nArrayOneLength; ++i)
	{
		int nValueY = arrPixelOne[i][1];
		if (arrBorder[nValueY - nMinY][2] < arrPixelOne[i][0])
		{
			arrBorder[nValueY - nMinY][2] = arrPixelOne[i][0];
			arrBorder[nValueY - nMinY][3] = arrPixelOne[i][2];
		}
		if (arrBorder[nValueY - nMinY][0] > arrPixelOne[i][0])
		{
			arrBorder[nValueY - nMinY][0] = arrPixelOne[i][0];
			arrBorder[nValueY - nMinY][1] = arrPixelOne[i][2];
		}
		delete(arrPixelOne[i]);
	}
	delete(arrPixelOne);

	for (int i = 0; i != nArrayTwoLength; ++i)
	{
		int nValueY = arrPixelTwo[i][1];
		if (arrBorder[nValueY - nMinY][2] < arrPixelTwo[i][0])
		{
			arrBorder[nValueY - nMinY][2] = arrPixelTwo[i][0];
			arrBorder[nValueY - nMinY][3] = arrPixelTwo[i][2];
		}
		if (arrBorder[nValueY - nMinY][0] > arrPixelTwo[i][0])
		{
			arrBorder[nValueY - nMinY][0] = arrPixelTwo[i][0];
			arrBorder[nValueY - nMinY][1] = arrPixelTwo[i][2];
		}
		delete(arrPixelTwo[i]);
	}
	delete(arrPixelTwo);

	for (int i = 0; i != nArrayThreeLength; ++i)
	{
		int nValueY = arrPixelThree[i][1];
		if (arrBorder[nValueY - nMinY][2] < arrPixelThree[i][0])
		{
			arrBorder[nValueY - nMinY][2] = arrPixelThree[i][0];
			arrBorder[nValueY - nMinY][3] = arrPixelThree[i][2];
		}
		if (arrBorder[nValueY - nMinY][0] > arrPixelThree[i][0])
		{
			arrBorder[nValueY - nMinY][0] = arrPixelThree[i][0];
			arrBorder[nValueY - nMinY][1] = arrPixelThree[i][2];
		}
		delete(arrPixelThree[i]);
	}
	delete(arrPixelThree);
	
	for (int i = 0; i != nBorderArrayLength; ++i)
	{
		ASSERT(arrBorder[i][0] != INT_MAX);
		ASSERT(arrBorder[i][2] != INT_MIN);

		float fOneStep = (arrBorder[i][3] - arrBorder[i][1] + 1) * 1.0f / (arrBorder[i][2] - arrBorder[i][0] + 1);
		for (int j = arrBorder[i][0]; j <= arrBorder[i][2]; ++j)
		{
			BYTE r = (BYTE)(255.0f / nBorderArrayLength * (i + 1));
			BYTE g = (BYTE)(255.0f / nBorderArrayLength * (i + 1));
			BYTE b = (BYTE)(255.0f / nBorderArrayLength * (i + 1));
			COLORREF color = RGB(r, g, b);
			
			int nBufferZ = (int)(arrBorder[i][1] + ((j - arrBorder[i][0]) * fOneStep) + 0.5f);

			if (arrOneData[j][nMinY + i].m_nBufferZ > nBufferZ)
			{
				arrOneData[j][nMinY + i].m_nBufferZ = nBufferZ;
				arrOneData[j][nMinY + i].m_Color = RGB(r, g, b);
			}
		}
		delete(arrBorder[i]);
	}
	delete(arrBorder);

	return NULL;
}

// 调用时需保证nXCoord >= 0
int *Rasterize::GetPixelCoordFromLine(int nXCoord, int nYCoord, bool &bSwitchXY, bool &bNegtiveSlope)
{
	bNegtiveSlope = (nYCoord < 0);
	nYCoord = (nYCoord < 0) ? -nYCoord : nYCoord;

	bSwitchXY = nYCoord > nXCoord;
	if (nYCoord > nXCoord || 0 == nXCoord)
	{
		int nForSwitch = nXCoord;
		nXCoord = nYCoord;
		nYCoord = nForSwitch;
	}
	
	int nB = 0;
	int *nResult = new int[nXCoord + 1];
	for (int i = 0; i != nXCoord + 1; ++i)
	{
		if (0 == i)
		{
			nResult[0] = 0;
		}
		else if (1 == i)
		{
			nB = 2*nYCoord + (-nXCoord);
			nResult[i] = (nB >= 0) ? (nResult[i - 1] + 1) : nResult[i - 1];
		}
		else
		{
			if (nResult[i - 1] > nResult[i - 2])
			{
				nB += (2*nYCoord + 2*(-nXCoord));
				nResult[i] = (nB >= 0) ? (nResult[i - 1] + 1) : nResult[i - 1];
			}
			else
			{
				nB += (2*nYCoord);
				nResult[i] = (nB >= 0) ? (nResult[i - 1] + 1) : nResult[i - 1];
			}
		}
	}

	return nResult;
}
