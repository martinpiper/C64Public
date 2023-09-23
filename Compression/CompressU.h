#include "common.h"

class CompressionU {
public:
	CompressionU();
	virtual ~CompressionU();

	int Compress( const u8*src, u32 src_len,u8 *dst, u32 *dst_len,int level);

private:
};
