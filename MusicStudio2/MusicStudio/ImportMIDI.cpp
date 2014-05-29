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
	, mBytesUsed0(0)
	, mBytesUsed1(0)
	, mBytesUsed2(0)
	, mBytesUsed3(0)
	, mBytesUsed4(0)
	, mBytesUsed5(0)
	, mBytesUsed6(0)
	, mBytesUsed7(0)
	, mBytesUsed8(0)
	, mBytesUsed9(0)
	, mBytesUsed10(0)
	, mBytesUsed11(0)
	, mBytesUsed12(0)
	, mBytesUsed13(0)
	, mBytesUsed14(0)
	, mBytesUsed15(0)
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
	DDX_Text(pDX, IDC_EDIT13, mBytesUsed0);
	DDX_Text(pDX, IDC_EDIT68, mBytesUsed1);
	DDX_Text(pDX, IDC_EDIT69, mBytesUsed2);
	DDX_Text(pDX, IDC_EDIT70, mBytesUsed3);
	DDX_Text(pDX, IDC_EDIT52, mBytesUsed4);
	DDX_Text(pDX, IDC_EDIT17, mBytesUsed5);
	DDX_Text(pDX, IDC_EDIT18, mBytesUsed6);
	DDX_Text(pDX, IDC_EDIT53, mBytesUsed7);
	DDX_Text(pDX, IDC_EDIT20, mBytesUsed8);
	DDX_Text(pDX, IDC_EDIT71, mBytesUsed9);
	DDX_Text(pDX, IDC_EDIT72, mBytesUsed10);
	DDX_Text(pDX, IDC_EDIT73, mBytesUsed11);
	DDX_Text(pDX, IDC_EDIT74, mBytesUsed12);
	DDX_Text(pDX, IDC_EDIT25, mBytesUsed13);
	DDX_Text(pDX, IDC_EDIT26, mBytesUsed14);
	DDX_Text(pDX, IDC_EDIT75, mBytesUsed15);
}


BEGIN_MESSAGE_MAP(CImportMIDI, CDialogEx)
END_MESSAGE_MAP()


// CImportMIDI message handlers
