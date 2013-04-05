#pragma once
#include <vector>
#include "../MusicStudioConvert/MusicFile.h"

class CInstrument
{
public:
	CInstrument(void);
	virtual ~CInstrument(void);

	bool Serialize(CArchive& ar);

	CString mName;
	unsigned char mAttackDecay;
	unsigned char mSustainRelease;

	unsigned char mTranspose;

	bool mAllowVoiceEffects;

	// Unrolled versions of the Envelope tables
	std::vector<unsigned char> mTableControls[MusicStudio1::MusicFile::kMaxTables];
	std::vector<unsigned char> mTableValues[MusicStudio1::MusicFile::kMaxTables];
	bool mTableActive[MusicStudio1::MusicFile::kMaxTables];
	int mTableLoops[MusicStudio1::MusicFile::kMaxTables];
};
