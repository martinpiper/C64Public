// MusicStudioView.cpp : implementation of the CMusicStudioView class
//

#include "stdafx.h"
#include <assert.h>
#include "MusicStudio.h"

#include "MusicStudioDoc.h"
#include "MusicStudioView.h"
#include "../RESID-FP/trunk/sidplay-2.0.9/src/SimpleInterface.h"
#include "../RESID-FP/trunk/sidplay-2.0.9/src/audio/mmsystem/ExternalSystem.h"
#include "MainFrm.h"
#include "ExportToC64.h"
#include "Instrument.h"
#include "RipSID.h"
#include "RNPlatform/Inc/StringUtils.h"
#include "RNPlatform/Inc/MessageHelper.h"
#include "RNPlatform/Inc/PlatformInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int gSoftViewRefreshTime = 100;
int gSoftFrequency = 44100;

// CMusicStudioView

IMPLEMENT_DYNCREATE(CMusicStudioView, CFormView)

BEGIN_MESSAGE_MAP(CMusicStudioView, CFormView)
	ON_EN_CHANGE(IDC_EDIT14, &CMusicStudioView::OnEnvelopeNumChange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMusicStudioView::OnDeltaposSpin2)
	ON_EN_CHANGE(IDC_EDIT3, &CMusicStudioView::OnBlockNumChange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMusicStudioView::OnDeltaposSpin1)
	ON_COMMAND(ID_EDIT_OPTIMISE, &CMusicStudioView::OnEditOptimise)
	ON_BN_CLICKED(IDC_BUTTON1, &CMusicStudioView::OnBnClickedPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CMusicStudioView::OnBnClickedStop)
	ON_EN_CHANGE(IDC_EDIT1, &CMusicStudioView::OnChangeBlockData)
	ON_EN_CHANGE(IDC_EDIT34, &CMusicStudioView::OnEnChangeEditEnvelopeName)
	ON_EN_CHANGE(IDC_EDIT6, &CMusicStudioView::OnEnChangeEditEnvelope0)
	ON_EN_CHANGE(IDC_EDIT7, &CMusicStudioView::OnEnChangeEditEnvelope1)
	ON_EN_CHANGE(IDC_EDIT2, &CMusicStudioView::OnEnChangeEditEnvelopeTranspose)
	ON_COMMAND(ID_EDIT_COPY, &CMusicStudioView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CMusicStudioView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CMusicStudioView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CMusicStudioView::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMusicStudioView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, &CMusicStudioView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMusicStudioView::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, &CMusicStudioView::OnEditUndo)
	ON_BN_CLICKED(IDC_RADIO1, &CMusicStudioView::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMusicStudioView::OnBnClickedRadio2)
	ON_COMMAND(ID_FILE_EXPORTTOC64, &CMusicStudioView::OnFileExportToC64)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTOC64, &CMusicStudioView::OnUpdateFileExportToC64)
	ON_MESSAGE(kEditNumInsert, OnEditNumInsert)
	ON_MESSAGE(kEditNumRemove, OnEditNumRemove)
	ON_MESSAGE(kEditNumSetValue, OnEditNumOnSetValue)
	ON_MESSAGE(kEditNumOnSetFocus, OnEditNumOnSetFocus)
	ON_MESSAGE(kEditNumOnArrowKey, OnEditNumOnArrowKey)
	ON_MESSAGE(kEditNumEmptyKeyDelete, OnEditNumEmptyKeyDelete)
	ON_MESSAGE(kEditNumKeyInsert, OnEditNumKeyInsert)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CMusicStudioView::OnDeltaposSpinTrack1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CMusicStudioView::OnDeltaposSpinTrack2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CMusicStudioView::OnDeltaposSpinTrack3)
	ON_BN_CLICKED(IDC_CHECK1, &CMusicStudioView::OnBnClickedCheckMuteTrack1)
	ON_BN_CLICKED(IDC_CHECK2, &CMusicStudioView::OnBnClickedCheckMuteTrack2)
	ON_BN_CLICKED(IDC_CHECK3, &CMusicStudioView::OnBnClickedCheckMuteTrack3)
	ON_COMMAND(ID_VIEW_EXTENDEDVIEW, &CMusicStudioView::OnViewExtendedview)
	ON_EN_VSCROLL(IDC_EDIT1, &CMusicStudioView::OnEnVscrollEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMusicStudioView::OnBnClickedPlayEnvelope)
	ON_BN_CLICKED(IDC_CHECK4, &CMusicStudioView::OnBnClickedCheck4ForceUsed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CMusicStudioView::OnDeltaposSpin7)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMusicStudioView::OnTableComboChange)
	ON_EN_CHANGE(IDC_EDIT49, &CMusicStudioView::OnChangeTablePos)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMusicStudioView::OnTableDelta)
	ON_BN_CLICKED(IDC_CHECK5, &CMusicStudioView::OnWaveTableButton)
	ON_BN_CLICKED(IDC_CHECK6, &CMusicStudioView::OnNoteTableButton)
	ON_BN_CLICKED(IDC_CHECK7, &CMusicStudioView::OnPulseTableButton)
	ON_BN_CLICKED(IDC_CHECK8, &CMusicStudioView::OnFilterTableButton)
	ON_EN_CHANGE(IDC_EDIT15, &CMusicStudioView::OnChangeEnvelopeWaveTable)
	ON_EN_CHANGE(IDC_EDIT16, &CMusicStudioView::OnChangeEnvelopeNoteTable)
	ON_EN_CHANGE(IDC_EDIT17, &CMusicStudioView::OnChangeEnvelopePulseTable)
	ON_EN_CHANGE(IDC_EDIT18, &CMusicStudioView::OnChangeEnvelopeFilterTable)
	ON_BN_CLICKED(IDC_CHECK9, &CMusicStudioView::OnAllowVoiceEffects)
	ON_BN_CLICKED(IDC_BUTTON4, &CMusicStudioView::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT55, &CMusicStudioView::OnChangeBlockEditTrackerNum1)
	ON_EN_CHANGE(IDC_EDIT56, &CMusicStudioView::OnChangeBlockEditTrackerNum2)
	ON_EN_CHANGE(IDC_EDIT57, &CMusicStudioView::OnChangeBlockEditTrackerNum3)
	ON_EN_CHANGE(IDC_EDIT58, &CMusicStudioView::OnChangeBlockEditTrackerTempoEdit1)
	ON_EN_CHANGE(IDC_EDIT59, &CMusicStudioView::OnChangeBlockEditTrackerTempoEdit2)
	ON_EN_CHANGE(IDC_EDIT60, &CMusicStudioView::OnChangeBlockEditTrackerTempoEdit3)
	ON_BN_CLICKED(IDC_BUTTON5, &CMusicStudioView::OnBnClickedSaveEnvelope)
	ON_BN_CLICKED(IDC_BUTTON6, &CMusicStudioView::OnBnClickedLoadEnvelope)
	ON_COMMAND(ID_FILE_RIPSID, &CMusicStudioView::OnFileRipSID)
	ON_EN_SETFOCUS(IDC_EDIT1, &CMusicStudioView::OnEnSetfocusEdit1)
	ON_BN_CLICKED(IDC_BUTTON7, &CMusicStudioView::OnBnClickedInsertEnvelope)
	ON_COMMAND(ID_FILE_EXPORTTOORIC, &CMusicStudioView::OnFileExporttoOric)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTOORIC, &CMusicStudioView::OnUpdateFileExporttoOric)
	ON_COMMAND(ID_EDIT_CLEARALLDATA, &CMusicStudioView::OnEditClearalldata)
	ON_COMMAND(ID_EDIT_SETDEFAULTDATA, &CMusicStudioView::OnEditSetdefaultdata)
	ON_COMMAND(ID_FILE_EXPORTTOMIDI, &CMusicStudioView::OnFileExporttoMIDI)
END_MESSAGE_MAP()

// CMusicStudioView construction/destruction
CMusicStudioView *CMusicStudioView::mLastView = 0;

CMusicStudioView::CMusicStudioView()
	: CFormView(CMusicStudioView::IDD) , mPlayerHandle(0) , mIgnoreAnyUpdates(0), mLastPlayWasSFX(false) ,
	mCurrentTableEditIndex(0) , mHelpState(kNone)
{
	mLastView = this;
	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
	{
		mTableEditingOffsets[i] = 1;
	}

	ZeroMemory(mBlockEditTrackerOffsets,sizeof(mBlockEditTrackerOffsets));
	ZeroMemory(mBlockEditTrackerNum,sizeof(mBlockEditTrackerNum));
	// MPi: TODO: Temporary until I add scrolling updates
	mBlockEditTrackerOffsets[0] = kVisibleTrackerModeLines/2;
	mBlockEditTrackerOffsets[1] = kVisibleTrackerModeLines/2;
	mBlockEditTrackerOffsets[2] = kVisibleTrackerModeLines/2;
}

CMusicStudioView::~CMusicStudioView()
{
	SafePlayerFree();
}

void CMusicStudioView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, mBlockNum);
	DDX_Control(pDX, IDC_EDIT1, mBlockData);
	DDX_Control(pDX, IDC_EDIT14, mEnvelopeNum);
	DDX_Control(pDX, IDC_EDIT34, mEnvelopeName);
	DDX_Control(pDX, IDC_EDIT6, mEnvelope0);
	DDX_Control(pDX, IDC_EDIT7, mEnvelope1);
	DDX_Control(pDX, IDC_EDIT36, mChosenSong);
	DDX_Control(pDX, IDC_EDIT37, mBlockByteSize);
	DDX_Control(pDX, IDC_EDIT38, mBlockDataSide);
	DDX_Control(pDX, IDC_EDIT2, mEnvelopeTranspose);
	DDX_Control(pDX, IDC_EDIT39, mTrack1PlayingBlock);
	DDX_Control(pDX, IDC_EDIT40, mTrack2PlayingBlock);
	DDX_Control(pDX, IDC_EDIT41, mTrack3PlayingBlock);
	DDX_Control(pDX, IDC_CHECK1, mMuteTrack1);
	DDX_Control(pDX, IDC_CHECK2, mMuteTrack2);
	DDX_Control(pDX, IDC_CHECK3, mMuteTrack3);
	DDX_Control(pDX, IDC_EDIT4, mPlayingTime);
	DDX_Control(pDX, IDC_EDIT45, mOverrideStartTrack1);
	DDX_Control(pDX, IDC_EDIT46, mOverrideStartTrack2);
	DDX_Control(pDX, IDC_EDIT47, mOverrideStartTrack3);
	DDX_Control(pDX, IDC_EDIT5, mCurrentTrackEditPos);
	DDX_Control(pDX, IDC_EDIT48, mBlockDurSize);
	DDX_Control(pDX, IDC_EDIT49, mTableEditingPosition);
	DDX_Control(pDX, IDC_COMBO1, mTableCombo);
	DDX_Control(pDX, IDC_EDIT50, mCurrentTableEditPosition);
	DDX_Control(pDX, IDC_EDIT15, mEditWaveTable);
	DDX_Control(pDX, IDC_EDIT16, mEditNoteTable);
	DDX_Control(pDX, IDC_EDIT17, mEditPulseTable);
	DDX_Control(pDX, IDC_EDIT18, mEditFilterTable);
	DDX_Control(pDX, IDC_CHECK5, mButtonWaveTable);
	DDX_Control(pDX, IDC_CHECK6, mButtonNoteTable);
	DDX_Control(pDX, IDC_CHECK7, mButtonPulseTable);
	DDX_Control(pDX, IDC_CHECK8, mButtonFilterTable);
	DDX_Control(pDX, IDC_CHECK9, mAllowVoiceEffects);
	DDX_Control(pDX, IDC_BUTTON4, mTrackerBlockEditStateButton);
	DDX_Control(pDX, IDC_SPIN1, mBlockEditSpinCtrl);
	DDX_Control(pDX, IDC_SPIN7, mBlockEditSpinCtrl2);
	DDX_Control(pDX, IDC_BETRANSPOSE, mBlockEditTransposeText);
	DDX_Control(pDX, IDC_BEBLOCKTEXT, mBlockEditBlockText);
	DDX_Control(pDX, IDC_BEBLOCKNUMBERS, mBlockEditTrackerText);
	DDX_Control(pDX, IDC_EDIT55, mBlockEditTrackerNum1);
	DDX_Control(pDX, IDC_EDIT56, mBlockEditTrackerNum2);
	DDX_Control(pDX, IDC_EDIT57, mBlockEditTrackerNum3);
	DDX_Control(pDX, IDC_BEBLOCKTEXT2, mBlockEditTrackerTempoText1);
	DDX_Control(pDX, IDC_EDIT58, mBlockEditTrackerTempoEdit1);
	DDX_Control(pDX, IDC_BEBLOCKTEXT3, mBlockEditTrackerTempoText2);
	DDX_Control(pDX, IDC_BEBLOCKTEXT4, mBlockEditTrackerTempoText3);
	DDX_Control(pDX, IDC_EDIT59, mBlockEditTrackerTempoEdit2);
	DDX_Control(pDX, IDC_EDIT60, mBlockEditTrackerTempoEdit3);
	DDX_Control(pDX, IDC_EDIT8, mDynamicHelp);
	DDX_Control(pDX, IDC_EDIT9, mEnvelopeNumDecimal);
}

