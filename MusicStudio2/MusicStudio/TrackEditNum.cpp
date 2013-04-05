// TrackEditNum.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudio.h"
#include "MusicStudioDoc.h"
#include "MusicStudioView.h"
#include "MainFrm.h"
#include "TrackEditNum.h"


// CTrackEditNum

IMPLEMENT_DYNAMIC(CTrackEditNum, CEdit)

bool CTrackEditNum::mProcessChanges = true;

CTrackEditNum::CTrackEditNum() : mIndex(-1) , mTrack(-1) , mTextBkColour(-1) , mTextColour(-1) , mLocalPushingChanges(false)
{
	SetColour();
}

CTrackEditNum::~CTrackEditNum()
{
}


BEGIN_MESSAGE_MAP(CTrackEditNum, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_MAXTEXT, &CTrackEditNum::OnEnMaxtext)
	ON_CONTROL_REFLECT(EN_CHANGE, &CTrackEditNum::OnEnChange)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CTrackEditNum::OnEnSetfocus)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CTrackEditNum message handlers


HBRUSH CTrackEditNum::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkColor(mTextBkColour);
	return mBrush;
}


void CTrackEditNum::OnEnMaxtext()
{
	if (!mProcessChanges)
	{
		return;
	}

	int start,end;
	GetSel(start,end);
	int limit = (int)GetLimitText();
	if ((start == limit) && (end == limit))
	{
		GetParent()->SendMessage(WM_NEXTDLGCTL);
		GetParent()->SendMessage(kEditNumInsert,MAKEWORD(mIndex+1,mTrack),1);
		GetParent()->SendMessage(kEditNumSetValue,MAKEWORD(mIndex+1,mTrack),0);
	}
	else if ( (start == 0) && (end == 0))
	{
		GetParent()->SendMessage(kEditNumInsert,MAKEWORD(mIndex,mTrack),0);
		GetParent()->SendMessage(kEditNumSetValue,MAKEWORD(mIndex,mTrack),0);
	}
}

void CTrackEditNum::OnEnChange()
{
	if (!mProcessChanges)
	{
		return;
	}
	if (mLocalPushingChanges)
	{
		return;
	}
	mLocalPushingChanges = true;
	int start,end;
	GetSel(start,end);
	int limit = (int)GetLimitText();
	if ((start == limit) && (end == limit))
	{
		GetParent()->SendMessage(kEditNumSetValue,MAKEWORD(mIndex,mTrack),0);
		GetParent()->SendMessage(WM_NEXTDLGCTL);
		mLocalPushingChanges = false;
		return;
	}
	else if ((start == 0) && (end == 0))
	{
		// If the update creates an empty cell then remove it
		CString text;
		GetWindowText(text);
		if (text.IsEmpty())
		{
			GetParent()->SendMessage(kEditNumRemove,MAKEWORD(mIndex,mTrack),0);
			mLocalPushingChanges = false;
			return;
		}
	}

	GetParent()->SendMessage(kEditNumSetValue,MAKEWORD(mIndex,mTrack),0);
	mLocalPushingChanges = false;
}

bool CTrackEditNum::SetColour(const COLORREF background,const COLORREF textColour)
{
	bool changed = false;
	if (mTextBkColour != background)
	{
		changed = true;
		mTextBkColour = background;
		if (mBrush.m_hObject)
		{
			mBrush.DeleteObject();
		}
		mBrush.CreateSolidBrush(mTextBkColour);
	}
	if (mTextColour != textColour)
	{
		changed = true;
		mTextColour = textColour;
	}
	return changed;
}

void CTrackEditNum::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	mChar = nChar;
	mRepCnt = nRepCnt;
	mFlags = nFlags;
	CEdit::OnChar(nChar,nRepCnt,nFlags);
}

void CTrackEditNum::ResendLastChar(void)
{
	SendMessage(WM_CHAR,mChar,MAKELONG(mRepCnt,mFlags));
}

static UINT sControlKeyMask = 0;

void CTrackEditNum::OnEnSetfocus()
{
//	sControlKeyMask = 0;
	GetParent()->SendMessage(kEditNumOnSetFocus,MAKEWORD(mIndex,mTrack),0);
}

UINT CTrackEditNum::OnGetDlgCode()
{
	// If |DLGC_WANTALLKEYS is also used then the tab key does not progress to the next control
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}

void CTrackEditNum::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	LRESULT ret = 0;
	if ( nRepCnt && ((nChar == VK_UP) || (nChar == VK_DOWN) || (nChar == VK_LEFT) || (nChar == VK_RIGHT)) )
	{
		// See if anyone in the parent is interested in the key
		ret = GetParent()->SendMessage(kEditNumOnArrowKey,MAKEWORD(mIndex,mTrack),nChar | sControlKeyMask);
	}
	else if (nRepCnt && ((nChar == VK_DELETE) || (nChar == VK_BACK)))
	{
		CString text;
		GetWindowText(text);
		if (text.IsEmpty())
		{
			ret = GetParent()->SendMessage(kEditNumEmptyKeyDelete,MAKEWORD(mIndex,mTrack),0);
		}
	}
	else if (nRepCnt && (nChar == VK_INSERT))
	{
		ret = GetParent()->SendMessage(kEditNumKeyInsert,MAKEWORD(mIndex,mTrack),0);
	}

	if (nChar == VK_CONTROL)
	{
		sControlKeyMask = kControlKeyFlag;
	}

	if (!ret)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CTrackEditNum::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL)
	{
		sControlKeyMask = 0;
	}

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
