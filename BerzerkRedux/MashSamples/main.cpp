#include <stdio.h>
#include <stdlib.h>

const int kWaveHeader = 0x2c;

int main(int argc,char **argv)
{
	FILE *fp = fopen(argv[1],"rb");
	fseek(fp,0,SEEK_END);
	int len = ftell(fp) - kWaveHeader;
	fseek(fp,0x2c,SEEK_SET);

	unsigned char *sample = (unsigned char *)malloc(len);
	fread(sample,len,1,fp);
	fclose(fp);

	// Input is at 22050Hz, so we scale down
	double downToRate = double(atoi(argv[2]));
	fp = fopen(argv[3],"wb");
	double pos;
	for (pos = 0.0f ; pos < double(len) ; pos += 22050.0f / downToRate)
	{
		unsigned char twoNybbles;
		unsigned char aNybble;
		twoNybbles = sample[int(pos)] >> 4;
		pos += 22050.0f / downToRate;
		aNybble = sample[int(pos)] >> 4;
		twoNybbles = twoNybbles | (aNybble << 4);
		fputc(twoNybbles,fp);
	}
	fclose(fp);

	return 0;
}
