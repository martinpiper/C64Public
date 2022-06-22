#pragma once


// CImportMIDI dialog

class CImportMIDI : public CDialogEx
{
	DECLARE_DYNAMIC(CImportMIDI)

public:
	CImportMIDI(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportMIDI();

// Dialog Data
	enum { IDD = IDD_IMPORTMIDI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int mTimeMultiplier;
	int mBlockLength;
	int mChannelMap0;
	int mChannelMap1;
	int mChannelMap2;
	int mChannelMap3;
	int mChannelMap4;
	int mChannelMap5;
	int mChannelMap6;
	int mChannelMap7;
	int mChannelMap8;
	int mChannelMap9;
	int mChannelMap10;
	int mChannelMap11;
	int mChannelMap12;
	int mChannelMap13;
	int mChannelMap14;
	int mChannelMap15;
	BOOL mReleaseNoteAfterHalfDuration;
	int mBytesUsed0;
	int mBytesUsed1;
	int mBytesUsed2;
	int mBytesUsed3;
	int mBytesUsed4;
	int mBytesUsed5;
	int mBytesUsed6;
	int mBytesUsed7;
	int mBytesUsed8;
	int mBytesUsed9;
	int mBytesUsed10;
	int mBytesUsed11;
	int mBytesUsed12;
	int mBytesUsed13;
	int mBytesUsed14;
	int mBytesUsed15;
};

