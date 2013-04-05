// MusicStudioView2.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudio.h"
#include "MusicStudioDoc.h"
#include "MusicStudioView.h"
#include "MusicStudioView2.h"


// CMusicStudioView2

IMPLEMENT_DYNCREATE(CMusicStudioView2, CFormView)

CMusicStudioView2::CMusicStudioView2()
	: CFormView(CMusicStudioView2::IDD) , mIgnoreAnyUpdates(0)
{

}

CMusicStudioView2::~CMusicStudioView2()
{
}

void CMusicStudioView2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, mArpeggioNum);
	DDX_Control(pDX, IDC_EDIT34, mArpeggioName);
	DDX_Control(pDX, IDC_EDIT42, mArpeggioData0);
	DDX_Control(pDX, IDC_EDIT7, mArpeggioData1);
	DDX_Control(pDX, IDC_EDIT8, mArpeggioData2);
	DDX_Control(pDX, IDC_EDIT9, mArpeggioData3);
	DDX_Control(pDX, IDC_EDIT10, mArpeggioData4);
	DDX_Control(pDX, IDC_EDIT11, mArpeggioData5);
	DDX_Control(pDX, IDC_EDIT12, mArpeggioData6);
	DDX_Control(pDX, IDC_EDIT13, mArpeggioData7);
	DDX_Control(pDX, IDC_EDIT43, mArpeggioLength);
	DDX_Control(pDX, IDC_EDIT1, mGenericInfo);
	DDX_Control(pDX, IDC_EDIT51, mZeroPageStart);
	DDX_Control(pDX, IDC_EDIT50, mHardRestartAttackDecay[0]);
	DDX_Control(pDX, IDC_EDIT14, mHardRestartAttackDecay[1]);
	DDX_Control(pDX, IDC_EDIT15, mHardRestartAttackDecay[2]);
	DDX_Control(pDX, IDC_EDIT52, mHardRestartSustainRelease[0]);
	DDX_Control(pDX, IDC_EDIT17, mHardRestartSustainRelease[1]);
	DDX_Control(pDX, IDC_EDIT18, mHardRestartSustainRelease[2]);
	DDX_Control(pDX, IDC_EDIT53, mHardRestartWaveform[0]);
	DDX_Control(pDX, IDC_EDIT20, mHardRestartWaveform[1]);
	DDX_Control(pDX, IDC_EDIT21, mHardRestartWaveform[2]);
	DDX_Control(pDX, IDC_EDIT54, mHardRestartFrames[0]);
	DDX_Control(pDX, IDC_EDIT23, mHardRestartFrames[1]);
	DDX_Control(pDX, IDC_EDIT24, mHardRestartFrames[2]);
}

BEGIN_MESSAGE_MAP(CMusicStudioView2, CFormView)
	ON_EN_CHANGE(IDC_EDIT34, &CMusicStudioView2::OnEnChangeEditArpeggioName)
	ON_EN_CHANGE(IDC_EDIT42, &CMusicStudioView2::OnEnChangeEditArpeggioData0)
	ON_EN_CHANGE(IDC_EDIT7, &CMusicStudioView2::OnEnChangeEditArpeggioData1)
	ON_EN_CHANGE(IDC_EDIT8, &CMusicStudioView2::OnEnChangeEditArpeggioData2)
	ON_EN_CHANGE(IDC_EDIT9, &CMusicStudioView2::OnEnChangeEditArpeggioData3)
	ON_EN_CHANGE(IDC_EDIT10, &CMusicStudioView2::OnEnChangeEditArpeggioData4)
	ON_EN_CHANGE(IDC_EDIT11, &CMusicStudioView2::OnEnChangeEditArpeggioData5)
	ON_EN_CHANGE(IDC_EDIT12, &CMusicStudioView2::OnEnChangeEditArpeggioData6)
	ON_EN_CHANGE(IDC_EDIT13, &CMusicStudioView2::OnEnChangeEditArpeggioData7)
	ON_EN_CHANGE(IDC_EDIT43, &CMusicStudioView2::OnEnChangeEditArpeggioLength)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMusicStudioView2::OnDeltaposSpinArpeggio)
	ON_EN_CHANGE(IDC_EDIT6, &CMusicStudioView2::OnEnChangeEditArpeggioNum)
	ON_EN_CHANGE(IDC_EDIT1, &CMusicStudioView2::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CMusicStudioView2::OnEnChangeEdit2Title)
	ON_EN_CHANGE(IDC_EDIT39, &CMusicStudioView2::OnEnChangeEdit39Author)
	ON_EN_CHANGE(IDC_EDIT40, &CMusicStudioView2::OnEnChangeEdit40Released)
	ON_EN_CHANGE(IDC_EDIT51, &CMusicStudioView2::OnChangeZeroPageStart)
	ON_EN_CHANGE(IDC_EDIT50, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT14, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT15, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT52, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT17, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT18, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT53, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT20, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT21, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT54, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT23, &CMusicStudioView2::OnChangeCommonHardRestart)
	ON_EN_CHANGE(IDC_EDIT24, &CMusicStudioView2::OnChangeCommonHardRestart)


