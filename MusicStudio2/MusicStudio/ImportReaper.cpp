// ImportReaper.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudio.h"
#include "ImportReaper.h"
#include "afxdialogex.h"


// CImportReaper dialog

IMPLEMENT_DYNAMIC(CImportReaper, CDialogEx)

CImportReaper::CImportReaper(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportReaper::IDD, pParent)
	, mRemoveMutedItems(TRUE)
	, mRemoveTracksWithoutItems(TRUE)
	, mProcessTrack1(TRUE)
	, mProcessTrack2(TRUE)
	, mProcessTrack3(TRUE)
	, mReaperTempo(120)
	, mTempoMultiplier(16)
	, mIncludeEmptyTime(FALSE)
	, mBeatAdjustment(FALSE)
	, mRemoveDuplicateBlocks(TRUE)
	, mC64FramesPerSecond(50)
	, mOutputBlockTempo(1)
{

}

CImportReaper::~CImportReaper()
{
}

void CImportReaper::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, mRemoveMutedItems);
	DDX_Check(pDX, IDC_CHECK2, mRemoveTracksWithoutItems);
	DDX_Check(pDX, IDC_CHECK3, mProcessTrack1);
	DDX_Check(pDX, IDC_CHECK10, mProcessTrack2);
	DDX_Check(pDX, IDC_CHECK11, mProcessTrack3);
	DDX_Text(pDX, IDC_EDIT1, mReaperTempo);
	DDV_MinMaxInt(pDX, mReaperTempo, 1, 32768);
	DDX_Text(pDX, IDC_EDIT61, mTempoMultiplier);
	DDV_MinMaxInt(pDX, mTempoMultiplier, 1, 256);
	DDX_Check(pDX, IDC_CHECK4, mIncludeEmptyTime);
	DDX_Check(pDX, IDC_CHECK5, mBeatAdjustment);
	DDX_Check(pDX, IDC_CHECK6, mRemoveDuplicateBlocks);
	DDX_Text(pDX, IDC_EDIT63, mC64FramesPerSecond);
	DDV_MinMaxInt(pDX, mC64FramesPerSecond, 1, 256);
	DDX_Text(pDX, IDC_EDIT76, mOutputBlockTempo);
	DDV_MinMaxInt(pDX, mOutputBlockTempo, 1, 20);
}


BEGIN_MESSAGE_MAP(CImportReaper, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK2, &CImportReaper::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CImportReaper message handlers


void CImportReaper::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
}
