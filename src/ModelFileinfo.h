//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef MODEL_FILE_INFO_H
#define MODEL_FILE_INFO_H

#include <string.h>

#define OBJECT_NAME_SIZE   32
#define OBJECT_NAME_SIZE   32
#define TEXTURE_NAME_SIZE  32

struct ModelFileInfo
{
	// object name
	char	objName[OBJECT_NAME_SIZE];

	// vertex buffer
	int		numVerts;
	DWORD	vertBufferOffset;

	// trilist index 
	int		numTriList;
	DWORD	triListBufferOffset;

	// texture info
	// char textName[TEXTURE_NAME_SIZE];
	// unsigned int minFilter;
	// unsigned int magFilter;
	// unsigned int horizWrapMode;
	// unsigned int vertWrapMode;

	ModelFileInfo()
	{
		memset(this, 0x0, sizeof(ModelFileInfo));
	}
};
#endif