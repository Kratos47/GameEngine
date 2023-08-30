#include "SkinModel.h"

#include <assert.h>
#include <sb7.h>

#include "ModelFileinfo.h"
#include "File.h"
#include "eat.h"
#include "BoneWeight.h"
struct Skin_Trilist
{
	int v0;
	int v1;
	int v2;
};


struct Skin_Vertex_vsn
{
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
};

struct Weights 
{
	float a;
	float b;
	float c;
	float d;
};


struct MatIndx 
{
	int a;
	int b;
	int c;
	int d;
};


Weights* W8s;
MatIndx* pMatIndx;


void baseSetW8S(const char* const pModelFileName)
{
	//NEW CALLED
	char* Trunc_File_Name = new char[100];

	unsigned int i = 0;

	//Debugging varialble
	unsigned  int strSize = strlen(pModelFileName);

	i = 0;

	while (i < strSize - 4)
	{
		Trunc_File_Name[i] = pModelFileName[i];

		if (i == strSize - 5)
		{
			Trunc_File_Name[i + 1] = (char)0;
		}

		i++;
	}

	//NEW CALLED
	char* clip_Chunk = new char[100];
	strcpy(clip_Chunk, Trunc_File_Name);
	strcat(clip_Chunk, "_w8s");


	unsigned char* w8sChunkBuff;
	unsigned int chunkSize;

	eat(pModelFileName, ChunkType::ANIM_TYPE, clip_Chunk, w8sChunkBuff, chunkSize);

	ChunkHeader    chnkHdr;
	memcpy(&chnkHdr, &w8sChunkBuff[0], sizeof(ChunkHeader));

	// create a header
	W8S_HDR modelHdr;

	memcpy(&modelHdr, &w8sChunkBuff[0], sizeof(W8S_HDR));

	int numWeights = modelHdr.totalNumVerts;


	//Hrad coded for testing 
	FinalVertEntry* skin_W8S = new FinalVertEntry[numWeights];

	//// Read verts
	memcpy(&skin_W8S[0], &w8sChunkBuff[sizeof(W8S_HDR)], sizeof(FinalVertEntry) * numWeights);

	i = 0;

	W8s = new Weights[numWeights];
	pMatIndx = new MatIndx[numWeights];

	for (i = 0; i < (unsigned int)numWeights; i++) 
	{
		W8s[i].a = skin_W8S[i].A.fBoneWeight;
		W8s[i].b = skin_W8S[i].B.fBoneWeight;
		W8s[i].c = skin_W8S[i].C.fBoneWeight;
		W8s[i].d = skin_W8S[i].D.fBoneWeight;
		
		
		pMatIndx[i].a = skin_W8S[i].A.iBoneIndex;
		pMatIndx[i].b = skin_W8S[i].B.iBoneIndex;
		pMatIndx[i].c = skin_W8S[i].C.iBoneIndex;
		pMatIndx[i].d = skin_W8S[i].D.iBoneIndex;
	}

	delete[] Trunc_File_Name;
	delete[] w8sChunkBuff;
	delete[] clip_Chunk;
	delete[] skin_W8S;
}

//void LoadSkinTextures(const char* const pModelFileName, const Texture::Name Name)
//{
//	UNUSED_VAR(pModelFileName);
//	//NEW CALLED
//	char* Trunc_File_Name = new char[100];
//
//	unsigned int i = 0;
//
//	while (i <= strlen(pModelFileName) - 4)
//	{
//		Trunc_File_Name[i] = pModelFileName[i];
//
//		if (i == strlen(pModelFileName) - 4)
//		{
//			Trunc_File_Name[i] = (char)0;
//		}
//
//		i++;
//	}
//
//
//	//NEW CALLED
//	char* Tex_Chunk = new char[100];
//	strcpy(Tex_Chunk, Trunc_File_Name);
//	strcat(Tex_Chunk, "_Tex");
//
//	unsigned char* TexchunkBuff;
//	unsigned int chunkSize;
//
//	/*const Texture::Name type = Texture::Name::SPACE_FRIGATE;*/
//
//	const char* fileName = Tex_Chunk;
//
//	eat(pModelFileName, ChunkType::TEXTURE_TYPE, Tex_Chunk, TexchunkBuff, chunkSize);
//
//
//
//	delete[] Trunc_File_Name;
//	delete[] Tex_Chunk;
//
//	Texture::Add(TexchunkBuff, fileName, Name);
//}


