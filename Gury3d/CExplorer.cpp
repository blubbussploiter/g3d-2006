#include "CExplorer.h"
// CExplorer.cpp : implementation file
//


// CExplorer

IMPLEMENT_DYNCREATE(CExplorer, CMDIFrameWnd)

CExplorer::CExplorer()
{

}

CExplorer::~CExplorer()
{
}


BEGIN_MESSAGE_MAP(CExplorer, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CExplorer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
