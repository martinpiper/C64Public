// TestVerlet.h : main header file for the TestVerlet application
//

#if !defined(AFX_TestVerlet_H__0568ADD9_3615_430D_A14C_8E9764D946E4__INCLUDED_)
#define AFX_TestVerlet_H__0568ADD9_3615_430D_A14C_8E9764D946E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestVerletApp:
// See TestVerlet.cpp for the implementation of this class
//

class CTestVerletApp : public CWinApp
{
public:
	CTestVerletApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestVerletApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestVerletApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TestVerlet_H__0568ADD9_3615_430D_A14C_8E9764D946E4__INCLUDED_)
