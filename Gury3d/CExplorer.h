#pragma once

#include "3DStudio.h"

// CExplorer frame

class CExplorer : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CExplorer)

protected:

	CTreeCtrl    m_Tree;

	DECLARE_MESSAGE_MAP()
public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

		CExplorer();           // protected constructor used by dynamic creation
		virtual ~CExplorer();
};


