
// MainFrm.cpp: CMainFrame 类的实现
//

#include "stdafx.h"
#include "Rendering.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED;
	if (cs.hMenu != NULL)
	{
		::DestroyMenu(cs.hMenu);      // delete menu if loaded
		cs.hMenu = NULL;              // no menu for this window
	}

	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

