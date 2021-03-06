
// RenderingView.cpp: CRenderingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Rendering.h"
#endif

#include "RenderingDoc.h"
#include "RenderingView.h"
#include "gdipluscolor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926

// CRenderingView

IMPLEMENT_DYNCREATE(CRenderingView, CView)

BEGIN_MESSAGE_MAP(CRenderingView, CView)
ON_WM_TIMER()
END_MESSAGE_MAP()

// CRenderingView 构造/析构

CRenderingView::CRenderingView()
{
}

CRenderingView::~CRenderingView()
{
}

void CRenderingView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	SetTimer(1, 1, NULL);
}

BOOL CRenderingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CRenderingView 绘图
void CRenderingView::OnDraw(CDC* pDC)
{
	CRenderingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 设 屏幕正中央为原点 正方体边长100 中心为常规中心 中心距离屏幕300 屏幕距离人眼距离100 人眼高度比正方形顶点高50
	float origin[4]		= { 0.0f,	0.0f,	0.0f, 1.0f };	// 自身坐标
	float fCubeWidth	= 200.0f;							// 正方形边长
	float Position[3]	= { 500.0f, 300.0f, 300.0f };		// 平移 设为(500， 300， 100)
	float Scale[3]		= { 1.0f,	1.0f,	1.0f };			// 缩放 设为(2, 2, 2)
	float Rotation[3]	= { 0.0f,	0.0f,	45.0f };		// 旋转 设为绕z轴旋转45°

	float eyePosition[3]	= { 200.0f, 100.0f, -100.0f };	// 摄像机偏移
	float eyeRotation[3]	= { 15.0f, 15.0f, 15.0f };			// 摄像机旋转
	float fFOV = 60.0f;										// FOV
	float fNear = 100.0f;									// 视锥距离摄像机的Near距离
	float fThickness = 300;									// 投影坐标时的视锥体的厚度

	DrawRotatedCube(pDC, fCubeWidth, Position, Rotation, Scale, eyePosition, eyeRotation, fFOV, fNear, fThickness);

	//DrawRotatedTrangle(pDC, m_OriginPoint, m_FirstPoint, m_SecPoint, m_ThirdPoint, (m_Time % 360) * 3.0f, MatrixTransformer::Axis::Z);
}

int *CRenderingView::CalcProjectionCoord(float vertexCoord[3], float eyeCoord[3])
{
	int nX = (int)(vertexCoord[2] / (vertexCoord[2] - eyeCoord[2]) * (vertexCoord[0] - eyeCoord[0]) + eyeCoord[0] + 0.5f);
	int nY = (int)(vertexCoord[2] / (vertexCoord[2] - eyeCoord[2]) * (vertexCoord[1] - eyeCoord[1]) + eyeCoord[1] + 0.5f);
	int *arrResult = new int[2] { nX, nY };
	return arrResult;
}