BOOL CMusicStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CMusicStudioView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	IgnoreAnyUpdates();
	mBlockNum.SetWindowText(_T("00"));
	mEnvelopeNum.SetWindowText(_T("00"));
	mChosenSong.SetWindowText(_T("0"));
	mTrack1PlayingBlock.SetWindowText(_T(""));
	mTrack2PlayingBlock.SetWindowText(_T(""));
	mTrack3PlayingBlock.SetWindowText(_T(""));
	CommonUpdateRadio();

	// MPi: TODO: Find a better place to do this one time initialisation of the controls.
	// The window create, for example.
	int track,i,j;
	for (track = 0;track < MusicStudio1::MusicFile::kMaxTracks;track++)
	{
		const int kTrackSeparator = 67;
		mTrackOffsets[track] = 0;
		CRect rectLeft(346,58 + (track * kTrackSeparator),346 + 23,58 + (track * kTrackSeparator) + 18);

		mEditStart[track].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_DISABLED,rectLeft,this,(UINT)-1);
		mEditStart[track].SetFont(GetFont());

		rectLeft.OffsetRect(25,-2);
		for (i=0;i<64;i+=16)
		{
			CRect rect = rectLeft;
			int j;
			for (j=0;j<16;j++)
			{
				int k = i+j;
				if (k < kVisibleEditsPerTrack)
				{
					mEdits[track][k].mTrack = track;
	//				mEdits[k].CreateEx(WS_EX_CLIENTEDGE,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_UPPERCASE | ES_WANTRETURN,rect,this,(UINT)-1);
					mEdits[track][k].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_UPPERCASE | ES_WANTRETURN,rect,this,(UINT)-1);
					mEdits[track][k].SetFont(GetFont());
					mEdits[track][k].SetLimitText(2);
					rect.OffsetRect(32,0);
				}
			}
			rectLeft.OffsetRect(0,22);
		}

		UpdateTrackView(track);
	}


	const int kTableVSeparator = 24;
	const int kTableHSeparator = 26;
	const int kTableLeft = 360;
	const int kTableTop = 290;
	CRect rect(kTableLeft,kTableTop,kTableLeft + 23,kTableTop + 18);

	for (j=0;j<kVisibleEditsPerTable;j++)
	{
		mTableEdits[0][j].mTrack = kTableEditingIndex;
		mTableEdits[0][j].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_UPPERCASE | ES_WANTRETURN,rect,this,(UINT)-1);
		mTableEdits[0][j].SetFont(GetFont());
		mTableEdits[0][j].SetLimitText(2);
		rect.OffsetRect(0,kTableVSeparator);

		mTableEdits[1][j].mTrack = kTableEditingValueIndex;
		mTableEdits[1][j].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_UPPERCASE | ES_WANTRETURN,rect,this,(UINT)-1);
		mTableEdits[1][j].SetFont(GetFont());
		mTableEdits[1][j].SetLimitText(2);
		rect.OffsetRect(0,kTableVSeparator);

		mTableEditsTicks[j].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_DISABLED,rect,this,(UINT)-1);
		mTableEditsTicks[j].SetFont(GetFont());

		rect.OffsetRect(0,-kTableVSeparator);
		rect.OffsetRect(0,-kTableVSeparator);

		rect.OffsetRect(kTableHSeparator,0);
	}

	mCurrentTableEditIndex = 0;
	mTableCombo.SetCurSel(mCurrentTableEditIndex);
	SetNumToEdit(mTableEditingPosition,mTableEditingOffsets[mCurrentTableEditIndex]);

	// Setup the tracker mode edit view
	int columns[MusicStudio1::MusicFile::kMaxTracks];
	assert(MusicStudio1::MusicFile::kMaxTracks == 3);
	columns[0] = 0;
	columns[1] = 115;
	columns[2] = 230;

	mBlockEditTrackerNum1.SetWindowText(_T("00"));
	mBlockEditTrackerNum2.SetWindowText(_T("01"));
	mBlockEditTrackerNum3.SetWindowText(_T("02"));

	for (j=0;j<kVisibleTrackerModeLines;j++)
	{
		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			CRect rectLeft(columns[i],60 + (j*15),columns[i] + 20,60 + 14 + (j*15));
			CString text;
			text.Format(_T("%d"),j);
			mTrackerModeRowNum[i][j].Create(text,SS_RIGHT,rectLeft,this);
			mTrackerModeRowNum[i][j].SetFont(GetFont());

			rectLeft.OffsetRect(20,0);
			rectLeft.InflateRect(0,0,10,0);

			mTrackerModeEdits[i][j].mTrack = kTrackerBlockEditingIndex + i;
			mTrackerModeEdits[i][j].mIndex = j;
			mTrackerModeEdits[i][j].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_UPPERCASE | ES_WANTRETURN,rectLeft,this,(UINT)-1);
			mTrackerModeEdits[i][j].SetFont(GetFont());
			mTrackerModeEdits[i][j].SetLimitText(3);

			rectLeft.OffsetRect(31,0);
			rectLeft.InflateRect(0,0,-15,0);
			mTrackerModeEnvelope[i][j].mTrack = kTrackerBlockEditingIndex + i;
			mTrackerModeEnvelope[i][j].mIndex = j;
			mTrackerModeEnvelope[i][j].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_UPPERCASE | ES_WANTRETURN,rectLeft,this,(UINT)-1);
			mTrackerModeEnvelope[i][j].SetFont(GetFont());
			mTrackerModeEnvelope[i][j].SetLimitText(2);

			rectLeft.OffsetRect(16,0);
			rectLeft.InflateRect(0,0,30,0);
			mTrackerModeEffect[i][j].mTrack = kTrackerBlockEditingIndex + i;
			mTrackerModeEffect[i][j].mIndex = j;
			mTrackerModeEffect[i][j].CreateEx(0,_T("EDIT"),_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_UPPERCASE | ES_WANTRETURN | ES_AUTOHSCROLL,rectLeft,this,(UINT)-1);
			mTrackerModeEffect[i][j].SetFont(GetFont());
		}
	}

	GetDocument()->mTrackerBlockEditState = !GetDocument()->mTrackerBlockEditState;
	OnBnClickedButton4();

	mBlockEditTrackerNum1.LimitText(2);
	mBlockEditTrackerNum2.LimitText(2);
	mBlockEditTrackerNum3.LimitText(2);
	mBlockEditTrackerTempoEdit1.LimitText(2);
	mBlockEditTrackerTempoEdit2.LimitText(2);
	mBlockEditTrackerTempoEdit3.LimitText(2);

	// Draw the whole view
	RedrawView();

#if 0
	// Debugging, remove later. Saves having to select the menu option every time the application starts.
	OnFileRipSID();
#endif
}

void CMusicStudioView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMusicStudioView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMusicStudioView diagnostics

#ifdef _DEBUG
void CMusicStudioView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMusicStudioView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMusicStudioDoc* CMusicStudioView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMusicStudioDoc)));
	return (CMusicStudioDoc*)m_pDocument;
}
#endif //_DEBUG


// CMusicStudioView message handlers

void CMusicStudioView::OnEnvelopeNumChange()
{
	IgnoreAnyUpdates();
	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		mEnvelope0.SetWindowText(_T(""));
		mEnvelope1.SetWindowText(_T(""));
		mEnvelopeName.SetWindowText(_T(""));
		mEnvelopeTranspose.SetWindowText(_T(""));
		GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
		return;
	}

	GetDlgItem(IDC_CHECK4)->EnableWindow();
	CheckDlgButton(IDC_CHECK4,GetDocument()->mEnvelopes[envelope].mIsUsed?BST_CHECKED:BST_UNCHECKED);
	SetNumToEdit(mEnvelope0,GetDocument()->mEnvelopes[envelope].mAttackDecay);
	SetNumToEdit(mEnvelope1,GetDocument()->mEnvelopes[envelope].mSustainRelease);
	SetNumToEdit(mEnvelopeTranspose,GetDocument()->mEnvelopes[envelope].mTranspose);
	mEnvelopeName.SetWindowText(GetDocument()->mEnvelopeNames[envelope]);
	
	SetNumToEdit(mEditWaveTable,GetDocument()->mEnvelopes[envelope].mTableWave);
	SetNumToEdit(mEditNoteTable,GetDocument()->mEnvelopes[envelope].mTableNote);
	SetNumToEdit(mEditPulseTable,GetDocument()->mEnvelopes[envelope].mTablePulse);
	SetNumToEdit(mEditFilterTable,GetDocument()->mEnvelopes[envelope].mTableFilter);
	mEditWaveTable.EnableWindow(GetDocument()->mEnvelopes[envelope].mActiveTableWave);
	mEditNoteTable.EnableWindow(GetDocument()->mEnvelopes[envelope].mActiveTableNote);
	mEditPulseTable.EnableWindow(GetDocument()->mEnvelopes[envelope].mActiveTablePulse);
	mEditFilterTable.EnableWindow(GetDocument()->mEnvelopes[envelope].mActiveTableFilter);
	mButtonWaveTable.SetCheck(GetDocument()->mEnvelopes[envelope].mActiveTableWave?BST_CHECKED:BST_UNCHECKED);
	mButtonNoteTable.SetCheck(GetDocument()->mEnvelopes[envelope].mActiveTableNote?BST_CHECKED:BST_UNCHECKED);
	mButtonPulseTable.SetCheck(GetDocument()->mEnvelopes[envelope].mActiveTablePulse?BST_CHECKED:BST_UNCHECKED);
	mButtonFilterTable.SetCheck(GetDocument()->mEnvelopes[envelope].mActiveTableFilter?BST_CHECKED:BST_UNCHECKED);
	mAllowVoiceEffects.SetCheck(GetDocument()->mEnvelopes[envelope].mAllowVoiceEffects?BST_CHECKED:BST_UNCHECKED);
}

void CMusicStudioView::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int blockNum = GetCurrentBlockNum();
	blockNum -= pNMUpDown->iDelta;
	if (blockNum < 0)
	{
		blockNum = 0;
	}
	if (blockNum >= MusicStudio1::MusicFile::kMaxBlocks)
	{
		blockNum = MusicStudio1::MusicFile::kMaxBlocks-1;
	}
	SetNumToEdit(mBlockNum,blockNum);
}

void CMusicStudioView::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int envelopeNum = GetCurrentEnvelopeNum();
	envelopeNum += pNMUpDown->iDelta;
	if (envelopeNum < 0)
	{
		envelopeNum = 0;
	}
	if (envelopeNum >= MusicStudio1::MusicFile::kMaxEnvelopes)
	{
		envelopeNum = MusicStudio1::MusicFile::kMaxEnvelopes-1;
	}
	SetNumToEdit(mEnvelopeNum,envelopeNum);
	mEnvelopeNumDecimal.SetWindowText(CString(RNReplicaNet::ToString(envelopeNum).c_str()));
}

void CMusicStudioView::OnBlockNumChange()
{
	IgnoreAnyUpdates();
	int block = GetCurrentBlockNum();
	if ((block < 0) || (block >= MusicStudio1::MusicFile::kMaxBlocks))
	{
		mBlockData.SetWindowText(_T(""));
		return;
	}
	mBlockData.SetWindowText(GetDocument()->mBlocks[block]);

	// mBlockData is a multi-line edit box and it doesn't seem to be sending updates during
	// initialisation so we force one since IgnoreAnyUpdates() has been used.
	OnChangeBlockData();
}

int CMusicStudioView::GetCurrentBlockNum(void)
{
	return GetNumFromEdit(mBlockNum);
}

int CMusicStudioView::GetCurrentEnvelopeNum(void)
{
	return GetNumFromEdit(mEnvelopeNum);
}

void CMusicStudioView::OnEditOptimise()
{
	GetDocument()->OptimiseTables(true);

	RedrawView();
}

void CMusicStudioView::RedrawView(void)
{
	IgnoreAnyUpdates();
	OnBlockNumChange();
	DrawTracks();
	OnEnvelopeNumChange();

	DrawTrackerBlocks();
	OnChangeTablePos();
}

void CMusicStudioView::DrawTracks(void)
{
	IgnoreAnyUpdates();
	UpdateTrackView(0);
	UpdateTrackView(1);
	UpdateTrackView(2);
}

void CMusicStudioView::CommonSaveC64File(const char *address,const bool includeSoundEffectCode,const bool runningInEditor)
{
	OnBnClickedStop();

	// Needed because the file window sets the current directory to where the user browsed.
	SetCurrentDirectory(theApp.mCurrentDirectory);

	GetDocument()->UpdateDocumentDataToInternalFile();
	MusicStudio1::MusicFile &theFile = GetDocument()->mMusicFile;

	int codeSize = 0;
	int trackSize = 0;
	int blockSize = 0;
	int envelopeSize = 0;
	int tableSize = 0;
	int requestedSong = 0;
	CString requested;
	mChosenSong.GetWindowText(requested);
	requestedSong = _ttoi(requested);
	if (includeSoundEffectCode && runningInEditor)
	{
		requestedSong = 0;
	}
	if (GetNumFromEdit(mOverrideStartTrack1) || GetNumFromEdit(mOverrideStartTrack2) || GetNumFromEdit(mOverrideStartTrack3))
	{
		requestedSong = -requestedSong - 1;
	}
	CStringA title(GetDocument()->mTitleInfo,32);
	CStringA author(GetDocument()->mAuthorInfo,32);
	CStringA released(GetDocument()->mReleasedInfo,32);
	if (!theFile.OptimiseAndWrite("..\\..\\acme.exe","--msvc -v3 --lib ..\\MusicStudioConvert\\ --lib ..\\..\\ ..\\MusicStudioConvert\\",address,codeSize,trackSize,blockSize,envelopeSize,tableSize,requestedSong,includeSoundEffectCode,title,author,released,GetDocument()->mUsing6581,runningInEditor))
	{
		CString text(_T("There was a build error. Please send the music file and an explanation of what you were trying to do to martin.piper@gmail.com\nDebug text follows...\n"));
		((CMainFrame*)theApp.m_pMainWnd)->m_wndOutput.AddString(text);

		CString debugOut(theFile.getLastDebugOutput().c_str());
		((CMainFrame*)theApp.m_pMainWnd)->m_wndOutput.AddString(debugOut);
		return;
	}


	CString text;
	text.Format(_T("Code size = $%x"),codeSize);
	((CMainFrame*)theApp.m_pMainWnd)->m_wndOutput.AddString(text);
	text.Format(_T("Track size = $%x  Block size = $%x"),trackSize,blockSize);
	((CMainFrame*)theApp.m_pMainWnd)->m_wndOutput.AddString(text);
	text.Format(_T("Envelope size = $%x  Table size = $%x"),envelopeSize,tableSize);
	((CMainFrame*)theApp.m_pMainWnd)->m_wndOutput.AddString(text);
	text.Format(_T("Total size = $%x"),codeSize + trackSize + blockSize + tableSize + envelopeSize);
	((CMainFrame*)theApp.m_pMainWnd)->m_wndOutput.AddString(text);

	mC64BlockTimerLo = GetDocument()->mMusicFile.GetAddressFromLabel(".blockTimerLo");
	mC64BlockTimerHi = GetDocument()->mMusicFile.GetAddressFromLabel(".blockTimerHi");
}

static volatile bool sThreadRunning = false;
static volatile bool sThreadKillMusic = false;
static DWORD WINAPI sPlayerPlayThread(LPVOID lpThreadParameter)
{
	DWORD procMask = 0,sysMask = 0;
	GetProcessAffinityMask(GetCurrentProcess(),&procMask,&sysMask);

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

	while (!sThreadKillMusic)
	{
		PlayerPlay(lpThreadParameter);
		Sleep(10);
	}
	
	sThreadRunning = false;
	return 0;
}


void CMusicStudioView::CommonStartPlaying(const bool playSFX)
{
	CommonSaveC64File("sid",playSFX,true);

	GetDocument()->ClearCapturedSIDData();
//	MemoryHook::mMemoryHook = GetDocument();

	char params[32];
	sprintf(params,"-f%d",gSoftFrequency);
	mPlayerHandle = AllocatePlayer(params,"t.sid");

	MusicStudio1::MusicFile &theFile = GetDocument()->mMusicFile;
	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		mLastDisplayedBlockPos[i] = -1;
		mLastHighlightedBlockRowOffset[i] = -1;

		int j;
		for (j=0;j<kVisibleTrackerModeLines;j++)
		{
			mTrackerModeEdits[i][j].SetColour();
			mTrackerModeEdits[i][j].RedrawWindow();
		}
	}

	// Communicate any block override settings to the C64
	if (theFile.GetAddressFromLabel(".editorTrackStart"))
	{
		PlayerSetMemoryByte(mPlayerHandle,theFile.GetAddressFromLabel(".editorTrackStart"),		theFile.MapTrackPosToOptimisedPos(0,GetNumFromEdit(mOverrideStartTrack1)));
		PlayerSetMemoryByte(mPlayerHandle,theFile.GetAddressFromLabel(".editorTrackStart")+1,	theFile.MapTrackPosToOptimisedPos(1,GetNumFromEdit(mOverrideStartTrack2)));
		PlayerSetMemoryByte(mPlayerHandle,theFile.GetAddressFromLabel(".editorTrackStart")+2,	theFile.MapTrackPosToOptimisedPos(2,GetNumFromEdit(mOverrideStartTrack3)));
	}

	mC64FrameCounter = theFile.GetAddressFromLabel(".frameCounter");

	OnBnClickedCheckMuteTrack1();
	OnBnClickedCheckMuteTrack2();
	OnBnClickedCheckMuteTrack3();

	sThreadRunning = true;
	sThreadKillMusic = false;
	CreateThread(0,0,sPlayerPlayThread,mPlayerHandle,0,0);

	SetTimer(1,gSoftViewRefreshTime,0);

	mLastPlayWasSFX = playSFX;
}

afx_msg void CMusicStudioView::OnBnClickedPlay()
{
	CommonStartPlaying(false);
}

afx_msg void CMusicStudioView::OnBnClickedStop()
{
	mLastPlayWasSFX = false;
	if (mPlayerHandle)
	{
		KillTimer(1);
		SafePlayerFree();
		mBlockDataSide.SetWindowText(_T(""));
		mTrack1PlayingBlock.SetWindowText(_T(""));
		mTrack2PlayingBlock.SetWindowText(_T(""));
		mTrack3PlayingBlock.SetWindowText(_T(""));
		mPlayingTime.SetWindowText(_T(""));
		DrawTracks();

		int i;
		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			int j;
			for (j=0;j<kVisibleTrackerModeLines;j++)
			{
				mTrackerModeEdits[i][j].SetColour();
				mTrackerModeEdits[i][j].RedrawWindow();
			}
		}
	}
}

