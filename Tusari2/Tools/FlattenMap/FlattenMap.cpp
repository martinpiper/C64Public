// FlattenMap.cpp : Converts the flip screen data format from the original Tusari into 
//
// c:\temp\map.bin c:\temp\tilesColours.bin "C:\CCS64\MyOldE114Software\TusariDevTusari\MAP.PRG"

#include <iostream>
#include <fstream>
#include "FlattenMap.h"

const int kMapWidth = 190;
const int kMapHeight = 64;

int main(int argc, char **argv)
{
	int currentScreenPositionX = 90;
	int currentScreenPositionY = 59;
	int entryScreenNum = 1;

	unsigned char theMapTiles[kMapWidth * kMapHeight];
	memset(theMapTiles, 0x00, sizeof(theMapTiles));
	unsigned char theTileColours[128];
	memset(theTileColours, 0x0f, sizeof(theTileColours));
	bool screenVisited[256];
	memset(screenVisited, 0, sizeof(screenVisited));

	std::ifstream input(argv[3], std::ios::binary);
	// Skip prg header and first screen
	input.get();
	input.get();


	processScreen(input, entryScreenNum, theMapTiles, currentScreenPositionX, currentScreenPositionY, theTileColours, screenVisited);

	std::ofstream out(argv[1], std::ios::binary);
	out.write((char*)theMapTiles, sizeof(theMapTiles));
	out.flush();
	out.close();

	std::ofstream outCols(argv[2], std::ios::binary);
	outCols.write((char*)theTileColours, sizeof(theTileColours));
	outCols.flush();
	outCols.close();
}

void processScreen(std::ifstream &input, int entryScreenNum, unsigned char *theMapTiles, int currentScreenPositionX, int currentScreenPositionY, unsigned char *theTileColours, bool *screenVisited)
{
	std::cout << "Screen " << entryScreenNum << " at " << currentScreenPositionX << " , " << currentScreenPositionY << std::endl;
	if (currentScreenPositionX < 0 || currentScreenPositionY < 0 || currentScreenPositionX >(kMapWidth - 10) || currentScreenPositionY >(kMapHeight - 5))
	{
		std::cout << "****Out of range" << std::endl;
		return;
	}

	screenVisited[entryScreenNum] = true;
	input.seekg(2 + (104 * entryScreenNum), std::ifstream::beg);

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			int col = input.get() & 0x0f;
			int tile = input.get() & 0x7f;

			theMapTiles[(currentScreenPositionX + x) + ((currentScreenPositionY + y) * kMapWidth)] = tile;
			// Preserve the last known colour
			theTileColours[tile] = col;
		}
	}

	// Process the exits
	int exitScreenUp = input.get();
	int exitScreenDown = input.get();
	int exitScreenLeft = input.get();
	int exitScreenRight = input.get();

	if (exitScreenLeft && !screenVisited[exitScreenLeft])
	{
		processScreen(input, exitScreenLeft, theMapTiles, currentScreenPositionX - 10, currentScreenPositionY, theTileColours, screenVisited);
	}
	if (exitScreenRight && !screenVisited[exitScreenRight])
	{
		processScreen(input, exitScreenRight, theMapTiles, currentScreenPositionX + 10, currentScreenPositionY, theTileColours, screenVisited);
	}
	if (exitScreenUp && !screenVisited[exitScreenUp])
	{
		processScreen(input, exitScreenUp, theMapTiles, currentScreenPositionX, currentScreenPositionY - 4, theTileColours, screenVisited);
	}
	if (exitScreenDown && !screenVisited[exitScreenDown])
	{
		processScreen(input, exitScreenDown, theMapTiles, currentScreenPositionX, currentScreenPositionY + 4, theTileColours, screenVisited);
	}
}
