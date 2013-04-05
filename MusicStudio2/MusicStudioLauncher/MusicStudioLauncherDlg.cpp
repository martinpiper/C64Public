
// MusicStudioLauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudioLauncher.h"
#include "MusicStudioLauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicStudioLauncherDlg dialog

BEGIN_DHTML_EVENT_MAP(CMusicStudioLauncherDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



CMusicStudioLauncherDlg::CMusicStudioLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CMusicStudioLauncherDlg::IDD, CMusicStudioLauncherDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMusicStudioLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMusicStudioLauncherDlg, CDHtmlDialog)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMusicStudioLauncherDlg message handlers

BOOL CMusicStudioLauncherDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(12345,4*1000,0);

	STARTUPINFO			si;
	PROCESS_INFORMATION pi;

	memset( &si, 0, sizeof(si) );
	si.cb = sizeof(si);

	TCHAR directory[1024];
	TCHAR path[1024];
	directory[0] = 0;
	path[0] = 0;

	GetCurrentDirectory(1024,directory);

	_tcscpy(path,directory);
	_tcscat(path,_T("\\MusicStudio.exe"));

	// We create the new process
	BOOL ret2;
	if ( !(ret2 = CreateProcess( NULL,path,NULL,NULL,FALSE,CREATE_NEW_PROCESS_GROUP,NULL,directory,&si,&pi )) )
	{
		_tcscpy(path,directory);
		_tcscat(path,_T("\\MusicStudio2Bin\\MusicStudio2\\MusicStudio\\MusicStudio.exe"));
		if ( !(ret2 = CreateProcess( NULL,path,NULL,NULL,FALSE,CREATE_NEW_PROCESS_GROUP,NULL,directory,&si,&pi )) )
		{
			MessageBox(_T("Music Studio Launcher could not find the EXE."));
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMusicStudioLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMusicStudioLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CMusicStudioLauncherDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CMusicStudioLauncherDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CMusicStudioLauncherDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 12345)
	{
		PostMessage(WM_QUIT);
	}
	CDHtmlDialog::OnTimer(nIDEvent);
}
