//TestScreen.bmp 4 9 12 chars.bin screen.bin colours.bin
//-c64remap 7 15 -c64remap 3 11 -c64remap 9 11 -c64remap 15 12 ..\Animation\RawFrames5\frm00012.bmp 1 11 12 c:\temp\t.chr c:\temp\t.scr c:\temp\t.col
//-bitmap -m ..\AnimationBitmap\Animation1\frm-0.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-bitmap -m TestScreen.bmp 4 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-bitmap -m TestFull.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-bitmap -m TestScreen.bmp 4 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetcolours 0 11 12 -bitmap -m TestFullWithSprites.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation6\frm00019.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetframes c:\temp\t.spr 16 -sheetinfo c:\temp\t.sno 1024 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation6\frm00016.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetframes c:\temp\t.spr 16 -sheetinfo c:\temp\t.sno 1024 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation6\frm00019.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetframes c:\temp\t.spr -1 -sheetinfo c:\temp\t.sno -1 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation6\frm00019.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetthresholdmax -sheetthreshold 11 8 -sheetthreshold 12 8 -sheetthreshold 15 8 -sheetthreshold 1 8 -sheetframes c:\temp\t.spr 16 -sheetinfo c:\temp\t.sno 1024 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation7\frm00085.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetthresholdmax -sheetthreshold 11 2 -sheetthreshold 12 2 -sheetthreshold 15 2 -sheetthreshold 1 2 -sheetframes c:\temp\t.spr 16 -sheetinfo c:\temp\t.sno 1024 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation7\frm00085.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-sheetthresholdbase 2 -sheetframes c:\temp\t.spr 16 -sheetinfo c:\temp\t.sno 1024 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation7\frm00085.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-addscreeninfo -addsheetinfo -sheetthresholdbase 5 -sheetframes c:\temp\t.spr -1 -sheetinfo c:\temp\t.sno -1 48 -sheetcolours 14 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation3\frm00142.bmp 14 0 0 c:\temp\t.chr c:\temp\t.scr c:\temp\t.col
//-sheetthresholdbase 0 -sheettargetnumspritesmax 8 -sheetframes c:\temp\t.spr -1 -sheetinfo c:\temp\t.sno -1 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation8\frm00682.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-spriteminy 0 -spritemaxy 75 -spriteminx 80 -spritemaxx 216 -sheetthresholdbase 0 -sheettargetnumspritesmax 8 -sheetframes c:\temp\t.spr -1 -sheetinfo c:\temp\t.sno -1 48 -sheetcolours 0 11 12 -bitmap -m C:\Work\C64\AnimationBitmap\Animation8\frm00048.bmp 0 0 0 c:\temp\t.bin c:\temp\t.scr c:\temp\t.col
//-forceordering -bitmap -r  ..\AnimationBitmap\Animation10\frm00060.bmp 0 0 0 c:\temp\t.chr c:\temp\t.scr c:\temp\t.col
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <math.h>
#include "RNPlatform/Inc/MessageHelper.h"

// The following library is free for non-commercial use.
// Available from http://www.replicanet.com/contact.html
#include "RNPlatform/Inc/MessageHelper.h"

using namespace RNReplicaNet;

static bool sDoSprites = false;
static size_t sUDGByteSize = 8;

// The final UDG bytes
struct C64UDG
{
	C64UDG() {}
	void Clear()
	{
		memset(mBytes,0,sUDGByteSize);
	}
	unsigned char mBytes[64];	// Up to 64 bytes to store characters and sprites
};

struct SpriteSheetInformation
{
	SpriteSheetInformation() {}
	std::map<C64UDG,int>::iterator mSprite;
	int mX , mY;
	unsigned int mColour;
};

// Some handy operators for comparing the character data
inline bool operator== (const C64UDG& a,const C64UDG& b)
{
	return (memcmp(a.mBytes,b.mBytes,sUDGByteSize) == 0);
}

inline bool operator!= (const C64UDG& a,const C64UDG& b)
{
	return !(a == b);
}

// Can be used with STL less<> tests. In this code the std::map and std::set both use this
// to optimise the insert and find operations.
inline bool operator< (const C64UDG& a,const C64UDG& b)
{
	return (memcmp(a.mBytes,b.mBytes,sUDGByteSize) < 0);
}

static char *colourNames[] = {
	"Black",
	"White",
	"Red",
	"Cyan",
	"Purple",
	"Green",
	"Blue",
	"Yellow",
	"Orange",
	"Brown",
	"LightRed",
	"DarkGrey",
	"Grey",
	"LightGreen",
	"LightBlue",
	"LightGrey"
};

/*
// Palette entries based off the colours measured from a real C64.
static int C64Palette[] = {
						0x00, 0x00, 0x00,
						0xFF, 0xFF, 0xFF,
						0x68, 0x37, 0x2B,
						0x70, 0xA4, 0xB2,
						0x6F, 0x3D, 0x86,
						0x58, 0x8D, 0x43,
						0x35, 0x28, 0x79,
						0xB8, 0xC7, 0x6F,
						0x6F, 0x4F, 0x25,
						0x43, 0x39, 0x00,
						0x9A, 0x67, 0x59,
						0x44, 0x44, 0x44,
						0x6C, 0x6C, 0x6C,
						0x9A, 0xD2, 0x84,
						0x6C, 0x5E, 0xB5,
						0x95, 0x95, 0x95,
						};
*/

// Palette entries from VICE
static int C64Palette[] = {
					0x00, 0x00, 0x00,
					0xff, 0xff, 0xff,
					0x89, 0x40, 0x36,
					0x7a, 0xbf, 0xc7,
					0x8a, 0x46, 0xae,
					0x68, 0xa9, 0x41,
					0x3e, 0x31, 0xa2,
					0xd0, 0xdc, 0x71,
					0x90, 0x5f, 0x25,
					0x5c, 0x47, 0x00,
					0xbb, 0x77, 0x6d,
					0x55, 0x55, 0x55,
					0x80, 0x80, 0x80,
					0xac, 0xea, 0x88,
					0x7c, 0x70, 0xda,
					0xab, 0xab, 0xab
					};


// Calculates a metric for the "distance" of an RGB colour from the C64 palette entry.
// The smaller the number the better the match.
static float RGBFromC64Index(unsigned int RGB,unsigned char i)
{
	float dr,dg,db;
	dr = float((RGB>>16)&0xff) - float(C64Palette[(i*3)+0]);
	dg = float((RGB>>8)&0xff) - float(C64Palette[(i*3)+1]);
	db = float(RGB&0xff) - float(C64Palette[(i*3)+2]);
	return sqrt((dr*dr)+(dg*dg)+(db*db));
}

static int getRGBFromC64Index(unsigned char i)
{
	int rgb = C64Palette[i*3] | (C64Palette[(i*3)+1] << 8) | (C64Palette[(i*3)+2] << 16);
	return rgb;
}

// When converting an RGB to C64 palette index this table defines if the palette entry can
// be picked as the closest match to the RGB colour.
// Plus one extra for an unused colour
static int kUnusedColourIndex = 16;
static bool usedC64Palette[17];

static unsigned char sC64RemapColour[16] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 };

// Calculates the best match of an RGB colour to C64 palette index.
static unsigned char RGBToC64(const unsigned int RGB)
{
//	printf("0x%02x, 0x%02x, 0x%02x,\n",(RGB>>16)&255,(RGB>>8)&255,(RGB)&255);
	unsigned char bestIndex = 0;
	float bestDistance = -1.0f;
	unsigned char i;
	for (i=0;i<16;i++)
	{
		// If the colour is not flagged to be used then ignore it.
		if (!usedC64Palette[i])
		{
			continue;
		}
		float dist = RGBFromC64Index(RGB,i);
		if ((bestDistance < 0.0f) || (dist < bestDistance))
		{
			bestIndex = i;
			bestDistance = dist;
		}
	}

	if ( bestIndex == 3 )
	{
		int a = 0;
	}

	return sC64RemapColour[bestIndex];
}