void CRenderingView::DrawRotatedCube(CDC* pDC, float fEdgeLength, float Position[3], float Rotation[3], float Scale[3], float eyePosition[3], float eyeRotation[3], float fFOV, float fNear, float fThickness)
{
	float arrCubeVertexCoord[8][3] = { { -fEdgeLength / 2, -fEdgeLength / 2, -fEdgeLength / 2 },
									   { fEdgeLength / 2, -fEdgeLength / 2, -fEdgeLength / 2 },
									   { fEdgeLength / 2, fEdgeLength / 2, -fEdgeLength / 2 },
									   { -fEdgeLength / 2, fEdgeLength / 2, -fEdgeLength / 2 },
									   { -fEdgeLength / 2, -fEdgeLength / 2, fEdgeLength / 2 },
									   { fEdgeLength / 2, -fEdgeLength / 2, fEdgeLength / 2 },
									   { fEdgeLength / 2, fEdgeLength / 2, fEdgeLength / 2 },
									   { -fEdgeLength / 2, fEdgeLength / 2, fEdgeLength / 2 } };

	int arrVertexToFace[12][3] = 
	{
		{3, 6, 7},
		{3, 2, 6},
		{0, 1, 4},
		{1, 4, 5},
		{0, 3, 4},
		{3, 4, 7},
		{2, 5, 1},
		{2, 5, 6},
		{4, 6, 5},
		{4, 6, 7},
		{1, 3, 0},
		{1, 3, 2}
	};

	Color arrVertexColor[8] =
	{
		{ 255,	255,	255 },
		{ 255,	0,		0 },
		{ 255,	156,	0 },
		{ 255,	255,	0 },
		{ 0,	255,	0 },
		{ 0,	255,	255 },
		{ 0,	0,		255 },
		{ 255,	0,		255 }
	};

	// 1. 顶点变换
	int *arrNewCoord[8];
	for (int i = 0; i != 8; ++i)
	{
		float *result = new float[4]{ arrCubeVertexCoord[i][0],
									  arrCubeVertexCoord[i][1], 
									  arrCubeVertexCoord[i][2], 
									  1.0f };
		// 自身坐标 -> 世界坐标
		result = MatrixTransformer::MatrixScale(result, Scale[0], Scale[1], Scale[2]);				// 缩放
		result = MatrixTransformer::MatrixRotate(result, Rotation[0], MatrixTransformer::Axis::X);	// 旋转X轴
		result = MatrixTransformer::MatrixRotate(result, ((int)Rotation[1] + m_Time) % 360 * 2.0f, MatrixTransformer::Axis::Y);	// 旋转Y轴
		result = MatrixTransformer::MatrixRotate(result, Rotation[1], MatrixTransformer::Axis::Z);	// 旋转Z轴
		result = MatrixTransformer::MatrixMove(result, Position[0], Position[1], Position[2]);		// 平移
		
		// 世界坐标 -> 视口坐标
		result = MatrixTransformer::MatrixMove(result, -eyePosition[0], -eyePosition[1], -eyePosition[2]);	// 平移
		result = MatrixTransformer::MatrixRotate(result, eyeRotation[2], MatrixTransformer::Axis::Z);	// 旋转Z轴
		result = MatrixTransformer::MatrixRotate(result, eyeRotation[1], MatrixTransformer::Axis::Y);	// 旋转Y轴
		result = MatrixTransformer::MatrixRotate(result, eyeRotation[0], MatrixTransformer::Axis::X);	// 旋转X轴

		// 视口坐标 -> 投影坐标 设视锥长宽比为1：1
// 		double dRadian = fFOV * PI / 180;
// 		result[0] /= tan(dRadian / 2);
// 		result[1] /= tan(dRadian / 2);
// 		result[3] = result[2];
// 		float fNearHeight	= 2 * fNear * tan(dRadian / 2);
// 		float fFarHeight	= 2 * (fNear + fThickness) * tan(dRadian / 2);
// 		result[2]  = -(fFarHeight + fNearHeight) / (fFarHeight - fNearHeight) * result[2]
// 					 - 2 * fNearHeight * fFarHeight / (fFarHeight - fNearHeight);
		
		arrNewCoord[i] = new int[3]{ (int)result[0], (int)result[1], (int)result[2] };
	}

	CRect rectDlg = NULL;
	GetClientRect(rectDlg);

	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);

	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rectDlg.right, rectDlg.bottom);
	MemDC.SelectObject(&MemBitmap);

	HDC hdc = MemDC.GetSafeHdc();

	for (int i = 0; i != 2000; ++i)
	{
		for (int j = 0; j != 1000; ++j)
		{
			m_arrColorData[i][j] = OneData();
		}
	}
	
	for (int i = 0; i != 12; ++i)
	{
		Rasterize::DrawSolidRectangle(
			new int[3]{ (int)(arrNewCoord[arrVertexToFace[i][0]][0] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][0]][1] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][0]][2] + 0.5f) },
			new int[3]{ (int)(arrNewCoord[arrVertexToFace[i][1]][0] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][1]][1] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][1]][2] + 0.5f) },
			new int[3]{ (int)(arrNewCoord[arrVertexToFace[i][2]][0] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][2]][1] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][2]][2] + 0.5f) },
			NULL, NULL, NULL, m_arrColorData);

