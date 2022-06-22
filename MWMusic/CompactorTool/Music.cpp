#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>
#include "Music.h"
#include "FileOps.h"

Music::Music()
{

}

Music::~Music()
{

}

bool Music::LoadMusic(const char *filename)
{
	FILE *fp = fopen(filename , "rb");

	if (0 == fp)
	{
		printf("Cannot open file for reading: %s\n" , filename);
		exit(-1);
	}
	// Skip the header bytes
	skipBytes(fp , 2);

	unsigned char fileVersion = (unsigned char) fgetc(fp);
	if (fileVersion < 2)
	{
		printf("File version < 2\n");
		exit(-1);
	}

	blocksVibratoPattern1 = readBytes(fp , 16);
	blocksVibratoPattern2 = readBytes(fp , 16);
	blocksVibratoDelays = readBytes(fp , 7);	// Unused now, but still in the file as padding data to maintain compatibility with older files

	for (int i = 0 ; i < kMusicPlayer_NumEffects ; i++)
	{
		blocksEffect1.push_back(readBytes(fp , kMusicPlayer_EffectsSize/2));
	}
	for (int i = 0 ; i < kMusicPlayer_NumEffects ; i++)
	{
		blocksEffect2.push_back(readBytes(fp , kMusicPlayer_EffectsSize/2));
	}

	for (int i = 0 ; i < kMusicPlayer_NumTunes ; i++)
	{
		Block tune = readBytes(fp , kMusicPlayer_TunesSize);
		// Calculate some temporary index values, convert to lo/hi pairs on output
		tune[0] = (i*kMusicPlayer_NumChannels);
		tune[1] = (i*kMusicPlayer_NumChannels)+1;
		tune[2] = (i*kMusicPlayer_NumChannels)+2;
		blocksTunes.push_back(tune);
	}

	skipBytes(fp , kMusicPlayer_NumSequences * 2);	// sequenceLo/sequenceHi table
	skipBytes(fp , 0x58);	// Alignment

	for (int i = 0 ; i < kMusicPlayer_NumSequences ; i++)
	{
		blocksSequences.push_back(readBytes(fp , kMusicPlayer_SequenceSize));
	}

	for (int i = 0 ; i < kMusicPlayer_NumTunes ; i++)
	{
		for (int j = 0 ; j < kMusicPlayer_NumChannels ; j++)
		{
			blocksTracks.push_back(readBytes(fp , 128));
		}
	}

	fclose(fp);

	return true;
}

static void trimToEndBlock(Block &block)
{
	Block::iterator pos = std::find(block.begin(), block.end(), 0xff);

	if (block.end() != pos)
	{
		pos++;
		block.erase(pos , block.end());
	}
	else
	{
		block.clear();
	}
}

