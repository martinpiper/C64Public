
// MusicStudio.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MusicStudio.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MusicStudioDoc.h"
#include "MusicStudioView.h"
#include "Configuration.h"
#include "../RESID-FP/trunk/sidplay-2.0.9/src/audio/mmsystem/ExternalSystem.h"

#include "crtDbg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicStudioApp

BEGIN_MESSAGE_MAP(CMusicStudioApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMusicStudioApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_HELP_CONFIGURATION, &CMusicStudioApp::OnHelpConfiguration)
	ON_COMMAND(ID_HELP_CONFIGURATION32777, &CMusicStudioApp::OnHelpConfiguration)
END_MESSAGE_MAP()


// CMusicStudioApp construction

CMusicStudioApp::CMusicStudioApp()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF);

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMusicStudioApp object

CMusicStudioApp theApp;


// CMusicStudioApp initialization

BOOL CMusicStudioApp::InitInstance()
{
	// Detect where ACME is and set the current directory if needed
	FILE *fp;
	LPWSTR str = GetCommandLineW();
	int argc;
	LPWSTR *argv;
	argv = CommandLineToArgvW(str,&argc);
	CStringW path = argv[0];
//	LocalFree(argv);

	do
	{
		int pos = path.ReverseFind('\\');
		if (pos >= 0)
		{
			path.Delete(pos,path.GetLength());
		}
		SetCurrentDirectoryW(path);

		fp = fopen("..\\..\\acme.exe","rb");
	} while (!fp && (path.GetLength() > 3));

	if (fp)
	{
		fclose(fp);
	}
	else
	{
		// MPi: TODO: Convert all these message boxes to use proper string resources
		MessageBox(0,_T("Could not find the ACME tool, so music files cannot be played or exported."),_T("Error"),MB_OK);
	}

	GetCurrentDirectory(2048,mCurrentDirectory);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need

	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("Element 114 Software"));
	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	gSoftViewRefreshTime = GetProfileInt(CONFIG_NAME,_T("ViewRefreshTime"),100);
	gSoftFrequency = GetProfileInt(CONFIG_NAME,_T("Frequency"),44100);
	gSoftMAXBUFBLOCKS = GetProfileInt(CONFIG_NAME,_T("MAXBUFBLOCKS"),3);
	gSoftBufSizeDivisor = GetProfileInt(CONFIG_NAME,_T("BufSizeDivisor"),1);

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MusicStudioTYPE,
		RUNTIME_CLASS(CMusicStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMusicStudioView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_MusicStudioTYPE2,
		RUNTIME_CLASS(CMusicStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMusicStudioView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_MusicStudioTYPE3,
		RUNTIME_CLASS(CMusicStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMusicStudioView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_MusicStudioTYPE4,
		RUNTIME_CLASS(CMusicStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMusicStudioView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	// Stop the window from opening with a new file when a blank window was needed.
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Parse application specific command line options
	// For example:
	// /hide /openFile $(ProjectDir)\..\MeanStreak.msmus /exportToSID c:\temp\t.sid /exportToSelf y c:\temp\t.prg /exportToC64 $400 y c:\temp\t2.prg /exit
	// /hide /openFile $(ProjectDir)\..\MeanStreak.msmus /play
	// /openFile $(ProjectDir)\..\MeanStreak.msmus /loadEnvelope 2 "$(ProjectDir)\..\Tom tom1.msenv" /saveEnvelope 1 "c:\temp\t.msenv" /insertEnvelope 1 /deleteTrackPos 1 3 3
	int i;
	for (i = 1 ; i < argc ; i++)
	{
		LPWSTR arg = argv[i];
		if (arg[0] == '/' || arg[0] == '-')
		{
			arg = arg+1;
			if (wcscmp(arg , L"hide") == 0)
			{
				ShowWindow(GetActiveWindow() , SW_HIDE);
				continue;
			}
			if (wcscmp(arg , L"exit") == 0)
			{
				return FALSE;
				continue;
			}
			if (wcscmp(arg , L"openFile") == 0)
			{
				i++;
				OpenDocumentFile(argv[i] , FALSE);
				continue;
			}
			if (wcscmp(arg , L"exportToSID") == 0)
			{
				CMusicStudioView::mLastView->CommonSaveC64File("sid");
				i++;
				_tremove(argv[i]);
				_trename(_T("t.sid"),argv[i]);

				continue;
			}
			if (wcscmp(arg , L"exportToSelf") == 0)
			{
				i++;
				if (argv[i][0] == 'y')
				{
					CMusicStudioView::mLastView->CommonSaveC64File("self" , true);
				}
				else
				{
					CMusicStudioView::mLastView->CommonSaveC64File("self" , false);
				}
				i++;
				_tremove(argv[i]);
				_trename(_T("t.prg"),argv[i]);

				continue;
			}
			if (wcscmp(arg , L"exportToC64") == 0)
			{
				i++;
				CStringA ansi(argv[i]);
				i++;
				if (argv[i][0] == 'y')
				{
					CMusicStudioView::mLastView->CommonSaveC64File(ansi , true);
				}
				else
				{
					CMusicStudioView::mLastView->CommonSaveC64File(ansi , false);
				}
				i++;
				_tremove(argv[i]);
				_trename(_T("t.prg"),argv[i]);

				continue;
			}
			if (wcscmp(arg , L"play") == 0)
			{
				CMusicStudioView::mLastView->OnBnClickedPlay();
				continue;
			}
			if (wcscmp(arg , L"loadEnvelope") == 0)
			{
				i++;
				CMusicStudioView::mLastView->mEnvelopeNum.SetWindowTextW(argv[i]);
				i++;
				CMusicStudioView::mLastView->OnBnClickedLoadEnvelope(CString(argv[i]));
				continue;
			}
			if (wcscmp(arg , L"saveEnvelope") == 0)
			{
				i++;
				CMusicStudioView::mLastView->mEnvelopeNum.SetWindowTextW(argv[i]);
				i++;
				CMusicStudioView::mLastView->OnBnClickedSaveEnvelope(CString(argv[i]));
				continue;
			}
			if (wcscmp(arg , L"insertEnvelope") == 0)
			{
				i++;
				CMusicStudioView::mLastView->mEnvelopeNum.SetWindowTextW(argv[i]);
				CMusicStudioView::mLastView->OnBnClickedInsertEnvelope();
				continue;
			}
			if (wcscmp(arg , L"pressEnvelopeForceUsedFX") == 0)
			{
				i++;
				CMusicStudioView::mLastView->mEnvelopeNum.SetWindowTextW(argv[i]);
				CMusicStudioView::mLastView->GetDlgItem(IDC_CHECK4)->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON , 0);
				CMusicStudioView::mLastView->GetDlgItem(IDC_CHECK4)->SendMessage(WM_LBUTTONUP, MK_LBUTTON , 0);
				CMusicStudioView::mLastView->OnBnClickedCheck4ForceUsed();
				continue;
			}
			if (wcscmp(arg , L"deleteTrackPos") == 0)
			{
				i++;
				int track = 0;
				_stscanf(argv[i],_T("%x"),&track);
				i++;
				int index = 0;
				_stscanf(argv[i],_T("%x"),&index);
				i++;
				int count = 0;
				_stscanf(argv[i],_T("%x"),&count);
				while (count > 0)
				{
					CMusicStudioView::mLastView->OnEditNumRemove(MAKEWORD(index , track) , 0);
					count--;
				}
				continue;
			}
		}
	}

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

#ifdef _DEBUG
	// Causes the file to be opened when the application starts.
	// Useful when debugging and when you don't want to keep on going to the menu.
//	OpenDocumentFile( _T("C:\\Work\\C64\\MusicStudio2\\moz_k299.mid") );
//	OpenDocumentFile( _T("C:\\Work\\C64\\MusicStudio2\\ExampleFiles\\Loopz Musix.msmus") );
//	OpenDocumentFile( _T("C:\\Downloads\\GTC_Citadel_2.sng") );
//	OpenDocumentFile( _T("C:\\work\\C64\\MWMusic\\bin\\warbles2.mus.prg") );
	OpenDocumentFile( _T("C:\\Work\\C64\\MusicStudio2\\ExampleFiles\\OriginalC64Music\\P.AIR WOLF.PRG") );
	
#endif

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CMusicStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMusicStudioApp customization load/save methods

void CMusicStudioApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMusicStudioApp::LoadCustomState()
{
}

void CMusicStudioApp::SaveCustomState()
{
}

// CMusicStudioApp message handlers




void CMusicStudioApp::OnHelpConfiguration()
{
	CConfiguration dlg;

	dlg.mFrequency = gSoftFrequency;
	dlg.mViewRefreshFPS = 1000 / gSoftViewRefreshTime;
	dlg.mBufferSize = 11 - gSoftBufSizeDivisor;
	dlg.mNumBuffers = gSoftMAXBUFBLOCKS;

	INT_PTR ret = dlg.DoModal();

	if (ret == IDOK)
	{
		gSoftFrequency = dlg.mFrequency;
		gSoftViewRefreshTime = 1000 / dlg.mViewRefreshFPS;
		gSoftBufSizeDivisor = 11 - dlg.mBufferSize;
		gSoftMAXBUFBLOCKS = dlg.mNumBuffers;

		WriteProfileInt(CONFIG_NAME,_T("ViewRefreshTime"),gSoftViewRefreshTime);
		WriteProfileInt(CONFIG_NAME,_T("Frequency"),gSoftFrequency);
		WriteProfileInt(CONFIG_NAME,_T("MAXBUFBLOCKS"),gSoftMAXBUFBLOCKS);
		WriteProfileInt(CONFIG_NAME,_T("BufSizeDivisor"),gSoftBufSizeDivisor);
	}
}
