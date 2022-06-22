#include "FileOps.h"

Block readBytes(FILE *fp , Block::size_type numBytes)
{
	Block toRet;
	toRet.resize(numBytes);
	size_t ret = fread(toRet.data() , sizeof(unsigned char) , numBytes , fp);
	if (ret != numBytes)
	{
		printf("Could not read expected %d bytes at file pos $%x, is the file corrupt?\n" , (int)numBytes , (int)ftell(fp));
		exit(-1);
	}
	return toRet;
}

void skipBytes(FILE *fp , Block::size_type numBytes)
{
	fseek(fp , numBytes , SEEK_CUR);
}
