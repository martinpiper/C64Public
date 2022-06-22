#pragma once
#include <vector>
#include "Block.h"

class Music
{
public:
	Music();
	virtual ~Music();

	bool LoadMusic(const char *filename);
	void Compact();
	void ExportASM(const char *filename);

	// From MWMusic\MusicPlayer_Defs.a
	static const int kMusicPlayer_NumEffects		= 32;
	static const int kMusicPlayer_EffectsSize		= 16;
	static const int kMusicPlayer_NumSequences		= 128;
	static const int kMusicPlayer_SequenceSize		= 128;
	static const int kMusicPlayer_NumTunes			= 16;
	static const int kMusicPlayer_TunesSize			= 8;
	static const int kMusicPlayer_TunesVoiceSize	= 128;
	static const int kMusicPlayer_NumChannels		= 3;

	const std::vector<Block> &getTracks(void)
	{
		return blocksTracks;
	}

	const std::vector<Block> &getSequences(void)
	{
		return blocksSequences;
	}

	const std::vector<Block> &getEffect1(void)
	{
		return blocksEffect1;
	}

	const std::vector<Block> &getEffect2(void)
	{
		return blocksEffect2;
	}

private:

	void RemoveTrack(const size_t index);
	void RemoveSequence(const size_t index);
	void RemoveEffect(const size_t index);

	Block blocksVibratoPattern1;
	Block blocksVibratoPattern2;
	Block blocksVibratoDelays;
	std::vector<Block> blocksEffect1;
	std::vector<Block> blocksEffect2;
	std::vector<Block> blocksTunes;
	std::vector<Block> blocksSequences;
	std::vector<Block> blocksTracks;
};
