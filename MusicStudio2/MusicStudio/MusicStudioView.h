// MusicStudioView.h : interface of the CMusicStudioView class
//


#pragma once
#include "afxwin.h"
#include "TrackEditNum.h"
#include "ViewUtility.h"
#include "afxcmn.h"

class CMusicStudioView : public CFormView
{
protected: // create from serialization only
	CMusicStudioView();
	DECLARE_DYNCREATE(CMusicStudioView)

public:
	enum{ IDD = IDD_MUSICSTUDIO_FORM };
	// For the extended editing class to communicate back for the mTrack value
	// Indexes less than 8 are used for tracks
	enum
	{
		kTableEditingIndex = 8,
		kTableEditingValueIndex,
		kTrackerBlockEditingIndex	// For kMaxTracks
	};


// Attributes
public:
	CMusicStudioDoc* GetDocument() const;
	static CMusicStudioView *mLastView;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMusicStudioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CEdit mEnvelope0;
	CEdit mEnvelope1;
	CEdit mEnvelopeNum;
	afx_msg void OnEnvelopeNumChange();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit mBlockData;
	CEdit mBlockNum;
	afx_msg void OnBlockNumChange();
	int mIgnoreAnyUpdates;
protected:
	int GetCurrentBlockNum(void);
	int GetCurrentEnvelopeNum(void);
	void CommonSetModified(void)
	{
		// If any updates come from our own rendering then ignore them
		if (mIgnoreAnyUpdates)
		{
			return;
		}
		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(this);
	}

	void CommonUpdateRadio(void);

public:
	void CommonSaveC64File(const char *address,const bool includeSoundEffectCode = false,const bool runningInEditor = false);

public:
	afx_msg void OnEditOptimise();
	void RedrawView(void);
	void DrawTracks(void);
	void CommonStartPlaying(const bool playSFX);
	afx_msg void OnBnClickedPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
	
	void *mPlayerHandle;
	afx_msg void OnChangeBlockData();
	CEdit mEnvelopeName;
	afx_msg void OnEnChangeEditEnvelope0();
	afx_msg void OnEnChangeEditEnvelope1();
	afx_msg void OnEnChangeEditEnvelopeTranspose();
	afx_msg void OnEnChangeEditEnvelopeName();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnFileExportToC64();

	void CommonPreExport()
	{
		OnBnClickedStop();

		// Make sure the SID doesn't have silly block overrides set
		mOverrideStartTrack1.SetWindowText(_T(""));
		mOverrideStartTrack2.SetWindowText(_T(""));
		mOverrideStartTrack3.SetWindowText(_T(""));
	}

	afx_msg void OnUpdateFileExportToC64(CCmdUI *pCmdUI);
	afx_msg LRESULT OnEditNumInsert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditNumRemove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditNumOnSetValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditNumOnSetFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditNumOnArrowKey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditNumEmptyKeyDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditNumKeyInsert(WPARAM wParam, LPARAM lParam);

