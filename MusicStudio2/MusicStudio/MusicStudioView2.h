#pragma once

#include "afxwin.h"
#include "ViewUtility.h"


// CMusicStudioView2 form view

class CMusicStudioView2 : public CFormView
{
	DECLARE_DYNCREATE(CMusicStudioView2)

protected:
	CMusicStudioView2();           // protected constructor used by dynamic creation
	virtual ~CMusicStudioView2();

public:
	enum { IDD = IDD_MUSICSTUDIO_FORM2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

// Attributes
public:
	CMusicStudioDoc* GetDocument() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	// MPi: TODO: Make this really common
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
public:
	void RedrawView(void);
	int mIgnoreAnyUpdates;
	virtual void OnInitialUpdate();
	CEdit mArpeggioNum;
	void DrawArpeggio(void);
	CEdit mArpeggioName;
	CEdit mArpeggioData0;
	CEdit mArpeggioData1;
	CEdit mArpeggioData2;
	CEdit mArpeggioData3;
	CEdit mArpeggioData4;
	CEdit mArpeggioData5;
	CEdit mArpeggioData6;
	CEdit mArpeggioData7;
	CEdit mArpeggioLength;
	afx_msg void OnEnChangeEditArpeggioName();
	afx_msg void OnEnChangeEditArpeggioData0();
	afx_msg void OnEnChangeEditArpeggioData1();
	afx_msg void OnEnChangeEditArpeggioData2();
	afx_msg void OnEnChangeEditArpeggioData3();
	afx_msg void OnEnChangeEditArpeggioData4();
	afx_msg void OnEnChangeEditArpeggioData5();
	afx_msg void OnEnChangeEditArpeggioData6();
	afx_msg void OnEnChangeEditArpeggioData7();
	afx_msg void OnEnChangeEditArpeggioLength();
	void CommonArpeggioUpdated(void);
	afx_msg void OnDeltaposSpinArpeggio(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditArpeggioNum();
	CEdit mGenericInfo;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2Title();
	afx_msg void OnEnChangeEdit39Author();
	afx_msg void OnEnChangeEdit40Released();
	CEdit mZeroPageStart;
	afx_msg void OnChangeZeroPageStart();
	CEdit mHardRestartAttackDecay[MusicStudio1::MusicFile::kMaxTracks];
	CEdit mHardRestartSustainRelease[MusicStudio1::MusicFile::kMaxTracks];
	CEdit mHardRestartWaveform[MusicStudio1::MusicFile::kMaxTracks];
	CEdit mHardRestartFrames[MusicStudio1::MusicFile::kMaxTracks];
	afx_msg void OnChangeCommonHardRestart();
};

#ifndef _DEBUG  // debug version in MusicStudioView2.cpp
inline CMusicStudioDoc* CMusicStudioView2::GetDocument() const
   { return reinterpret_cast<CMusicStudioDoc*>(m_pDocument); }
#endif