void CMusicStudioView::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	CEdit *playingBlocks[] = {&mTrack1PlayingBlock,&mTrack2PlayingBlock,&mTrack3PlayingBlock};

	// TODO: Add your message handler code here and/or call default
	if (mPlayerHandle)
	{
		MusicStudio1::MusicFile &theFile = GetDocument()->mMusicFile;

//		PlayerPlay(mPlayerHandle);

//		float theTime = PlayerGetTime(mPlayerHandle);
		int intTime = PlayerGetMemoryByte(mPlayerHandle,mC64FrameCounter) | (PlayerGetMemoryByte(mPlayerHandle,mC64FrameCounter+1) << 8) | (PlayerGetMemoryByte(mPlayerHandle,mC64FrameCounter+2) << 16);
		float theTime = float(intTime) / 50.0f;
		CString realTime;
		realTime.Format(_T("%.2f secs"),theTime);
		mPlayingTime.SetWindowText(realTime);

		int displayingBlockNum = GetNumFromEdit(mBlockNum);
		// Block y pos and the track number
		std::map<int,int> blockPositions;

		bool anyTrackerBlockActvity[MusicStudio1::MusicFile::kMaxTracks];
		ZeroMemory(anyTrackerBlockActvity,sizeof(anyTrackerBlockActvity));

		int i;
		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			unsigned char pos = PlayerGetMemoryByte(mPlayerHandle,(unsigned short)(theFile.mC64trcn + i));
			int realPos = theFile.mTrackReverseIndex[i][pos];
			int j;
			for (j=0;j<kVisibleEditsPerTrack;j++)
			{
				bool changed = false;
				if (mEdits[i][j].mIndex == realPos)
				{
					changed = mEdits[i][j].SetColour(RGB(0,255,0));
				}
				else
				{
					changed = mEdits[i][j].SetColour(GetBackgroundColourForTrackCommand(GetDocument()->mTracks[i][mEdits[i][j].mIndex]));
				}
				if (changed)
				{
					mEdits[i][j].RedrawWindow();
				}
				if (mLastDisplayedBlockPos[i] != realPos)
				{
					mLastDisplayedBlockPos[i] = realPos;
					SetNumToEdit(*playingBlocks[i],GetDocument()->mTracks[i][realPos],_T("pos"),realPos);
				}
				int blockNum = GetDocument()->mTracks[i][realPos];
				// Draw the highlighted line in the tracker view
				int bi;
				for (bi = 0;bi < MusicStudio1::MusicFile::kMaxTracks;bi++)
				{
					if (blockNum == mBlockEditTrackerNum[bi])
					{
						anyTrackerBlockActvity[bi] = true;
						int currentTime = PlayerGetMemoryByte(mPlayerHandle,(unsigned short)mC64BlockTimerLo+i) | (((int)PlayerGetMemoryByte(mPlayerHandle,(unsigned short)mC64BlockTimerHi+i))<<8);
						int rowOffset = (currentTime / GetDocument()->mBlockTrackerTempos[blockNum]) - mBlockEditTrackerOffsets[bi] + (kVisibleTrackerModeLines/2);
						if (rowOffset != mLastHighlightedBlockRowOffset[bi])
						{
							if ((mLastHighlightedBlockRowOffset[bi] >= 0) && (mLastHighlightedBlockRowOffset[bi] < kVisibleTrackerModeLines))
							{
								mTrackerModeEdits[bi][mLastHighlightedBlockRowOffset[bi]].SetColour();
								mTrackerModeEdits[bi][mLastHighlightedBlockRowOffset[bi]].RedrawWindow();
								mLastHighlightedBlockRowOffset[bi] = -1;
							}

							mLastHighlightedBlockRowOffset[bi] = rowOffset;
							if ((mLastHighlightedBlockRowOffset[bi] >= 0) && (mLastHighlightedBlockRowOffset[bi] < kVisibleTrackerModeLines))
							{
								mTrackerModeEdits[bi][mLastHighlightedBlockRowOffset[bi]].SetColour(RGB(0,255,0));
								mTrackerModeEdits[bi][mLastHighlightedBlockRowOffset[bi]].RedrawWindow();
							}
						}
					}
				}
				// MPi: TODO: Highlight the active block line with a customer draw and background fill
				if (blockNum == displayingBlockNum)
				{
					int byteOffset = PlayerGetMemoryByte(mPlayerHandle,(unsigned short)(theFile.mC64blcn + i));
					std::pair<std::map<int,int>::iterator,bool> ret = blockPositions.insert(std::pair<int,int>(byteOffset,1<<i));
					// It didn't insert to update the existing entry
					if (!ret.second)
					{
						(*ret.first).second |= 1<<i;
					}
				}
			}
		}

		int bi;
		for (bi = 0;bi < MusicStudio1::MusicFile::kMaxTracks;bi++)
		{
			if (!anyTrackerBlockActvity[bi])
			{
				if ((mLastHighlightedBlockRowOffset[bi] >= 0) && (mLastHighlightedBlockRowOffset[bi] < kVisibleTrackerModeLines))
				{
					mTrackerModeEdits[bi][mLastHighlightedBlockRowOffset[bi]].SetColour();
					mTrackerModeEdits[bi][mLastHighlightedBlockRowOffset[bi]].RedrawWindow();
					mLastHighlightedBlockRowOffset[bi] = -1;
				}
			}
		}

		// Render any block positions
		if (!blockPositions.empty())
		{
			int lastInserted = 0;
			CString toDraw(_T(""));
			int yposOffset = mBlockData.GetScrollPos(SB_VERT);
			std::map<int,int>::iterator st = blockPositions.begin();
			while (st != blockPositions.end())
			{
				int byteOffset = (*st).first;
				// byteOffset-1 is used because the player stores the next byte after the command
				std::map<int,int>::iterator toLine = theFile.mBlockByteToLine[displayingBlockNum].find(byteOffset-1);
				if (toLine != theFile.mBlockByteToLine[displayingBlockNum].end())
				{
					int realLine = (*toLine).second - yposOffset;
					if (realLine >= 0)
					{
						while (lastInserted < realLine)
						{
							lastInserted++;
							toDraw += _T("\r\n");
						}
						lastInserted++;
						switch((*st).second)
						{
							case 0:
							default:
								// Shouldn't ever get here
								toDraw += _T(">>\r\n");
								break;
							case 1:
								toDraw += _T("1>\r\n");
								break;
							case 2:
								toDraw += _T("2>\r\n");
								break;
							case 3:
								toDraw += _T("12>\r\n");
								break;
							case 4:
								toDraw += _T("3>\r\n");
								break;
							case 5:
								toDraw += _T("13>\r\n");
								break;
							case 6:
								toDraw += _T("23>\r\n");
								break;
							case 7:
								toDraw += _T("123\r\n");
								break;
						}
					}
				}
				st++;
			}
			mBlockDataSide.SetWindowText(toDraw);
		}
		else
		{
			mBlockDataSide.SetWindowText(_T(""));
		}
	}
	CFormView::OnTimer(nIDEvent);

	SetTimer(1,gSoftViewRefreshTime,0);
}

void CMusicStudioView::OnChangeBlockData()
{
	int block = GetCurrentBlockNum();
	if ((block < 0) || (block >= MusicStudio1::MusicFile::kMaxBlocks))
	{
		return;
	}
	CString newText;
	mBlockData.GetWindowText(newText);
	bool wasUpdated = false;
	if (newText != GetDocument()->mBlocks[block])
	{
		GetDocument()->mBlockLastEditedAsTracker[block] = false;
		GetDocument()->mBlockTrackerRowsCachedAsTempo[block] = 0;
		GetDocument()->mBlocks[block] = newText;
		wasUpdated = true;
	}


	CStringA ansiBlock(GetDocument()->mBlocks[block]);
	int byteSize = 0;
	int totalDuration = 0;
	GetDocument()->mMusicFile.SetBlockFromText(block,ansiBlock,GetDocument()->mBlockErrorReport[block],byteSize,totalDuration);
	if (wasUpdated)
	{
		GetDocument()->mBlockTrackerLengths[block] = totalDuration;
	}
	CString theSize;
	theSize.Format(_T("%x"),byteSize);
	mBlockByteSize.SetWindowText(theSize);
	theSize.Format(_T("%x"),totalDuration);
	mBlockDurSize.SetWindowText(theSize);
	DrawBlockErrorReport();

	CommonSetModified();
}

void CMusicStudioView::OnEnChangeEditEnvelopeName()
{
	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	mEnvelopeName.GetWindowText(GetDocument()->mEnvelopeNames[envelope]);
	CommonSetModified();
}

void CMusicStudioView::OnEnChangeEditEnvelope0()
{
	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	// Discard the cached version
	if (!mIgnoreAnyUpdates)
	{
		mLastPlayWasSFX = false;
	}

	GetDocument()->mEnvelopes[envelope].mAttackDecay = GetNumFromEdit(mEnvelope0);
	CommonSetModified();
}

void CMusicStudioView::OnEnChangeEditEnvelope1()
{
	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	// Discard the cached version
	if (!mIgnoreAnyUpdates)
	{
		mLastPlayWasSFX = false;
	}

	GetDocument()->mEnvelopes[envelope].mSustainRelease = GetNumFromEdit(mEnvelope1);
	CommonSetModified();
}

void CMusicStudioView::OnEnChangeEditEnvelopeTranspose()
{
	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	// Discard the cached version
	if (!mIgnoreAnyUpdates)
	{
		mLastPlayWasSFX = false;
	}

	GetDocument()->mEnvelopes[envelope].mTranspose = GetNumFromEdit(mEnvelopeTranspose);
	CommonSetModified();
}



void CMusicStudioView::OnEditCopy()
{
	CWnd *theFocus = GetFocus();
	if (theFocus && theFocus->IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		CEdit *theEditFocus = dynamic_cast<CEdit *>(theFocus);
		if (theEditFocus)
		{
			theEditFocus->Copy();
		}
	}
}

void CMusicStudioView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
	CWnd *theFocus = GetFocus();
	if (theFocus && theFocus->IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		CEdit *theEditFocus = dynamic_cast<CEdit *>(theFocus);
		int selL=0,selH=0;
		theEditFocus->GetSel(selL,selH);
		if (theEditFocus && (selL != selH))
		{
			pCmdUI->Enable();
		}
	}
}

void CMusicStudioView::OnEditCut()
{
	CWnd *theFocus = GetFocus();
	if (theFocus && theFocus->IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		CEdit *theEditFocus = dynamic_cast<CEdit *>(theFocus);
		if (theEditFocus)
		{
			theEditFocus->Cut();
		}
	}
}

void CMusicStudioView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	OnUpdateEditCopy(pCmdUI);
}

void CMusicStudioView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);

	CWnd *theFocus = GetFocus();
	if (theFocus && theFocus->IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		CEdit *theEditFocus = dynamic_cast<CEdit *>(theFocus);
		if (theEditFocus)
		{
			pCmdUI->Enable();
		}
	}
}

void CMusicStudioView::OnEditPaste()
{
	CWnd *theFocus = GetFocus();
	if (theFocus && theFocus->IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		CEdit *theEditFocus = dynamic_cast<CEdit *>(theFocus);
		if (theEditFocus)
		{
			theEditFocus->Paste();
		}
	}
}

void CMusicStudioView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	OnUpdateEditPaste(pCmdUI);
}

void CMusicStudioView::OnEditUndo()
{
	CWnd *theFocus = GetFocus();
	if (theFocus && theFocus->IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		CEdit *theEditFocus = dynamic_cast<CEdit *>(theFocus);
		if (theEditFocus)
		{
			theEditFocus->Undo();
		}
	}
}

void CMusicStudioView::CommonUpdateRadio(void)
{
	if (GetDocument()->mUsing6581)
	{
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
	}
	else
	{
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
	}
}

void CMusicStudioView::OnBnClickedRadio1()
{
	GetDocument()->mUsing6581 = true;
	CommonSetModified();
}


void CMusicStudioView::OnBnClickedRadio2()
{
	GetDocument()->mUsing6581 = false;
	CommonSetModified();
}

void CMusicStudioView::OnUpdateFileExportToC64(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CMusicStudioView::OnFileExportToC64()
{
	CommonPreExport();

	CExportToC64 dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CFileDialog getFile(FALSE);
	INT_PTR ret = getFile.DoModal();

	if (ret != IDOK)
	{
		return;
	}

	CString realPathName = getFile.GetPathName();

	_tremove(realPathName);

	if (dlg.mSaveSID)
	{
		CommonSaveC64File("sid");
		_trename(_T("t.sid"),realPathName);
	}
	else if (dlg.mSaveSelfRunning)
	{
		CommonSaveC64File("self",dlg.mIncludeSoundEffectCode);
		_trename(_T("t.prg"),realPathName);
	}
	else
	{
		CStringA ansi(dlg.mAddress);
		CommonSaveC64File(ansi,dlg.mIncludeSoundEffectCode);
		_trename(_T("t.prg"),realPathName);
	}
}

void CMusicStudioView::OnUpdateFileExporttoOric(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CMusicStudioView::OnFileExporttoOric()
{
	CommonPreExport();

	CFileDialog getFile(FALSE);
	INT_PTR ret = getFile.DoModal();

	if (ret != IDOK)
	{
		return;
	}

	CString realPathName = getFile.GetPathName();

	_tremove(realPathName);

	CommonSaveC64File("oric");
	_trename(_T("t.tap"),realPathName);
}

LRESULT CMusicStudioView::OnEditNumInsert(WPARAM wParam, LPARAM lParam)
{
	CommonSetModified();

	int index = LOBYTE(wParam);
	int track = HIBYTE(wParam);
	if ((track >= 0) && (track <= 7))
	{
		if ( (index >= 0) && (index <= MusicStudio1::MusicFile::kMaxTrackLength-1) )
		{
			int i;
			for (i=MusicStudio1::MusicFile::kMaxTrackLength-2;i>=index;i--)
			{
				GetDocument()->mTracks[track][i+1] = GetDocument()->mTracks[track][i];
				GetDocument()->mMusicFile.mTrackReverseIndex[track][i+1] = GetDocument()->mMusicFile.mTrackReverseIndex[track][i];
			}

			UpdateTrackView(track);

			CTrackEditNum::mProcessChanges = false;

			int sourceIndex = index - mEdits[track][0].mIndex;
			if ((sourceIndex >= 0) && (sourceIndex < kVisibleEditsPerTrack))
			{
				if ((sourceIndex > 0) && lParam)
				{
					mEdits[track][sourceIndex].mChar = mEdits[track][sourceIndex-1].mChar;
					mEdits[track][sourceIndex].mRepCnt = mEdits[track][sourceIndex-1].mRepCnt;
					mEdits[track][sourceIndex].mFlags = mEdits[track][sourceIndex-1].mFlags;
				}

				mEdits[track][sourceIndex].SetWindowText(_T(""));
				mEdits[track][sourceIndex].ResendLastChar();
			}

			CTrackEditNum::mProcessChanges = true;
		}
	}
	
	if ( track == kTableEditingIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			GetDocument()->AddTableByteAt(mCurrentTableEditIndex,index-1);

			UpdateTableView(mCurrentTableEditIndex,mTableEditingOffsets[mCurrentTableEditIndex]);
			OnEnvelopeNumChange();

			CTrackEditNum::mProcessChanges = false;

			int sourceIndex = index - mTableEdits[0][0].mIndex;
			if ((sourceIndex >= 0) && (sourceIndex < kVisibleEditsPerTable))
			{
				if ((sourceIndex > 0) && lParam)
				{
					mTableEdits[0][sourceIndex].mChar = mTableEdits[0][sourceIndex-1].mChar;
					mTableEdits[0][sourceIndex].mRepCnt = mTableEdits[0][sourceIndex-1].mRepCnt;
					mTableEdits[0][sourceIndex].mFlags = mTableEdits[0][sourceIndex-1].mFlags;
				}

				mTableEdits[0][sourceIndex].SetWindowText(_T(""));
				mTableEdits[0][sourceIndex].ResendLastChar();
			}

			CTrackEditNum::mProcessChanges = true;
		}
	}

	if ( track == kTableEditingValueIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			GetDocument()->AddTableByteAt(mCurrentTableEditIndex,index-1);

			UpdateTableView(mCurrentTableEditIndex,mTableEditingOffsets[mCurrentTableEditIndex]);
			OnEnvelopeNumChange();

			CTrackEditNum::mProcessChanges = false;

			int sourceIndex = index - mTableEdits[1][0].mIndex;
			if ((sourceIndex >= 0) && (sourceIndex < kVisibleEditsPerTable))
			{
				if ((sourceIndex > 0) && lParam)
				{
					mTableEdits[1][sourceIndex].mChar = mTableEdits[1][sourceIndex-1].mChar;
					mTableEdits[1][sourceIndex].mRepCnt = mTableEdits[1][sourceIndex-1].mRepCnt;
					mTableEdits[1][sourceIndex].mFlags = mTableEdits[1][sourceIndex-1].mFlags;
				}

				mTableEdits[1][sourceIndex].SetWindowText(_T(""));
				mTableEdits[1][sourceIndex].ResendLastChar();
			}

			CTrackEditNum::mProcessChanges = true;
		}
	}

	return 0;
}


