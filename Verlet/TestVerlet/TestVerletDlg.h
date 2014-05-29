// TestVerletDlg.h : header file
//

#if !defined(AFX_TestVerletDLG_H__B33935BE_D4FB_4BA2_85AD_013DA28D5B00__INCLUDED_)
#define AFX_TestVerletDLG_H__B33935BE_D4FB_4BA2_85AD_013DA28D5B00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestVerletDlg dialog

class CTestVerletDlg : public CDialog
{
// Construction
public:
	CTestVerletDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestVerletDlg)
	enum { IDD = IDD_TestVerlet_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestVerletDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestVerletDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	size_t mTimes;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TestVerletDLG_H__B33935BE_D4FB_4BA2_85AD_013DA28D5B00__INCLUDED_)