// 		Rasterize::DrawHollowRectangle(hdc,
// 			new int[2]{ (int)(arrNewCoord[arrVertexToFace[i][0]][0] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][0]][1] + 0.5f) },
// 			new int[2]{ (int)(arrNewCoord[arrVertexToFace[i][1]][0] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][1]][1] + 0.5f) },
// 			new int[2]{ (int)(arrNewCoord[arrVertexToFace[i][2]][0] + 0.5f),	(int)(arrNewCoord[arrVertexToFace[i][2]][1] + 0.5f) });
	}

	for (int i = 0; i != 2000; ++i)
	{
		for (int j = 0; j != 1000; ++j)
		{
			if (m_arrColorData[i][j].m_nBufferZ != INT_MAX)
			{
				SetPixel(hdc, i, j, m_arrColorData[i][j].m_Color);
			}
		}
	}

	pDC->BitBlt(0, 0, rectDlg.right, rectDlg.bottom, &MemDC, 0, 0, SRCCOPY);

	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

	for (int i = 0; i != 8; ++i)
	{
		delete(arrNewCoord[i]);
	}
}

void CRenderingView::DrawRotatedTrangle(CDC* pDC, int originPoint[2], int firstPoint[2], int secPoint[2], int thirdPoint[2], float fAngle, MatrixTransformer::Axis axis)
{
	float firstPointX = (float)(firstPoint[0] - originPoint[0]);
	float firstPointY = (float)(firstPoint[1] - originPoint[1]);
	
	float secPointX = (float)(secPoint[0] - originPoint[0]);
	float secPointY = (float)(secPoint[1] - originPoint[1]);

	float thirdPointX = (float)(thirdPoint[0] - originPoint[0]);
	float thirdPointY = (float)(thirdPoint[1] - originPoint[1]);

	float *firstRes	= MatrixTransformer::MatrixRotate(new float[4]{ firstPointX,	firstPointY,	0.0f, 1.0f }, fAngle, MatrixTransformer::Axis::Z);
	float *secRes	= MatrixTransformer::MatrixRotate(new float[4]{ secPointX,		secPointY,		0.0f, 1.0f }, fAngle, MatrixTransformer::Axis::Z);
	float *thirdRes	= MatrixTransformer::MatrixRotate(new float[4]{ thirdPointX,	thirdPointY,	0.0f, 1.0f }, fAngle, MatrixTransformer::Axis::Z);

	CRect rectDlg = NULL;
	GetClientRect(rectDlg);

	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);

	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rectDlg.right, rectDlg.bottom);
	MemDC.SelectObject(&MemBitmap);

	HDC hdc = MemDC.GetSafeHdc();
	Rasterize::DrawHollowRectangle(hdc,
		new int[2]{ (int)(firstRes[0] + originPoint[0] + 0.5f),	(int)(firstRes[1] + originPoint[1] + 0.5f) },
		new int[2]{ (int)(secRes[0] + originPoint[0] + 0.5f),	(int)(secRes[1] + originPoint[1] + 0.5f) },
		new int[2]{ (int)(thirdRes[0] + originPoint[0] + 0.5f),	(int)(thirdRes[1] + originPoint[1] + 0.5f) });
	pDC->BitBlt(0, 0, rectDlg.right, rectDlg.bottom, &MemDC, 0, 0, SRCCOPY);

	//绘图完成后的清理 
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void CRenderingView::OnTimer(UINT_PTR nIDEvent)
{
	m_Time += 1;

	InvalidateRect(NULL, FALSE);
	CView::OnTimer(nIDEvent);
}

// CRenderingView 诊断

#ifdef _DEBUG
void CRenderingView::AssertValid() const
{
	CView::AssertValid();
}

void CRenderingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRenderingDoc* CRenderingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRenderingDoc)));
	return (CRenderingDoc*)m_pDocument;
}
#endif //_DEBUG


// CRenderingView 消息处理程序
