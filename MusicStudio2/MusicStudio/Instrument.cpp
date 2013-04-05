#include "StdAfx.h"
#include "Instrument.h"

CInstrument::CInstrument(void) : mAttackDecay(0) , mSustainRelease(0) , mAllowVoiceEffects(false)
{
	ZeroMemory(mTableActive,sizeof(mTableActive));
	ZeroMemory(mTableLoops,sizeof(mTableLoops));
}

CInstrument::~CInstrument(void)
{
}

bool CInstrument::Serialize(CArchive& ar)
{
	// A constant value at the start of every file
	const int kMagicHeader = 0xa981cdea;
	// Version 0 - ...	= 2.1
	// A version of the file that we save
	const int kCurrentVersion = 0;
	if (ar.IsStoring())
	{
		ar << kMagicHeader;
		ar << kCurrentVersion;

		ar << mName;
		ar << mAttackDecay;
		ar << mSustainRelease;
		ar << mTranspose;
		ar << mAllowVoiceEffects;

		int i;

		for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
		{
			ar << mTableActive[i];
			if (mTableActive[i])
			{
				ar << mTableLoops[i];
				ar << (int) mTableControls[i].size();
				std::vector<unsigned char>::iterator st;
				st = mTableControls[i].begin();
				while (st != mTableControls[i].end())
				{
					ar << *st++;
				}
				st = mTableValues[i].begin();
				while (st != mTableValues[i].end())
				{
					ar << *st++;
				}
			}
		}
	}
	else
	{
		int magicTest , version = -1;
		ar >> magicTest;

		if (magicTest != kMagicHeader)
		{
			return false;
		}
		ar >> version;

		// Parse the various released file versions
		if (version == 0)
		{
			ar >> mName;
			ar >> mAttackDecay;
			ar >> mSustainRelease;
			ar >> mTranspose;
			ar >> mAllowVoiceEffects;

			int i;

			for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
			{
				ar >> mTableActive[i];
				if (mTableActive[i])
				{
					ar >> mTableLoops[i];
					int tableSize;
					ar >> (int) tableSize;
					// size_t casting needed since size_t varies in size for 32 and 64 bit machines. Yuck.
					mTableControls[i].reserve((size_t) tableSize);
					mTableValues[i].reserve((size_t) tableSize);

					int j;
					for (j=0;j<tableSize;j++)
					{
						unsigned char t;
						ar >> t;
						mTableControls[i].push_back(t);
					}
					for (j=0;j<tableSize;j++)
					{
						unsigned char t;
						ar >> t;
						mTableValues[i].push_back(t);
					}
				}
			}
		}
	}

	return true;
}