static void extractIndexToSpriteSheet(const unsigned char theIndex , unsigned int blockH, unsigned int blockW, std::vector<unsigned char> &newScreen, unsigned int y, unsigned int wid, unsigned int x, std::vector<int> &oldScreen, unsigned char colourBG, std::vector<unsigned char> &newScreenSpriteSheet, std::vector<int> &oldScreenSpriteSheet)
{
	// Extract the colour into the sprite sheet
	for (unsigned int yi=0;yi<blockH;yi++)
	{
		for (unsigned int xi=0;xi<blockW;xi++)
		{
			unsigned char index = newScreen[((y+yi)*wid)+x+xi];
			if (index == theIndex)
			{
				int rgb = oldScreen[((y+yi)*wid)+x+xi];

				// Mask out what was there so we get the background colour
				newScreen[((y+yi)*wid)+x+xi] = colourBG;
				oldScreen[((y+yi)*wid)+x+xi] = getRGBFromC64Index(colourBG);

				newScreenSpriteSheet[((y+yi)*wid)+x+xi] = index;
				oldScreenSpriteSheet[((y+yi)*wid)+x+xi] = rgb;
			}
		}
	}
}


void dumpSpriteFromScreen(std::vector<unsigned char> &newScreenSpriteSheet , unsigned int x , unsigned int y , unsigned int wid , unsigned int hei)
{
	unsigned int xi , yi;
	for (yi=0;yi<21;yi++)
	{
		// MPi: TODO: Assume multicolour mode for the sprite sheet overlay since we do not want the extra complexity of setting the sprite hires/multicolour bitmask
		// Multicolour mode is half horizontal resolution.
		for (xi=0;xi<24;xi+=2)
		{
			if ((x+xi) < wid && (y+yi) < hei)
			{
				unsigned int spriteIndex = (xi/8) + (yi*3);
				unsigned int spriteColumnMask = 6 - (xi&7);
				unsigned char index = newScreenSpriteSheet[((y+yi)*wid)+x+xi];

				printf("%1x " , index);
			}
		}
		printf("\n");
	}
}