LRESULT CMusicStudioView::OnEditNumRemove(WPARAM wParam, LPARAM lParam)
{
	CommonSetModified();

	int index = LOBYTE(wParam);
	int track = HIBYTE(wParam);
	if ((track >= 0) && (track <= 7))
	{
		if ( (index >= 0) && (index < MusicStudio1::MusicFile::kMaxTrackLength-1) )
		{
			int i;
			for (i=index;i<MusicStudio1::MusicFile::kMaxTrackLength-1;i++)
			{
				GetDocument()->mTracks[track][i] = GetDocument()->mTracks[track][i+1];
				GetDocument()->mMusicFile.mTrackReverseIndex[track][i] = GetDocument()->mMusicFile.mTrackReverseIndex[track][i+1];
			}
		}
		UpdateTrackView(track);
	}

	if ( track == kTableEditingIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			GetDocument()->RemoveTableByteAt(mCurrentTableEditIndex,index-1);
		}
		UpdateTableView(mCurrentTableEditIndex,mTableEditingOffsets[mCurrentTableEditIndex]);
		OnEnvelopeNumChange();
	}

	if ( track == kTableEditingValueIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			GetDocument()->RemoveTableByteAt(mCurrentTableEditIndex,index-1);
		}
		UpdateTableView(mCurrentTableEditIndex,mTableEditingOffsets[mCurrentTableEditIndex]);
		OnEnvelopeNumChange();
	}

	if ( (track >= kTrackerBlockEditingIndex) && (track < (kTrackerBlockEditingIndex + MusicStudio1::MusicFile::kMaxTracks)) )
	{
		int columnIndex = track - kTrackerBlockEditingIndex;
		int realIndex = index + mBlockEditTrackerOffsets[columnIndex] - (kVisibleTrackerModeLines/2);
		int currentBlock = mBlockEditTrackerNum[columnIndex];
		if (realIndex == GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock])
		{
			GetDocument()->mBlockLastEditedAsTracker[currentBlock] = true;
			mTrackerModeEdits[columnIndex][index].SetWindowText(_T("END"));
		}
		else if ((realIndex >= 0) && (realIndex < GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]))
		{
			GetDocument()->mBlockLastEditedAsTracker[currentBlock] = true;
			CString text;
			mTrackerModeEdits[columnIndex][index].GetWindowText(text);
			if (text.IsEmpty())
			{
				GetDocument()->mBlockTrackerRows[currentBlock][realIndex][0].Empty();
			}
			mTrackerModeEnvelope[columnIndex][index].GetWindowText(text);
			if (text.IsEmpty())
			{
				GetDocument()->mBlockTrackerRows[currentBlock][realIndex][1].Empty();
			}
			mTrackerModeEffect[columnIndex][index].GetWindowText(text);
			if (text.IsEmpty())
			{
				GetDocument()->mBlockTrackerRows[currentBlock][realIndex][2].Empty();
			}
			GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
			SetNumToEdit(mBlockNum,currentBlock);
		}
	}

	return 0;
}

LRESULT CMusicStudioView::OnEditNumOnSetValue(WPARAM wParam, LPARAM lParam)
{
	CommonSetModified();

	int index = LOBYTE(wParam);
	int track = HIBYTE(wParam);
	if ((track >= 0) && (track <= 7))
	{
		if ( (index >= 0) && (index < MusicStudio1::MusicFile::kMaxTrackLength) )
		{
			int sourceIndex = index - mEdits[track][0].mIndex;
			if ((sourceIndex >= 0) && (sourceIndex < kVisibleEditsPerTrack))
			{
				GetDocument()->mTracks[track][index] = GetNumFromEdit(mEdits[track][sourceIndex]);
				if (mEdits[track][sourceIndex].GetWindowTextLength() == 2)
				{
					CTrackEditNum::mProcessChanges = true;
					unsigned char trackCommand = GetDocument()->mTracks[track][index];
					mEdits[track][sourceIndex].SetColour(GetBackgroundColourForTrackCommand(trackCommand));
					SetNumToEdit(mEdits[track][sourceIndex],trackCommand);
					CTrackEditNum::mProcessChanges = true;
				}
			}
		}
	}

	if ( track == kTableEditingIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			int sourceIndex = index - mTableEdits[0][0].mIndex;
			if ((sourceIndex >= 0) && (sourceIndex < kVisibleEditsPerTable))
			{
				GetDocument()->mTablesControls[mCurrentTableEditIndex][index] = GetNumFromEdit(mTableEdits[0][sourceIndex]);
				if (mTableEdits[0][sourceIndex].GetWindowTextLength() == 2)
				{
					CTrackEditNum::mProcessChanges = true;
					unsigned char trackCommand = GetDocument()->mTablesControls[mCurrentTableEditIndex][index];
					mTableEdits[0][sourceIndex].SetColour(GetBackgroundColourForTableControl(trackCommand));
					mTableEdits[1][sourceIndex].SetColour(GetBackgroundColourForTableControl(trackCommand));
					SetNumToEdit(mTableEdits[0][sourceIndex],trackCommand);
					mTableEdits[1][sourceIndex].RedrawWindow();
					CTrackEditNum::mProcessChanges = true;
				}
			}
		}
	}

	if ( track == kTableEditingValueIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			int sourceIndex = index - mTableEdits[0][0].mIndex;
			if ((sourceIndex >= 0) && (sourceIndex < kVisibleEditsPerTable))
			{
				GetDocument()->mTablesValues[mCurrentTableEditIndex][index] = GetNumFromEdit(mTableEdits[1][sourceIndex]);
				if (mTableEdits[1][sourceIndex].GetWindowTextLength() == 2)
				{
					CTrackEditNum::mProcessChanges = true;
					SetNumToEdit(mTableEdits[1][sourceIndex],GetDocument()->mTablesValues[mCurrentTableEditIndex][index]);
					mTableEdits[0][sourceIndex].RedrawWindow();
					CTrackEditNum::mProcessChanges = true;
				}
			}
		}
	}

	if ( (track >= kTrackerBlockEditingIndex) && (track < (kTrackerBlockEditingIndex + MusicStudio1::MusicFile::kMaxTracks)) )
	{
		int columnIndex = track - kTrackerBlockEditingIndex;
		int realIndex = index + mBlockEditTrackerOffsets[columnIndex] - (kVisibleTrackerModeLines/2);
		int currentBlock = mBlockEditTrackerNum[columnIndex];
		if (realIndex == GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock])
		{
			GetDocument()->mBlockLastEditedAsTracker[currentBlock] = true;
			mTrackerModeEdits[columnIndex][index].SetWindowText(_T("END"));
		}
		else if ((realIndex >= 0) && (realIndex < GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]))
		{
			GetDocument()->mBlockLastEditedAsTracker[currentBlock] = true;
			mTrackerModeEdits[columnIndex][index].GetWindowText(GetDocument()->mBlockTrackerRows[currentBlock][realIndex][0]);
			mTrackerModeEnvelope[columnIndex][index].GetWindowText(GetDocument()->mBlockTrackerRows[currentBlock][realIndex][1]);
			mTrackerModeEffect[columnIndex][index].GetWindowText(GetDocument()->mBlockTrackerRows[currentBlock][realIndex][2]);
			GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
			SetNumToEdit(mBlockNum,currentBlock);
		}
	}
	return 0;
}

LRESULT CMusicStudioView::OnEditNumOnSetFocus(WPARAM wParam, LPARAM lParam)
{
	int index = LOBYTE(wParam);
	int track = HIBYTE(wParam);
	if ((track >= 0) && (track <= 7))
	{
		if ( (index >= 0) && (index < MusicStudio1::MusicFile::kMaxTrackLength) )
		{
			SetNumToEdit(mCurrentTrackEditPos,index);
			SetHelpState(kTrack);
		}
	}

	if ( (track == kTableEditingIndex) || (track == kTableEditingValueIndex) )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			SetNumToEdit(mCurrentTableEditPosition,index);
			switch(mCurrentTableEditIndex)
			{
				case 0:
					SetHelpState(kWave);
					break;

				case 1:
					SetHelpState(kNote);
					break;

				case 2:
					SetHelpState(kPulse);
					break;

				case 3:
					SetHelpState(kFilter);
					break;

				default:
					break;

			}
		}
	}
	return 0;
}

LRESULT CMusicStudioView::OnEditNumOnArrowKey(WPARAM wParam, LPARAM lParam)
{
	int index = LOBYTE(wParam);
	int track = HIBYTE(wParam);
	if ( (track >= kTrackerBlockEditingIndex) && (track < (kTrackerBlockEditingIndex + MusicStudio1::MusicFile::kMaxTracks)) )
	{
		int key = (int)lParam;
		int columnIndex = track - kTrackerBlockEditingIndex;
		int currentBlock = mBlockEditTrackerNum[columnIndex];

		if (key & CTrackEditNum::kControlKeyFlag)
		{
			if (LOWORD(key) == VK_UP)
			{
				if (mBlockEditTrackerOffsets[columnIndex] > 0)
				{
					mBlockEditTrackerOffsets[columnIndex]--;
					DrawTrackerBlocks();
					if ((index < (kVisibleTrackerModeLines-1)) && !mTrackerModeEdits[columnIndex][index].IsWindowEnabled())
					{
						mTrackerModeEdits[columnIndex][index+1].SetFocus();
					}
					return 1;
				}
			}

			if (LOWORD(key) == VK_DOWN)
			{
				if (mBlockEditTrackerOffsets[columnIndex] < GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock])
				{
					mBlockEditTrackerOffsets[columnIndex]++;
					DrawTrackerBlocks();
					if ((index > 0) && !mTrackerModeEdits[columnIndex][index].IsWindowEnabled())
					{
						mTrackerModeEdits[columnIndex][index-1].SetFocus();
					}
					return 1;
				}
			}
			return 0;
		}

		if (LOWORD(key) == VK_UP)
		{
			if (index > 0)
			{
				mTrackerModeEdits[columnIndex][index-1].SetFocus();
				return 1;
			}

			if ((index == 0) && ((mBlockEditTrackerOffsets[columnIndex]-(kVisibleTrackerModeLines/2)) > 0))
			{
				mBlockEditTrackerOffsets[columnIndex]--;
				DrawTrackerBlocks();
				return 1;
			}
		}

		if (LOWORD(key) == VK_DOWN)
		{
			if (index < (kVisibleTrackerModeLines-1))
			{
				mTrackerModeEdits[columnIndex][index+1].SetFocus();
				return 1;
			}

			if ((index == (kVisibleTrackerModeLines-1)) && ((mBlockEditTrackerOffsets[columnIndex]+(kVisibleTrackerModeLines/2)) < GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]))
			{
				mBlockEditTrackerOffsets[columnIndex]++;
				DrawTrackerBlocks();
				return 1;
			}
		}
	}
	return 0;
}

LRESULT CMusicStudioView::OnEditNumEmptyKeyDelete(WPARAM wParam, LPARAM lParam)
{
	int index = LOBYTE(wParam);
	int track = HIBYTE(wParam);

	if ( (track >= kTrackerBlockEditingIndex) && (track < (kTrackerBlockEditingIndex + MusicStudio1::MusicFile::kMaxTracks)) )
	{
		int columnIndex = track - kTrackerBlockEditingIndex;
		int realIndex = index + mBlockEditTrackerOffsets[columnIndex] - (kVisibleTrackerModeLines/2);
		int currentBlock = mBlockEditTrackerNum[columnIndex];

		if ((GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock] > 0) && (realIndex >= 0) && (realIndex < GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]))
		{
			GetDocument()->mBlockLastEditedAsTracker[currentBlock] = true;
			int i;
			for (i=realIndex+1;i< GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock];i++)
			{
				int j;
				for (j=0;j<3;j++)
				{
					GetDocument()->mBlockTrackerRows[currentBlock][i-1][j] = GetDocument()->mBlockTrackerRows[currentBlock][i][j];
				}
			}
			GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]--;
			GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
			SetNumToEdit(mBlockNum,currentBlock);
			DrawTrackerBlocks();
		}
		return 1;	// Don't let the edit control process the key press.
	}

	return 0;
}

LRESULT CMusicStudioView::OnEditNumKeyInsert(WPARAM wParam, LPARAM lParam)
{
	int index = LOBYTE(wParam);
	int track = HIBYTE(wParam);

	if ((track >= 0) && (track <= 7))
	{
		if ( (index >= 0) && (index <= MusicStudio1::MusicFile::kMaxTrackLength-1) )
		{
			int i;
			for (i=MusicStudio1::MusicFile::kMaxTrackLength-2;i>=index;i--)
			{
				GetDocument()->mTracks[track][i+1] = GetDocument()->mTracks[track][i];
				GetDocument()->mMusicFile.mTrackReverseIndex[track][i+1] = GetDocument()->mMusicFile.mTrackReverseIndex[track][i];
			}
			GetDocument()->mTracks[track][i+1] = 0;

			UpdateTrackView(track);
		}
	}
	
	if ( track == kTableEditingIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			GetDocument()->AddTableByteAt(mCurrentTableEditIndex,index-1);

			UpdateTableView(mCurrentTableEditIndex,mTableEditingOffsets[mCurrentTableEditIndex]);
			OnEnvelopeNumChange();
		}
	}

	if ( track == kTableEditingValueIndex )
	{
		if ( (index >= 0) && (index < (MusicStudio1::MusicFile::kMaxTableEntries+1)) )
		{
			GetDocument()->AddTableByteAt(mCurrentTableEditIndex,index-1);

			UpdateTableView(mCurrentTableEditIndex,mTableEditingOffsets[mCurrentTableEditIndex]);
			OnEnvelopeNumChange();
		}
	}
	
	if ( (track >= kTrackerBlockEditingIndex) && (track < (kTrackerBlockEditingIndex + MusicStudio1::MusicFile::kMaxTracks)) )
	{
		int columnIndex = track - kTrackerBlockEditingIndex;
		int realIndex = index + mBlockEditTrackerOffsets[columnIndex] - (kVisibleTrackerModeLines/2);
		int currentBlock = mBlockEditTrackerNum[columnIndex];
		if (realIndex == GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock])
		{
			GetDocument()->mBlockLastEditedAsTracker[currentBlock] = true;
			int i = GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock];
			int j;
			for (j=0;j<3;j++)
			{
				GetDocument()->mBlockTrackerRows[currentBlock][i][j] = "";
			}
			GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]++;
			GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
			SetNumToEdit(mBlockNum,currentBlock);
			DrawTrackerBlocks();
		}
		else if ((realIndex >= 0) && (realIndex < GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]))
		{
			GetDocument()->mBlockLastEditedAsTracker[currentBlock] = true;
			int i;
			for (i=GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock];i > realIndex;i--)
			{
				int j;
				for (j=0;j<3;j++)
				{
					GetDocument()->mBlockTrackerRows[currentBlock][i][j] = GetDocument()->mBlockTrackerRows[currentBlock][i-1][j];
				}
			}
			int j;
			for (j=0;j<3;j++)
			{
				GetDocument()->mBlockTrackerRows[currentBlock][i][j] = "";
			}
			GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock]++;
			GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
			SetNumToEdit(mBlockNum,currentBlock);
			DrawTrackerBlocks();
		}
		return 1;	// Don't let the edit control process the key press.
	}

	return 0;
}