void Music::Compact()
{
	for (Block &block : blocksSequences)
	{
		trimToEndBlock(block);
	}

	for (Block &block : blocksTracks)
	{
		trimToEndBlock(block);
	}

	for (size_t i = 0 ; i < blocksTunes.size() ; i++)
	{
		bool anyUsed = false;
		for (int j = 0 ; j < kMusicPlayer_NumChannels ; j++)
		{
			int theTrack = blocksTunes[i][j];
			if (blocksTracks[theTrack].size() >= 2)
			{
				anyUsed = true;
				break;
			}
		}
		if (!anyUsed)
		{
			blocksTunes.erase(blocksTunes.begin() + i);
			i--;
			continue;
		}
	}

	// Remove tracks
	bool deleted = false;
	do 
	{
		deleted = false;
		for (size_t i = 0 ; i < blocksTracks.size() ; i++)
		{
			bool anyUsed = false;

			for (size_t j = 0 ; j < blocksTunes.size() ; j++)
			{
				for (int k = 0 ; k < kMusicPlayer_NumChannels ; k++)
				{
					int theTrack = blocksTunes[j][k];
					if (theTrack == i)
					{
						anyUsed = true;
						break;
					}
				}
				if (anyUsed)
				{
					break;
				}
			}
			if (!anyUsed)
			{
				RemoveTrack(i);
				deleted = true;
			}
		}
	} while (deleted);

	// Remove unused sequences
	do 
	{
		deleted = false;
		for (size_t i = 0 ; i < blocksSequences.size() ; i++)
		{
			bool anyUsed = false;

			for (size_t j = 0 ; j < blocksTracks.size() ; j++)
			{
				for (size_t stp = 0 ; stp < blocksTracks[j].size() ; stp++)
				{
					if (blocksTracks[j][stp] & 0x40)
					{
						continue;
					}
					stp++;
					if (stp >= blocksTracks[j].size())
					{
						continue;
					}

					if (blocksTracks[j][stp] < 0x80)
					{
						if (i == blocksTracks[j][stp])
						{
							anyUsed = true;
							break;
						}
					}

					if (anyUsed)
					{
						break;
					}
				}

				if (anyUsed)
				{
					break;
				}
			}
			if (!anyUsed)
			{
				RemoveSequence(i);
				deleted = true;
			}
		}
	} while (deleted);

	// Remove unused effects
	do 
	{
		deleted = false;
		for (size_t i = 0 ; i < blocksEffect1.size() ; i++)
		{
			bool anyUsed = false;

			for (size_t j = 0 ; j < blocksSequences.size() ; j++)
			{
				for (size_t stp = 0 ; stp < blocksSequences[j].size() ; stp++)
				{
					if (blocksSequences[j][stp] >= 0x80)
					{
						stp++;
						if (stp >= blocksSequences[j].size())
						{
							continue;
						}

						if (blocksSequences[j][stp] < kMusicPlayer_NumEffects)
						{
							if (i == blocksSequences[j][stp])
							{
								anyUsed = true;
								break;
							}
							stp++;
						}
					}
					else
					{
						stp++;
					}

					if (anyUsed)
					{
						break;
					}
				}

				if (anyUsed)
				{
					break;
				}
			}
			if (!anyUsed)
			{
				RemoveEffect(i);
				deleted = true;
			}
		}
	} while (deleted);

	printf("%d effects\n%d tunes\n%d sequences\n%d tracks\n" , (int)blocksEffect1.size() , (int)blocksTunes.size() , (int)blocksSequences.size() , (int)blocksTracks.size());
}

void Music::RemoveTrack(const size_t index)
{
	blocksTracks.erase(blocksTracks.begin() + index);

	// Renumber the remaining index references
	for (size_t i = 0 ; i < blocksTunes.size() ; i++)
	{
		for (int j = 0 ; j < kMusicPlayer_NumChannels ; j++)
		{
			assert(blocksTunes[i][j] != index);

			if (blocksTunes[i][j] > index)
			{
				blocksTunes[i][j]--;
			}
		}
	}
}

void Music::RemoveSequence(const size_t index)
{
	blocksSequences.erase(blocksSequences.begin() + index);

	// Renumber the remaining index references
	for (size_t i = 0 ; i < blocksTracks.size() ; i++)
	{
		for (size_t stp = 0 ; stp < blocksTracks[i].size() ; stp++)
		{
			if (blocksTracks[i][stp] & 0x40)
			{
				continue;
			}
			stp++;

			if (stp >= blocksTracks[i].size())
			{
				continue;
			}
			if (blocksTracks[i][stp] < 0x80)
			{
				assert(blocksTracks[i][stp] != index);
				if (blocksTracks[i][stp] > index)
				{
					blocksTracks[i][stp]--;
				}
			}
		}
	}
}

void Music::RemoveEffect(const size_t index)
{
	blocksEffect1.erase(blocksEffect1.begin() + index);
	blocksEffect2.erase(blocksEffect2.begin() + index);

	// Renumber the remaining index references
	for (size_t i = 0 ; i < blocksSequences.size() ; i++)
	{
		for (size_t stp = 0 ; stp < blocksSequences[i].size() ; stp++)
		{
			if (blocksSequences[i][stp] >= 0x80)
			{
				stp++;
				if (stp >= blocksSequences[i].size())
				{
					continue;
				}

				if (blocksSequences[i][stp] < kMusicPlayer_NumEffects)
				{
					if (blocksSequences[i][stp] > index)
					{
						blocksSequences[i][stp]--;
					}
					stp++;
				}
			}
			else
			{
				stp++;
			}
		}
	}
}

static void outputASMBlock(FILE *fp , Block &block)
{
	int num = -1;
	for(unsigned char c : block)
	{
		if (num < 0)
		{
			fprintf(fp , "\t!by ");
			num++;
		}
		if (num > 0)
		{
			fprintf(fp , " , ");
		}
		fprintf(fp , "$%02x" , c);

		num++;
		if (num >= 8)
		{
			fprintf(fp , "\n");
			num = -1;
		}
	}
	fprintf(fp , "\n");
}

