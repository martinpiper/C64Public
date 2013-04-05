
// MusicStudio.h : main header file for the MusicStudio application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#ifdef _DEBUG
#define CONFIG_NAME _T("DebugConfiguration")
#else
#define CONFIG_NAME _T("Configuration")
#endif

// CMusicStudioApp:
// See MusicStudio.cpp for the implementation of this class
//

class CMusicStudioApp : public CWinAppEx
{
public:
	CMusicStudioApp();


// Overrides
public:
	virtual BOOL InitInstance();

	TCHAR mCurrentDirectory[2048];

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelpConfiguration();
};

extern CMusicStudioApp theApp;
