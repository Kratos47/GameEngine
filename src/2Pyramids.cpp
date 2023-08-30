#include "2Pyramids.h"
#include <assert.h>
#include <sb7.h>

#include "ModelFileHeader.h"
#include "File.h"
struct _2Pyramid_Vert_xyzuv
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;

	/*void set(float _x, float _y, float _z, float _u, float _v, float _nx, float _ny, float _nz)
	{
		x = _x;
		y = _y;
		z = _z;
		u = _u;
		v = _v;
		nx = _nx;
		ny = _ny;
		nz = _nz;
	}*/
};

struct _2Pyramid_Tri_index
{
	int v0;
	int v1;
	int v2;

	//void set(int _v0, int _v1, int _v2)
	//{
	//	v0 = _v0;
	//	v1 = _v1;
	//	v2 = _v2;
	//}
};

_2PyramidModel::_2PyramidModel(const char * const pModelFileName)
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
	_2Pyramid_Vert_xyzuv  *p_2PyramidData = new _2Pyramid_Vert_xyzuv[(unsigned int)modelHdr.numVerts];
	_2Pyramid_Tri_index *pTriList = new _2Pyramid_Tri_index[(unsigned int)modelHdr.numTriList];

	// Read verts
	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, p_2PyramidData, sizeof(_2Pyramid_Vert_xyzuv) * this->numVerts);
	assert(ferror == File::Error::SUCCESS);

	// Read trilist
	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, pTriList, sizeof(_2Pyramid_Tri_index) * this->numTris);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	assert(ferror == File::Error::SUCCESS);

	UNUSED_VAR(pModelFileName);
	
	
	//// future proofing it for a file
	/*this->numVerts = PYRAMID_DATA_NUM_VERTS;
	this->numTris = TRILIST_NUM_TRIANGLES;*/

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
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(_2Pyramid_Vert_xyzuv) * this->numVerts), p_2PyramidData, GL_STATIC_DRAW);

	// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
	void *offsetVert = (void *)((unsigned int)&p_2PyramidData[0].x - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_2Pyramid_Vert_xyzuv), offsetVert);
	glEnableVertexAttribArray(0);

	// Texture data is location: 1  (used in vertex shader)
	void *offsetTex = (void *)((unsigned int)&p_2PyramidData[0].u - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(_2Pyramid_Vert_xyzuv), offsetTex);
	glEnableVertexAttribArray(1);

	// normals data in location 2 (used in vertex shader
	void *offsetNorm = (void *)((unsigned int)&p_2PyramidData[0].nx - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(_2Pyramid_Vert_xyzuv), offsetNorm);
	glEnableVertexAttribArray(2);

	// Load the index data: ---------------------------------------------------------


		/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

	/* Copy the index data to our buffer */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(_2Pyramid_Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);

	delete[] p_2PyramidData;
	delete[] pTriList;
}

_2PyramidModel::~_2PyramidModel()
{
}

void _2PyramidModel::privCreateVAO(const char * const pModelFileName)
{
	assert(pModelFileName);
	this->privCreateVAO(pModelFileName);
}


