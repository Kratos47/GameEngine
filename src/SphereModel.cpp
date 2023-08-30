#include "SphereModel.h"
#include <assert.h>
#include <sb7.h>

#include "ModelFileHeader.h"
#include "File.h"
struct Sphere_Vert_xyzuvn
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;

	void set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
		
		
	};
	void normal(float _nx, float _ny, float _nz) 
	{
		nx = _nx;
		ny = _ny;
		nz = _nz;
	};	
};

struct Sphere_Tri_index
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;

	void set(unsigned int _v0, unsigned int _v1, unsigned int _v2)
	{
		v0 = _v0;
		v1 = _v1;
		v2 = _v2;
	}
};

SphereModel::SphereModel(const char * const pModelFileName)
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
	Sphere_Vert_xyzuvn  *pSphere = new Sphere_Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
	Sphere_Tri_index *pTriList = new Sphere_Tri_index[(unsigned int)modelHdr.numTriList];

	// Read verts
	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, pSphere, sizeof(Sphere_Vert_xyzuvn) * this->numVerts);
	assert(ferror == File::Error::SUCCESS);

	// Read trilist
	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, pTriList, sizeof(Sphere_Tri_index) * this->numTris);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	assert(ferror == File::Error::SUCCESS);

	UNUSED_VAR(pModelFileName);


	// future proofing it for a file
	assert(pModelFileName);
	UNUSED_VAR(pModelFileName);

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
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Sphere_Vert_xyzuvn) * this->numVerts), pSphere, GL_STATIC_DRAW);

	// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
	void *offsetVert = (void *)((unsigned int)&pSphere[0].x - (unsigned int)pSphere);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetVert);
	glEnableVertexAttribArray(0);

	// Texture data is location: 1  (used in vertex shader)
	void *offsetTex = (void *)((unsigned int)&pSphere[0].u - (unsigned int)pSphere);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetTex);
	glEnableVertexAttribArray(1);

	// normals data in location 2 (used in vertex shader
	void *offsetNorm = (void *)((unsigned int)&pSphere[0].nx - (unsigned int)pSphere);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetNorm);
	glEnableVertexAttribArray(2);

	// Load the index data: ---------------------------------------------------------


		/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

	/* Copy the index data to our buffer */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Sphere_Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetTex);
	glEnableVertexAttribArray(1);

	delete[] pSphere;
	delete[] pTriList;
}

SphereModel::~SphereModel()
{
}

void SphereModel::privCreateVAO(const char * const pModelFileName)
{
	assert(pModelFileName);
	this->privCreateVAO(pModelFileName);
}




/////////////////////////////code for wrting to file//////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


