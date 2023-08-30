//-----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include <stdio.h>
#include <assert.h>

#include "eat.h"
#include "ChunkHeader.h"
#include "File.h"
#define UNUSED_VAR(x) ((void *)x)
//---------------------------------------------------------------------------
// Leave this signature - do your EAT magic here
// Feel free to add files and methods to this project
//---------------------------------------------------------------------------


void SetNameN_Version(PackageHeader& pkgHdr, char* argv_3, char* argv_5)
{
	if (argv_3 == 0 || argv_5 == 0)
	{
		return;
	}

	size_t argv_3_Len = strlen(argv_3);
	size_t argv_5_Len = strlen(argv_5);

	for (unsigned int i = 0; i <= argv_3_Len; i++)
	{
		pkgHdr.packageName[i] = argv_3[i];
	}

	for (unsigned int i = 0; i <= argv_5_Len; i++)
	{
		pkgHdr.versionString[i] = argv_5[i];
	}
}

File::Error SetPackageHeader(File::Handle inFileHandel, PackageHeader  &pkgHdr)
{
	File::Error  ferror;

	PackageHeader* tmpPkgHdr;

	//Seek to the begin
	ferror = File::Seek(inFileHandel, File::Location::BEGIN, 0);
	assert(ferror == File::Error::SUCCESS);

	//make a buffer to read PackageHeader stuff
	unsigned char* packageHeaderData = new unsigned char[sizeof(PackageHeader)];

	//Read the file data into buffer above^^
	ferror = File::Read(inFileHandel, &packageHeaderData[0], sizeof(PackageHeader));
	assert(ferror == File::Error::SUCCESS);

	//get the pkghdr info
	tmpPkgHdr = reinterpret_cast<PackageHeader*>(&packageHeaderData[0]);

	pkgHdr.numChunks = tmpPkgHdr->numChunks;
	pkgHdr.totalSize = tmpPkgHdr->totalSize;
	SetNameN_Version(pkgHdr, tmpPkgHdr->packageName, tmpPkgHdr->versionString);

	//delete PackageHeaderData buffer
	delete[] packageHeaderData;

	return ferror;
}

File::Error SetChunkHeader(File::Handle inFileHandel, ChunkHeader& chnkHdr)
{
	File::Error  ferror;

	ChunkHeader* tmpchnkHdr;

	//make a buffer to read ChunkHeader stuff
	unsigned char* chunkHeaderData = new unsigned char[sizeof(ChunkHeader)];

	//Seek to sizeof(PackageHeader) + sizeof(ChunkHeader)
	ferror = File::Seek(inFileHandel, File::Location::BEGIN, sizeof(PackageHeader));
	assert(ferror == File::Error::SUCCESS);

	//read the chunk head of the first incident chunk
	ferror = File::Read(inFileHandel, &chunkHeaderData[0], sizeof(ChunkHeader));
	assert(ferror == File::Error::SUCCESS);

	///assign chnkHdr its values 
	tmpchnkHdr = reinterpret_cast<ChunkHeader*>(&chunkHeaderData[0]);
	//name
	size_t name_len = strnlen_s(tmpchnkHdr->chunkName, 30);
	strncpy_s(chnkHdr.chunkName, name_len + 1, tmpchnkHdr->chunkName, _TRUNCATE);
	//size
	chnkHdr.chunkSize = tmpchnkHdr->chunkSize;
	//type
	chnkHdr.type = tmpchnkHdr->type;
	//hashNum
	chnkHdr.hashNum = tmpchnkHdr->hashNum;

	//delete PackageHeaderData buffer
	delete[] chunkHeaderData;

	return ferror;
}

