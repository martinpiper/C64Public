#pragma once


// CRipSID dialog

class CRipSID : public CDialogEx
{
	DECLARE_DYNAMIC(CRipSID)

public:
	CRipSID(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRipSID();

// Dialog Data
	enum { IDD = IDD_RIPSID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int mTrackNumber;
	int mMaxSongs;
	CString mForceNoteInstruments;
	BOOL mAutoDetectPercussion;
};
