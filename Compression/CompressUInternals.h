#define MIN_OFFSET 1
#define MAX_OFFSET 0xffff
#define COMPU_EOD 512


#define MAX_VARLEN 0xffff

#define BLOCK_SIZE 0x10000

// Anything above this, let the optimiser handle them
#define MAX_BLOCK_MATCH_LEN 0x2000

#define LCP_BITS 18
#define TAG_BITS 4
#define LCP_MAX ((1U<<(LCP_BITS - TAG_BITS)) - 1)

#define NINITIAL_ARRIVALS_PER_POSITION 140
#define NMAX_ARRIVALS_PER_POSITION 209
#define NMATCHES_PER_INDEX 150

#define LEAVE_ALONE_MATCH_SIZE 340


#define MIN_ENCODED_MATCH_SIZE   2
#define TOKEN_SIZE               1
#define OFFSET_COST(__offset)    (((__offset) <= 128) ? 8 : (7 + CompressionU_getPackedValueSize((((__offset) - 1) >> 7) + 1)))