File::Error IsFirstChunk(File::Handle inFileHandel, unsigned int chunkSize, ChunkHeader* tmpchnkHdr,
	unsigned char*& chunkBuff)
{
	File::Error  ferror;

	//make a buffer to read File stuff
	unsigned char* fileData = new unsigned char[chunkSize];

	//Seek to sizeof(PackageHeader) + sizeof(ChunkHeader)
	ferror = File::Seek(inFileHandel, File::Location::BEGIN, sizeof(PackageHeader) + sizeof(ChunkHeader));
	assert(ferror == File::Error::SUCCESS);

	//read the data of the first file in 
	ferror = File::Read(inFileHandel, &fileData[0], tmpchnkHdr->chunkSize);
	assert(ferror == File::Error::SUCCESS);

	chunkBuff = &fileData[0];

	return ferror;
}

bool eat(const char * const inFileName,
		 ChunkType type,
		 const char * const chunkName,
		 unsigned char *&chunkBuff,
		 unsigned int &chunkSize)
{
	UNUSED_VAR(inFileName);
	UNUSED_VAR(type);
	UNUSED_VAR(chunkName);
	UNUSED_VAR(chunkBuff);
	UNUSED_VAR(chunkSize);

	/////////////////data//////////////////
	File::Handle inFileHandel = 0;
	File::Error  ferror;
	PackageHeader  pkgHdr;
	ChunkHeader    chnkHdr;
	ChunkHeader*   tmpChnkHdr;
	//////////////////////////////////////

	//open inFileName
	ferror = File::Open(inFileHandel, inFileName, File::Mode::READ);
	assert(ferror == File::Error::SUCCESS);

	//Get PackageHeader Data
	ferror = SetPackageHeader(inFileHandel, pkgHdr);
	assert(ferror == File::Error::SUCCESS);
	
	//Get First ChunkHeaderData
	ferror = SetChunkHeader(inFileHandel, chnkHdr);
	assert(ferror == File::Error::SUCCESS);
	
	//Return size
	chunkSize = chnkHdr.chunkSize;

	tmpChnkHdr = &chnkHdr;

	//if the cunk is the first in the .spu
	if (strcmp(tmpChnkHdr->chunkName, chunkName) == 0 && tmpChnkHdr->type == type)
	{
		ferror = IsFirstChunk(inFileHandel, chunkSize, tmpChnkHdr, chunkBuff);
		assert(ferror == File::Error::SUCCESS);
	}

	else if(strcmp(tmpChnkHdr->chunkName, chunkName) != 0 || tmpChnkHdr->type != type)
	{

		//make a buffer to read File stuff
		unsigned char* seekChnkHdr = new unsigned char[sizeof(ChunkHeader)];

		while (strcmp(tmpChnkHdr->chunkName, chunkName) != 0 || tmpChnkHdr->type != type)
		{

			//Seek to Next chunk
			ferror = File::Seek(inFileHandel, File::Location::CURRENT, (int)tmpChnkHdr->chunkSize);
			assert(ferror == File::Error::SUCCESS);

			//read the data of the first file in 
			ferror = File::Read(inFileHandel, &seekChnkHdr[0], sizeof(ChunkHeader));
			assert(ferror == File::Error::SUCCESS);

			///assign chnkHdr its values 
			tmpChnkHdr = reinterpret_cast<ChunkHeader*>(&seekChnkHdr[0]);

			//jump over chunk header and start of chunk data
			//ferror = File::Seek(inFileHandel, File::Location::CURRENT, sizeof(ChunkHeader));

			chunkSize = tmpChnkHdr->chunkSize;
		}
		//make a buffer to read File stuff
		unsigned char* fileData = new unsigned char[chunkSize];

		//read the data of the first file in 
		ferror = File::Read(inFileHandel, &fileData[0], tmpChnkHdr->chunkSize);
		assert(ferror == File::Error::SUCCESS);

		chunkBuff = &fileData[0];

		delete[] seekChnkHdr;
	}

	// close file handles
	ferror = File::Close(inFileHandel);
	assert(ferror == File::Error::SUCCESS);

	return true;
}

// End of File