// As if this function needs a comment...
int main(int argc,char **argv)
{
	bool coloursPicked = false;
	unsigned char colourBG = 0;
	unsigned char colourMC1 = 12;
	unsigned char colourMC2 = 13;
	bool showHelp = false;
	bool doChars = true;
	bool doHiresFont = false;
	bool forceHires = false;
	bool forceMulticolour = false;
	int sAddIdx = 0;
	bool sBitmapMode = false;
	bool spriteSheet = false;
	unsigned char spriteSheetBG = 0;
	unsigned char spriteSheetMC1 = 12;
	unsigned char spriteSheetMC2 = 13;
	const char *sheetFrames = 0;
	const char *sheetInfo = 0;
	int sheetFramesPadTo = -1;
	int sheetInfoPadTo = -1;
	int sheetInfoFrameOffset = 0;
	unsigned char usedColoursSheetThreshold[16];
	bool addScreenInfo = false;
	bool addSheetInfo = false;
	int sheettargetnumspritesmax = -1;
	unsigned int spriteminx = 0;
	unsigned int spritemaxx = 320-24;
	unsigned int spriteminy = 0;
	unsigned int spritemaxy = 200-21;
	bool forceOrdering = false;
	DynamicMessageHelper *previousScreen = 0, *previousColours = 0;

	int sheetthresholdbase = 0;
	memset(usedColoursSheetThreshold , 0 , sizeof(usedColoursSheetThreshold));


	// Parse the command line options.
	if (argc < 2)
	{
		showHelp = true;
	}
	else
	{
		while ((argv[1][0] == '-') || (argv[1][0] == '/'))
		{
			if (strcmp( argv[1] , "-forceordering" ) == 0 )
			{
				forceOrdering = true;
			}
			else if (strcmp( argv[1] , "-addscreeninfo" ) == 0 )
			{
				addScreenInfo = true;
			}
			else if (strcmp( argv[1] , "-addsheetinfo" ) == 0 )
			{
				addSheetInfo = true;
			}
			else if (strcmp( argv[1] , "-sheetthresholdmax" ) == 0 )
			{
				memset(usedColoursSheetThreshold , 255 , sizeof(usedColoursSheetThreshold));
			}
			else if (strcmp( argv[1] , "-reducebitmapchange" ) == 0 )
			{
				if (argc >= 3 )
				{
					previousScreen = new DynamicMessageHelper();
					if (!previousScreen->Read(argv[2] , true))
					{
						delete previousScreen;
						previousScreen = 0;
					}
					argc--;
					argv++;
					previousColours = new DynamicMessageHelper();
					if (!previousColours->Read(argv[2] , true))
					{
						delete previousScreen;
						delete previousColours;
						previousScreen = 0;
						previousColours = 0;
					}
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-spriteminy" ) == 0 )
			{
				if (argc >= 3 )
				{
					spriteminy = (unsigned int) atoi(argv[2]);
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-spritemaxy" ) == 0 )
			{
				if (argc >= 3 )
				{
					spritemaxy = (unsigned int) atoi(argv[2]);
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-spriteminx" ) == 0 )
			{
				if (argc >= 3 )
				{
					spriteminx = (unsigned int) atoi(argv[2]);
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-spritemaxx" ) == 0 )
			{
				if (argc >= 3 )
				{
					spritemaxx = (unsigned int) atoi(argv[2]);
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-sheettargetnumspritesmax" ) == 0 )
			{
				if (argc >= 3 )
				{
					sheettargetnumspritesmax = atoi(argv[2]);
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-sheetthresholdbase" ) == 0 )
			{
				if (argc >= 3 )
				{
					sheetthresholdbase = atoi(argv[2]);
					memset(usedColoursSheetThreshold , sheetthresholdbase , sizeof(usedColoursSheetThreshold));
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-sheetthreshold" ) == 0 )
			{
				if (argc >= 3 )
				{
					usedColoursSheetThreshold[atoi(argv[2])] = atoi(argv[3]);
					argc-=2;
					argv+=2;
				}
			}
			else if (strcmp( argv[1] , "-sheetframes" ) == 0 )
			{
				if (argc >= 3 )
				{
					sheetFrames = argv[2];
					sheetFramesPadTo = atoi(argv[3]);
					argc-=2;
					argv+=2;
				}
			}
			else if (strcmp( argv[1] , "-sheetinfo" ) == 0 )
			{
				if (argc >= 4 )
				{
					sheetInfo = argv[2];
					sheetInfoPadTo = atoi(argv[3]);
					sheetInfoFrameOffset = atoi(argv[4]);
					argc-=3;
					argv+=3;
				}
			}
			else if (strcmp( argv[1] , "-sheetcolours" ) == 0 )
			{
				spriteSheet = true;
				if (argc >= 5 )
				{
					spriteSheetBG = atoi(argv[2]);
					spriteSheetMC1 = atoi(argv[3]);
					spriteSheetMC2 = atoi(argv[4]);
					argc -= 3;
					argv += 3;
				}
			}
			else if (strcmp( argv[1] , "-c64remap" ) == 0 )
			{
				if (argc >= 2 )
				{
					sC64RemapColour[ atoi(argv[2]) ] = atoi(argv[3]);
					argc--;
					argv++;
					argc--;
					argv++;
				}
			}
			else if ((argv[1][1] == 'h') || (argv[1][1] == 'H') || (argv[1][1] == '?'))
			{
				showHelp = true;
			}
			else if (argv[1][1] == 'f')
			{
				doChars = false;
				doHiresFont = true;
			}
			else if (argv[1][1] == 'r')
			{
				forceHires = true;
				forceMulticolour = false;
			}
			else if (argv[1][1] == 'm')
			{
				forceHires = false;
				forceMulticolour = true;
			}
			else if (argv[1][1] == 's')
			{
				sDoSprites = true;
				sUDGByteSize = 64;
			}
			else if (strcmp( argv[1] , "-addidx" ) == 0 )
			{
				if (argc >= 1 )
				{
					sAddIdx= atoi(argv[2]);
					argc--;
					argv++;
				}
			}
			else if (strcmp( argv[1] , "-bitmap" ) == 0 )
			{
				sBitmapMode = true;
			}
			argc--;
			argv++;
		}
	}
	

	if (showHelp)
	{
		printf("Usage: CharPack [-h] [-r] [-m] [-f] [-s] <-bitmap> <-c64remap a b> <-addidx a> <input 24bit BMP> [bg colour] [colour 1] [colour 2] [chars file] [screen file] [colours file]\n");
		printf("Usage for hires font conversion: CharPack -f <input 24bit BMP> [bg colour] [font data file]\n");
		printf("Usage for sprite conversion: CharPack -s <input 24bit BMP> [bg colour] [colour 1] [colour 2] [sprite data file] [sprite index file] [sprite colours file]\n");
		printf("Examples:\nTo show what colours are used: CharPack TestScreen3.bmp\n");
		printf("Note colours 0 (black), 10 (light red) and 1 (white) are often used so pick those for the three colours.\n");
		printf("To compress characters with the colours: CharPack TestScreen3.bmp 0 10 1 chars.bin screen.bin colours.bin\n");
		printf("-h/-? Help.\n");
		printf("-r Force hires UDG conversion.\n");
		printf("-m Force multicolour character conversion.\n");
		printf("-c64remap a b : Remaps a C64 colours from a to b. -c64remap 9 11 -c64remap 15 12 : Would remap colour 9 to 11 then 15 to 12\n");
		printf("-addidx a : Adds an optional index to the character map output\n");
		printf("-bitmap : Switches to bitmap conversion mode. -r or -m must be used for force hires or multicolour mode. Only the bg colour is used, colour 1 and 2 are ignored as these are specified by the char screen and colour RAM.\n");
		printf("-sheetthresholdmax : Sets maximum threshold for all colours to be output to the sprite sheet.\n");
		printf("-sheetthreshold <index> <value> : Sets the threshold value for a specific colour index\n");
		printf("-sheetframes <filename> <number> : Sets the output filename for the sprite sheet frames and the number of sprites to pad the output data to.\n");
		printf("-sheetinfo <filename> <pad> <offset>: Sets the output filename for the sprite information, plus the bytes to pad the data to and the frame offset to use when writing the info.\n");
		printf("-sheetcolours <background> <multicolour 1> <multicolour 2>: Defines the background and multicolour index values to use for the sprites.\n");
		printf("-sheetthresholdbase <value> : Sets the threshold value for all colours in the sprite sheet.\n");
		printf("-addscreeninfo : Adds screen background and multicolour to the output screen character information.\n");
		printf("-addsheetinfo : Adds sprite sheet multicolour information to the sheet info data.\n");
		printf("-sheetthresholdbase <num sprites> : Will keep on increasing sheetthresholdbase until the number of sprites is <= this target.\n");
		printf("-forceordering : Forces ordering of bitmap colour information to reduce changes in chosen intra-frame colours.\n");
		printf("-reducebitmapchange <screen file> <colours file>: Compares the current bitmap with previous colour data and choses an optimal set of colours that reduces inter-frame changes.\n");

		exit(0);
	}

	bool runConversion = true;
	while(runConversion)
	{
		runConversion = false;
		char *fileNameBMP = argv[1];

		if (doChars && (argc >= 5))
		{
			coloursPicked = true;
			colourBG = (unsigned char) atoi(argv[2]);
			colourMC1 = (unsigned char) atoi(argv[3]);
			colourMC2 = (unsigned char) atoi(argv[4]);
		}

		if (doHiresFont && (argc >= 3))
		{
			coloursPicked = true;
			colourBG = (unsigned char) atoi(argv[2]);
			colourMC1 = (unsigned char) kUnusedColourIndex;
			colourMC2 = (unsigned char) kUnusedColourIndex;
		}

		char *charsFilename = 0;
		char *screenFilename = 0;
		char *coloursFilename = 0;
		if (doChars && (argc >= 8))
		{
			charsFilename = argv[5];
			screenFilename = argv[6];
			coloursFilename = argv[7];
		}

		char *fontFilename = 0;
		if (doHiresFont && (argc >= 4))
		{
			fontFilename = argv[3];
		}

		// Check for the input file.
		FILE *fp = fopen(fileNameBMP,"rb");
		if (!fp)
		{
			printf("Could not open file '%s' for reading.\n",fileNameBMP);
			exit(-1);
		}
		fclose(fp);

		// Read the BMP file data.
		DynamicMessageHelper file;
		file.Read(fileNameBMP,true);

		// Parse and verify the BMP file data.
		if (file.GetBufferSize() < 0x20)
		{
			printf("File is too short for a BMP file.\n");
			exit(-1);
		}

		unsigned short id;
		file >> id;
		if (id != 0x4d42)
		{
			printf("File is not a BMP file.\n");
			exit(-1);
		}

		file.SetSize(0xa);
		unsigned int bitmapOffset;
		file >> bitmapOffset;

		file.SetSize(0x12);
		unsigned int wid,hei;
		file >> wid;
		file >> hei;
		unsigned char bits;
		file >> bits;
		file >> bits;
		file >> bits;

		if (bits != 24)
		{
			printf("File is not 24 bits.\n");
			exit(-1);
		}

		if ((wid < 8) || (hei < 8))
		{
			printf("File must be larger than 8 pixels horizontally and vertically.\n");
			exit(-1);
		}

		if (sDoSprites)
		{
			if ((wid % 24) || (hei % 21))
			{
				printf("File must be a multiple size of 24 pixels horizontally and 21 pixels vertically.\n");
				exit(-1);
			}
		}
		else
		{
			if ((wid & 7) || (hei & 7))
			{
				printf("File must be a multiple size of 8 pixels horizontally and vertically.\n");
				exit(-1);
			}
		}

		// Initialise the table so that all colours are initially considered for conversion.
		memset(usedC64Palette,1,sizeof(usedC64Palette));

		// Create a map of C64 colour and the count for the colour.
		std::map<unsigned char,int> colourMap;
		unsigned int x,y;
		unsigned int roundedScan = wid * 3;
		roundedScan += 3;
		roundedScan &= ~3;
		for (y=0;y<hei;y++)
		{
			file.SetSize(bitmapOffset + (((hei-1)-y)*roundedScan));
			unsigned int RGB = 0;

			for (x=0;x<wid;x++)
			{
				file.GetVariable(&RGB,3);
				unsigned char C64Index = RGBToC64(RGB);
				// Insert a new element
				std::pair<std::map<unsigned char,int>::iterator,bool> ret = colourMap.insert(std::pair<unsigned char,int>(C64Index,0));
				// Increment the counter for the element.
				// If it was inserted then 0 becomes 1, else it was found so the reference increases by 1.
				(*ret.first).second++;
			}
		}

		// Print the colour used report.
		printf("Num colours used %d\n",colourMap.size());

		std::map<unsigned char,int>::iterator st = colourMap.begin() , en = colourMap.end();
		while (st != en)
		{
			printf("Colour %2d %10s : %d\n",(*st).first,colourNames[(*st).first],(*st).second);
			st++;
		}

		if (!coloursPicked)
		{
			if (doHiresFont)
			{
				printf("Pick the BG colour value.\n");
			}
			else
			{
				printf("Pick the BG and two multi colour values.\n");
			}
			exit(0);
		}

		// Now set what colours to use for the background and two multicolours.

		// Set the base colours since they can be picked from colour memory.
		int i;
		for ( i = 0 ; i < 16 ; i++ )
		{
			usedC64Palette[i] = false;
			if ( sC64RemapColour[i] <= 7 )
			{
				usedC64Palette[i] = true;
			}
		}

		// Then set the background and multi colour index values.
		usedC64Palette[colourBG] = true;
		usedC64Palette[colourMC1] = true;
		usedC64Palette[colourMC2] = true;

		if (sBitmapMode)
		{
			// In bitmap mode all colours are potentially usable
			for ( i = 0 ; i < 16 ; i++ )
			{
				usedC64Palette[i] = true;
			}
		}

		// Process the RGB screen producing an optimised palette lookup screen
		colourMap.clear();
		std::vector<unsigned char> newScreen;
		std::vector<int> oldScreen;
		newScreen.reserve(wid*hei);
		oldScreen.reserve(wid*hei);

		std::vector<unsigned char> newScreenSpriteSheet;
		std::vector<int> oldScreenSpriteSheet;
		newScreenSpriteSheet.reserve(wid*hei);
		oldScreenSpriteSheet.reserve(wid*hei);
		int temp = getRGBFromC64Index(spriteSheetBG);
		for (y=0;y<hei * wid;y++)
		{
			newScreenSpriteSheet.push_back(spriteSheetBG);
			oldScreenSpriteSheet.push_back(temp);
		}

		for (y=0;y<hei;y++)
		{
			file.SetSize(bitmapOffset + (((hei-1)-y)*roundedScan));
			unsigned int RGB = 0;

			for (x=0;x<wid;x++)
			{
				// Get three bytes RGB, convert it to a C64 palette entry and and store it.
				file.GetVariable(&RGB,3);
				oldScreen.push_back(RGB);
				unsigned char C64Index = RGBToC64(RGB);
				newScreen.push_back(C64Index);

				std::pair<std::map<unsigned char,int>::iterator,bool> ret = colourMap.insert(std::pair<unsigned char,int>(C64Index,0));
				// Increment the counter for the element.
				// If it was inserted then 0 becomes 1, else it was found so the reference increases by 1.
				(*ret.first).second++;
			}
		}


		// Print the final colour used report.
		printf("Num colours used after character conversion %d\n",colourMap.size());

		st = colourMap.begin();
		en = colourMap.end();
		while (st != en)
		{
			printf("Colour %2d %10s : %d\n",(*st).first,colourNames[(*st).first],(*st).second);
			st++;
		}




		// Now discard the file data.
		file.FreeBuffer();

		// If we are doing hires font conversion we can use the converted screen data
		if (doHiresFont)
		{
			unsigned int topMost = 0;
			unsigned int botMost = 0;
			bool found = false;
			// Scan each line looking for the first non-blank line, this will be the first
			// line containing font data.
			for (y=0;y<hei;y++)
			{
				for (x=0;x<wid;x++)
				{
					if (newScreen[(y*wid)+x] != colourBG)
					{
						topMost = y;
						found = true;
						break;
					}
				}
				if (found)
				{
					break;
				}
			}

			botMost = hei;

			// Scan each line looking for the first blank line, this will be the bottom most
			// line containing font data.
			for (;y<hei;y++)
			{
				bool found = false;
				for (x=0;x<wid;x++)
				{
					if (newScreen[(y*wid)+x] != colourBG)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					botMost = y;
					break;
				}
			}

			size_t maxHeight = botMost - topMost;
			printf("Top most %d\nBottom most %d\nMax height = %d\n",topMost,botMost,maxHeight);

			// Now scan columns horizontally looking for each character
			unsigned int xPos;
			std::vector<unsigned int> charXStart;
			std::vector<unsigned int> charXEnd;
			for (xPos = 0 ; xPos < wid ; xPos++)
			{
				bool found = false;
				// Find the character start
				for (x=xPos;x<wid;x++)
				{
					found = false;
					for (y=topMost;y<botMost;y++)
					{
						if (newScreen[(y*wid)+x] != colourBG)
						{
							found = true;
							break;
						}
					}
					if (found)
					{
						charXStart.push_back(x);
						break;
					}
				}

				// No more chars
				if (!found)
				{
					break;
				}

				// Find the character end
				for (;x<wid-1;x++)
				{
					found = false;
					for (y=topMost;y<botMost;y++)
					{
	//					if (newScreen[(y*wid)+x] != colourBG)
						// Test for double column blank entries to handle chars like "
						if ((newScreen[(y*wid)+x] != colourBG) || (newScreen[(y*wid)+x+1] != colourBG))
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						break;
					}
				}
				charXEnd.push_back(x);

				xPos = x;
			}

			printf("Found %d chars\n",charXStart.size());

			std::vector<unsigned int> charTopMost;
			std::vector<unsigned int> charBotMost;
			// Now loop for each character and produce min/max y position information
			size_t i;
			for (i = 0 ; i < charXStart.size() ; i++)
			{
				// Display the x start and end information
	//			printf("%d,%d   ,",charXStart[i],charXEnd[i]);
				bool found = false;
				for (y=topMost;y<hei;y++)
				{
					for (x=charXStart[i];x<charXEnd[i];x++)
					{
						if (newScreen[(y*wid)+x] != colourBG)
						{
							charTopMost.push_back(y);
							found = true;
							break;
						}
					}
					if (found)
					{
						break;
					}
				}

				// Scan upwards.
				for (y=botMost-1;y>=topMost;y--)
				{
					bool found = false;
					for (x=charXStart[i];x<charXEnd[i];x++)
					{
						if (newScreen[(y*wid)+x] != colourBG)
						{
							found = true;
							break;
						}
					}
					if (found)
					{
						break;
					}
				}
				charBotMost.push_back(y+1);
			}

			// Now create the font data for each character
			// MPi: TODO: There is the potential to do a duplicate fontData check for each newly
			// added chunk of data and reference that instead of a new duplicate chunk.
			std::vector<unsigned char> fontData;
			std::vector<size_t> startOffset;
			std::vector<unsigned int> pixelWidth;
			std::vector<unsigned int> pixelHeight;
			std::vector<int> yOffset;
			for (i = 0 ; i < charXStart.size() ; i++)
			{
				startOffset.push_back(fontData.size());
				pixelWidth.push_back(charXEnd[i] - charXStart[i]);
				pixelHeight.push_back(charBotMost[i] - charTopMost[i]);
				yOffset.push_back(charTopMost[i] - topMost);
	#if 1
				// Column ordered data seems to be faster in the drawing code
				for (x = charXStart[i]; x < charXEnd[i] ; x+=8)
				{
					for (y=charTopMost[i] ; y<charBotMost[i] ; y++)
					{
						unsigned int pix;
						unsigned char pixelData = 0;
						for (pix = 0 ; pix < 8 ; pix++)
						{
							pixelData = pixelData << 1;
							if ((x+pix) < charXEnd[i])
							{
								if (newScreen[(y*wid)+x+pix] != colourBG)
								{
									pixelData |= 1;
								}
							}
						}

						fontData.push_back(pixelData);
					}
				}
	#else
				for (y=charTopMost[i] ; y<charBotMost[i] ; y++)
				{
					unsigned char pixelData = 0;
					int pixelsOut = 0;
					for (x = charXStart[i]; x < charXEnd[i] ; x++)
					{
						pixelData = pixelData << 1;
						if (newScreen[(y*wid)+x] != colourBG)
						{
							pixelData |= 1;
						}
						pixelsOut++;
						if (pixelsOut == 8)
						{
							fontData.push_back(pixelData);
							pixelData = 0;
							pixelsOut = 0;
						}
					}
					// Plus any remaining pixels
					if (pixelsOut)
					{
						fontData.push_back(pixelData << (8-pixelsOut));
					}
				}
	#endif
			}

			printf("Total font pixel data %d bytes\n",fontData.size());
			// Check to see if we can save the data.
			if (!fontFilename)
			{
				printf("Not writing any output as no filenames given.\n");
				exit(0);
			}

			fp = fopen(fontFilename,"wb");
			if (!fp)
			{
				printf("Could not open file '%s' for writing.\n",fontFilename);
				exit(-1);
			}

			// Write the following to the file
	/*
			std::vector<unsigned char> fontData;
			std::vector<size_t> startOffset;
			std::vector<unsigned int> pixelWidth;
			std::vector<unsigned int> pixelHeight;
			std::vector<int> yOffset;
	*/
			DynamicMessageHelper fileData;
			fileData << (unsigned char) i;
			for (i = 0 ; i < charXStart.size() ; i++)
			{
				unsigned short realOffset = (unsigned short) startOffset[i] + 1 + (5 * charXStart.size());
				fileData << realOffset;
				fileData << (unsigned char) pixelWidth[i];
				fileData << (unsigned char) pixelHeight[i];
				fileData << (unsigned char) yOffset[i];
			}
			fwrite(fileData.GetBuffer(),1,fileData.GetSize(),fp);
			fwrite(&fontData[0],1,fontData.size(),fp);
		
			fclose(fp);

			exit(0);
		}


		unsigned int blockW = 8;
		unsigned int blockH = 8;

		if (sBitmapMode)
		{
			// Check to see if we can save the data.
			if (!charsFilename)
			{
				printf("Not writing any output as no filenames given.\n");
				exit(0);
			}

			FILE *fpBitmap = fopen(charsFilename,"wb");
			if (!fpBitmap)
			{
				printf("Could not open file '%s' for writing.\n",charsFilename);
				exit(-1);
			}

			// Save the colours
			FILE *fpColours = fopen(coloursFilename,"wb");
			if (!fpColours)
			{
				printf("Could not open file '%s' for writing.\n",coloursFilename);
				exit(-1);
			}

			// Save the screen
			FILE *fpScreen = fopen(screenFilename,"wb");
			if (!fpScreen)
			{
				printf("Could not open file '%s' for writing.\n",screenFilename);
				exit(-1);
			}

			if (forceHires)
			{
				colourBG = 255;	// Force it to be ignored
			}

			int screenIndex = 0;
			// Now process each UDG in the optimised screen
			for (y=0;y<hei;y+=blockH)
			{
				for (x=0;x<wid;x+=blockW)
				{
					unsigned char usedColours[16];
					memset(usedColours , 0 , sizeof(usedColours));

					unsigned int xi,yi;
					for (yi=0;yi<blockH;yi++)
					{
						if (forceHires)
						{
							for (xi=0;xi<blockW;xi++)
							{
								unsigned char index = newScreen[((y+yi)*wid)+x+xi];
								usedColours[index]++;
							}
						}
						else if (forceMulticolour)
						{
							for (xi=0;xi<blockW;xi+=2)
							{
								unsigned char index = newScreen[((y+yi)*wid)+x+xi];
								usedColours[index]++;
							}
						}
					}

					// Figure out the most used colours for this block
					unsigned int i;

					if (spriteSheet)
					{
						int numColoursThisBlock = 0;
						for (i = 0 ; i < sizeof(usedColours) ; i++)
						{
							if (i == colourBG)
							{
								// Don't count the BG colour
								continue;
							}
							if (usedColours[i] > usedColoursSheetThreshold[i])
							{
								numColoursThisBlock++;
							}
						}
						// If this block has too many colours then start to attempt to use the sprite sheet
						if (numColoursThisBlock > 3)
						{
							// Try to pull out the sprite multi colours first
							//newScreenSpriteSheet
							if (usedColours[spriteSheetMC1] > usedColoursSheetThreshold[spriteSheetMC1])
							{
								usedColours[spriteSheetMC1] = 0;
								numColoursThisBlock--;
								extractIndexToSpriteSheet(spriteSheetMC1 , blockH, blockW, newScreen, y, wid, x, oldScreen, colourBG, newScreenSpriteSheet, oldScreenSpriteSheet);
							}
						}
						if (numColoursThisBlock > 3)
						{
							// Try to pull out the sprite multi colours first
							//newScreenSpriteSheet
							if (usedColours[spriteSheetMC2] > usedColoursSheetThreshold[spriteSheetMC2])
							{
								usedColours[spriteSheetMC2] = 0;
								numColoursThisBlock--;
								extractIndexToSpriteSheet(spriteSheetMC2 , blockH, blockW, newScreen, y, wid, x, oldScreen, colourBG, newScreenSpriteSheet, oldScreenSpriteSheet);
							}
						}

						while (numColoursThisBlock > 3)
						{
	#if 1
							bool extracted = false;
							// Last of all try to find a suitable colour to pull out
							// MPi: TODO: Create a lower resolution sprite colour map and try to choose that colour by preference?
							for (i = 0 ; i < sizeof(usedColours) ; i++)
							{
								if (i == colourBG)
								{
									// Don't count the BG colour
									continue;
								}
								if (usedColours[i] <= usedColoursSheetThreshold[i])
								{
									continue;
								}

								usedColours[i] = 0;
								numColoursThisBlock--;
								extractIndexToSpriteSheet(i , blockH, blockW, newScreen, y, wid, x, oldScreen, colourBG, newScreenSpriteSheet, oldScreenSpriteSheet);
								extracted = true;
								break;
							}
							if (!extracted)
							{
								break;
							}
	#else
							// Last of all try to find a suitable colour to pull out
							// MPi: TODO: Create a lower resolution sprite colour map and try to choose that colour by preference?
							unsigned char numUsedTotal = 0;
							unsigned int numUsedIndex = colourBG;
							for (i = 0 ; i < sizeof(usedColours) ; i++)
							{
								if (i == colourBG)
								{
									// Don't count the BG colour
									continue;
								}
								if (usedColours[i] <= usedColoursSheetThreshold[i])
								{
									continue;
								}
								if (usedColours[i] > numUsedTotal)
								{
									numUsedTotal = usedColours[i];
									numUsedIndex = i;
								}
							}
							if (numUsedIndex == colourBG)
							{
								break;
							}
							usedColours[numUsedIndex] = 0;
							numColoursThisBlock--;
							extractIndexToSpriteSheet(numUsedIndex , blockH, blockW, newScreen, y, wid, x, oldScreen, colourBG, newScreenSpriteSheet, oldScreenSpriteSheet);
	#endif
						}
					}

					unsigned char maxCol01Index = 0;
					unsigned char maxColVal = 0;
					for (i = 0 ; i < sizeof(usedColours) ; i++)
					{
						if (i == colourBG)
						{
							continue;
						}
						if (usedColours[i] > maxColVal)
						{
							maxCol01Index = i;
							maxColVal = usedColours[i];
						}
					}

					unsigned char maxCol10Index = 0;
					maxColVal = 0;
					for (i = 0 ; i < sizeof(usedColours) ; i++)
					{
						if (i == colourBG || i == maxCol01Index)
						{
							continue;
						}
						if (usedColours[i] > maxColVal)
						{
							maxCol10Index = i;
							maxColVal = usedColours[i];
						}
					}

					unsigned char maxCol11Index = 0;
					maxColVal = 0;
					for (i = 0 ; i < sizeof(usedColours) ; i++)
					{
						if (i == colourBG || i == maxCol01Index || i == maxCol10Index)
						{
							continue;
						}
						if (usedColours[i] > maxColVal)
						{
							maxCol11Index = i;
							maxColVal = usedColours[i];
						}
					}

					if (forceHires)
					{
						if (forceOrdering)
						{
							maxCol10Index = colourMC1;
						}
						maxCol01Index = 0;
						maxCol11Index = 0;
					}

					if (forceMulticolour && forceOrdering)
					{
						// Try sorting the colour indexes to generate more consistent runs of colours
						bool swapped = false;
						do
						{
							swapped = false;
#if 0
							if (maxCol01Index != colourBG && maxCol10Index != colourBG && maxCol01Index < maxCol10Index)
							{
								std::swap(maxCol01Index , maxCol10Index);
								swapped = true;
							}
							if (maxCol01Index != colourBG && maxCol11Index != colourBG && maxCol01Index < maxCol11Index)
							{
								std::swap(maxCol01Index , maxCol11Index);
								swapped = true;
							}
							if (maxCol10Index != colourBG && maxCol11Index != colourBG && maxCol10Index < maxCol11Index)
							{
								std::swap(maxCol10Index , maxCol11Index);
								swapped = true;
							}
#else
							if (maxCol01Index < maxCol10Index)
							{
								std::swap(maxCol01Index , maxCol10Index);
								swapped = true;
							}
							if (maxCol01Index < maxCol11Index)
							{
								std::swap(maxCol01Index , maxCol11Index);
								swapped = true;
							}
							if (maxCol10Index < maxCol11Index)
							{
								std::swap(maxCol10Index , maxCol11Index);
								swapped = true;
							}
#endif
						} while (swapped);
					}

					if (previousScreen && previousColours)
					{
						unsigned char prevScrLo = ((unsigned char*)previousScreen->GetBuffer())[screenIndex] & 0xf;
						unsigned char prevScrHi = (((unsigned char*)previousScreen->GetBuffer())[screenIndex] & 0xf0) >> 4;
						unsigned char prevColLo = ((unsigned char*)previousColours->GetBuffer())[screenIndex] & 0xf;

						// Usage:
						// Scr : (maxCol01Index << 4) | maxCol10Index
						// Col : maxCol11Index
						// Proceed to map colours to best fit any previous frame
						if (prevScrLo != colourBG && prevScrLo == maxCol01Index)
						{
							if (maxCol10Index != prevScrLo || maxCol10Index == colourBG)
							{
								std::swap(maxCol01Index , maxCol10Index);
							}
							else if(maxCol11Index != prevColLo || maxCol11Index == colourBG)
							{
								std::swap(maxCol01Index , maxCol11Index);
							}
						}
						else if (prevScrLo != colourBG && prevScrLo == maxCol11Index)
						{
							if (maxCol01Index != prevScrHi || maxCol01Index == colourBG)
							{
								std::swap(maxCol11Index , maxCol01Index);
							}
							else if(maxCol10Index != prevColLo || maxCol10Index == colourBG)
							{
								std::swap(maxCol11Index , maxCol10Index);
							}
						}

						if (prevScrHi != colourBG && prevScrHi == maxCol10Index)
						{
							if (maxCol01Index != prevScrHi || maxCol01Index == colourBG)
							{
								std::swap(maxCol10Index , maxCol01Index);
							}
							else if(maxCol11Index != prevColLo || maxCol11Index == colourBG)
							{
								std::swap(maxCol10Index , maxCol11Index);
							}
						}
						else if (prevScrHi != colourBG && prevScrHi == maxCol11Index)
						{
							if (maxCol01Index != prevScrHi || maxCol01Index == colourBG)
							{
								std::swap(maxCol11Index , maxCol01Index);
							}
							else if(maxCol10Index != prevColLo || maxCol10Index == colourBG)
							{
								std::swap(maxCol11Index , maxCol10Index);
							}
						}

						if (prevColLo != colourBG && prevColLo == maxCol01Index)
						{
							if (maxCol10Index != prevScrLo || maxCol10Index == colourBG)
							{
								std::swap(maxCol01Index , maxCol10Index);
							}
							else if(maxCol11Index != prevColLo || maxCol11Index == colourBG)
							{
								std::swap(maxCol01Index , maxCol11Index);
							}
						}
						else if (prevColLo != colourBG && prevColLo == maxCol10Index)
						{
							if (maxCol01Index != prevScrHi || maxCol01Index == colourBG)
							{
								std::swap(maxCol01Index , maxCol01Index);
							}
							else if(maxCol11Index != prevColLo || maxCol11Index == colourBG)
							{
								std::swap(maxCol01Index , maxCol11Index);
							}
						}
					}

	//				printf("%2d %2d %2d\n" , maxCol01Index , maxCol10Index , maxCol11Index);

					// Consider highest use colours only
					for ( i = 0 ; i < 16 ; i++ )
					{
						usedC64Palette[i] = false;
					}
					usedC64Palette[colourBG] = true;
					usedC64Palette[maxCol01Index] = true;
					usedC64Palette[maxCol10Index] = true;
					usedC64Palette[maxCol11Index] = true;

					// Now output the data
					for (yi=0;yi<blockH;yi++)
					{
						unsigned char pixelData = 0;

						if (forceHires)
						{
							for (xi=0;xi<blockW;xi++)
							{
								pixelData = pixelData << 1;

								unsigned char index = RGBToC64(oldScreen[((y+yi)*wid)+x+xi]);
								if (index == maxCol10Index)
								{
									pixelData = pixelData | 1;
								}
							}
						}
						else if (forceMulticolour)
						{
							for (xi=0;xi<blockW;xi+=2)
							{
								pixelData = pixelData << 2;

								// MPi: TODO: Use oldScreen and RGBToC64() is the character conversion phase, not just bitmap
								unsigned char index = RGBToC64(oldScreen[((y+yi)*wid)+x+xi]);
								if (index == colourBG)
								{
									continue;
								}
								else if (index == maxCol01Index)
								{
									pixelData = pixelData | 1;
								}
								else if (index == maxCol10Index)
								{
									pixelData = pixelData | 2;
								}
								else if (index == maxCol11Index)
								{
									pixelData = pixelData | 3;
								}
							}
						}

						fwrite(&pixelData, sizeof(pixelData) , 1 , fpBitmap);
					}

					unsigned char charBlock = (maxCol01Index << 4) | maxCol10Index;
					if (previousScreen && previousColours)
					{
						unsigned char prevScrLo = ((unsigned char*)previousScreen->GetBuffer())[screenIndex] & 0xf;
						unsigned char prevScrHi = (((unsigned char*)previousScreen->GetBuffer())[screenIndex] & 0xf0) >> 4;

						// Carry over any values we want to leave unchanged from the previous frame
						if (maxCol01Index == colourBG)
						{
							charBlock |= prevScrHi << 4;
						}
						if (maxCol10Index == colourBG)
						{
							charBlock |= prevScrLo;
						}
					}
					fwrite(&charBlock, sizeof(charBlock) , 1 , fpScreen);

					if (previousScreen && previousColours)
					{
						unsigned char prevColLo = ((unsigned char*)previousColours->GetBuffer())[screenIndex] & 0xf;
						// Carry over any values we want to leave unchanged from the previous frame
						if (maxCol11Index == colourBG)
						{
							charBlock = prevColLo;
						}
					}
					fwrite(&maxCol11Index, sizeof(maxCol11Index) , 1 , fpColours);

					screenIndex++;
				}
			}

			fclose(fpBitmap);
			if (addScreenInfo)
			{
				fputc(colourBG , fpScreen);
				fputc(colourMC1 , fp);
				fputc(colourMC2 , fp);
			}
			fclose(fpScreen);
			fclose(fpColours);

			if (!spriteSheet)
			{
				exit(0);
			}
		}

		// MPi: TODO: Process newScreenSpriteSheet here if the spriteSheet option is enabled
		if (spriteSheet)
		{
			// Switch to larger UDG size for the sprites
			sUDGByteSize = 64;

			bool extractedSprite = false;
			std::map<C64UDG,int> sprites;
			std::list<SpriteSheetInformation> sheetEntries;

			do
			{
				extractedSprite = false;
				// Find the first used row
				for (y=0;y<hei && !extractedSprite;y++)
				{
					for (x=0;x<wid && !extractedSprite;x+=2)
					{
						if (newScreenSpriteSheet[(y*wid)+x] != spriteSheetBG)
						{
							// Found something
							C64UDG sprite;
							sprite.Clear();
						
							// Search to the left a bit for the first used column from the first row position
							if (x > 22)
							{
								x -= 22;
							}
							else
							{
								x = 0;
							}

							unsigned int xs;
							for (xs = x;(xs < (x+24)) && (xs < wid) && !extractedSprite;xs+=2)
							{
								unsigned int ys;
								for (ys = y ; (ys < (y + 21)) && (ys < hei) && !extractedSprite; ys++)
								{
									if(newScreenSpriteSheet[(ys*wid)+xs] != spriteSheetBG)
									{
										// Extract from here
										x = xs;
										unsigned int xi , yi;

										// Find the sprite colour
										unsigned char spriteColour = spriteSheetBG;
										for (yi=0;yi<21 && spriteColour == spriteSheetBG;yi++)
										{
											// MPi: TODO: Assume multicolour mode for the sprite sheet overlay since we do not want the extra complexity of setting the sprite hires/multicolour bitmask
											// Multicolour mode is half horizontal resolution.
											for (xi=0;xi<24 && spriteColour == spriteSheetBG;xi+=2)
											{
												if ((x+xi) < wid && (y+yi) < hei)
												{
													unsigned char index = newScreenSpriteSheet[((y+yi)*wid)+x+xi];
													if (index != spriteSheetBG && index != spriteSheetMC1 && index != spriteSheetMC2)
													{
														spriteColour = index;
														break;
													}
												}
											}
										}

										if (x < spriteminx)
										{
											x = spriteminx;
										}
										if (x > spritemaxx)
										{
											x = spritemaxx;
										}
										if (y < spriteminy)
										{
											y = spriteminy;
										}
										if (y > spritemaxy)
										{
											y = spritemaxy;
										}

										printf("Extracting sprite from %d , %d colour %d\n" , x , y , spriteColour);
	//									dumpSpriteFromScreen(newScreenSpriteSheet , x , y , wid , hei);

										for (yi=0;yi<21;yi++)
										{
											// MPi: TODO: Assume multicolour mode for the sprite sheet overlay since we do not want the extra complexity of setting the sprite hires/multicolour bitmask
											// Multicolour mode is half horizontal resolution.
											for (xi=0;xi<24;xi+=2)
											{
												if ((x+xi) < wid && (y+yi) < hei)
												{
													unsigned int spriteIndex = (xi/8) + (yi*3);
													unsigned int spriteColumnMask = 6 - (xi&7);
													unsigned char index = newScreenSpriteSheet[((y+yi)*wid)+x+xi];

													if (index == spriteSheetBG)
													{
														continue;
													}

													if (index == spriteSheetMC1)
													{
														// Bit pattern for MC1
														sprite.mBytes[spriteIndex] |= 1 << spriteColumnMask;
														newScreenSpriteSheet[((y+yi)*wid)+x+xi] = spriteSheetBG;
														newScreenSpriteSheet[((y+yi)*wid)+x+xi+1] = spriteSheetBG;
													}
													else if (index == spriteSheetMC2)
													{
														// Bit pattern for MC2
														sprite.mBytes[spriteIndex] |= 3 << spriteColumnMask;
														newScreenSpriteSheet[((y+yi)*wid)+x+xi] = spriteSheetBG;
														newScreenSpriteSheet[((y+yi)*wid)+x+xi+1] = spriteSheetBG;
													}
													else if (index == spriteColour)
													{
														// Bit pattern for colour
														sprite.mBytes[spriteIndex] |= 2 << spriteColumnMask;
														newScreenSpriteSheet[((y+yi)*wid)+x+xi] = spriteSheetBG;
														newScreenSpriteSheet[((y+yi)*wid)+x+xi+1] = spriteSheetBG;
													}
												}
											}
										}

										std::map<unsigned char,int> leftoverColours;

										for (yi=0;yi<21;yi++)
										{
											for (xi=0;xi<24;xi+=2)
											{
												if ((x+xi) < wid && (y+yi) < hei)
												{
													unsigned char index = newScreenSpriteSheet[((y+yi)*wid)+x+xi];
													if (index != spriteSheetBG)
													{
														std::pair<std::map<unsigned char,int>::iterator,bool> inserted = leftoverColours.insert(std::pair<unsigned char , int>(index,0));
														inserted.first->second++;
													}
												}
											}
										}

										std::map<unsigned char,int>::iterator st = leftoverColours.begin();
										while (st != leftoverColours.end())
										{
											printf("Leftover %d count %d\n" , st->first , st->second);
											st++;
										}

	//									printf("After\n");
	//									dumpSpriteFromScreen(newScreenSpriteSheet , x , y , wid , hei);

										std::pair<std::map<C64UDG,int>::iterator,bool> inserted = sprites.insert(std::pair<C64UDG,int>(sprite,0));
										SpriteSheetInformation info;
										info.mSprite = inserted.first;
										info.mColour = spriteColour;
										info.mX = x;
										info.mY = y;
										sheetEntries.push_back(info);
										extractedSprite = true;

										break;
									}
								}
							}
						}
					}
				}
			} while (extractedSprite);

			printf("Sheet sprites num = %d\n" , (int) sprites.size());

			// Create index offsets for duplicate removed sprites
			std::map<C64UDG,int>::iterator st;
			st = sprites.begin();
			int index = 0;
			while (st != sprites.end())
			{
				st->second = index++;
				st++;
			}

			if (sheetFrames != 0)
			{
				FILE *fp = fopen(sheetFrames , "wb");

				if (0 == fp)
				{
					printf("Could not open file '%s' for writing.\n",sheetFrames);
					exit(-1);
				}

				st = sprites.begin();
				while (st != sprites.end())
				{
					fwrite(st->first.mBytes , 1 , 64 , fp);
					st++;
					sheetFramesPadTo--;
				}
				while (sheetFramesPadTo > 0)
				{
					for (int i = 0 ; i < 64 ; i++)
					{
						fputc(0 , fp);
					}
					sheetFramesPadTo--;
				}

				fclose(fp);
			}

			if (sheetFrames != 0)
			{
				DynamicMessageHelper data;

				if (addSheetInfo)
				{
					data << spriteSheetMC1;
					data << spriteSheetMC2;
				}

				std::list<SpriteSheetInformation>::iterator st = sheetEntries.begin();
				while (st != sheetEntries.end())
				{
					data << (unsigned char) st->mColour;
					data << (unsigned char) (st->mSprite->second + sheetInfoFrameOffset);
					data << (unsigned short) (st->mX + 24);
					data << (unsigned char) (st->mY + 50);
					st++;
				}

				data << (unsigned char) -1;

				while (data.GetSize() < sheetInfoPadTo)
				{
					data << (unsigned char) 0;
				}

				if (!data.Write(sheetInfo , true))
				{
					printf("Could not open file '%s' for writing.\n",sheetInfo);
					exit(-1);
				}
			}

			// No-retry looping so exit
			if (sheettargetnumspritesmax == -1 )
			{
				exit(0);
			}

			// Met the target, so exit
			if ((int)sheetEntries.size() <= sheettargetnumspritesmax)
			{
				exit(0);
			}

			// Re-run the conversion
			sheetthresholdbase++;
			memset(usedColoursSheetThreshold , sheetthresholdbase , sizeof(usedColoursSheetThreshold));

			printf("Now using -sheetthresholdbase %d\n" , sheetthresholdbase);

			runConversion = true;
			continue;
		}

		// If we are doing sprite conversion we can use the converted screen data
		if (sDoSprites)
		{
			blockW = 24;
			blockH = 21;
		}
	
		// Now process the char data from the palette index screen.
		unsigned int xs = wid/blockW;
		unsigned int ys = hei/blockH;

		printf("xs %d ys %d\n", xs,ys);

		// For each UDG position we store a bitmap char.
		std::vector<C64UDG> charScreen;
		charScreen.reserve(xs*ys);
		// And store the char colour
		std::vector<unsigned char> charColour;
		charColour.reserve(xs*ys);
		std::vector<unsigned char> charColourFlag;
		charColourFlag.reserve(xs*ys);
		std::vector<unsigned char> justBGFlag;
		justBGFlag.reserve(xs*ys);
		std::vector<unsigned char> charMulticolourFlag;
		charMulticolourFlag.reserve(xs*ys);

		// Try to keep colours between cells if possible, so this is stored outside the loops
		unsigned char theColour = colourBG;

		// Now process each UDG in the optimised screen
		for (y=0;y<hei;y+=blockH)
		{
			for (x=0;x<wid;x+=blockW)
			{
				// Find what C64 indexed colours are used in the UDG.
				std::set<unsigned char> usedColours;
				unsigned int xi,yi;
				for (yi=0;yi<blockH;yi++)
				{
					for (xi=0;xi<blockW;xi++)
					{
						unsigned char index = newScreen[((y+yi)*wid)+x+xi];
						usedColours.insert(index);
					}
				}
				C64UDG theUDG;
				memset(theUDG.mBytes,0,sUDGByteSize);
				bool theColourCoalesce = false;
				bool isMulticolour = false;
				bool added = false;
				bool justBG = false;
				// Is it one colour that uses one of the first eight or background colours?
				// If so then create a character block and use that colour information.
				if (usedColours.size() == 1)
				{
					if (*usedColours.begin() == colourBG)
					{
						// A completely transparent character
						memset(theUDG.mBytes,0,sUDGByteSize);
						added = true;
						theColourCoalesce = true;
						justBG = true;
					}
					else if (sDoSprites || (*usedColours.begin() < 8))	// Sprite colours can be any colour, not just base colours signifying multicolour mode
					{
						// A filled character with the colour
						memset(theUDG.mBytes,0xff,sUDGByteSize);
						if (sDoSprites)
						{
							theUDG.mBytes[sUDGByteSize-1] = 0;
						}
						theColour = *usedColours.begin();
						added = true;
					}
				}

				// If the above test did not add a character then test it further.
				if (!added)
				{
					// If there are two colours used for this char then it can be hires
					bool hires = false;
					if (usedColours.size() == 2)
					{
						// Check to see if it uses the background and one of the first eight colours, or any colours for sprite mode.
						if ( (sDoSprites && ((*usedColours.begin() == colourBG) || (*++usedColours.begin() == colourBG))) ||
						((*usedColours.begin() == colourBG) && (*++usedColours.begin() < 8)) ||
						((*usedColours.begin() < 8) && (*++usedColours.begin() == colourBG))
						)
						{
							hires = true;
						}
					}

					if (forceHires)
					{
						hires = true;
					}
					if (forceMulticolour)
					{
						hires = false;
					}

					// If it is hires then create char data that corresponds to the pixel data.
					if (hires)
					{
						for (yi=0;yi<blockH;yi++)
						{
							theUDG.mBytes[yi] = 0;
							for (xi=0;xi<blockW;xi++)
							{
								unsigned char index = newScreen[((y+yi)*wid)+x+xi];
								if (index != colourBG)
								{
									// Pixels are stored backwards.
									if (sDoSprites)
									{
										unsigned int spriteColumnMask = 7 - (xi&7);
										theUDG.mBytes[(xi/8) + (yi*3)] |= 1 << spriteColumnMask;
									}
									else
									{
										theUDG.mBytes[yi] |= 1 << (7-xi);
									}
									// And update the colour used.
									theColour = index;
								}
							}
						}
					}
					else
					{
						isMulticolour = true;
						theColourCoalesce = true;
						// Enable multicolour mode
						// If the char just uses background or multicolour 1 or 2 then this stays the same.
						// However if the char uses one of the other colours then this is updated with that value.
						if (!sDoSprites)
						{
							theColour |= 8;
						}
						// Else process the multicolour character.
						for (yi=0;yi<blockH;yi++)
						{
							// Multicolour mode is half horizontal resolution.
							for (xi=0;xi<blockW;xi+=2)
							{
								unsigned int spriteIndex = (xi/8) + (yi*3);
								unsigned int spriteColumnMask = 6 - (xi&7);
								unsigned char index = newScreen[((y+yi)*wid)+x+xi];
								if (index != 0)
								{
									index = index;
								}
								if (index == colourMC1)
								{
									// Bit pattern for MC1
									if (sDoSprites)
									{
										theUDG.mBytes[spriteIndex] |= 1 << spriteColumnMask;
									}
									else
									{
										theUDG.mBytes[yi] |= 1 << (6-xi);
									}
								}
								else if (index == colourMC2)
								{
									// Bit pattern for MC2
									if (sDoSprites)
									{
										theUDG.mBytes[spriteIndex] |= 2 << spriteColumnMask;
									}
									else
									{
										theUDG.mBytes[yi] |= 2 << (6-xi);
									}
								}
								else if (index != colourBG)
								{
									theColourCoalesce = false;
									// Bit pattern for colour
									if (sDoSprites)
									{
										theUDG.mBytes[spriteIndex] |= 3 << spriteColumnMask;
									}
									else
									{
										theUDG.mBytes[yi] |= 3 << (6-xi);
									}
									// Multicolour so OR 8
									// MPi: TODO: Maybe find the most commonly used colour for this char and use that instead.
									if (sDoSprites)
									{
										theColour = index;
									}
									else
									{
										theColour = index | 8;
									}
								}
							}
						}
					}
				}

				// Store the char data and colour.
				charScreen.push_back(theUDG);
				charColour.push_back(theColour);
				charColourFlag.push_back(theColourCoalesce);
				charMulticolourFlag.push_back(isMulticolour);
				justBGFlag.push_back(justBG);
			}
		}

		// Now optimise the colour map so that colours that can be coalesced are coalesced, this helps compression.
		// Must preserve multicolour boundaries
	#if 1
		bool colourOptimised = false;
		do
		{
			colourOptimised = false;
			for ( i = 0 ; i < (int)(xs*ys)-1 ; i++ )
			{
	#if 1
				if ( (justBGFlag[i] && !justBGFlag[i+1]) || (justBGFlag[i] && justBGFlag[i+1]) )
				{
					charColour[i] = charColour[i+1];
				}
	#endif
				if ( charColourFlag[i] && !charColourFlag[i+1] && ( charColour[i] & 0x08 ) == ( charColour[i+1] & 0x08 ) )
				{
					charColourFlag[i] = false;
					charColour[i] = charColour[i+1];
					colourOptimised = true;
				}
			}
			for ( i = (xs*ys)-1 ; i > 0 ; i-- )
			{
	#if 1
				if ( !justBGFlag[i] && justBGFlag[i-1] || (justBGFlag[i] && justBGFlag[i-1]) )
				{
					charColour[i-1] = charColour[i];
				}
	#endif
				if ( charColourFlag[i] && !charColourFlag[i-1] && ( charColour[i] & 0x08 ) == ( charColour[i-1] & 0x08 ) )
				{
					charColourFlag[i] = false;
					charColour[i] = charColour[i-1];
					colourOptimised = true;
				}
			}
		} while ( colourOptimised );
	#endif

		// Now process each character in the screen.
		std::map<C64UDG,unsigned char> chars;
		for (x=0;x<xs;x++)
		{
			for (y=0;y<ys;y++)
			{
				// Add the character to the list of chars used and automatically check for duplicates.
				chars.insert(std::pair<C64UDG,unsigned char>(charScreen[x+(y*xs)],0));
			}
		}

		// Display a report of how many chars are used.
		printf("Used %d UDGs\n",chars.size());

		// Check to see if we can save the data.
		if (!charsFilename)
		{
			printf("Not writing any output as no filenames given.\n");
			exit(0);
		}

		fp = fopen(charsFilename,"wb");
		if (!fp)
		{
			printf("Could not open file '%s' for writing.\n",charsFilename);
			exit(-1);
		}

		// Save each character.
		std::map<C64UDG,unsigned char>::iterator st2 = chars.begin() , en2 = chars.end();
		unsigned char index = 0;
		while (st2 != en2)
		{
			// Update the index for the character as it is saved. This is used to cross reference
			// the screen data save down below.
			(*st2).second = index++;
			// Write the data to the file and move to the next char entry.
			fwrite((*st2).first.mBytes,1,sUDGByteSize,fp);
			st2++;
		}
		fclose(fp);

		// Now match the unoptimised char screen bitmap data with the optimised chars list and
		// produce a screen.
		std::vector<unsigned char> indexedScreen;
		indexedScreen.reserve(xs*ys);
		for (y=0;y<ys;y++)
		{
			for (x=0;x<xs;x++)
			{
				C64UDG &theChar = charScreen[x+(y*xs)];
				// Quite a quick find operation since std::map is sorted according to the char data.
				std::map<C64UDG,unsigned char>::iterator found = chars.find(theChar);
				if (found != chars.end())
				{
					indexedScreen.push_back(found->second);
				}
				else
				{
					// If the char cannot be found then output 0xff as the car index.
					indexedScreen.push_back(0xff);
				}
			}
		}

		// Save the colours
		fp = fopen(coloursFilename,"wb");
		if (!fp)
		{
			printf("Could not open file '%s' for writing.\n",coloursFilename);
			exit(-1);
		}
		for (y=0;y<ys;y++)
		{
			for (x=0;x<xs;x++)
			{
				if(sDoSprites && charMulticolourFlag[x+(y*xs)])
				{
					fputc(128 | charColour[x+(y*xs)],fp);
				}
				else
				{
					fputc(charColour[x+(y*xs)],fp);
				}
			}
		}
		fclose(fp);

		// Save the screen
		fp = fopen(screenFilename,"wb");
		if (!fp)
		{
			printf("Could not open file '%s' for writing.\n",screenFilename);
			exit(-1);
		}
		for (y=0;y<ys;y++)
		{
			for (x=0;x<xs;x++)
			{
				fputc(((int)indexedScreen[x+(y*xs)])+sAddIdx,fp);
			}
		}

		if (addScreenInfo)
		{
			fputc(colourBG , fp);
			fputc(colourMC1 , fp);
			fputc(colourMC2 , fp);
		}

		fclose(fp);
	}

	return 0;
}
