// ExportToC64.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudio.h"
#include "ExportToC64.h"


// CExportToC64 dialog

IMPLEMENT_DYNAMIC(CExportToC64, CDialog)

CExportToC64::CExportToC64(CWnd* pParent /*=NULL*/)
	: CDialog(CExportToC64::IDD, pParent)
{
}

CExportToC64::~CExportToC64()
{
}

void CExportToC64::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExportToC64, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CExportToC64::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CExportToC64::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CExportToC64::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CExportToC64 message handlers

void CExportToC64::OnBnClickedRadio1()
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
	mSaveSID = true;
	mSaveSelfRunning = false;
	mSavePlayerAndData = false;
}

void CExportToC64::OnBnClickedRadio2()
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
	mSaveSID = false;
	mSaveSelfRunning = true;
	mSavePlayerAndData = false;
}

void CExportToC64::OnBnClickedRadio3()
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
	mSaveSID = false;
	mSaveSelfRunning = false;
	mSavePlayerAndData = true;
}

void CExportToC64::OnOK()
{
	GetDlgItemText(IDC_EDIT1,mAddress);
	if (IsDlgButtonChecked(IDC_CHECK1))
	{
		mIncludeSoundEffectCode = true;
	}
	// Make sure we have a sensible hex value typed in
	int realAddress = 0x400;
	_stscanf(mAddress,_T("%04X"),&realAddress);
	mAddress.Format(_T("$%04x"),realAddress);
	CDialog::OnOK();
}

BOOL CExportToC64::OnInitDialog()
{
	CDialog::OnInitDialog();

	mSaveSID = true;
	mSaveSelfRunning = false;
	mSavePlayerAndData = false;
	mIncludeSoundEffectCode = false;

	CheckRadioButton(IDC_RADIO1,IDC_RADIO3,IDC_RADIO1);
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1000"));
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
