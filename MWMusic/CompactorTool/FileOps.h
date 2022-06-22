#pragma once
#include <stdio.h>
#include "Block.h"

extern Block readBytes(FILE *fp , Block::size_type numBytes);

extern void skipBytes(FILE *fp , Block::size_type numBytes);
