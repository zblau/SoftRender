
// RenderingView.h: CRenderingView 类的接口
//
#include "Rasterize.h"
#include "MatrixTransformer.h"
#include <windows.h>

using namespace std;
#pragma once


class CRenderingView : public CView
{
protected: // 仅从序列化创建
	CRenderingView();
	DECLARE_DYNCREATE(CRenderingView)

// 特性
public:
	CRenderingDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CRenderingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	int *CalcProjectionCoord(float vertexCoord[3], float eyeCoord[3]);
	void DrawRotatedTrangle(CDC* pDC, int originPoint[2], int firstPoint[2], int secPoint[2], int thirdPoint[2], float fAngle, MatrixTransformer::Axis axis);
	void DrawRotatedCube(CDC* pDC, float fEdgeLength, float Position[3], float Rotation[3], float Scale[3], float eyePosition[3], float eyeRotation[3], float fFOV, float fNear, float fThickness);
	DWORD m_Time = 0;
	
protected:
	OneData m_arrColorData[2000][1000];

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // RenderingView.cpp 中的调试版本
inline CRenderingDoc* CRenderingView::GetDocument() const
   { return reinterpret_cast<CRenderingDoc*>(m_pDocument); }
#endif