void CMusicStudioView::OnDeltaposSpinTrack1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	mCurrentTrackEditPos.SetWindowText(_T(""));

	UpdateTrackView(0,pNMUpDown->iDelta * 16);
}

void CMusicStudioView::OnDeltaposSpinTrack2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	mCurrentTrackEditPos.SetWindowText(_T(""));

	UpdateTrackView(1,pNMUpDown->iDelta * 16);
}

void CMusicStudioView::OnDeltaposSpinTrack3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	mCurrentTrackEditPos.SetWindowText(_T(""));

	UpdateTrackView(2,pNMUpDown->iDelta * 16);
}

void CMusicStudioView::UpdateTrackView(const int track,const int spin)
{
	// If the edit controls are not created yet then don't do anything
	if (!mEdits[0][0].m_hWnd)
	{
		return;
	}
	CTrackEditNum::mProcessChanges = false;

	mTrackOffsets[track] += spin;
	if (mTrackOffsets[track] < 0)
	{
		mTrackOffsets[track] = 0;
	}
	else if (mTrackOffsets[track] > (MusicStudio1::MusicFile::kMaxTrackLength - kVisibleEditsPerTrack))
	{
		mTrackOffsets[track] = MusicStudio1::MusicFile::kMaxTrackLength - kVisibleEditsPerTrack;
	}

	// Update the track position indicator
	SetNumToEdit(mEditStart[track],mTrackOffsets[track]);

	// Update the track display
	int i;
	for (i=0;i<kVisibleEditsPerTrack;i++)
	{
		mEdits[track][i].mIndex = mTrackOffsets[track] + i;
		unsigned char trackCommand = GetDocument()->mTracks[track][mTrackOffsets[track] + i];

		mEdits[track][i].SetColour(GetBackgroundColourForTrackCommand(trackCommand));
		SetNumToEdit(mEdits[track][i],trackCommand);
	}

	CTrackEditNum::mProcessChanges = true;
}

COLORREF CMusicStudioView::GetBackgroundColourForTrackCommand(const unsigned char trackCommand)
{
	if (trackCommand >= MusicStudio1::kMusicPlayer_StopTrack)
	{
		return RGB(255,128,128);
	}
	if (trackCommand >= MusicStudio1::kMusicPlayer_TransposeBlock)
	{
		return RGB(192,192,255);
	}
	if (trackCommand >= MusicStudio1::kMusicPlayer_RepeatBlock)
	{
		return RGB(252,224,67);
	}
	return RGB(255,255,255);
}

COLORREF CMusicStudioView::GetBackgroundColourForTableControl(const unsigned char value)
{
	if (value == 0xff)
	{
		return RGB(255,128,128);
	}
	return RGB(255,255,255);
}


void CMusicStudioView::CommonHandleMute(CButton &button,const int voice)
{
	if (mPlayerHandle && !mLastPlayWasSFX)
	{
		if (button.GetCheck() == BST_CHECKED)
		{
			PlayerMute(mPlayerHandle,voice,true);
		}
		else
		{
			PlayerMute(mPlayerHandle,voice,false);
		}
	}
}

void CMusicStudioView::OnBnClickedCheckMuteTrack1()
{
	CommonHandleMute(mMuteTrack1,0);
}

void CMusicStudioView::OnBnClickedCheckMuteTrack2()
{
	CommonHandleMute(mMuteTrack2,1);
}

void CMusicStudioView::OnBnClickedCheckMuteTrack3()
{
	CommonHandleMute(mMuteTrack3,2);
}

void CMusicStudioView::OnViewExtendedview()
{
	CDocTemplate* pDocTemplate = GetDocument()->GetTemplateExtended();

	ASSERT_VALID(pDocTemplate);
	CFrameWnd* pFrame = pDocTemplate->CreateNewFrame(GetDocument(), GetParentFrame());
	if (pFrame == NULL)
	{
		TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
		return;     // command failed
	}

	pFrame->InitialUpdateFrame(GetDocument(),TRUE);
}

void CMusicStudioView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	RedrawView();
}

void CMusicStudioView::DrawBlockErrorReport(void)
{
	DrawTrackerBlocksErrors();
	mBlockDataSide.SetWindowText(_T(""));
	int block = GetCurrentBlockNum();
	if ((block < 0) || (block >= MusicStudio1::MusicFile::kMaxBlocks))
	{
		return;
	}
	std::list<int> &blockErrorReport = GetDocument()->mBlockErrorReport[block];
	if (blockErrorReport.empty())
	{
		return;
	}

	CString toDraw;
	int yposOffset = mBlockData.GetScrollPos(SB_VERT);
	std::list<int>::iterator st = blockErrorReport.begin();
	while (st != blockErrorReport.end())
	{
		int realLine = *st++;
		if (realLine < yposOffset)
		{
			continue;
		}

		while (yposOffset < realLine)
		{
			yposOffset++;
			toDraw += _T("\r\n");
		}
		yposOffset++;
		toDraw += _T("E>\r\n");
	}
	mBlockDataSide.SetWindowText(toDraw);
	return;
}

void CMusicStudioView::OnEnVscrollEdit1()
{
	DrawBlockErrorReport();
}

void CMusicStudioView::OnBnClickedPlayEnvelope()
{
	// Cache the result of the last SFX play so we don't spam the machine too much
	if (!mLastPlayWasSFX)
	{
		bool backupmTrackerBlockEditState = GetDocument()->mTrackerBlockEditState;
		GetDocument()->mTrackerBlockEditState = false;
		int i,j;
		// Create a temporary block with all the envelopes used
		const int kNumBackupBlocks = 5;
		CString backupBlock[kNumBackupBlocks];
		bool backupmBlockLastEditedAsTracker[kNumBackupBlocks];
		for (i=0;i<kNumBackupBlocks;i++)
		{
			backupBlock[i] = GetDocument()->mBlocks[i];
			backupmBlockLastEditedAsTracker[i] = GetDocument()->mBlockLastEditedAsTracker[i];
		}
		CString tempBlock;

		tempBlock.Format(_T("HRD\r\nDUR:40\r\nENV:%02x\r\nB-3\r\n+++\r\n+++\r\n+++\r\n+++\r\n"),GetCurrentEnvelopeNum());
		GetDocument()->mBlocks[0] = tempBlock;

		for (j=0;j<kNumBackupBlocks-1;j++)
		{
			GetDocument()->mBlockLastEditedAsTracker[j] = false;
			tempBlock = _T("DUR:02\r\n");
			CString temp;
			for (i = (j * MusicStudio1::MusicFile::kMaxEnvelopes) / (kNumBackupBlocks-1);i<((j+1) * MusicStudio1::MusicFile::kMaxEnvelopes) / (kNumBackupBlocks-1);i++)
			{
				temp.Format(_T("ENV:%02x\r\nC-0\r\n"),i);
				tempBlock += temp;
			}
			GetDocument()->mBlocks[j+1] = tempBlock;
		}

		// Create a blank bit of music except for the first kNumBackupBlocks blocks
		unsigned char trackBackup[MusicStudio1::MusicFile::kMaxTracks][MusicStudio1::MusicFile::kMaxTrackLength];
		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			for (j=0;j<MusicStudio1::MusicFile::kMaxTrackLength;j++)
			{
				trackBackup[i][j] = GetDocument()->mTracks[i][j];
				GetDocument()->mTracks[i][j] = 0;
			}
			GetDocument()->mTracks[i][0] = MusicStudio1::kMusicPlayer_StopTrack;
		}		
		GetDocument()->mTracks[0][0] = 0;
		GetDocument()->mTracks[0][1] = MusicStudio1::kMusicPlayer_StopTrack;
		GetDocument()->mTracks[0][2] = 1;
		GetDocument()->mTracks[0][3] = 2;
		GetDocument()->mTracks[0][4] = 3;
		GetDocument()->mTracks[0][5] = 4;
		GetDocument()->mTracks[0][6] = MusicStudio1::kMusicPlayer_StopTrack;

		// Play it
		CommonStartPlaying(true);
		
		// Restore the data
		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			for (j=0;j<MusicStudio1::MusicFile::kMaxTrackLength;j++)
			{
				GetDocument()->mTracks[i][j] = trackBackup[i][j];
			}
		}		
		for (i=0;i<kNumBackupBlocks;i++)
		{
			GetDocument()->mBlockLastEditedAsTracker[i] = backupmBlockLastEditedAsTracker[i];
			GetDocument()->mBlocks[i] = backupBlock[i];

			CStringA ansiBlock(GetDocument()->mBlocks[i]);
			int byteSize = 0;
			int totalDuration = 0;
			GetDocument()->mMusicFile.SetBlockFromText(i,ansiBlock,GetDocument()->mBlockErrorReport[i],byteSize,totalDuration);
		}
		GetDocument()->mTrackerBlockEditState = backupmTrackerBlockEditState;

		PlayerMute(mPlayerHandle,0,false);
		PlayerMute(mPlayerHandle,1,false);
		PlayerMute(mPlayerHandle,2,false);
	}
	else
	{
		int address;
		address = GetDocument()->mMusicFile.GetAddressFromLabel(".playSFXEnvelope");
		PlayerSetMemoryByte(mPlayerHandle,address,GetCurrentEnvelopeNum());

		address = GetDocument()->mMusicFile.GetAddressFromLabel(".playSFXChannel");
		PlayerSetMemoryByte(mPlayerHandle,address,1);
	}
}

void CMusicStudioView::OnBnClickedCheck4ForceUsed()
{
	int envelope = GetCurrentEnvelopeNum();
	if ((envelope >= 0) && (envelope < MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		GetDocument()->mEnvelopes[envelope].mIsUsed = (IsDlgButtonChecked(IDC_CHECK4) != 0);
		CommonSetModified();
	}
}

void CMusicStudioView::OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	int block = GetCurrentBlockNum();
	if ((block < 0) || (block >= MusicStudio1::MusicFile::kMaxBlocks))
	{
		return;
	}

	CString blockText;
	CString newBlockText;
	mBlockData.GetWindowText(blockText);

	int start = 0;
	CString token;
	token = blockText.Tokenize(_T("\n"),start);
	while (!token.IsEmpty())
	{
		CStringA aText(token);
		MusicStudio1::BlockEntry *entry = GetDocument()->mMusicFile.ParseText(aText);
		if (entry)
		{
			if (entry->Transpose(-pNMUpDown->iDelta))
			{
				char tempLine[256];
				entry->GetFormatted(tempLine);
				token = tempLine;
				token += "\r";
			}
			delete entry;
		}
		newBlockText += token;
		newBlockText += "\n";
		token = blockText.Tokenize(_T("\n"),start);
	}

	mBlockData.SetWindowText(newBlockText);
	OnChangeBlockData();
}

void CMusicStudioView::OnTableComboChange()
{
	mCurrentTableEditIndex = mTableCombo.GetCurSel();
	SetNumToEdit(mTableEditingPosition,mTableEditingOffsets[mCurrentTableEditIndex]);
}

void CMusicStudioView::UpdateTableView(const int table,const int pos)
{
	IgnoreAnyUpdates();
	int i;
	for (i=0;i<kVisibleEditsPerTable;i++)
	{
		int j = i + pos;
		if (j < (MusicStudio1::MusicFile::kMaxTableEntries+1))
		{
			mTableEdits[0][i].mIndex = j;
			mTableEdits[1][i].mIndex = j;
			unsigned char control = GetDocument()->mTablesControls[table][j];
			SetNumToEdit(mTableEdits[0][i],control);
			SetNumToEdit(mTableEdits[1][i],GetDocument()->mTablesValues[table][j]);

			mTableEdits[0][i].SetColour(GetBackgroundColourForTableControl(control));
			mTableEdits[1][i].SetColour(GetBackgroundColourForTableControl(control));
		}
	}

	for (i=0;i<kVisibleEditsPerTable;i++)
	{
		SetNumToEdit(mTableEditsTicks[i],pos + i);
	}
}

void CMusicStudioView::OnChangeTablePos()
{
	int value = GetNumFromEdit(mTableEditingPosition);
	int value2 = value;
	value = max(1,value);
	value = min((MusicStudio1::MusicFile::kMaxTableEntries - kVisibleEditsPerTable)+1,value);
	mTableEditingOffsets[mCurrentTableEditIndex] = value;
	UpdateTableView(mCurrentTableEditIndex,mTableEditingOffsets[mCurrentTableEditIndex]);
}

void CMusicStudioView::OnTableDelta(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	int value = GetNumFromEdit(mTableEditingPosition);
	value -= pNMUpDown->iDelta * 8;
	value = max(1,value);
	value = min((MusicStudio1::MusicFile::kMaxTableEntries - kVisibleEditsPerTable)+1,value);
	SetNumToEdit(mTableEditingPosition,value);
}

void CMusicStudioView::OnWaveTableButton()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mActiveTableWave = mButtonWaveTable.GetCheck()==BST_CHECKED;
	CommonSetModified();

	mEditWaveTable.EnableWindow(mButtonWaveTable.GetCheck()==BST_CHECKED);
}

void CMusicStudioView::OnNoteTableButton()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mActiveTableNote = mButtonNoteTable.GetCheck()==BST_CHECKED;
	CommonSetModified();

	mEditNoteTable.EnableWindow(mButtonNoteTable.GetCheck()==BST_CHECKED);
}

void CMusicStudioView::OnPulseTableButton()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mActiveTablePulse = mButtonPulseTable.GetCheck()==BST_CHECKED;
	CommonSetModified();

	mEditPulseTable.EnableWindow(mButtonPulseTable.GetCheck()==BST_CHECKED);
}

void CMusicStudioView::OnFilterTableButton()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mActiveTableFilter = mButtonFilterTable.GetCheck()==BST_CHECKED;
	CommonSetModified();

	mEditFilterTable.EnableWindow(mButtonFilterTable.GetCheck()==BST_CHECKED);
}

void CMusicStudioView::OnChangeEnvelopeWaveTable()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mTableWave = GetNumFromEdit(mEditWaveTable);
	CommonSetModified();
}

void CMusicStudioView::OnChangeEnvelopeNoteTable()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mTableNote = GetNumFromEdit(mEditNoteTable);
	CommonSetModified();
}

void CMusicStudioView::OnChangeEnvelopePulseTable()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mTablePulse = GetNumFromEdit(mEditPulseTable);
	CommonSetModified();
}

void CMusicStudioView::OnChangeEnvelopeFilterTable()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mTableFilter = GetNumFromEdit(mEditFilterTable);
	CommonSetModified();
}

void CMusicStudioView::OnAllowVoiceEffects()
{
	IGNOREANYUPDATES();

	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	GetDocument()->mEnvelopes[envelope].mAllowVoiceEffects = mAllowVoiceEffects.GetCheck()==BST_CHECKED;
	CommonSetModified();
}