//#include "SphereModel.h"
//#include <assert.h>
//#include <sb7.h>
//
//#include "ModelFileHeader.h"
//#include "File.h"
//struct Sphere_Vert_xyzuvn
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
//	void set(float _x, float _y, float _z)
//	{
//		x = _x;
//		y = _y;
//		z = _z;
//
//
//	};
//	void normal(float _nx, float _ny, float _nz)
//	{
//		nx = _nx;
//		ny = _ny;
//		nz = _nz;
//	};
//};
//
//struct Sphere_Tri_index
//{
//	unsigned int v0;
//	unsigned int v1;
//	unsigned int v2;
//
//	void set(unsigned int _v0, unsigned int _v1, unsigned int _v2)
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
//#define TRILIST_NUM_TRIANGLES (sizeof(triList)/sizeof(Sphere_Tri_index))
//#define SPHERE_DATA_NUM_VERTS (sizeof(SphereData)/sizeof(Sphere_Vert_xyzuvn))
//
//unsigned int vslice = 100;
//unsigned int hslice = 100;
//unsigned int nverts = ((hslice - 2) * vslice) + 2;
//unsigned ntri = ((hslice - 3)*(vslice) * 2) + (vslice * 2);
//
//Sphere_Vert_xyzuvn  SphereData[(((100 - 2) * 100) + 2)];
//Sphere_Tri_index triList[19600];
//
//void SphereDataSeter()
//{
//	float radius = 0.3f;
//	float r = radius;
//
//	float sphereYaw = 0.0f;
//	float spherePitch = 0.0f;
//
//	Vect currVertPos = Vect(0.0f, 0.0f, r);
//
//	//north pole
//	SphereData[0].set(0.0f, 0.0f, -r);
//	SphereData[0].normal(0, 0, -r);
//
//	SphereData[0].u = (0.5f + ((atan2(-r, 0.f)) / (2 * MATH_PI)));
//	SphereData[0].v = (0.5f + ((asin(0.f)) / MATH_PI));
//
//	unsigned int i = 0;
//	unsigned int j = 0;
//	//middle stuff
//	for (i = 0; i < hslice - 2; ++i)
//	{
//		spherePitch = (i + 1) * (MATH_PI / (hslice - 1));
//		Matrix Rotationx = Matrix(Matrix::Rot::X, spherePitch);
//		for (j = 0; j < vslice; ++j)
//		{
//			sphereYaw = j * (MATH_2PI / (vslice));
//			Matrix Rotationz = Matrix(Matrix::Rot::Z, sphereYaw);
//			currVertPos = Vect(0.0f, 0.0f, -r)* Rotationx * Rotationz;
//			currVertPos.getNorm();
//
//			SphereData[i*vslice + j + 1].set(currVertPos.x(), currVertPos.y(), currVertPos.z());
//			SphereData[i*vslice + j + 1].normal(currVertPos.x(), currVertPos.y(), currVertPos.z());
//
//			SphereData[i*vslice + j + 1].u = (0.5f + ((atan2(currVertPos.getNorm().z(), currVertPos.getNorm().x())) / (2 * MATH_PI)));
//			SphereData[i*vslice + j + 1].v = (0.5f + ((asin(currVertPos.getNorm().y())) / MATH_PI));
//		}
//	}
//
//	//south pole
//	SphereData[nverts - 1].set(0.0f, 0.0f, r);
//	SphereData[nverts - 1].normal(0, 0, r);
//
//	SphereData[nverts - 1].u = (0.5f + ((atan2(r, 0.f)) / (2 * MATH_PI)));
//	SphereData[nverts - 1].v = (0.5f + ((asin(0.f)) / MATH_PI));
//
//	unsigned int k = 0;
//
//	//north pole
//	for (unsigned int l = 0; l < vslice - 1; ++l)
//	{
//		triList[k].set(0, l + 1, l + 2);
//		k++;
//	}
//	triList[k].set(0, vslice, 1);
//	k++;
//
//	//middle stuff
//	for (/*unsigned int*/ i = 0; i < hslice - 3; ++i)
//	{
//		for (/*unsigned int*/ j = 0; j < vslice - 1; ++j)
//		{
//			triList[k].set(i * vslice + j + 1,
//				(i + 1)*vslice + j + 1,
//				i * vslice + j + 2);
//			triList[k + 1].set((i + 1)*vslice + j + 1,
//				(i + 1)*vslice + j + 2,
//				i * vslice + j + 2);
//
//			k += 2;
//		}
//
//		triList[k].set((i*vslice) + vslice,
//			((i + 1)*vslice) + vslice,
//			(i*vslice) + 1);
//
//		triList[k + 1].set(((i + 1)*vslice) + vslice,
//			((i + 1)*vslice) + 1,
//			(i*vslice) + 1);
//
//		k += 2;
//	}
//
//	//south pole
//
//	for (unsigned int l = 0; l < vslice - 1; ++l)
//	{
//		triList[k].set(nverts - 1,
//			(nverts - 1) - (l + 1),
//			(nverts - 1) - (l + 2));
//		k++;
//	}
//	triList[k].set(nverts - 1,
//		(nverts - 1) - vslice,
//		(nverts - 2));
//}
//
//#endif
//
//SphereModel::SphereModel(const char * const pModelFileName)
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
//	SphereDataSeter();
//
//	// WRITE data:
//
//	this->numVerts = SPHERE_DATA_NUM_VERTS;
//	this->numTris = TRILIST_NUM_TRIANGLES;
//
//	// object name
//	strncpy_s(modelHdr.objName, OBJECT_NAME_SIZE, "Sphere", _TRUNCATE);
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
//	ferror = File::Write(fh, &SphereData[0], sizeof(Sphere_Vert_xyzuvn) * this->numVerts);
//	assert(ferror == File::Error::SUCCESS);
//
//	// write the index buffer
//	ferror = File::Tell(fh, modelHdr.triListBufferOffset);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Write(fh, &triList[0], sizeof(Sphere_Tri_index) * this->numTris);
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
//	Sphere_Vert_xyzuvn  *pSphere = new Sphere_Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
//	Sphere_Tri_index *pTriList = new Sphere_Tri_index[(unsigned int)modelHdr.numTriList];
//
//	// Read verts
//	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Read(fh, pSphere, sizeof(Sphere_Vert_xyzuvn) * this->numVerts);
//	assert(ferror == File::Error::SUCCESS);
//
//	// Read trilist
//	ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Read(fh, pTriList, sizeof(Sphere_Tri_index) * this->numTris);
//	assert(ferror == File::Error::SUCCESS);
//
//	ferror = File::Close(fh);
//	assert(ferror == File::Error::SUCCESS);
//
//	UNUSED_VAR(pModelFileName);
//
//
//	// future proofing it for a file
//	assert(pModelFileName);
//	UNUSED_VAR(pModelFileName);
//
//	/*this->numVerts = SPHERE_DATA_NUM_VERTS;
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
//	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Sphere_Vert_xyzuvn) * this->numVerts), pSphere, GL_STATIC_DRAW);
//
//	// Define an array of generic vertex attribute data
//
//		// todo make a table or enum - locations needs enums...
//
//		// Vert data is location: 0  (used in vertex shader)
//	void *offsetVert = (void *)((unsigned int)&pSphere[0].x - (unsigned int)pSphere);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetVert);
//	glEnableVertexAttribArray(0);
//
//	// Texture data is location: 1  (used in vertex shader)
//	void *offsetTex = (void *)((unsigned int)&pSphere[0].u - (unsigned int)pSphere);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetTex);
//	glEnableVertexAttribArray(1);
//
//	// normals data in location 2 (used in vertex shader
//	void *offsetNorm = (void *)((unsigned int)&pSphere[0].nx - (unsigned int)pSphere);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetNorm);
//	glEnableVertexAttribArray(2);
//
//	// Load the index data: ---------------------------------------------------------
//
//
//		/* Bind our 2nd VBO as being the active buffer and storing index ) */
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_1);
//
//	/* Copy the index data to our buffer */
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Sphere_Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sphere_Vert_xyzuvn), offsetTex);
//	glEnableVertexAttribArray(1);
//
//	delete pSphere;
//	delete pTriList;
//}
//
//SphereModel::~SphereModel()
//{
//}
//
//void SphereModel::privCreateVAO(const char * const pModelFileName)
//{
//	assert(pModelFileName);
//	this->privCreateVAO(pModelFileName);
//}