END_MESSAGE_MAP()


// CMusicStudioView2 diagnostics

#ifdef _DEBUG
void CMusicStudioView2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMusicStudioView2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

CMusicStudioDoc* CMusicStudioView2::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMusicStudioDoc)));
	return (CMusicStudioDoc*)m_pDocument;
}
#endif //_DEBUG


// CMusicStudioView2 message handlers

void CMusicStudioView2::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	RedrawView();
}

void CMusicStudioView2::RedrawView(void)
{
	IgnoreAnyUpdates();
	mGenericInfo.SetWindowText(GetDocument()->mGenericInfo);
	DrawArpeggio();
	SetDlgItemText(IDC_EDIT2,GetDocument()->mTitleInfo);
	SetDlgItemText(IDC_EDIT39,GetDocument()->mAuthorInfo);
	SetDlgItemText(IDC_EDIT40,GetDocument()->mReleasedInfo);

	SetNumToEdit(mZeroPageStart,GetDocument()->mZeroPageStart);

	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		SetNumToEdit(mHardRestartAttackDecay[i],GetDocument()->mHardRestartAttackDecay[i]);
		SetNumToEdit(mHardRestartSustainRelease[i],GetDocument()->mHardRestartSustainRelease[i]);
		SetNumToEdit(mHardRestartWaveform[i],GetDocument()->mHardRestartWaveform[i]);
		SetNumToEdit(mHardRestartFrames[i],GetDocument()->mHardRestartFrames[i]);
	}
}

void CMusicStudioView2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	IgnoreAnyUpdates();
	mArpeggioNum.SetWindowText(_T("00"));
}

void CMusicStudioView2::OnEnChangeEditArpeggioNum()
{
	DrawArpeggio();
}

void CMusicStudioView2::DrawArpeggio(void)
{
	IgnoreAnyUpdates();
	int arpNum = GetNumFromEdit(mArpeggioNum);
	if ((arpNum < 0) || (arpNum >= MusicStudio1::MusicFile::kMaxExtendedArpeggios))
	{
		mArpeggioName.SetWindowText(_T(""));
		mArpeggioData0.SetWindowText(_T(""));
		mArpeggioData1.SetWindowText(_T(""));
		mArpeggioData2.SetWindowText(_T(""));
		mArpeggioData3.SetWindowText(_T(""));
		mArpeggioData4.SetWindowText(_T(""));
		mArpeggioData5.SetWindowText(_T(""));
		mArpeggioData6.SetWindowText(_T(""));
		mArpeggioData7.SetWindowText(_T(""));
		mArpeggioLength.SetWindowText(_T(""));
		return;
	}

	mArpeggioName.SetWindowText(GetDocument()->mExtendedArpeggiosNames[arpNum]);
	SetNumToEdit(mArpeggioData0,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[0]);
	SetNumToEdit(mArpeggioData1,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[1]);
	SetNumToEdit(mArpeggioData2,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[2]);
	SetNumToEdit(mArpeggioData3,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[3]);
	SetNumToEdit(mArpeggioData4,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[4]);
	SetNumToEdit(mArpeggioData5,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[5]);
	SetNumToEdit(mArpeggioData6,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[6]);
	SetNumToEdit(mArpeggioData7,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[7]);
	SetNumToEdit(mArpeggioLength,GetDocument()->mExtendedArpeggios[arpNum].mLength);
}

