// This runs many iterations of the compression options with different "compressible random" data and validates the data can be decompressed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <random>
#include <list>
#include "RNPlatform/Inc/MessageHelper.h"
#include "RNPlatform/Inc/ThreadClass.h"
#include "RNPlatform/Inc/Thread.h"
#include "RNPlatform/Inc/JobManager.h"

using namespace RNReplicaNet;
enum {
	kTypeLZMPi = 0,
	kTypeLZMPiE,
	kTypeLZMPiRLE,
	kTypeLZMPiU,
	kTypeEnd
};

static int getInt8(std::default_random_engine &dataGen)
{
	int ret = dataGen();
	ret = ret & 0xff;
	return ret;
}

static int runIterationWith(int thread, int type , int seed)
{
//	printf("Thread %d : Type %d Seed %d : Starting\n" , thread , type , seed);

	std::default_random_engine randGen;
	randGen.seed(seed + 1);

	std::string commandLine;
#ifdef _DEBUG
	commandLine = "..\\Debug\\";
#else
	commandLine = "..\\Release\\";
#endif
	commandLine += "Compression.exe ";

	std::string commandLineOptions = "";
	switch(type % kTypeEnd)
	{
	default:
	case kTypeLZMPi:
		commandLineOptions += "-c ";
		break;
	case kTypeLZMPiE:
		commandLineOptions += "-ce ";
		break;
	case kTypeLZMPiRLE:
		commandLineOptions += "-cr ";
		break;
	case kTypeLZMPiU:
		commandLineOptions += "-cu ";
		break;
	}
	// Produce compressible data
	DynamicMessageHelper testData;
	int dataAddIterations = (randGen() % 256) + 8;
	int dataNumPatterns = (randGen() % 8) + 2;
	while (dataAddIterations-- > 0)
	{
		// Calculate the length and discard from the previous generator
		int patternLength = 4 + (randGen() % 256);
		int patternDiscard = (randGen() % 8);	// To cause some initial pattern bytes to be discard...

		// Cap to a sensible maximum
		if ( (testData.GetSize() + patternLength) >= 65530)
		{
			break;
		}

		// Produces runs of patterns based on seeded random generator
		// Which generates the same repeating patterns randomly in the data
		std::default_random_engine dataGen;
		dataGen.seed((randGen() % dataNumPatterns) + 1);

		int minValue = getInt8(dataGen);
		int maxValue = getInt8(dataGen) + 1;
		if (minValue > maxValue)
		{
			int t = minValue;
			minValue = maxValue;
			maxValue = t;
		}
		int variance = maxValue - minValue;
		// Make RLE patterns slightly more common
		if ( (getInt8(dataGen) % 10) == 0)
		{
			variance = 1;
		}
		dataGen.discard(patternDiscard);
		while (patternLength-- > 0)
		{
			int data = getInt8(dataGen) % variance;
			data += minValue;

			testData << (unsigned char) data;
		}
	}

	std::string filenameOriginal = "Temp\\data" + std::to_string(thread) + "original.bin";
	// Completely raw data, no headers, nothing
	FILE *fp = fopen(filenameOriginal.c_str() , "wb");
	fwrite(testData.GetBuffer() , 1 , testData.GetSize() , fp);
	fclose(fp);

	std::string filenameCompressed = "Temp\\data" + std::to_string(thread) + "compressed.bin";
	commandLineOptions += filenameOriginal + " " + filenameCompressed;

	std::string commandLineLast = " >Temp\\data" + std::to_string(thread) + "stdout.bin";

	int ret = system((commandLine + commandLineOptions + commandLineLast).c_str());
	if (ret != 0)
	{
		printf("Thread %d : Type %d Seed %d : Compression problem\n" , thread , type , seed);
		return ret;
	}

	commandLineOptions = "";
	switch(type % kTypeEnd)
	{
	default:
	case kTypeLZMPi:
		commandLineOptions += "-d ";
		break;
	case kTypeLZMPiE:
		commandLineOptions += "-de ";
		break;
	case kTypeLZMPiRLE:
//		commandLineOptions += "-dr ";
		// Still don't have a RLE decompress?!
		return 0;
		break;
	case kTypeLZMPiU:
		commandLineOptions += "-du ";
		break;
	}

	std::string filenameDecompressed = "Temp\\data" + std::to_string(thread) + "decompressed.bin";
	commandLineOptions += filenameCompressed + " " + filenameDecompressed;
	ret = system((commandLine + commandLineOptions + commandLineLast).c_str());
	if (ret != 0)
	{
		printf("Thread %d : Type %d Seed %d : Decompression problem\n" , thread , type , seed);
		return ret;
	}

	DynamicMessageHelper decompressedData;
	bool bret = decompressedData.Read(filenameDecompressed.c_str() , true);
	if (!bret)
	{
		printf("Thread %d : Type %d Seed %d : Decompressed data read error\n" , thread , type , seed);
		return -1;
	}
	decompressedData.SetSize(decompressedData.GetLastSizeAdded());

	if (testData != decompressedData)
	{
		printf("Thread %d : Type %d Seed %d : Comparison failed\n" , thread , type , seed);
		return -1;
	}
	

	return 0;
}

class JobCompressionTest : public Job
{
	int mIteration;
public:
	JobCompressionTest(int iteration) : mIteration(iteration)
	{
	}

	virtual ~JobCompressionTest()
	{
	}
private:
	virtual void Run(ThreadClass *thread)
	{
		for (int i = 0 ; i < kTypeEnd ; i++)
		{
			int ret = runIterationWith(thread->GetThreadID() , i , mIteration);
			if (ret != 0)
			{
				exit(-1);
			}
		}
	}
};

int main(int argc , char ** argv)
{
	int numIterations = 10000;
	int addSeed = 0;

	if (argc >= 4)
	{
		if (_stricmp(argv[1] , "-s") == 0)
		{
			int theType = atoi(argv[2]);
			int theSeed = atoi(argv[3]);

			return runIterationWith(-1 , theType , theSeed);
		}
		if (_stricmp(argv[1] , "-i") == 0)
		{
			numIterations = atoi(argv[2]);
			addSeed = atoi(argv[2]);
		}
	}

	printf("Running %d iterations with add %d\n" , numIterations , addSeed);
	JobManager jobs;
	jobs.Start();

	int toRun = numIterations;
	while (toRun-- > 0 )
	{
		jobs.AddPoolJob(new JobCompressionTest(toRun + addSeed));
	}

	while (jobs.GetNumberOfJobsRemaining() > 0)
	{
		printf("Remaining %d\n" , jobs.GetNumberOfJobsRemaining());
		CurrentThread::Sleep(1000);
	}
}