void CMusicStudioView::OnBnClickedButton4()
{
	GetDocument()->mTrackerBlockEditState = !GetDocument()->mTrackerBlockEditState;
	bool trackerBlockEditState = GetDocument()->mTrackerBlockEditState;
	
	if (trackerBlockEditState)
	{
		DrawTrackerBlocks();
	}
	
	// Update the button depending on what mode we are in
	if (GetDocument()->mTrackerBlockEditState)
	{
		mTrackerBlockEditStateButton.SetWindowText(_T("DUR Block edit mode"));
	}
	else
	{
		mTrackerBlockEditStateButton.SetWindowText(_T("Tracker block edit mode"));
	}

	// Enable or disable the various controls depending on what they are for
	mBlockData.ShowWindow(trackerBlockEditState?SW_HIDE:SW_SHOW);
	mBlockDataSide.ShowWindow(trackerBlockEditState?SW_HIDE:SW_SHOW);
	mBlockEditSpinCtrl.ShowWindow(trackerBlockEditState?SW_HIDE:SW_SHOW);
	mBlockEditSpinCtrl2.ShowWindow(trackerBlockEditState?SW_HIDE:SW_SHOW);
	mBlockEditTransposeText.ShowWindow(trackerBlockEditState?SW_HIDE:SW_SHOW);
	mBlockEditBlockText.ShowWindow(trackerBlockEditState?SW_HIDE:SW_SHOW);
	mBlockNum.ShowWindow(trackerBlockEditState?SW_HIDE:SW_SHOW);

	// Tracker edit mode
	mBlockEditTrackerText.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerNum1.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerNum2.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerNum3.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerTempoText1.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerTempoEdit1.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerTempoText2.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerTempoEdit2.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerTempoText3.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
	mBlockEditTrackerTempoEdit3.ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);

	int i,j;
	for (j=0;j<kVisibleTrackerModeLines;j++)
	{
		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			mTrackerModeRowNum[i][j].ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
			mTrackerModeEdits[i][j].ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
			mTrackerModeEnvelope[i][j].ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
			mTrackerModeEffect[i][j].ShowWindow(trackerBlockEditState?SW_SHOW:SW_HIDE);
		}
	}
}

void CMusicStudioView::OnChangeBlockEditTrackerNum1()
{
	IGNOREANYUPDATES();
	DrawTrackerBlocks();
}

void CMusicStudioView::OnChangeBlockEditTrackerNum2()
{
	IGNOREANYUPDATES();
	DrawTrackerBlocks();
}

void CMusicStudioView::OnChangeBlockEditTrackerNum3()
{
	IGNOREANYUPDATES();
	DrawTrackerBlocks();
}

void CMusicStudioView::OnChangeBlockEditTrackerTempoEdit1()
{
	IGNOREANYUPDATES();
	int currentBlock = GetNumFromEdit(mBlockEditTrackerNum1);
	GetDocument()->mBlockTrackerTempos[currentBlock] = GetNumFromEdit(mBlockEditTrackerTempoEdit1);
	if (GetDocument()->mBlockTrackerTempos[currentBlock] < 1)
	{
		GetDocument()->mBlockTrackerTempos[currentBlock] = 1;
		return;
	}
	if (GetDocument()->mBlockLastEditedAsTracker[currentBlock])
	{
		GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
	}
	DrawTrackerBlocks();
	CommonSetModified();
	SetNumToEdit(mBlockNum,currentBlock);
}

void CMusicStudioView::OnChangeBlockEditTrackerTempoEdit2()
{
	IGNOREANYUPDATES();
	int currentBlock = GetNumFromEdit(mBlockEditTrackerNum2);
	GetDocument()->mBlockTrackerTempos[currentBlock] = GetNumFromEdit(mBlockEditTrackerTempoEdit2);
	if (GetDocument()->mBlockTrackerTempos[currentBlock] < 1)
	{
		GetDocument()->mBlockTrackerTempos[currentBlock] = 1;
		return;
	}
	if (GetDocument()->mBlockLastEditedAsTracker[currentBlock])
	{
		GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
	}
	DrawTrackerBlocks();
	CommonSetModified();
	SetNumToEdit(mBlockNum,currentBlock);
}

void CMusicStudioView::OnChangeBlockEditTrackerTempoEdit3()
{
	IGNOREANYUPDATES();
	int currentBlock = GetNumFromEdit(mBlockEditTrackerNum3);
	GetDocument()->mBlockTrackerTempos[currentBlock] = GetNumFromEdit(mBlockEditTrackerTempoEdit3);
	if (GetDocument()->mBlockTrackerTempos[currentBlock] < 1)
	{
		GetDocument()->mBlockTrackerTempos[currentBlock] = 1;
		return;
	}
	if (GetDocument()->mBlockLastEditedAsTracker[currentBlock])
	{
		GetDocument()->UpdateTrackerBlockToInternal(currentBlock);
	}
	DrawTrackerBlocks();
	CommonSetModified();
	SetNumToEdit(mBlockNum,currentBlock);
}

volatile static bool sUpdating = false;
void CMusicStudioView::DrawTrackerBlocks(void)
{
	if (!mTrackerModeRowNum[0][0].m_hWnd)
	{
		return;
	}
	if (sUpdating)
	{
		return;
	}

	sUpdating = true;

	IgnoreAnyUpdates();
	CTrackEditNum::mProcessChanges = false;

//	mTrackerModeEdits[i][j].

	MusicStudio1::MusicFile &theFile = GetDocument()->mMusicFile;	

	int currentBlock = GetNumFromEdit(mBlockEditTrackerNum1);
	SetNumToEdit(mBlockEditTrackerTempoEdit1,GetDocument()->mBlockTrackerTempos[currentBlock]);
	currentBlock = GetNumFromEdit(mBlockEditTrackerNum2);
	SetNumToEdit(mBlockEditTrackerTempoEdit2,GetDocument()->mBlockTrackerTempos[currentBlock]);
	currentBlock = GetNumFromEdit(mBlockEditTrackerNum3);
	SetNumToEdit(mBlockEditTrackerTempoEdit3,GetDocument()->mBlockTrackerTempos[currentBlock]);

	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		int currentBlock = 0;
		switch(i)
		{
			case 0:
			default:
				currentBlock = GetNumFromEdit(mBlockEditTrackerNum1);
				break;

			case 1:
				currentBlock = GetNumFromEdit(mBlockEditTrackerNum2);
				break;

			case 2:
				currentBlock = GetNumFromEdit(mBlockEditTrackerNum3);
				break;
		}
		mBlockEditTrackerNum[i] = currentBlock;

		int j;

		int tempo = GetDocument()->mBlockTrackerTempos[currentBlock];
		if (GetDocument()->mBlockLastEditedAsTracker[currentBlock])
		{
			GetDocument()->mBlockTrackerRowsCachedAsTempo[currentBlock] = tempo;
		}
		else if (!GetDocument()->mBlockLastEditedAsTracker[currentBlock] && (GetDocument()->mBlockTrackerRowsCachedAsTempo[currentBlock] != tempo))
		{
			int maxCalcedRow = GetDocument()->mBlockTrackerLengths[currentBlock] / tempo;
			GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock] = maxCalcedRow;
			GetDocument()->mBlockTrackerRowsCachedAsTempo[currentBlock] = tempo;

			for (j=0;j<maxCalcedRow;j++)
			{
				GetDocument()->mBlockTrackerRows[currentBlock][j][0] = "";
				GetDocument()->mBlockTrackerRows[currentBlock][j][1] = "";
				GetDocument()->mBlockTrackerRows[currentBlock][j][2] = "";
			}

			std::list<MusicStudio1::BlockEntry*>::iterator st = theFile.mBlocks[currentBlock].begin();
			char currentEnvelope[4] = {0};
			bool slurEncountered = false;
			bool hardResetEncountered = false;
			int lastDurationReleaseTick = 0;
			std::string lastNoteOutWhilstSluring;
			while (st != theFile.mBlocks[currentBlock].end())
			{
				MusicStudio1::BlockEntry *block = *st++;
				if (block->GetBlockTime() < 0)
				{
					continue;
				}
				int blockRow = block->GetBlockTime() / tempo;
				if (blockRow >= CMusicStudioDoc::kMaxInternalTrackerRows)
				{
					continue;
				}
				char text[128];
				block->GetFormatted(text);
				if (block->GetCommandName() == "SlurEnable")
				{
					slurEncountered = true;
				}
				else if (block->GetCommandName() == "SlurRelease")
				{
					slurEncountered = false;
				}

				if (block->GetCommandName() == "HardRestartEnable")
				{
					hardResetEncountered = true;
				}
				else if (block->GetCommandName() == "HardRestartDisable")
				{
					hardResetEncountered = false;
				}


				if (block->GetCommandName() == "Duration")
				{
					lastDurationReleaseTick = block->GetDataByte0();
				}
				else if (block->GetCommandName() == "DurationTicks")
				{
					lastDurationReleaseTick = block->GetDataByte0() - block->GetDataByte1();
				}
				else if (block->GetCommandName() == "RestNote")
				{
					if (blockRow != 0)
					{
						GetDocument()->mBlockTrackerRows[currentBlock][blockRow][0] = "+++";
					}
				}
				else if (block->GetCommandName() == "RestNoteRelease")
				{
					if (blockRow != 0)
					{
						GetDocument()->mBlockTrackerRows[currentBlock][blockRow][0] = "===";
					}
				}
				else if (block->GetCommandName() == "Envelope")
				{
					sprintf(currentEnvelope,"%02X",block->GetDataByte0());
				}
				else if (block->IsNote())
				{
					if (!slurEncountered)
					{
						lastNoteOutWhilstSluring.clear();
					}

					if (slurEncountered && (lastNoteOutWhilstSluring == text))
					{
						continue;
					}

					GetDocument()->mBlockTrackerRows[currentBlock][blockRow][0] = text;
					GetDocument()->mBlockTrackerRows[currentBlock][blockRow][1] = currentEnvelope;
					if (slurEncountered)
					{
						lastNoteOutWhilstSluring = text;
						continue;
					}

					int newBlockRow = (block->GetBlockTime() + lastDurationReleaseTick) / tempo;
					if ((newBlockRow == blockRow) || (newBlockRow >= CMusicStudioDoc::kMaxInternalTrackerRows))
					{
						continue;
					}

					GetDocument()->mBlockTrackerRows[currentBlock][newBlockRow][0] = "===";
				}
				else			
				{
					if (!GetDocument()->mBlockTrackerRows[currentBlock][blockRow][2].IsEmpty())
					{
						GetDocument()->mBlockTrackerRows[currentBlock][blockRow][2] += " ";
					}
					GetDocument()->mBlockTrackerRows[currentBlock][blockRow][2] += text;
				}
			}
		} //< if (!GetDocument()->mBlockLastEditedAsTracker[currentBlock])

		int maxCalcedRow = GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock];

		if (maxCalcedRow > CMusicStudioDoc::kMaxInternalTrackerRows)
		{
			for (j=0;j<kVisibleTrackerModeLines;j++)
			{
				mTrackerModeRowNum[i][j].EnableWindow(FALSE);
				mTrackerModeRowNum[i][j].SetWindowText(_T(""));

				mTrackerModeEdits[i][0].SetWindowText(_T("Block"));
				mTrackerModeEnvelope[i][0].SetWindowText(_T("too"));
				mTrackerModeEffect[i][0].SetWindowText(_T("complex"));

				mTrackerModeEdits[i][j].EnableWindow(FALSE);
				mTrackerModeEnvelope[i][j].EnableWindow(FALSE);
				mTrackerModeEffect[i][j].EnableWindow(FALSE);
			}
			continue;
		}

		for (j=0;j<kVisibleTrackerModeLines;j++)
		{
			int localOffset = mBlockEditTrackerOffsets[i] - (kVisibleTrackerModeLines/2) + j;

			if (localOffset == maxCalcedRow)
			{
				CString text;
				text.Format(_T("%x"),localOffset * tempo);
				mTrackerModeRowNum[i][j].SetWindowText(text);
				mTrackerModeRowNum[i][j].EnableWindow(TRUE);

				mTrackerModeEdits[i][j].EnableWindow(TRUE);
				mTrackerModeEdits[i][j].SetWindowText(_T("END"));
				mTrackerModeEnvelope[i][j].EnableWindow(FALSE);
				mTrackerModeEnvelope[i][j].SetWindowText(_T("**"));
				mTrackerModeEffect[i][j].EnableWindow(FALSE);
				mTrackerModeEffect[i][j].SetWindowText(_T("END"));
			}
			else if ((localOffset >= 0) && (localOffset < maxCalcedRow))
			{
				CString text;
				text.Format(_T("%x"),localOffset * tempo);
				mTrackerModeRowNum[i][j].SetWindowText(text);
				mTrackerModeRowNum[i][j].EnableWindow(TRUE);

				mTrackerModeEdits[i][j].EnableWindow(TRUE);
				mTrackerModeEdits[i][j].SetWindowText(GetDocument()->mBlockTrackerRows[currentBlock][localOffset][0]);
				mTrackerModeEnvelope[i][j].EnableWindow(TRUE);
				mTrackerModeEnvelope[i][j].SetWindowText(GetDocument()->mBlockTrackerRows[currentBlock][localOffset][1]);
				mTrackerModeEffect[i][j].EnableWindow(TRUE);
				mTrackerModeEffect[i][j].SetWindowText(GetDocument()->mBlockTrackerRows[currentBlock][localOffset][2]);
			}
			else
			{
				mTrackerModeRowNum[i][j].EnableWindow(FALSE);
				mTrackerModeRowNum[i][j].SetWindowText(_T(""));
				mTrackerModeEdits[i][j].EnableWindow(FALSE);
				mTrackerModeEdits[i][j].SetWindowText(_T(""));
				mTrackerModeEnvelope[i][j].EnableWindow(FALSE);
				mTrackerModeEnvelope[i][j].SetWindowText(_T(""));
				mTrackerModeEffect[i][j].EnableWindow(FALSE);
				mTrackerModeEffect[i][j].SetWindowText(_T(""));
			}
		}
	}

	DrawTrackerBlocksErrors();

	CTrackEditNum::mProcessChanges = true;

	sUpdating = false;
}

void CMusicStudioView::DrawTrackerBlocksErrors(void)
{
	if (!mTrackerModeRowNum[0][0].m_hWnd)
	{
		return;
	}

	IgnoreAnyUpdates();
	MusicStudio1::MusicFile &theFile = GetDocument()->mMusicFile;	

	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		int currentBlock = mBlockEditTrackerNum[i];

		int maxCalcedRow = GetDocument()->mBlockEditTrackerMaxCalcedRow[currentBlock];

		int j;
		for (j=0;j<kVisibleTrackerModeLines;j++)
		{
			int localOffset = mBlockEditTrackerOffsets[i] - (kVisibleTrackerModeLines/2) + j;

			bool changed = false;
			if ((localOffset >= 0) && (localOffset < maxCalcedRow) && GetDocument()->mBlockTrackerErrorRows[currentBlock][localOffset])
			{
				changed = mTrackerModeEdits[i][j].SetColour(RGB(255,0,0));
				mTrackerModeEnvelope[i][j].SetColour(RGB(255,0,0));
				mTrackerModeEffect[i][j].SetColour(RGB(255,0,0));
			}
			else
			{
				changed = mTrackerModeEdits[i][j].SetColour();
				mTrackerModeEnvelope[i][j].SetColour();
				mTrackerModeEffect[i][j].SetColour();
			}

			if (changed)
			{
				mTrackerModeEdits[i][j].RedrawWindow();
				mTrackerModeEnvelope[i][j].RedrawWindow();
				mTrackerModeEffect[i][j].RedrawWindow();
			}
		}
	}
}

void CMusicStudioView::SafePlayerFree(void)
{
	sThreadKillMusic = true;
	if (mPlayerHandle)
	{
		while(sThreadRunning)
		{
			Sleep(5);
		}
		MemoryHook::mMemoryHook = 0;
		PlayerFree(mPlayerHandle);
		mPlayerHandle = 0;
	}
}

void CMusicStudioView::OnBnClickedSaveEnvelope()
{
	CString defExt;
	defExt.LoadString(IDS_MSENV);
	CString filter;
	filter.LoadString(IDS_MSENVFILTER);
	CFileDialog dlg(FALSE,defExt,GetDocument()->mEnvelopeNames[GetCurrentEnvelopeNum()],OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT,filter);
	dlg.DoModal();
	OnBnClickedSaveEnvelope(dlg.GetPathName());
}

