// ImportMIDI.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudio.h"
#include "ImportMIDI.h"
#include "afxdialogex.h"


// CImportMIDI dialog

IMPLEMENT_DYNAMIC(CImportMIDI, CDialogEx)

CImportMIDI::CImportMIDI(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportMIDI::IDD, pParent)
	, mTimeMultiplier(0)
	, mBlockLength(0)
	, mChannelMap0(0)
	, mChannelMap1(0)
	, mChannelMap2(0)
	, mChannelMap3(0)
	, mChannelMap4(0)
	, mChannelMap5(0)
	, mChannelMap6(0)
	, mChannelMap7(0)
	, mChannelMap8(0)
	, mChannelMap9(0)
	, mChannelMap10(0)
	, mChannelMap11(0)
	, mChannelMap12(0)
	, mChannelMap13(0)
	, mChannelMap14(0)
	, mChannelMap15(0)
	, mReleaseNoteAfterHalfDuration(FALSE)
{

}

CImportMIDI::~CImportMIDI()
{
}

void CImportMIDI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mTimeMultiplier);
	DDV_MinMaxInt(pDX, mTimeMultiplier, 1, 100);
	DDX_Text(pDX, IDC_EDIT61, mBlockLength);
	DDV_MinMaxInt(pDX, mBlockLength, 1, 1024);
	DDX_Text(pDX, IDC_EDIT2, mChannelMap0);
	DDV_MinMaxInt(pDX, mChannelMap0, 0, 3);
	DDX_Text(pDX, IDC_EDIT39, mChannelMap1);
	DDV_MinMaxInt(pDX, mChannelMap1, 0, 3);
	DDX_Text(pDX, IDC_EDIT40, mChannelMap2);
	DDV_MinMaxInt(pDX, mChannelMap2, 0, 3);
	DDX_Text(pDX, IDC_EDIT41, mChannelMap3);
	DDV_MinMaxInt(pDX, mChannelMap3, 0, 3);
	DDX_Text(pDX, IDC_EDIT49, mChannelMap4);
	DDV_MinMaxInt(pDX, mChannelMap4, 0, 3);
	DDX_Text(pDX, IDC_EDIT6, mChannelMap5);
	DDV_MinMaxInt(pDX, mChannelMap5, 0, 3);
	DDX_Text(pDX, IDC_EDIT4, mChannelMap6);
	DDV_MinMaxInt(pDX, mChannelMap6, 0, 3);
	DDX_Text(pDX, IDC_EDIT43, mChannelMap7);
	DDV_MinMaxInt(pDX, mChannelMap7, 0, 3);
	DDX_Text(pDX, IDC_EDIT5, mChannelMap8);
	DDV_MinMaxInt(pDX, mChannelMap8, 0, 3);
	DDX_Text(pDX, IDC_EDIT50, mChannelMap9);
	DDV_MinMaxInt(pDX, mChannelMap9, 0, 3);
	DDX_Text(pDX, IDC_EDIT64, mChannelMap10);
	DDV_MinMaxInt(pDX, mChannelMap10, 0, 3);
	DDX_Text(pDX, IDC_EDIT65, mChannelMap11);
	DDV_MinMaxInt(pDX, mChannelMap11, 0, 3);
	DDX_Text(pDX, IDC_EDIT66, mChannelMap12);
	DDV_MinMaxInt(pDX, mChannelMap12, 0, 3);
	DDX_Text(pDX, IDC_EDIT10, mChannelMap13);
	DDV_MinMaxInt(pDX, mChannelMap13, 0, 3);
	DDX_Text(pDX, IDC_EDIT11, mChannelMap14);
	DDV_MinMaxInt(pDX, mChannelMap14, 0, 3);
	DDX_Text(pDX, IDC_EDIT67, mChannelMap15);
	DDV_MinMaxInt(pDX, mChannelMap15, 0, 3);
	DDX_Check(pDX, IDC_CHECK1, mReleaseNoteAfterHalfDuration);
}


BEGIN_MESSAGE_MAP(CImportMIDI, CDialogEx)
END_MESSAGE_MAP()


// CImportMIDI message handlers
