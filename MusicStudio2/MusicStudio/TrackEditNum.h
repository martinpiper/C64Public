#pragma once


// For the expanded edit control messages
enum
{
	kEditNumInsert = WM_APP,
	kEditNumRemove = WM_APP+1,
	kEditNumSetValue = WM_APP+2,
	kEditNumOnSetFocus = WM_APP+3,
	kEditNumOnArrowKey = WM_APP+4,
	kEditNumEmptyKeyDelete = WM_APP+5,
	kEditNumKeyInsert = WM_APP+6
};

// CTrackEditNum

class CTrackEditNum : public CEdit
{
	DECLARE_DYNAMIC(CTrackEditNum)

	enum
	{
		kControlKeyFlag = 0x10000
	};

public:
	CTrackEditNum();
	virtual ~CTrackEditNum();

	bool SetColour(const COLORREF background = RGB(255,255,255),const COLORREF textColour = RGB(0,0,0));
	void ResendLastChar(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnEnMaxtext();
	afx_msg void OnEnChange();

protected:
	COLORREF mTextBkColour;
	COLORREF mTextColour;
	CBrush mBrush;
	bool mLocalPushingChanges;

public:
	int mIndex;
	int mTrack;
	UINT mChar;
	UINT mRepCnt;
	UINT mFlags;
	static bool mProcessChanges;
	afx_msg void OnEnSetfocus();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
