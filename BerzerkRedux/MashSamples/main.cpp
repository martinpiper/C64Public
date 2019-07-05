// -h <header offset>
// Output single values: -v
// -s <source rate in Hz>
// -r <multiplier ratio>
// -l <max run length>
// -l 1 -r 2.0f -h $4e -v -s 22050 ..\..\AnimationBitmap\Animation11\t.wav 8000 ..\..\AnimationBitmap\Data\smp.raw
// Before: 1,300,479 bytes
// -r 2.0f -h $4e -v -s 22050 ..\..\AnimationBitmap\Animation11\t.wav 8000 ..\..\AnimationBitmap\Data\smp.raw
// After: 565,026 bytes
// At 5KHz
// -r 2.0f -h $4e -v -s 22050 ..\..\AnimationBitmap\Animation11\t.wav 5000 ..\..\AnimationBitmap\Data\smp.raw
// After: 372,859 bytes

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../Common/ParamToNum.h"

static int kWaveHeader = 0x2c;
static double sourceRate = 22050.0f;
static double ratio = 1.0f;

static int sampleMin = 128;
static int sampleMax = 128;

static int sMaxRunLength = 16;
static int sRLECount = 0;
static int sLastValue = -1;
static void fputcRLE(int value , FILE *fp)
{
	// Writes the last value if there is a change or if the count is at maximum
	if (value != sLastValue || sRLECount >= sMaxRunLength)
	{
		if (sLastValue != -1)
		{
			fputc((((sRLECount-1) & 0xf)<<4) | (sLastValue & 0xf), fp);
		}
		sRLECount = 0;
	}

	sLastValue = value;
	sRLECount++;
}

static unsigned int safeScaleClamp(const int input)
{
	double value = input - 0x80;
	value = value * ratio;
	int ivalue = (int)value + 0x80;
	if (ivalue < sampleMin)
	{
		sampleMin = ivalue;
	}
	if (ivalue > sampleMax)
	{
		sampleMax = ivalue;
	}
	if (ivalue < 0)
	{
		ivalue = 0;
	}
	if (ivalue > 255)
	{
		ivalue = 255;
	}
	return (unsigned int) ivalue;
}

int main(int argc,char **argv)
{
	bool singleValueMode = false;
	double fps = 25.0f;
	int startFrame = 0;
	int endFrame = -1;

	while (argv[1][0] == '-')
	{
		if (argv[1][1] == 'h')
		{
			kWaveHeader = ParamToNum(argv[2]);
			argc--;
			argv++;
		}
		else if (argv[1][1] == 'f' && argv[1][2] == 'p' && argv[1][3] == 's')
		{
			fps = atof(argv[2]);
			argc--;
			argv++;
		}
		else if (argv[1][1] == 's' && argv[1][2] == 'f')
		{
			startFrame = ParamToNum(argv[2]);
			argc--;
			argv++;
		}
		else if (argv[1][1] == 'e' && argv[1][2] == 'f')
		{
			endFrame = ParamToNum(argv[2]);
			argc--;
			argv++;
		}
		else if (argv[1][1] == 'v')
		{
			singleValueMode = true;
		}
		else if (argv[1][1] == 'l')
		{
			sMaxRunLength = ParamToNum(argv[2]);
			argc--;
			argv++;
		}
		else if (argv[1][1] == 's')
		{
			sourceRate = (double) ParamToNum(argv[2]);
			argc--;
			argv++;
		}
		else if (argv[1][1] == 'r')
		{
			ratio = atof(argv[2]);
			argc--;
			argv++;
		}
		argc--;
		argv++;
	}

	int samplesToGo = INT_MAX;

	if (startFrame >= 0)
	{
		int skipSamples = (int) ((double(startFrame) * double(sourceRate)) / fps);
		kWaveHeader += skipSamples;

		if (endFrame > startFrame)
		{
			samplesToGo = (int) ((double(endFrame-startFrame) * double(sourceRate)) / fps);
		}
	}

	FILE *in = fopen(argv[1],"rb");
	fseek(in,0,SEEK_END);
	int len = ftell(in) - kWaveHeader;
	fseek(in,kWaveHeader,SEEK_SET);

	// Input is at a higher rate, so we scale down
	double downToRate = (double) ParamToNum(argv[2]);
	FILE *fp = fopen(argv[3],"wb");
	if (singleValueMode)
	{
		unsigned int sample = 0;
		unsigned int samples = 0;

		double pos = sourceRate / downToRate;
		unsigned int readSamples = 0;
		while (!feof(in) && samplesToGo >= 0)
		{
			int samp = fgetc(in);
			samplesToGo--;
			if (feof(in))
			{
				break;
			}
			sample += safeScaleClamp(samp);
			samples++;
			readSamples++;

			if (readSamples >= (unsigned int) pos)
			{
				pos += sourceRate / downToRate;

				sample = (sample/samples) >> 4;
				fputcRLE(sample, fp);

				sample = 0;
				samples = 0;
			}
		}
		fputcRLE(-1, fp);	// Flush
	}
	else
	{
		unsigned char *sample = (unsigned char *)malloc(len);
		fread(sample,len,1,in);
		fclose(in);

		double pos;
		for (pos = 0.0f ; pos < double(len) ; pos += sourceRate / downToRate)
		{
			unsigned char twoNybbles;
			unsigned char aNybble;
			twoNybbles = (unsigned char) safeScaleClamp(sample[int(pos)]) >> 4;
			pos += sourceRate / downToRate;
			aNybble = (unsigned char) safeScaleClamp(sample[int(pos)]) >> 4;
			twoNybbles = twoNybbles | (aNybble << 4);
			fputc(twoNybbles,fp);
		}
	}
	fclose(fp);

	printf("min/max %d/%d\n" , sampleMin , sampleMax);

	return 0;
}