void CMusicStudioView2::OnEnChangeEditArpeggioName()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData0()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData1()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData2()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData3()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData4()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData5()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData6()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioData7()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::OnEnChangeEditArpeggioLength()
{
	IGNOREANYUPDATES();

	CommonArpeggioUpdated();
}

void CMusicStudioView2::CommonArpeggioUpdated(void)
{
	IGNOREANYUPDATES();

	int arpNum = GetNumFromEdit(mArpeggioNum);
	if ((arpNum < 0) || (arpNum >= MusicStudio1::MusicFile::kMaxExtendedArpeggios))
	{
		return;
	}

	mArpeggioName.GetWindowText(GetDocument()->mExtendedArpeggiosNames[arpNum]);
	GetNumFromEdit(mArpeggioData0,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[0]);
	GetNumFromEdit(mArpeggioData1,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[1]);
	GetNumFromEdit(mArpeggioData2,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[2]);
	GetNumFromEdit(mArpeggioData3,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[3]);
	GetNumFromEdit(mArpeggioData4,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[4]);
	GetNumFromEdit(mArpeggioData5,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[5]);
	GetNumFromEdit(mArpeggioData6,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[6]);
	GetNumFromEdit(mArpeggioData7,GetDocument()->mExtendedArpeggios[arpNum].mArpeggios[7]);
	GetNumFromEdit(mArpeggioLength,GetDocument()->mExtendedArpeggios[arpNum].mLength);
	if (GetDocument()->mExtendedArpeggios[arpNum].mLength > MusicStudio1::ExtendedArpeggio::kMaxExtendedArpeggioData)
	{
		GetDocument()->mExtendedArpeggios[arpNum].mLength = MusicStudio1::ExtendedArpeggio::kMaxExtendedArpeggioData;
	}

	CommonSetModified();
}

void CMusicStudioView2::OnDeltaposSpinArpeggio(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int arpNum = GetNumFromEdit(mArpeggioNum);
	arpNum += pNMUpDown->iDelta;
	if (arpNum < 0)
	{
		arpNum = 0;
	}
	if (arpNum >= MusicStudio1::MusicFile::kMaxExtendedArpeggios)
	{
		arpNum = MusicStudio1::MusicFile::kMaxExtendedArpeggios-1;
	}
	SetNumToEdit(mArpeggioNum,arpNum);
}

void CMusicStudioView2::OnEnChangeEdit1()
{
	IGNOREANYUPDATES();

	mGenericInfo.GetWindowText(GetDocument()->mGenericInfo);
	CommonSetModified();
}

void CMusicStudioView2::OnEnChangeEdit2Title()
{
	IGNOREANYUPDATES();

	GetDlgItemText(IDC_EDIT2,GetDocument()->mTitleInfo);
	CommonSetModified();
}

void CMusicStudioView2::OnEnChangeEdit39Author()
{
	IGNOREANYUPDATES();

	GetDlgItemText(IDC_EDIT39,GetDocument()->mAuthorInfo);
	CommonSetModified();
}

void CMusicStudioView2::OnEnChangeEdit40Released()
{
	IGNOREANYUPDATES();

	GetDlgItemText(IDC_EDIT40,GetDocument()->mReleasedInfo);
	CommonSetModified();
}

void CMusicStudioView2::OnChangeZeroPageStart()
{
	IGNOREANYUPDATES();

	GetDocument()->mZeroPageStart = GetNumFromEdit(mZeroPageStart);
	CommonSetModified();
}

void CMusicStudioView2::OnChangeCommonHardRestart()
{
	IGNOREANYUPDATES();

	int i;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		GetDocument()->mHardRestartAttackDecay[i] = GetNumFromEdit(mHardRestartAttackDecay[i]);
		GetDocument()->mHardRestartSustainRelease[i] = GetNumFromEdit(mHardRestartSustainRelease[i]);
		GetDocument()->mHardRestartWaveform[i] = GetNumFromEdit(mHardRestartWaveform[i]);
		GetDocument()->mHardRestartFrames[i] = GetNumFromEdit(mHardRestartFrames[i]);
	}

	CommonSetModified();
}
