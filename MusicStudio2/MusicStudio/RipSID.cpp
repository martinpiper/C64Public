// RipSID.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudio.h"
#include "RipSID.h"
#include "afxdialogex.h"


// CRipSID dialog

IMPLEMENT_DYNAMIC(CRipSID, CDialogEx)

CRipSID::CRipSID(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRipSID::IDD, pParent)
	, mTrackNumber(0)
	, mMaxSongs(0)
	, mForceNoteInstruments(_T(""))
	, mAutoDetectPercussion(FALSE)
{

}

CRipSID::~CRipSID()
{
}

void CRipSID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mTrackNumber);
	DDV_MinMaxInt(pDX, mTrackNumber, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, mMaxSongs);
	DDV_MinMaxInt(pDX, mMaxSongs, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, mForceNoteInstruments);
	DDX_Check(pDX, IDC_CHECK1, mAutoDetectPercussion);
}


BEGIN_MESSAGE_MAP(CRipSID, CDialogEx)
END_MESSAGE_MAP()


// CRipSID message handlers