void CMusicStudioView::OnBnClickedSaveEnvelope(CString &filename)
{
	MusicStudio1::Envelope &envelope = GetDocument()->mEnvelopes[GetCurrentEnvelopeNum()];
	CInstrument instrument;
	instrument.mName = GetDocument()->mEnvelopeNames[GetCurrentEnvelopeNum()];
	instrument.mAttackDecay = envelope.mAttackDecay;
	instrument.mSustainRelease = envelope.mSustainRelease;
	instrument.mTranspose = envelope.mTranspose;
	instrument.mAllowVoiceEffects = envelope.mAllowVoiceEffects;

	int starts[MusicStudio1::MusicFile::kMaxTables];
	starts[0] = envelope.mActiveTableWave?envelope.mTableWave:-1;
	starts[1] = envelope.mActiveTableNote?envelope.mTableNote:-1;
	starts[2] = envelope.mActiveTablePulse?envelope.mTablePulse:-1;
	starts[3] = envelope.mActiveTableFilter?envelope.mTableFilter:-1;

	// Unroll any used Envelope tables and note their loop points into mTableLoops
	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
	{
		instrument.mTableActive[i] = false;
		if (starts[i] == 0)
		{
			instrument.mTableActive[i] = true;
		}
		else if (starts[i] > 0)
		{
			instrument.mTableActive[i] = true;
			// Unroll the used table
			int pos = starts[i];
			int num = 0;
			bool output[MusicStudio1::MusicFile::kMaxTableEntries+1];
			int outputPos[MusicStudio1::MusicFile::kMaxTableEntries+1];
			ZeroMemory(output,sizeof(output));
			ZeroMemory(outputPos,sizeof(outputPos));
			outputPos[0] = -1;
			// While there is a position to output and it has not been output yet.
			// When it has been output then this indicates a loop being used.
			// This means a non-looping jump is stored as a continuation of the linear array.
			while ((pos > 0) && !output[pos])
			{
				output[pos] = true;
				outputPos[pos] = num;
				if (GetDocument()->mTablesControls[i][pos] != 0xff)
				{
					instrument.mTableControls[i].push_back(GetDocument()->mTablesControls[i][pos]);
					instrument.mTableValues[i].push_back(GetDocument()->mTablesValues[i][pos]);
					num++;
					pos++;
				}
				else
				{
					pos = GetDocument()->mTablesValues[i][pos];
				}
			}

			// Output what to do for the loop
			instrument.mTableLoops[i] = outputPos[pos];
		}
	}

	CFile file;
	if (!file.Open(filename,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
	{
		AfxMessageBox(_T("Couldn't create the file!"));
		return;
	}

	CArchive archive(&file, CArchive::store);

	instrument.Serialize(archive);
}

void CMusicStudioView::OnBnClickedLoadEnvelope()
{
	CString defExt;
	defExt.LoadString(IDS_MSENV);
	CString filter;
	filter.LoadString(IDS_MSENVFILTER);
	CFileDialog dlg(TRUE,defExt,0,OFN_FILEMUSTEXIST | OFN_READONLY,filter);
	dlg.DoModal();
	OnBnClickedLoadEnvelope(dlg.GetPathName());
}

void CMusicStudioView::OnBnClickedLoadEnvelope(CString &filename)
{
	MusicStudio1::Envelope &envelope = GetDocument()->mEnvelopes[GetCurrentEnvelopeNum()];
	CInstrument instrument;

	CFile file;
	if (!file.Open(filename,CFile::modeRead|CFile::shareDenyWrite))
	{
		AfxMessageBox(_T("Couldn't open the file!"));
		return;
	}

	CArchive archive(&file, CArchive::load);

	TRY
	{
		CWaitCursor wait;
		if (file.GetLength())
		{
			instrument.Serialize(archive);
		}
	}
	CATCH_ALL(e)
	{
		TRY
		{
			AfxMessageBox(_T("Couldn't open the file, bad format!"));
//			ReportSaveLoadException(file.GetFilePath(), e, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
//		DELETE_EXCEPTION(e);
		return;
	}
	END_CATCH_ALL

	// Now find the highest empty positions for loading the instrument tables
	int highestPos[MusicStudio1::MusicFile::kMaxTables];
	ZeroMemory(highestPos,sizeof(highestPos));

	bool output[MusicStudio1::MusicFile::kMaxTables][MusicStudio1::MusicFile::kMaxTableEntries+1];
	ZeroMemory(output,sizeof(output));
	int env;
	for (env=0;env<MusicStudio1::MusicFile::kMaxEnvelopes;env++)
	{
		if (env == GetCurrentEnvelopeNum())
		{
			continue;
		}
		MusicStudio1::Envelope &envelope = GetDocument()->mEnvelopes[env];

		int starts[MusicStudio1::MusicFile::kMaxTables];
		starts[0] = envelope.mActiveTableWave?envelope.mTableWave:-1;
		starts[1] = envelope.mActiveTableNote?envelope.mTableNote:-1;
		starts[2] = envelope.mActiveTablePulse?envelope.mTablePulse:-1;
		starts[3] = envelope.mActiveTableFilter?envelope.mTableFilter:-1;

		int i;
		for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
		{
			int pos = starts[i];
			while ((pos > 0) && !output[i][pos])
			{
				output[i][pos] = true;
				if (pos > highestPos[i])
				{
					highestPos[i] = pos;
				}
				if (GetDocument()->mTablesControls[i][pos] != 0xff)
				{
					pos++;
				}
				else
				{
					pos = GetDocument()->mTablesValues[i][pos];
				}
			}
		}
	}

	// Now copy the loaded instrument data into the current envelope.
	GetDocument()->mEnvelopeNames[GetCurrentEnvelopeNum()] = instrument.mName;
	envelope.mAttackDecay = instrument.mAttackDecay;
	envelope.mSustainRelease = instrument.mSustainRelease;
	envelope.mTranspose = instrument.mTranspose;
	envelope.mAllowVoiceEffects = instrument.mAllowVoiceEffects;

	envelope.mActiveTableWave = instrument.mTableActive[0];
	envelope.mActiveTableNote = instrument.mTableActive[1];
	envelope.mActiveTablePulse = instrument.mTableActive[2];
	envelope.mActiveTableFilter = instrument.mTableActive[3];

	envelope.mTableWave = !instrument.mTableControls[0].empty()?highestPos[0]+1:0;
	envelope.mTableNote = !instrument.mTableControls[1].empty()?highestPos[1]+1:0;
	envelope.mTablePulse = !instrument.mTableControls[2].empty()?highestPos[2]+1:0;
	envelope.mTableFilter = !instrument.mTableControls[3].empty()?highestPos[3]+1:0;

	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
	{
		if (instrument.mTableActive[i] && !instrument.mTableControls[i].empty())
		{
			int pos = highestPos[i]+1;
			size_t j;
			for (j=0;j<instrument.mTableControls[i].size();j++)
			{
				GetDocument()->mTablesControls[i][pos] = instrument.mTableControls[i][j];
				GetDocument()->mTablesValues[i][pos++] = instrument.mTableValues[i][j];
			}
			GetDocument()->mTablesControls[i][pos] = 0xff;
			if (instrument.mTableLoops[i] >= 0)
			{
				GetDocument()->mTablesValues[i][pos++] = highestPos[i]+1 + instrument.mTableLoops[i];
			}
			else
			{
				GetDocument()->mTablesValues[i][pos++] = 0;
			}
		}
	}

	CommonSetModified();
	RedrawView();
}

void CMusicStudioView::OnFileRipSID()
{
	OnBnClickedStop();

#if 1
	CFileDialog dlg(TRUE,0,0,OFN_FILEMUSTEXIST | OFN_READONLY,_T("SID Music Files (*.sid;*.prg)|*.sid;*.prg||"));
	dlg.DoModal();

	if (dlg.GetPathName().IsEmpty())
	{
		return;
	}

	CStringA path(dlg.GetPathName());
#else
	// Hard coded debugging to save my poor little fingers from having to do the same thing over and over whilst testing
	//
//	CStringA path("C:\\Users\\Martin Piper\\Downloads\\Secret_Project.sid");
//	CStringA path("C:\\Users\\Martin Piper\\Downloads\\Wizball.sid");
	CStringA path("C:\\CCS64\\Games\\Turrican_3.sid");
#endif

	GetDocument()->ClearCapturedSIDData();
	MemoryHook::mMemoryHook = GetDocument();

	char params[32];
	sprintf(params,"-f%d",gSoftFrequency);
	mPlayerHandle = AllocatePlayer(params,path);

	CRipSID ripSID;
	ripSID.mMaxSongs = PlayerGetMaxSong(mPlayerHandle);
	ripSID.mTrackNumber = PlayerGetCurrentSong(mPlayerHandle);
	ripSID.DoModal();

	std::set<int> forceNoteTable;
	int tokens = 0;
	CString tok = ripSID.mForceNoteInstruments.Tokenize(_T(", ") , tokens);
	while (!tok.IsEmpty())
	{
		int theNum = 0;
		_stscanf(tok,_T("%x"),&theNum);
		forceNoteTable.insert(theNum);
//		forceNoteTable.insert(_ttoi(tok));
		tok = ripSID.mForceNoteInstruments.Tokenize(_T(", ") , tokens);
	}

	PlayerSetCurrentSong(mPlayerHandle , ripSID.mTrackNumber);

	sThreadRunning = true;
	sThreadKillMusic = false;
	CreateThread(0,0,sPlayerPlayThread,mPlayerHandle,0,0);

	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	int tmpFlag2 = tmpFlag & ~_CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag( tmpFlag2 );

	MessageBox(_T("Press OK to stop ripping this SID."));

	OnBnClickedStop();

	_CrtCheckMemory();
	// Restore the old debug state if it was changed
//	_CrtSetDbgFlag( tmpFlag );


	GetDocument()->ClearDocument();
	GetDocument()->ProcessSIDCaptureData(forceNoteTable);
	int baselineScore = GetDocument()->GetComplexityScore();

	if (ripSID.mAutoDetectPercussion)
	{
		int consideringEnvelope = 0;
		while (consideringEnvelope < MusicStudio1::MusicFile::kMaxEnvelopes)
		{
			if (forceNoteTable.find(consideringEnvelope) != forceNoteTable.end() || !(GetDocument()->mEnvelopes[consideringEnvelope].mActiveTableWave || GetDocument()->mEnvelopes[consideringEnvelope].mWaveControl) || GetDocument()->mEnvelopes[consideringEnvelope].mActiveTableNote)
			{
				consideringEnvelope++;
				continue;
			}
			// Check for percussion sounding instruments
			int mergedControl = 0;
			int wavePos = GetDocument()->mEnvelopes[consideringEnvelope].mTableWave;
			while (wavePos < MusicStudio1::MusicFile::kMaxTableEntries)
			{
				int control = GetDocument()->mTablesControls[MusicStudio1::kTableIndex_Wave][wavePos];
				if (control == 0xff)
				{
					break;
				}
				mergedControl |= control;
				wavePos++;
			}
			if ((mergedControl & MusicStudio1::kSIDVoiceControl_Mask_Noise) && (mergedControl & (MusicStudio1::kSIDVoiceControl_Mask_Pulse | MusicStudio1::kSIDVoiceControl_Mask_Triangle | MusicStudio1::kSIDVoiceControl_Mask_Sawtooth)))
			{
				// Probably percussion?
				// TODO: Any other checks needed?
			}
			else
			{
				// Skip it
				consideringEnvelope++;
				continue;
			}

			int numEnvelopesBefore = GetDocument()->GetNumEnvelopes();


			forceNoteTable.insert(consideringEnvelope);

			GetDocument()->ClearDocument();
			GetDocument()->ProcessSIDCaptureData(forceNoteTable);
			int newScore = GetDocument()->GetComplexityScore();

			int numEnvelopesAfter = GetDocument()->GetNumEnvelopes();

			// Don't allow a new note based envelope to increase the total number of envelopes
			if (newScore >= baselineScore || numEnvelopesAfter > numEnvelopesBefore)
			{
				forceNoteTable.erase(consideringEnvelope);
			}
			else
			{
				baselineScore = newScore;
			}

			consideringEnvelope++;
		}

		// Run the final conversion again with the optimal list
		GetDocument()->ClearDocument();
		GetDocument()->ProcessSIDCaptureData(forceNoteTable);
		GetDocument()->GetComplexityScore();
	}

	GetDocument()->ClearCapturedSIDData();

	// Add tune information
	GetDocument()->mGenericInfo.Append(CString("File: ") + CString(path));
	GetDocument()->mGenericInfo.Append(CString("\r\nTune: ") + CString(RNReplicaNet::ToString(ripSID.mTrackNumber).c_str()));
	if (ripSID.mAutoDetectPercussion)
	{
		GetDocument()->mGenericInfo.Append(CString("\r\nAuto detect percussion instruments"));
	}
	GetDocument()->mGenericInfo.Append(CString("\r\nForce notes instruments: "));
	std::set<int>::iterator st = forceNoteTable.begin();
	while (st != forceNoteTable.end())
	{
		int env = *st++;

		GetDocument()->mGenericInfo.Append( GetHexNum(env) );
		GetDocument()->mGenericInfo.Append( _T(" ") );
	}

	SetHelpState(kBlock);
	CommonSetModified();
	RedrawView();
	OnEditOptimise();
}


void CMusicStudioView::OnEnSetfocusEdit1()
{
	SetHelpState(kBlock);
}


void CMusicStudioView::SetHelpState(const HelpState state)
{
	if (state == mHelpState)
	{
		return;
	}

	mHelpState = state;
	switch(mHelpState)
	{
		case kTrack:
			mDynamicHelp.SetWindowText(
				L"'00'-'3F'	- Play specified block.\r\n"
				L"'40'-'7F'	- Repeats next block by 'xx-$40' times.\r\n"
				L"'80'-'EF'	- Play all following blocks transposed up by 'xx-$80' semitones. If the number is >= $b0 then the note is transposed down rather than up.\r\n"
				L"'FD'	- Stops track.\r\n"
				L"'FE'	- Stops all tracks.\r\n"
				L"'FF'	- Loops track to beginning of song.\r\n"
			);
			break;
		case kBlock:
			mDynamicHelp.SetWindowTextW(
				L"VOL:XX	- This block command that allows you to set the volume. Setting a volume of 0 will stop the player. So valid number ranges are VOL:01 to VOL:0F\r\n"
				L"GL:01,4	- Glides the last note played upwards (quit slowly) after counting four (half a minim).\r\n"
				L"GL:02,2	- Glides the last note played upwards (a bit faster) after counting two (quarter minim). As can be seen the larger the number the faster the glide.\r\n"
				L"GL:83,0	- Glides the note down very fast after no delay. This means glide down.\r\n"
				L"ARP:XY - Controls arpeggio for the voice and is in the format XY where the first root note is unchanged, next the root note is transposed X semitones. Next the root note is transposed Y semitones. The note pattern then loops. Arpeggios will stay on during envelope changes until an \"ARP:00\" command is used to remove the arpeggio.\r\n"
				L"ERP:XX - Uses the extended arpeggio code to play an arpeggio from the extended arpeggio table, check \"extended view\".\r\n"
				L"ARS    - Stops any arpeggio.\r\n"
				L"FLL:XX	- low pass filter: Controls the value sent to $d415 SIDFilterCutoffFreqLo. So FLL:10 will put $10 into $d415\r\n"
				L"FLH:XX	- high pass filter: does the same as FLL but for $d416 SIDFilterCutoffFreqHi\r\n"
				L"FLC:XX	- Does the same but for $d417 SIDFilterControl. So FLC:F7 will set filter resonance F with voices 0,1 and 2 active (bits 0/1/2 = 7).\r\n"
				L"FLP:XX	- Does the same with $d418 SIDVolumeFilter. So FLP:10 will set bit 4 which is the low pass filter. The lower nybble maps to the volume control, don't set these values, keep it at 0 for now.\r\n"
				L"FG:XX,YY - Filter glide controls the filter frequency in a sinus pattern. XX controls the step size. YY controls the speed of the sinus pattern. If YY is 01 then the change will be slow, 02 is faster, 03 is even faster and so on. If YY has $80 added then the initial sinus pattern is falling instead of rising. For example $81 will fall slowly, $82 will fall faster etc.\r\n"
				L"HRE - Enables the hard restart for this voice.\r\n"
				L"HRD - Disables the hard restart for this voice.\r\n"
				L"HAD:XX - Sets the hard restart AttackDecay.\r\n"
				L"HSR:XX - Sets the hard restart SustainRelease.\r\n"
				L"HWV:XX - Sets the hard restart waveform.\r\n"
				L"HTI:XX - Sets the hard restart frame time, must be greater than 0.\r\n"
				L"SLE - This block command enables slurring (or ties) of the next notes, the note will note release until SLD is disabled. Envelope commands have no effect on the sound until the SLD command is used. Hard restart is also disabled for the voice by this command.\r\n"
				L"SLR - Releases the slur (or ties) of the next note. Hard restart is also enabled for the voice by this command.\r\n"
				L"VIB:XY,VV - Semitone small vibrato. This vibrato will continue for all notes on the channel until it is stopped with VBS.\r\n"
				L"	X - Vibrato shift going up. 1 is the larger vibrato of 2 semitones. 5 is smaller at a fraction of a semitone.\r\n"
				L"	Y - Vibrato shift going down. 1 is the larger vibrato of 2 semitones. 5 is smaller at a fraction of a semitone.\r\n"
				L"	X or Y must not be 0 and will report an error.\r\n"
				L"	VV - Vibrato frequency. Larger numbers produce faster vibratos.\r\n"
				L"VBD:ZZ - ZZ delays the start of the small vibrato for ZZ frames.\r\n"
				L"VBS - Stops the small vibrato\r\n"
				L"FVB:XX,VV - Semitone fixed vibrato. This vibrato will continue for all notes on the channel until it is stopped with FVS.\r\n"
				L"	X - Vibrato shift. 1 is the larger vibrato. 5 is smaller at a fraction of a semitone. There is no separate up and down size as there is for the small vibrato.\r\n"
				L"	X must not be 0 and will report an error.\r\n"
				L"	VV - Vibrato frequency. Larger numbers produce faster vibratos.\r\n"
				L"	As an example FVB:3,3 is nearly the same sound as VIB:33,3. Both have the same speed vibrato, both have similar tone range and the small vibrato has a smoother sine wave for the tone.\r\n"
				L"FVD:ZZ - ZZ delays the start of the fixed vibrato for ZZ frames.\r\n"
				L"FVS - Stops the fixed vibrato\r\n"
				L"TWV:XX - Set the wave table.\r\n"
				L"TNT:XX - Set the note table.\r\n"
				L"TPL:XX - Set the pulse table.\r\n"
				L"TFL:XX - Set the filter table.\r\n"
			);
			break;
		case kWave:
			mDynamicHelp.SetWindowTextW(
				L"XX\r\n"
				L"YY\r\n"
				L"YY is always the frame delay.\r\n"
				L"XX:\r\n"
				L"0 = Don't change wave this time, previous waveform is used.\r\n"
				L"FF = Jump to position. pos 0 = stop\r\n"
				L"Any other value is used as the waveform using the SID format which can be ORed together:\r\n"
				L"bit7	80	Noise\r\n"
				L"bit6	40	Pulse\r\n"
				L"bit5	20	Sawtooth\r\n"
				L"bit4	10	Triangle\r\n"
				L"bit3	08	Test\r\n"
				L"bit2	04	Ring modulation\r\n"
				L"bit1	02	Sync\r\n"
				L"bit0	01	Gate\r\n"
				L"Remember to set the gate bit to get any sounds.\t\n"
			);
			break;
		case kNote:
			mDynamicHelp.SetWindowTextW(
				L"XY\r\n"
				L"ZZ\r\n"
				L"Y = Except command 0 frame delay (0-15) (ticks zero based)\r\n"
				L"X = Command\r\n"
				L"Command:\r\n"
				L"00 (both nybbles XY) = No note change. Do nothing. Delay time in ZZ\r\n"
				L"1 = Set absolute note ZZ = note. Does this once at the start of the command. Effects can be active.\r\n"
				L"8 = Timed note step add upper nybble Z+1 with time of lower nybble+1 (zero based).\r\n"
				L"9 = Timed note step sub upper nybble Z-1 with time of lower nybble+1 (zero based).\r\n"
				L"A = Relative note step ZZ = step signed 8 bit value each frame. For fast glides it is better to use this, the code is quicker and shorter.\r\n"
				L"B = Set hi frequency to ZZ lo is zeroed. Skips other effects for the duration of this effect.\r\n"
				L"FF = Jump to position and frame delay ignored. XX = pos 0 = stop\r\n"
			);
			break;
		case kPulse:
			mDynamicHelp.SetWindowTextW(
				L"If a pulse waveform is active for a frame (bit 6) then the pulse table will execute for that frame:\r\n"
				L"XY\r\n"
				L"ZZ\r\n"
				L"X = 0 Set pulse Y & ZZ = Pulse\r\n"
				L"X = 1 Pulse add with unsigned ZZ for Y ticks (ticks zero based)\r\n"
				L"X = 2 Pulse subtract with unsigned ZZ for Y ticks (ticks zero based)\r\n"
				L"FF = Jump to position. pos 0 = stop\r\n"
			);
			break;
		case kFilter:
			mDynamicHelp.SetWindowTextW(
				L"XX\r\n"
				L"YY\r\n"
				L"XX 00 = Set hi cutoff with YY\r\n"
				L"XX 7F = Set lo cutoff with YY\r\n"
				L"XX 01-7E = Alter filter cutoff by signed YY for XX frames\r\n"
				L"XX 80-FE = Set band pass flags (SIDVolumeFilter = XX & 70 | volume) YY = Resonance and channel mask (SIDFilterControl)\r\n"
				L"For example: Using 90 F7 will set the low pass filter (80+10=90) with filter resonance F and enabled on all voices (1+2+4=7)\r\n"
				L"For example: Using A0 F4 will set the band pass filter (80+20=A0) with filter resonance F and enabled on voice 3 (4 = voice 3)\r\n"
				L"For example: Using C0 E1 will set the band pass filter (80+40=C0) with filter resonance E and enabled on voice 1 (1 = voice 1)\r\n"
				L"XX FF = Jump\r\n"
				L"\r\n"
				L"If Set band pass ($80-FE) is followed by 00 which can be followed by an optional 7F then these will be executed in the same frame.\r\n"
				L"If hi cutoff (00) is followed by lo cutoff (7F) it will be executed in the same frame.\r\n"
			);
			break;
		default:
			break;
	}
}

void CMusicStudioView::OnBnClickedInsertEnvelope()
{
	int envelope = GetCurrentEnvelopeNum();
	if ((envelope < 0) || (envelope>= MusicStudio1::MusicFile::kMaxEnvelopes))
	{
		return;
	}

	// Discard the cached version
	if (!mIgnoreAnyUpdates)
	{
		mLastPlayWasSFX = false;
	}

	CMusicStudioDoc *doc = GetDocument();

	int i;
	for (i = MusicStudio1::MusicFile::kMaxEnvelopes-2 ; i >= envelope ; i--)
	{
		doc->mEnvelopes[i+1] = doc->mEnvelopes[i];
		doc->mEnvelopeNames[i+1] = doc->mEnvelopeNames[i];
	}

	doc->mEnvelopes[envelope] = MusicStudio1::Envelope();
	doc->mEnvelopeNames[envelope] = _T("");

	for (i = 0 ; i < MusicStudio1::MusicFile::kMaxBlocks ; i++)
	{
		if (doc->mBlockLastEditedAsTracker[i])
		{
			bool changed = false;
			//doc->mBlockEditTrackerMaxCalcedRow[i]
			int j;
			for (j=0 ; j < CMusicStudioDoc::kMaxInternalTrackerRows ; j++)
			{
				CString env = doc->mBlockTrackerRows[i][j][1];

				if (env.IsEmpty())
				{
					continue;
				}

				int theNum = -1;
				_stscanf(env,_T("%x"),&theNum);

				if (theNum >= envelope)
				{
					theNum++;

					CString newNum;
					newNum.Format(_T("%02X"),theNum);

					doc->mBlockTrackerRows[i][j][1] = newNum;

					changed = true;
				}
				
			}
			if(changed)
			{
				doc->UpdateTrackerBlockToInternal(i);
			}
		}
		else
		{
			if (doc->mBlocks[i].IsEmpty())
			{
				continue;
			}
			bool changed = false;
			CString block = doc->mBlocks[i];

			int j;
			for (j = MusicStudio1::MusicFile::kMaxEnvelopes-2 ; j >= envelope ; j--)
			{
				CString oldNum;
				CString newNum;
				block.MakeUpper();
				oldNum.Format(_T("ENV:%02X"),j);
				newNum.Format(_T("ENV:%02X"),j+1);
				if (block.Replace(oldNum , newNum) > 0)
				{
					changed = true;
				}
			}

			if (changed)
			{
				doc->mBlocks[i] = block;

				CStringA ansiBlock(block);
				int byteSize = 0;
				int totalDuration = 0;
				GetDocument()->mMusicFile.SetBlockFromText(i,ansiBlock,doc->mBlockErrorReport[i],byteSize,totalDuration);
			}
		}
	}

	CommonSetModified();
	RedrawView();
}




void CMusicStudioView::OnEditClearalldata()
{
	int ret = MessageBox(_T("This will clear all data") , _T("Are you sure?") , MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
	if (ret != IDYES)
	{
		return;
	}

	GetDocument()->ClearDocument();

	GetDocument()->UpdateDocumentDataToInternalFile();
	CommonSetModified();
	RedrawView();
}


void CMusicStudioView::OnEditSetdefaultdata()
{
	int ret = MessageBox(_T("This will clear all data and set default values") , _T("Are you sure?") , MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
	if (ret != IDYES)
	{
		return;
	}

	GetDocument()->ClearDocument();
	GetDocument()->SetDefaultData();

	GetDocument()->UpdateDocumentDataToInternalFile();
	CommonSetModified();
	RedrawView();
}

// Yuck
template <typename T>
static T MakeBigEndian(const T value)
{
	T ret = value;
	if (sizeof(T) > 1 && RNReplicaNet::PlatformInfo::IsLittleEndian())
	{
		// Anything larger than one byte
		for (size_t i = 0 ; i < sizeof(T) ; i++)
		{
			((unsigned char*)&ret)[i] = ((const unsigned char*)&value)[sizeof(T) - 1 - i];
		}
	}
	return ret;
}

static void WriteVTime(RNReplicaNet::DynamicMessageHelper &fileData , int delta)
{
	unsigned char toOutput = 0;
	while (delta > 0)
	{
		toOutput = (delta & 0x7f) | 0x80;
		fileData << toOutput;
		delta = delta >> 7;
	}
	toOutput = delta & 0x7f;
	fileData << toOutput;
}

void CMusicStudioView::OnFileExporttoMIDI()
{
	CommonPreExport();

	OnBnClickedStop();

	// http://midi.teragonaudio.com/tech/midifile/ppqn.htm
	// "PPQN clock ticks at the above tempo should be 500,000 / 96"
	const short kTicksPerQuarter = 500000 / 96 / 2;

	RNReplicaNet::DynamicMessageHelper theFile;
	theFile << CMusicStudioDoc::kMIDIHeader;
	theFile << MakeBigEndian((int) 6);
	theFile << MakeBigEndian((short) 1);
	theFile << MakeBigEndian((short) MusicStudio1::MusicFile::kMaxTracks);
	theFile << MakeBigEndian(kTicksPerQuarter);

	GetDocument()->UpdateDocumentDataToInternalFile();
	MusicStudio1::MusicFile &music = GetDocument()->mMusicFile;
	music.Optimise();

	for (int track = 0 ; track < MusicStudio1::MusicFile::kMaxTracks ; track++)
	{
		RNReplicaNet::DynamicMessageHelper theTrack;
		int lastTimeInFrames = 0;
		int currentTimeInFrames = 0;
		int trackIndex = 0;
		int currentSustain = 0 , currentRelease = 0;
		int currentEnvelope = 0;
		int currentTranspose = 0;
		int currentRepeat = 0;
		// Loop until the end of the track
		while (music.mTracks[track][trackIndex] < MusicStudio1::kMusicPlayer_StopTrack)
		{
			int theBlockIndex = music.mTracks[track][trackIndex];
			trackIndex++;
			if (theBlockIndex >= MusicStudio1::kMusicPlayer_TransposeBlock && theBlockIndex < MusicStudio1::kMusicPlayer_TransposeBlockNeg)
			{
				currentTranspose = theBlockIndex - MusicStudio1::kMusicPlayer_TransposeBlock;
				continue;
			}
			if (theBlockIndex >= MusicStudio1::kMusicPlayer_TransposeBlockNeg && theBlockIndex < MusicStudio1::kMusicPlayer_StopTrack)
			{
				currentTranspose = -(theBlockIndex - MusicStudio1::kMusicPlayer_TransposeBlockNeg);
				continue;
			}
			if (theBlockIndex >= MusicStudio1::kMusicPlayer_RepeatBlock && theBlockIndex < MusicStudio1::kMusicPlayer_TransposeBlock)
			{
				currentRepeat = theBlockIndex - MusicStudio1::kMusicPlayer_RepeatBlock;
				continue;
			}

			if (theBlockIndex < MusicStudio1::MusicFile::kMaxBlocks)
			{
				do
				{
					std::list<MusicStudio1::BlockEntry*> &theBlock = music.mBlocks[theBlockIndex];
					std::list<MusicStudio1::BlockEntry*>::iterator st = theBlock.begin();
					while (st != theBlock.end())
					{
						MusicStudio1::BlockEntry *entry = *st++;
						std::string command = entry->GetCommandName();
						if (command.compare("Duration") == 0)
						{
							currentSustain = entry->GetDataByte0();
							currentRelease = currentSustain;
							continue;
						}
						if (command.compare("DurationTicks") == 0)
						{
							currentSustain = entry->GetDataByte1();
							currentRelease = entry->GetDataByte0() - entry->GetDataByte1();
							continue;
						}
						if (command.compare("Envelope") == 0)
						{
							currentEnvelope = entry->GetDataByte0();
							continue;
						}
						if (command.compare("RestNote") == 0 || command.compare("RestNoteRelease") == 0)
						{
							currentTimeInFrames += currentSustain;
							currentTimeInFrames += currentRelease;
							continue;
						}
						if (entry->IsNote())
						{
							WriteVTime(theTrack , currentTimeInFrames - lastTimeInFrames);
							lastTimeInFrames = currentTimeInFrames;
							char theNote = (char) (entry->GetDataByte0() + currentTranspose);
							theTrack << (char) (0x90 | track);
							theTrack << theNote;
							theTrack << (char) 127;
							currentTimeInFrames += currentSustain;

							WriteVTime(theTrack , currentTimeInFrames - lastTimeInFrames);
							lastTimeInFrames = currentTimeInFrames;
							theTrack << (char) (0x80 | track);
							theTrack << theNote;
							theTrack << (char) 127;
							currentTimeInFrames += currentRelease;
							continue;
						}
					}
					currentRepeat--;
				} while (currentRepeat >= 0);
			}

		}

		// Write a final note off at the end of the track
		WriteVTime(theTrack , currentTimeInFrames - lastTimeInFrames);
		lastTimeInFrames = currentTimeInFrames;
		theTrack << (char) (0x80 | track);
		theTrack << 0;
		theTrack << (char) 127;

		// End of track
		theTrack << (char) 0x00;
		theTrack << (char) 0xff;
		theTrack << (char) 0x2f;
		theTrack << (char) 0x00;

		theFile << CMusicStudioDoc::kMIDITrack;
		// Again endian yuck
		theFile << MakeBigEndian(theTrack.GetSize());
		theFile.AddData(theTrack.GetBuffer(),theTrack.GetSize());
	}

	CFileDialog getFile(FALSE);
	INT_PTR ret = getFile.DoModal();

	if (ret != IDOK)
	{
		return;
	}

	CString realPathName = getFile.GetPathName();
	_tremove(realPathName);

	CT2CA theNameANSI(realPathName);
	std::string theName(theNameANSI);
	theFile.Write(theName.c_str() , true);
}
