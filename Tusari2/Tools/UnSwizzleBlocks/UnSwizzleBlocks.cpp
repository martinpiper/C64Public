// UnSwizzleBlocks.cpp : Converts original Tusari block format data to a binary file suitable to be imported into other map editing tools
//
// c:\temp\blks.bin "C:\CCS64\MyOldE114Software\TusariDevTusari\BLKS.PRG" "C:\CCS64\MyOldE114Software\TusariDevTusari\BLKS2.PRG"

#include <iostream>
#include <fstream>

void convertFile(std::ofstream &out, std::ifstream &blks)
{
	for (int i = 0; i < 64; i++)
	{
		for (int y = 0; y < 4; y++)
		{
			// +2 to skip the prg header
			blks.seekg(2 + (y * 0x100) + (i * 4), std::ifstream::beg);
			for (int x = 0; x < 4; x++)
			{
				out.put(blks.get());
			}
		}
	}
}

int main(int argc, char **argv)
{
	std::ofstream out(argv[1], std::ios::binary);

	std::ifstream blks(argv[2], std::ios::binary);
	convertFile(out, blks);

	std::ifstream blks2(argv[3], std::ios::binary);
	convertFile(out, blks2);

	out.flush();
	out.close();
}