SkinModel::SkinModel(const char* const pModelFileName, const char* const pWeightsFileName, const Texture::Name Name)
{
	//NEW CALLED
	char* Trunc_File_Name = new char[100];

	unsigned int i = 0;

	while (i <= strlen(pModelFileName) - 10)
	{
		Trunc_File_Name[i] = pModelFileName[i];

		if (i == strlen(pModelFileName) - 10)
		{
			Trunc_File_Name[i] = (char)0;
		}

		i++;
	}


	//NEW CALLED
	char* verts_Chunk = new char[100];
	strcpy(verts_Chunk, Trunc_File_Name);
	strcat(verts_Chunk, "_Verts");

	unsigned char* VertschunkBuff;
	unsigned int chunkSize;

	eat(pModelFileName, ChunkType::VERTS_TYPE, verts_Chunk, VertschunkBuff, chunkSize);

	// create a header
	ModelFileInfo modelHdr;

	memcpy(&modelHdr, &VertschunkBuff[0], sizeof(ModelFileInfo));

	// Using the hdr, allocate the space for the buffers
	this->numVerts = modelHdr.numVerts;
	this->numTris = modelHdr.numTriList;

	// allocate buffers
	Skin_Vertex_vsn* p_2PyramidData = new Skin_Vertex_vsn[(unsigned int)modelHdr.numVerts];
	Skin_Trilist* pTriList = new Skin_Trilist[(unsigned int)modelHdr.numTriList];

	//// Read verts
	memcpy(&p_2PyramidData[0], &VertschunkBuff[sizeof(ModelFileInfo)], sizeof(Skin_Vertex_vsn) * this->numVerts);

	//// Read trilist
	memcpy(&pTriList[0], &VertschunkBuff[sizeof(ModelFileInfo) + sizeof(Skin_Vertex_vsn) * this->numVerts],
		sizeof(Skin_Trilist) * this->numTris);

	baseSetW8S(pWeightsFileName);

	//// future proofing it for a file
	/*this->numVerts = PYRAMID_DATA_NUM_VERTS;
	this->numTris = TRILIST_NUM_TRIANGLES;*/
	// Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	// Create a VBO
	glGenBuffers(1, &this->vbo[0]);
	assert(this->vbo[0] != 0);// Create a VBO
	
	glGenBuffers(1, &this->vbo[1]);
	assert(this->vbo[1] != 0);// Create a VBO
	
	glGenBuffers(1, &this->vbo[2]);
	assert(this->vbo[2] != 0);// Create a VBO
	
	glGenBuffers(1, &this->vbo[3]);
	assert(this->vbo[3] != 0);

	/*glGenBuffers(1, &this->vbo[1]);
	assert(this->vbo[1] != 0);*/

	// MOVE this to converter!!!!!
	Vect* pVerts = new Vect[(unsigned int)this->numVerts];

	int i2 = 0;

	for (i2 = 0; i2 < this->numVerts; i2++)
	{
		pVerts[i2].set(p_2PyramidData[i2].x, p_2PyramidData[i2].y, p_2PyramidData[i2].z);
		//Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
	}

	assert(this->pRefSphere);
	this->pRefSphere->RitterSphere(pVerts, this->numVerts);
	//Trace::out("Ritter: cntr:%f %f %f  rad: %f ", this->pRefSphere->cntr[x], this->pRefSphere->cntr[y], this->pRefSphere->cntr[z], this->pRefSphere->rad);

	// Load the combined data: ---------------------------------------------------------

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

	// load the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Skin_Vertex_vsn) * this->numVerts), p_2PyramidData, GL_STATIC_DRAW);

	// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
	void* offsetVert = (void*)((unsigned int)&p_2PyramidData[0].x - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skin_Vertex_vsn), offsetVert);
	glEnableVertexAttribArray(0);

	// Texture data is location: 1  (used in vertex shader)
	void* offsetTex = (void*)((unsigned int)&p_2PyramidData[0].s - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Skin_Vertex_vsn), offsetTex);
	glEnableVertexAttribArray(1);

	// normals data in location 2 (used in vertex shader
	void* offsetNorm = (void*)((unsigned int)&p_2PyramidData[0].nx - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Skin_Vertex_vsn), offsetNorm);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]);

	// load the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Weights) * this->numVerts), W8s, GL_STATIC_DRAW);
	
	// weights data in location 3 (used in vertex shader
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Weights), 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[2]);

	// load the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(MatIndx) * this->numVerts), pMatIndx, GL_STATIC_DRAW);
	
	// indices data in location 4 (used in vertex shader
	glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(MatIndx), 0);
	glEnableVertexAttribArray(4);

	// Load the index data: ---------------------------------------------------------


		/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

	/* Copy the index data to our buffer */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Skin_Trilist) * this->numTris), pTriList, GL_STATIC_DRAW);

	delete[] p_2PyramidData;
	delete[] pTriList;
	delete[] Trunc_File_Name;
	delete[]verts_Chunk;
	delete[]VertschunkBuff;
	delete[] pVerts;
	delete[] W8s;
	delete[] pMatIndx;

	//if default texture nop need to load
	if (Name == Texture::Name::DEFAULT)
	{
		return;
	}

	//LoadSkinTextures(pModelFileName, Name);
}



SkinModel::~SkinModel()
{
}

void SkinModel::privCreateVAO(const char* const pModelFileName)
{
	assert(pModelFileName);
	this->privCreateVAO(pModelFileName);
}
