//-----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef EAT_H
#define EAT_H

#include "ChunkHeader.h"
#include "PackageHeader.h"
bool eat(const char * const inName,
		 ChunkType type,
		 const char * const chunkName,
		 unsigned char *&chunkBuff,
		 unsigned int &chunkSize);

#endif

// End of File