	CEdit mChosenSong;
	CEdit mBlockByteSize;
	CEdit mBlockDataSide;
	static const int kVisibleEditsPerTrack = 32;
	CTrackEditNum mEdits[MusicStudio1::MusicFile::kMaxTracks][kVisibleEditsPerTrack];
	CEdit mEditStart[MusicStudio1::MusicFile::kMaxTracks];
	int mTrackOffsets[MusicStudio1::MusicFile::kMaxTracks];
	int mLastDisplayedBlockPos[MusicStudio1::MusicFile::kMaxTracks];
	int mLastHighlightedBlockRowOffset[MusicStudio1::MusicFile::kMaxTracks];
	afx_msg void OnDeltaposSpinTrack1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTrack2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTrack3(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateTrackView(const int track,const int spin = 0);
	static COLORREF GetBackgroundColourForTrackCommand(const unsigned char trackCommand);
	static COLORREF GetBackgroundColourForTableControl(const unsigned char value);
	CEdit mEnvelopeTranspose;
	CEdit mTrack1PlayingBlock;
	CEdit mTrack2PlayingBlock;
	CEdit mTrack3PlayingBlock;
	CButton mMuteTrack1;
	CButton mMuteTrack2;
	CButton mMuteTrack3;
	void CommonHandleMute(CButton &button,const int voice);
	afx_msg void OnBnClickedCheckMuteTrack1();
	afx_msg void OnBnClickedCheckMuteTrack2();
	afx_msg void OnBnClickedCheckMuteTrack3();
	CEdit mPlayingTime;
	afx_msg void OnViewExtendedview();

	static const int kVisibleEditsPerTable = 20;
	CTrackEditNum mTableEdits[2][kVisibleEditsPerTable];
	CEdit mTableEditsTicks[kVisibleEditsPerTable];
	int mCurrentTableEditIndex;
	int mTableEditingOffsets[MusicStudio1::MusicFile::kMaxTables];

	void UpdateTableView(const int table,const int pos);

protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	void DrawBlockErrorReport(void);
	afx_msg void OnEnVscrollEdit1();
	afx_msg void OnBnClickedPlayEnvelope();
	bool mLastPlayWasSFX;
	afx_msg void OnBnClickedCheck4ForceUsed();
	afx_msg void OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit mOverrideStartTrack1;
	CEdit mOverrideStartTrack2;
	CEdit mOverrideStartTrack3;
	CEdit mCurrentTrackEditPos;
	CEdit mBlockDurSize;
	CEdit mTableEditingPosition;
	afx_msg void OnTableComboChange();
	CComboBox mTableCombo;
	afx_msg void OnChangeTablePos();
	afx_msg void OnTableDelta(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit mCurrentTableEditPosition;
	afx_msg void OnWaveTableButton();
	afx_msg void OnNoteTableButton();
	afx_msg void OnPulseTableButton();
	afx_msg void OnFilterTableButton();
	CEdit mEditWaveTable;
	CEdit mEditNoteTable;
	CEdit mEditPulseTable;
	CEdit mEditFilterTable;
	CButton mButtonWaveTable;
	CButton mButtonNoteTable;
	CButton mButtonPulseTable;
	CButton mButtonFilterTable;
	afx_msg void OnChangeEnvelopeWaveTable();
	afx_msg void OnChangeEnvelopeNoteTable();
	afx_msg void OnChangeEnvelopePulseTable();
	afx_msg void OnChangeEnvelopeFilterTable();
	afx_msg void OnAllowVoiceEffects();
	CButton mAllowVoiceEffects;
	afx_msg void OnBnClickedButton4();

	CButton mTrackerBlockEditStateButton;
	CSpinButtonCtrl mBlockEditSpinCtrl;
	CSpinButtonCtrl mBlockEditSpinCtrl2;
	CStatic mBlockEditTransposeText;
	CStatic mBlockEditBlockText;
	CStatic mBlockEditTrackerText;
	CEdit mBlockEditTrackerNum1;
	CEdit mBlockEditTrackerNum2;
	CEdit mBlockEditTrackerNum3;
	afx_msg void OnChangeBlockEditTrackerNum1();
	afx_msg void OnChangeBlockEditTrackerNum2();
	afx_msg void OnChangeBlockEditTrackerNum3();

	static const int kVisibleTrackerModeLines = 27;
	
	CStatic mTrackerModeRowNum[MusicStudio1::MusicFile::kMaxTracks][kVisibleTrackerModeLines];
	CTrackEditNum mTrackerModeEdits[MusicStudio1::MusicFile::kMaxTracks][kVisibleTrackerModeLines];
	CTrackEditNum mTrackerModeEnvelope[MusicStudio1::MusicFile::kMaxTracks][kVisibleTrackerModeLines];
	CTrackEditNum mTrackerModeEffect[MusicStudio1::MusicFile::kMaxTracks][kVisibleTrackerModeLines];
	int mBlockEditTrackerOffsets[MusicStudio1::MusicFile::kMaxTracks];
	int mBlockEditTrackerNum[MusicStudio1::MusicFile::kMaxTracks];
	CStatic mBlockEditTrackerTempoText1;
	CEdit mBlockEditTrackerTempoEdit1;
	CStatic mBlockEditTrackerTempoText2;
	CStatic mBlockEditTrackerTempoText3;
	CEdit mBlockEditTrackerTempoEdit2;
	CEdit mBlockEditTrackerTempoEdit3;

	int mC64BlockTimerLo;
	int mC64BlockTimerHi;

	int mC64FrameCounter;

	afx_msg void OnChangeBlockEditTrackerTempoEdit1();
	afx_msg void OnChangeBlockEditTrackerTempoEdit2();
	afx_msg void OnChangeBlockEditTrackerTempoEdit3();

	void DrawTrackerBlocks(void);

	void DrawTrackerBlocksErrors(void);
	
	void SafePlayerFree(void);
	afx_msg void OnBnClickedSaveEnvelope();
	afx_msg void OnBnClickedSaveEnvelope(CString &filename);
	afx_msg void OnBnClickedLoadEnvelope();
	afx_msg void OnBnClickedLoadEnvelope(CString &filename);
	afx_msg void OnFileRipSID();
	CStatic mDynamicHelp;
	afx_msg void OnEnSetfocusEdit1();

	enum HelpState {
		kNone,
		kTrack,
		kBlock,
		kWave,
		kPulse,
		kNote,
		kFilter
	};
	HelpState mHelpState;

	void SetHelpState(const HelpState state);
	afx_msg void OnBnClickedInsertEnvelope();
	afx_msg void OnFileExporttoOric();
	afx_msg void OnUpdateFileExporttoOric(CCmdUI *pCmdUI);
	CEdit mEnvelopeNumDecimal;
	afx_msg void OnEditClearalldata();
	afx_msg void OnEditSetdefaultdata();
	afx_msg void OnFileExporttoMIDI();
};

#ifndef _DEBUG  // debug version in MusicStudioView.cpp
inline CMusicStudioDoc* CMusicStudioView::GetDocument() const
   { return reinterpret_cast<CMusicStudioDoc*>(m_pDocument); }
#endif

