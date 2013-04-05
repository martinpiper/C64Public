
// MusicStudioLauncherDlg.h : header file
//

#pragma once


// CMusicStudioLauncherDlg dialog
class CMusicStudioLauncherDlg : public CDHtmlDialog
{
// Construction
public:
	CMusicStudioLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MUSICSTUDIOLAUNCHER_DIALOG, IDH = IDR_HTML_MUSICSTUDIOLAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