// Output can be built using:
// ..\acme.exe -f cbm -o t.prg --cpu 6502 -v4 --setpc $c000 ..\stdlib\stdlib.a MusicPlayer_Defs.a MusicPlayer.a t.a
void Music::ExportASM(const char *filename)
{
	FILE *fp = fopen(filename , "w");

	fprintf(fp , "MusicPlayerDataStart = *\n");

	fprintf(fp , "vibratoPattern1\n");
	outputASMBlock(fp , blocksVibratoPattern1);

	fprintf(fp , "vibratoPattern2\n");
	outputASMBlock(fp , blocksVibratoPattern2);

	Block temp;
	for (Block block : blocksEffect1)
	{
		temp.insert(temp.end() , block.begin() , block.end());
	}
	fprintf(fp , "effect_AttackDecay\n");
	outputASMBlock(fp , Block(temp.begin()+0 , temp.begin()+1));
	fprintf(fp , "effect_SustainReleas\n");
	outputASMBlock(fp , Block(temp.begin()+1 , temp.begin()+2));
	fprintf(fp , "effect_Control1\n");
	outputASMBlock(fp , Block(temp.begin()+2 , temp.begin()+3));
	fprintf(fp , "effect_Special\n");
	outputASMBlock(fp , Block(temp.begin()+3 , temp.begin()+4));
	fprintf(fp , "effect_PulseWidth\n");
	outputASMBlock(fp , Block(temp.begin()+4 , temp.begin()+5));
	fprintf(fp , "effect_PulseDepth\n");
	outputASMBlock(fp , Block(temp.begin()+5 , temp.begin()+6));
	fprintf(fp , "effect_TopBottom\n");
	outputASMBlock(fp , Block(temp.begin()+6 , temp.begin()+7));
	fprintf(fp , "effect_AttackTime\n");
	outputASMBlock(fp , Block(temp.begin()+7 , temp.end()));

	temp.clear();
	for (Block block : blocksEffect2)
	{
		temp.insert(temp.end() , block.begin() , block.end());
	}
	fprintf(fp , "effect_AttackControl\n");
	outputASMBlock(fp , Block(temp.begin()+0 , temp.begin()+1));
	fprintf(fp , "effect_Note1\n");
	outputASMBlock(fp , Block(temp.begin()+1 , temp.begin()+2));
	fprintf(fp , "effect_Note2\n");
	outputASMBlock(fp , Block(temp.begin()+2 , temp.begin()+3));
	fprintf(fp , "effect_Note3\n");
	outputASMBlock(fp , Block(temp.begin()+3 , temp.begin()+4));
	fprintf(fp , "effect_Note4\n");
	outputASMBlock(fp , Block(temp.begin()+4 , temp.begin()+5));
	fprintf(fp , "effect_Trlnote\n");
	outputASMBlock(fp , Block(temp.begin()+5 , temp.begin()+6));
	fprintf(fp , "effect_Vibrato\n");
	outputASMBlock(fp , Block(temp.begin()+6 , temp.begin()+7));
	fprintf(fp , "effect_VibratoDelay\n");
	outputASMBlock(fp , Block(temp.begin()+7 , temp.end()));

	fprintf(fp , "tuneInfo\n");
	for (Block block : blocksTunes)
	{
		fprintf(fp , "\t!by <track_%d , <track_%d , <track_%d , " , block[0] , block[1] , block[2]);
		fprintf(fp , ">track_%d , >track_%d , >track_%d , " , block[0] , block[1] , block[2]);
		fprintf(fp , "%d , %d\n" , block[6] , block[7]);
	}

	fprintf(fp , "sequenceLo\n");
	for (size_t i=0 ; i<blocksSequences.size(); i++)
	{
		fprintf(fp , "\t!by <sequence_%d\n" , (int)i);
	}
	fprintf(fp , "sequenceHi\n");
	for (size_t i=0 ; i<blocksSequences.size(); i++)
	{
		fprintf(fp , "\t!by >sequence_%d\n" , (int)i);
	}

	for (size_t i=0 ; i<blocksSequences.size(); i++)
	{
		fprintf(fp , "sequence_%d\n" , (int)i);
		outputASMBlock(fp , blocksSequences.at(i));
	}

	for (size_t i=0 ; i<blocksTracks.size(); i++)
	{
		fprintf(fp , "track_%d\n" , (int)i);
		outputASMBlock(fp , blocksTracks.at(i));
	}

	fprintf(fp , "tune0_0 = track_0\n");
	fprintf(fp , "tune0_1 = track_1\n");
	fprintf(fp , "tune0_2 = track_2\n");

	fclose(fp);
}
