#pragma once

#include <list>
#include <vector>
#include <map>

enum PulseTypes
{
	kShort = 0,
	kMedium,
	kLong,
	kZero,
	kOne
};

class C64Tape
{
public:
	C64Tape();
	virtual ~C64Tape();

	PulseTypes CommodoreReadPulse( unsigned char input , const bool updateShortPulse = false );
	PulseTypes TurboReadPulse( unsigned char input );
	void TapeWriteByte( const bool useTurbo , const unsigned char output );
	void TapeChecksumByte( std::vector<unsigned char> &data, const unsigned char output );

	bool IsNum(const char *arg);
	int ParamToNum(const char *arg);

	int HandleParams( int argc , char ** argv );

	void DisplayHelp(void);

private:
	class Stream
	{
	public:
		Stream();
		virtual ~Stream();

		int mPriority;
		int mFile;
		bool mNewfile;
		std::vector<char> mData;
	};

	void SetDefaultLabels(void);
	void ReadTurboLabels(void);
	void WriteTurboLeader(int rep = 2);
	void AddStream(const bool newFile = true);
	void FlushStreams(void);

	int mObservedShortPulse;
	int mCalculatedMediumPulse;
	int mCalculatedLongPulse;
	unsigned char mTapeFileVersion;
	bool mForWriting;
	int mOpenedFileLength;
	int mTurboZeroPulse;
	int mTurboOnePulse;
	bool mTurboLittleEndian;
	std::vector<unsigned char> mTurboHeaderBytes;
	unsigned char mChecksumRegister;
	bool mTurboGotSync;
	FILE *mTapeFile;
	std::map<std::string , int> mLabelToAddress;

	// From labels or configuration
	unsigned char mTapePilotCode1;
	unsigned char mTapePilotCode2;
	unsigned char mTapeSyncCode;
	unsigned char mTapeHeaderByteEx;
	unsigned char mTapeHeaderByteEx2;
	unsigned char mTapeHeaderByteEx2RLE;
	unsigned char mTapeHeaderByteEx3;

	int mTapeTurboSpeed;

	Stream *mCurrentStream;
	std::list< Stream* > mStreams;
};
