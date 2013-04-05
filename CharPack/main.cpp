//TestScreen.bmp 4 9 12 chars.bin screen.bin colours.bin
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <set>
#include <math.h>

// The following library is free for non-commercial use.
// Available from http://www.replicanet.com/contact.html
#include "RNPlatform/Inc/MessageHelper.h"

using namespace RNReplicaNet;

// The final character bytes
struct C64Char
{
	C64Char() {}
	unsigned char mBytes[8];
};

// Some handy operators for comparing the character data
inline bool operator== (const C64Char& a,const C64Char& b)
{
	return (memcmp(a.mBytes,b.mBytes,sizeof(a.mBytes)) == 0);
}

inline bool operator!= (const C64Char& a,const C64Char& b)
{
	return !(a == b);
}

// Can be used with STL less<> tests. In this code the std::map and std::set both use this
// to optimise the insert and find operations.
inline bool operator< (const C64Char& a,const C64Char& b)
{
	return (memcmp(a.mBytes,b.mBytes,sizeof(a.mBytes)) < 0);
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

	// Parse the command line options.
	if (argc < 2)
	{
		showHelp = true;
	}
	else
	{
		while ((argv[1][0] == '-') || (argv[1][0] == '/'))
		{
			if (strcmp( argv[1] , "-c64remap" ) == 0 )
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
			argc--;
			argv++;
		}
	}
	
	// MPi: TODO: Add extra parameters to remap input C64 colours to other colours before conversion
	// -c64remap 9 11 : Would remap colour 9 to 11
	// -c64remap 9 11 -c64remap 15 12 : Would remap colour 9 to 11 then 15 to 12

	// MPi: TODO: Add the option to set the input C64Palette

	if (showHelp)
	{
		printf("Usage: CharPack [-h] [-r] [-m] <-c64remap a b> <input 24bit BMP> [bg colour] [colour 1] [colour 2] [chars file] [screen file] [colours file]\n");
		printf("Usage for hires font conversion: CharPack -f <input 24bit BMP> [bg colour] [font data file]\n");
		printf("Examples:\nTo show what colours are used: CharPack TestScreen3.bmp\n");
		printf("Note colours 0 (black), 10 (light red) and 1 (white) are often used so pick those for the three colours.\n");
		printf("To compress characters with the colours: CharPack TestScreen3.bmp 0 10 1 chars.bin screen.bin colours.bin\n");
		printf("-h/-? Help.\n");
		printf("-r Force hires character conversion.\n");
		printf("-m Force multicolour character conversion.\n");
		printf("-c64remap a b : Remaps a C64 colours from a to b. -c64remap 9 11 -c64remap 15 12 : Would remap colour 9 to 11 then 15 to 12\n");
		
		exit(0);
	}

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

	if ((wid & 7) || (hei & 7))
	{
		printf("File must be a multiple size of 8 pixels horizontally and vertically.\n");
		exit(-1);
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

	// Process the RGB screen producing an optimised palette lookup screen
	colourMap.clear();
	std::vector<unsigned char> newScreen;
	newScreen.reserve(wid*hei);
	for (y=0;y<hei;y++)
	{
		file.SetSize(bitmapOffset + (((hei-1)-y)*roundedScan));
		unsigned int RGB = 0;

		for (x=0;x<wid;x++)
		{
			// Get three bytes RGB, convert it to a C64 palette entry and and store it.
			file.GetVariable(&RGB,3);
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
	
	// Now process the char data from the palette index screen.
	unsigned int xs = wid/8;
	unsigned int ys = hei/8;

	// For each char position we store a bitmap char.
	std::vector<C64Char> charScreen;
	charScreen.reserve(xs*ys);
	// And store the char colour
	std::vector<unsigned char> charColour;
	charColour.reserve(xs*ys);
	std::vector<unsigned char> charColourFlag;
	charColourFlag.reserve(xs*ys);

	// Now process each character in the optimised screen
	for (y=0;y<hei;y+=8)
	{
		for (x=0;x<wid;x+=8)
		{
			// Find what C64 indexed colours are used in the character.
			std::set<unsigned char> usedColours;
			unsigned int xi,yi;
			for (yi=0;yi<8;yi++)
			{
				for (xi=0;xi<8;xi++)
				{
					unsigned char index = newScreen[((y+yi)*wid)+x+xi];
					usedColours.insert(index);
				}
			}
			C64Char theChar;
			unsigned char theColour = 0;
			bool theColourCoalesce = false;
			bool added = false;
			// Is it one colour that uses one of the first eight or background colours?
			// If so then create a character block and use that colour information.
			if (usedColours.size() == 1)
			{
				if (*usedColours.begin() == colourBG)
				{
					// A transparent character
					memset(theChar.mBytes,0,sizeof(theChar.mBytes));
					theColour = 0;
					added = true;
					theColourCoalesce = true;
				}
				else if (*usedColours.begin() < 8)
				{
					// A filled character with the colour
					memset(theChar.mBytes,0xff,sizeof(theChar.mBytes));
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
					// Check to see if it uses the background and one of the first eight colours.
					if (
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
					for (yi=0;yi<8;yi++)
					{
						theChar.mBytes[yi] = 0;
						for (xi=0;xi<8;xi++)
						{
							unsigned char index = newScreen[((y+yi)*wid)+x+xi];
							if (index != colourBG)
							{
								// Pixels are stored backwards.
								theChar.mBytes[yi] |= 1 << (7-xi);
								// And update the colour used.
								theColour = index;
							}
						}
					}
				}
				else
				{
					theColourCoalesce = true;
					// Choose a default colour to enable multicolour mode for this character.
					// If the char just uses background or multicolour 1 or 2 then this stays the same.
					// However if the char uses one of the other colours then this is updated with that value.
					theColour = 8;
					// Else process the multicolour character.
					for (yi=0;yi<8;yi++)
					{
						theChar.mBytes[yi] = 0;
						// Multicolour mode is half horizontal resolution.
						for (xi=0;xi<8;xi+=2)
						{
							unsigned char index = newScreen[((y+yi)*wid)+x+xi];
							if (index == colourMC1)
							{
								// Bit pattern for MC1
								theChar.mBytes[yi] |= 1 << (6-xi);
							}
							else if (index == colourMC2)
							{
								// Bit pattern for MC2
								theChar.mBytes[yi] |= 2 << (6-xi);
							}
							else if (index != colourBG)
							{
								theColourCoalesce = false;
								// Bit pattern for colour
								theChar.mBytes[yi] |= 3 << (6-xi);
								// Multicolour so OR 8
								// MPi: TODO: Maybe find the most commonly used colour for this char and use that instead.
								theColour = index | 8;
							}
						}
					}
				}
			}

			// Store the char data and colour.
			charScreen.push_back(theChar);
			charColour.push_back(theColour);
			charColourFlag.push_back(theColourCoalesce);
		}
	}

	// Now optimise the colour map so that colours that can be coalesced are coalesced, this helps compression.
	// Must preserve multicolour boundaries
	bool colourOptimised = false;
	do
	{
		colourOptimised = false;
		for ( i = 0 ; i < (xs*ys)-1 ; i++ )
		{
			if ( charColourFlag[i] && !charColourFlag[i+1] && ( ( charColour[i] & 0x08 ) == ( charColour[i+1] & 0x08 ) ) )
			{
				charColourFlag[i] = false;
				charColour[i] = charColour[i+1];
				colourOptimised = true;
			}
		}
		for ( i = (xs*ys)-1 ; i > 0 ; i-- )
		{
			if ( charColourFlag[i] && !charColourFlag[i-1] && ( ( charColour[i] & 0x08 ) == ( charColour[i-1] & 0x08 ) ) )
			{
				charColourFlag[i] = false;
				charColour[i] = charColour[i-1];
				colourOptimised = true;
			}
		}
	} while ( colourOptimised );


	// Now process each character in the screen.
	std::map<C64Char,unsigned char> chars;
	for (x=0;x<xs;x++)
	{
		for (y=0;y<ys;y++)
		{
			// Add the character to the list of chars used and automatically check for duplicates.
			chars.insert(std::pair<C64Char,unsigned char>(charScreen[x+(y*xs)],0));
		}
	}

	// Display a report of how many chars are used.
	printf("Used %d chars\n",chars.size());

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
	std::map<C64Char,unsigned char>::iterator st2 = chars.begin() , en2 = chars.end();
	unsigned char index = 0;
	while (st2 != en2)
	{
		// Update the index for the character as it is saved. This is used to cross reference
		// the screen data save down below.
		(*st2).second = index++;
		// Write the data to the file and move to the next char entry.
		fwrite((*st2).first.mBytes,1,8,fp);
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
			C64Char &theChar = charScreen[x+(y*xs)];
			// Quite a quick find operation since std::map is sorted according to the char data.
			std::map<C64Char,unsigned char>::iterator found = chars.find(theChar);
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
			fputc(charColour[x+(y*xs)],fp);
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
			fputc(indexedScreen[x+(y*xs)],fp);
		}
	}
	fclose(fp);

	return 0;
}