///////////////////////////////code for wrting to file//////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//#include "2Pyramids.h"
//#include <assert.h>
//#include <sb7.h>
//
//#include "ModelFileHeader.h"
//#include "File.h"
//struct _2Pyramid_Vert_xyzuv
//{
//	float x;
//	float y;
//	float z;
//	float u;
//	float v;
//	float nx;
//	float ny;
//	float nz;
//
//	void set(float _x, float _y, float _z, float _u, float _v, float _nx, float _ny, float _nz)
//	{
//		x = _x;
//		y = _y;
//		z = _z;
//		u = _u;
//		v = _v;
//		nx = _nx;
//		ny = _ny;
//		nz = _nz;
//	}
//};
//
//struct _2Pyramid_Tri_index
//{
//	int v0;
//	int v1;
//	int v2;
//
//	void set(int _v0, int _v1, int _v2)
//	{
//		v0 = _v0;
//		v1 = _v1;
//		v2 = _v2;
//	}
//};
//
//#define WRITE_DATA_TO_FILE 1
//
//#if WRITE_DATA_TO_FILE
//
//
//#define PYRAMID_DATA_NUM_VERTS (sizeof(PyramidData)/sizeof(_2Pyramid_Vert_xyzuv))
//
//#define TRILIST_NUM_TRIANGLES (sizeof(triList)/sizeof(_2Pyramid_Tri_index))
//
//
//_2Pyramid_Vert_xyzuv  PyramidData[35];
//
//_2Pyramid_Tri_index triList[11];
//
//void _2PyramidDataSeter()
//{
//	//pretty dum odering of triangles verts
//
//	PyramidData[0].set(0.0f, 0.25f, 0.0f, 0.5, 0, 0, 0, -1);
//	PyramidData[1].set(-0.25f, -0.25f, -0.25f, 1, 1, 0, 0, -1);
//	PyramidData[2].set(+0.25f, -0.25f, -0.25f, 0, 1, 0, 0, -1);
//
//	PyramidData[3].set(0.0f, 0.25f, 0.0f, 0.5, 0, 0, 0, 1);
//	PyramidData[4].set(-0.25f, -0.25f, +0.25f, 0, 1, 0, 0, 1);
//	PyramidData[5].set(+0.25f, -0.25f, +0.25f, 1, 1, 0, 0, 1);
//
//	PyramidData[6].set(0.0f, 0.25f, 0.0f, 0.5, 0, 1, 0, 0);
//	PyramidData[7].set(+0.25f, -0.25f, -0.25f, 1, 1, 1, 0, 0);
//	PyramidData[8].set(+0.25f, -0.25f, +0.25f, 0, 1, 1, 0, 0);
//
//	PyramidData[9].set(0.0f, 0.25f, 0.0f, 0.5, 0, -1, 0, 0);
//	PyramidData[10].set(-0.25f, -0.25f, -0.25f, 0, 1, -1, 0, 0);
//	PyramidData[11].set(-0.25f, -0.25f, +0.25f, 1, 1, -1, 0, 0);
//
//	PyramidData[12].set(-0.25f, -0.25f, -0.25f, 0, 0, 0, -1, 0);
//	PyramidData[13].set(+0.25f, -0.25f, +0.25f, 1, 1, 0, -1, 0);
//	PyramidData[14].set(+0.25f, -0.25f, -0.25f, 0, 1, 0, -1, 0);
//
//	PyramidData[15].set(-0.25f, -0.25f, -0.25f, 0, 0, 0, -1, 0);
//	PyramidData[16].set(-0.25f, -0.25f, +0.25f, 1, 0, 0, -1, 0);
//	PyramidData[17].set(+0.25f, -0.25f, +0.25f, 1, 1, 0, -1, 0);
//
//	//pretty dum odering of triangles verts
//
//	PyramidData[18].set(0.0f, -0.75f, 0.0f, 0.5, 0, 0, 0, -1);
//	PyramidData[19].set(-0.25f, -0.25f, -0.25f, 1, 1, 0, 0, -1);
//	PyramidData[20].set(+0.25f, -0.25f, -0.25f, 0, 1, 0, 0, -1);
//
//	PyramidData[21].set(0.0f, -0.75f, 0.0f, 0.5, 0, 0, 0, 1);
//	PyramidData[22].set(-0.25f, -0.25f, +0.25f, 0, 1, 0, 0, 1);
//	PyramidData[23].set(+0.25f, -0.25f, +0.25f, 1, 1, 0, 0, 1);
//
//	PyramidData[24].set(0.0f, -0.75f, 0.0f, 0.5, 0, 1, 0, 0);
//	PyramidData[25].set(+0.25f, -0.25f, -0.25f, 1, 1, 1, 0, 0);
//	PyramidData[26].set(+0.25f, -0.25f, +0.25f, 0, 1, 1, 0, 0);
//
//	PyramidData[27].set(0.0f, -0.75f, 0.0f, 0.5, 0, -1, 0, 0);
//	PyramidData[28].set(-0.25f, -0.25f, -0.25f, 0, 1, -1, 0, 0);
//	PyramidData[29].set(-0.25f, -0.25f, +0.25f, 1, 1, -1, 0, 0);
//
//
//	//pretty dum odering of triangles verts
//
//	// back face
//	triList[0].set(0, 1, 2);
//	// front face
//	triList[1].set(3, 5, 4);
//	// left face
//	triList[2].set(6, 7, 8);
//	// right face
//	triList[3].set(9, 11, 10);
//	// bottom faces
//	triList[4].set(12, 13, 14);
//
//	triList[5].set(15, 16, 17);
//
//	// back face
//	triList[6].set(18, 20, 19);
//	// front face
//	triList[7].set(21, 22, 23);
//	//// left face
//	triList[8].set(24, 26, 25);
//	//// right face
//	triList[9].set(27, 28, 29);
//}
//#endif
//
//_2PyramidModel::_2PyramidModel(const char * const pModelFileName)
//{
//	// File stuff
//	File::Handle fh;
//	File::Error  ferror;
//
//	// create a header
//	ModelFileHeader modelHdr;
//
//#if WRITE_DATA_TO_FILE
//	//set data
//
//	_2PyramidDataSeter();
//
//	// WRITE data:
//
//	this->numVerts = PYRAMID_DATA_NUM_VERTS;
//	this->numTris = TRILIST_NUM_TRIANGLES;
//
//	// object name
//	strncpy_s(modelHdr.objName, OBJECT_NAME_SIZE, "_2Pyramids", _TRUNCATE);
//
//	// vertex buffer
//	modelHdr.numVerts = this->numVerts;
//	modelHdr.vertBufferOffset = 0;
//
//	// trilist index 
//	modelHdr.numTriList = this->numTris;
//	modelHdr.triListBufferOffset = 0;
//
//	// Write to a file
//
//	// Write the data to a file ---------------------------------------------
//
//	//----------- WRITE ------------------------------------------
//	ferror = File::Open(fh, pModelFileName, File::Mode::READ_WRITE);
//	assert(ferror == File::Error::SUCCESS);
//
//	// write the Header
//	ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHeader));
//	assert(ferror == File::Error::SUCCESS);
//
//	// write the vert buffer data
//	ferror = File::Tell(fh, modelHdr.vertBufferOffset);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Write(fh, &PyramidData[0], sizeof(_2Pyramid_Vert_xyzuv) * this->numVerts);
//	assert(ferror == File::Error::SUCCESS);
//
//	// write the index buffer
//	ferror = File::Tell(fh, modelHdr.triListBufferOffset);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Write(fh, &triList[0], sizeof(_2Pyramid_Tri_index) * this->numTris);
//	assert(ferror == File::Error::SUCCESS);
//
//	// Finish the write
//	ferror = File::Flush(fh);
//	assert(ferror == File::Error::SUCCESS);
//
//	// Rewind and overwrite model hdr
//	ferror = File::Seek(fh, File::Location::BEGIN, 0);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHeader));
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Flush(fh);
//	assert(ferror == File::Error::SUCCESS);
//
//	// VERIFY
//	ModelFileHeader modelHdr2;
//
//	ferror = File::Seek(fh, File::Location::BEGIN, 0);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Read(fh, &modelHdr2, sizeof(ModelFileHeader));
//	assert(ferror == File::Error::SUCCESS);
//
//	// CLOSE
//	ferror = File::Close(fh);
//	assert(ferror == File::Error::SUCCESS);
//
//#endif
//
//	ferror = File::Open(fh, pModelFileName, File::Mode::READ);
//	assert(ferror == File::Error::SUCCESS);
//
//	// Read the Hdr
//	ferror = File::Read(fh, &modelHdr, sizeof(ModelFileHeader));
//	assert(ferror == File::Error::SUCCESS);
//
//	// Using the hdr, allocate the space for the buffers
//	this->numVerts = modelHdr.numVerts;
//	this->numTris = modelHdr.numTriList;
//
//	// allocate buffers
//	_2Pyramid_Vert_xyzuv  *p_2PyramidData = new _2Pyramid_Vert_xyzuv[(unsigned int)modelHdr.numVerts];
//	_2Pyramid_Tri_index *pTriList = new _2Pyramid_Tri_index[(unsigned int)modelHdr.numTriList];
//
//	// Read verts
//	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Read(fh, p_2PyramidData, sizeof(_2Pyramid_Vert_xyzuv) * this->numVerts);
//	assert(ferror == File::Error::SUCCESS);
//
//	// Read trilist
//	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Read(fh, pTriList, sizeof(_2Pyramid_Tri_index) * this->numTris);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Close(fh);
//	assert(ferror == File::Error::SUCCESS);
//
//	UNUSED_VAR(pModelFileName);
//
//
//	//// future proofing it for a file
//	/*this->numVerts = PYRAMID_DATA_NUM_VERTS;
//	this->numTris = TRILIST_NUM_TRIANGLES;*/
//
//	// Create a VAO
//	glGenVertexArrays(1, &this->vao);
//	assert(this->vao != 0);
//	glBindVertexArray(this->vao);
//
//	// Create a VBO
//	glGenBuffers(1, &this->vbo_0);
//	assert(this->vbo_0 != 0);
//
//	glGenBuffers(1, &this->vbo_1);
//	assert(this->vbo_1 != 0);
//
//	// Load the combined data: ---------------------------------------------------------
//
//	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_0);
//
//	// load the data to the GPU
//	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(_2Pyramid_Vert_xyzuv) * this->numVerts), p_2PyramidData, GL_STATIC_DRAW);
//
//	// Define an array of generic vertex attribute data
//
//		// todo make a table or enum - locations needs enums...
//
//		// Vert data is location: 0  (used in vertex shader)
//	void *offsetVert = (void *)((unsigned int)&p_2PyramidData[0].x - (unsigned int)p_2PyramidData);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_2Pyramid_Vert_xyzuv), offsetVert);
//	glEnableVertexAttribArray(0);
//
//	// Texture data is location: 1  (used in vertex shader)
//	void *offsetTex = (void *)((unsigned int)&p_2PyramidData[0].u - (unsigned int)p_2PyramidData);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(_2Pyramid_Vert_xyzuv), offsetTex);
//	glEnableVertexAttribArray(1);
//
//	// normals data in location 2 (used in vertex shader
//	void *offsetNorm = (void *)((unsigned int)&p_2PyramidData[0].nx - (unsigned int)p_2PyramidData);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(_2Pyramid_Vert_xyzuv), offsetNorm);
//	glEnableVertexAttribArray(2);
//
//	// Load the index data: ---------------------------------------------------------
//
//
//		/* Bind our 2nd VBO as being the active buffer and storing index ) */
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_1);
//
//	/* Copy the index data to our buffer */
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(_2Pyramid_Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
//
//	delete[] p_2PyramidData;
//	delete[] pTriList;
//}
//
//_2PyramidModel::~_2PyramidModel()
//{
//}
//
//void _2PyramidModel::privCreateVAO(const char * const pModelFileName)
//{
//	assert(pModelFileName);
//	this->privCreateVAO(pModelFileName);
//}
