#pragma once


// CImportReaper dialog

class CImportReaper : public CDialogEx
{
	DECLARE_DYNAMIC(CImportReaper)

public:
	CImportReaper(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportReaper();

// Dialog Data
	enum { IDD = IDD_IMPORTREAPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL mRemoveMutedItems;
	BOOL mRemoveTracksWithoutItems;
	afx_msg void OnBnClickedCheck2();
	BOOL mProcessTrack1;
	BOOL mProcessTrack2;
	BOOL mProcessTrack3;
	int mReaperTempo;
	int mTempoMultiplier;
	BOOL mIncludeEmptyTime;
	BOOL mBeatAdjustment;
	BOOL mRemoveDuplicateBlocks;
	int mC64FramesPerSecond;
	int mOutputBlockTempo;
};
