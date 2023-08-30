//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>
#include <sb7.h>
#include "CubeModel.h"

#include "ModelFileHeader.h"
#include "File.h"

struct Vert_xyzuvn
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
};

struct Tri_index
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
};

void CubeModel::privCreateVAO(const char * const pModelFileName)
{
	// File stuff
	File::Handle fh;
	File::Error  ferror;

	// create a header
	ModelFileHeader modelHdr;

	ferror = File::Open(fh, pModelFileName, File::Mode::READ);
	assert(ferror == File::Error::SUCCESS);

	// Read the Hdr
	ferror = File::Read(fh, &modelHdr, sizeof(ModelFileHeader));
	assert(ferror == File::Error::SUCCESS);

	// Using the hdr, allocate the space for the buffers
	this->numVerts = modelHdr.numVerts;
	this->numTris = modelHdr.numTriList;

	// allocate buffers
	Vert_xyzuvn  *pCubeData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
	Tri_index *pTriList = new Tri_index[(unsigned int)modelHdr.numTriList];

	// Read verts
	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, pCubeData, sizeof(Vert_xyzuvn) * this->numVerts);
	assert(ferror == File::Error::SUCCESS);

	// Read trilist
	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, pTriList, sizeof(Tri_index) * this->numTris);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	assert(ferror == File::Error::SUCCESS);

	// future proofing it for a file
	assert(pModelFileName);
	UNUSED_VAR(pModelFileName);

	// future proofing it for a file
	assert(pModelFileName);
	UNUSED_VAR(pModelFileName);

	//this->numVerts = CUBE_DATA_NUM_VERTS;
	//this->numTris = TRILIST_NUM_TRIANGLES; 

	// Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	// Create a VBO
	glGenBuffers(1, &this->vbo[0]);
	assert(this->vbo[0] != 0);

	glGenBuffers(1, &this->vbo[1]);
	assert(this->vbo[1] != 0);

	// Load the combined data: ---------------------------------------------------------

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

	// load the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pCubeData, GL_STATIC_DRAW);

	// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
			void *offsetVert = (void *)((unsigned int)&pCubeData[0].x - (unsigned int)pCubeData);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
			glEnableVertexAttribArray(0);

			// Texture data is location: 1  (used in vertex shader)
			void *offsetTex = (void *)((unsigned int)&pCubeData[0].u - (unsigned int)pCubeData);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
			glEnableVertexAttribArray(1);

			// normals data in location 2 (used in vertex shader
			void *offsetNorm = (void *)((unsigned int)&pCubeData[0].nx - (unsigned int)pCubeData);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
			glEnableVertexAttribArray(2);

	// Load the index data: ---------------------------------------------------------


		/* Bind our 2nd VBO as being the active buffer and storing index ) */
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

			/* Copy the index data to our buffer */
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);

			delete[] pCubeData;
			delete[] pTriList;
}

CubeModel::CubeModel(const char * const pModelFileName)
	: Model()
{
	assert(pModelFileName);
	this->privCreateVAO(pModelFileName);
}

CubeModel::~CubeModel()
{
	// remove anything dynamic here
	
}


// --- End of File ----------
