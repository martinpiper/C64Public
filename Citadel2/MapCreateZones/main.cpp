#include <stdio.h>
#include <stdlib.h>
#include "../../Common/ParamToNum.h"
#include "RNPlatform/Inc/MessageHelper.h"

using namespace RNReplicaNet;

static void recursiveGrow(unsigned char *mapData , unsigned char mapWidth , unsigned char mapHeight , int x , int y , unsigned char currentZone)
{
	int index;
	if (x > 0)
	{
		index = ((int)y * (int)mapWidth) + (int)(x-1);
		if (mapData[index] == ' ')
		{
			mapData[index] = currentZone;
			recursiveGrow(mapData , mapWidth , mapHeight , x-1 , y , currentZone);
		}
	}
	if (x < mapWidth-1)
	{
		index = ((int)y * (int)mapWidth) + (int)(x+1);
		if (mapData[index] == ' ')
		{
			mapData[index] = currentZone;
			recursiveGrow(mapData , mapWidth , mapHeight , x+1 , y , currentZone);
		}
	}
	if (y > 0)
	{
		index = ((int)(y-1) * (int)mapWidth) + (int)x;
		if (mapData[index] == ' ')
		{
			mapData[index] = currentZone;
			recursiveGrow(mapData , mapWidth , mapHeight , x , y-1 , currentZone);
		}
	}
	if (y < mapHeight-1)
	{
		index = ((int)(y+1) * (int)mapWidth) + (int)x;
		if (mapData[index] == ' ')
		{
			mapData[index] = currentZone;
			recursiveGrow(mapData , mapWidth , mapHeight , x , y+1 , currentZone);
		}
	}
}

int main(int argc , char **argv)
{
	char *inputFilename = argv[1];
	int mapOffset = ParamToNum(argv[2]);
	unsigned char solidBlock = ParamToNum(argv[3]);
	char *outputFilename = argv[4];

	DynamicMessageHelper inputData;
	if (!inputData.Read(inputFilename , true))
	{
		printf("Could not open '%s'\n" , inputFilename);
		exit(-1);
	}
	unsigned short inputOffset;
	inputData >> inputOffset;

	unsigned char *mapData = (unsigned char *)inputData.GetCurrentPosition();
	mapData += mapOffset-inputOffset;
	unsigned char mapWidth = mapData[0];
	unsigned char mapHeight = mapData[1];
	mapData += 2;

	// First parse the game block map into simple walls 0xff and floors ' '
	for (int y = 0 ; y < (int)mapHeight ; y++)
	{
		for (int x = 0 ; x < (int)mapWidth ; x++)
		{
			int index = ((int)y * (int)mapWidth) + (int)x;
			if (mapData[index] >= solidBlock)
			{
				mapData[index] = 0xff;
			}
			else
			{
				mapData[index] = ' ';
			}
		}
	}

	unsigned char currentZone = 0;
	bool anyFound = false;
	// Now create zones in spaces
	for (int y = 0 ; y < (int)mapHeight ; y++)
	{
		for (int x = 0 ; x < (int)mapWidth ; x++)
		{
			int index = ((int)y * (int)mapWidth) + (int)x;
			if (mapData[index] == ' ')
			{
				mapData[index] = currentZone;
				recursiveGrow(mapData , mapWidth , mapHeight , x , y , currentZone);
				currentZone++;
			}
		}
	}

	printf("num zones = %d\n" , currentZone);

	FILE *fp = fopen(outputFilename , "wb");
	fwrite(mapData , 1 , (int)mapWidth * (int)mapHeight , fp);
	fclose(fp);

	return 0;
}
