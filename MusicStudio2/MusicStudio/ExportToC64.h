#pragma once


// CExportToC64 dialog

class CExportToC64 : public CDialog
{
	DECLARE_DYNAMIC(CExportToC64)

public:
	CExportToC64(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportToC64();

// Dialog Data
	enum { IDD = IDD_EXPORTTOC64 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

public:
	CString mAddress;
	bool mIncludeSoundEffectCode;
	bool mSaveSID;
	bool mSaveSelfRunning;
	bool mSavePlayerAndData;
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	virtual BOOL OnInitDialog();
};
