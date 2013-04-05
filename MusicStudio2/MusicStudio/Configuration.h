#pragma once


// CConfiguration dialog

class CConfiguration : public CDialog
{
	DECLARE_DYNAMIC(CConfiguration)

public:
	CConfiguration(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfiguration();

// Dialog Data
	enum { IDD = IDD_CONFIGURATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int mFrequency;
	int mViewRefreshFPS;
	int mBufferSize;
	int mNumBuffers;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
