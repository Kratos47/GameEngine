#include "CustomModel.h"

#include <assert.h>
#include <sb7.h>

#include "ModelFileinfo.h"
#include "File.h"
#include "eat.h"
struct VBO_Trilist
{
	int v0;
	int v1;
	int v2;
};


struct VBO_Vertex_vsn
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

void LoadTextures(const char* const pModelFileName, const Texture::Name Name)
{
	UNUSED_VAR(pModelFileName);
	//NEW CALLED
	char* Trunc_File_Name = new char[100];

	unsigned int i = 0;

	while (i <= strlen(pModelFileName) - 4)
	{
		Trunc_File_Name[i] = pModelFileName[i];

		if (i == strlen(pModelFileName) - 4)
		{
			Trunc_File_Name[i] = (char)0;
		}

		i++;
	}


	//NEW CALLED
	char* Tex_Chunk = new char[100];
	strcpy(Tex_Chunk, Trunc_File_Name);
	strcat(Tex_Chunk, "_Tex");

	unsigned char* TexchunkBuff;
	unsigned int chunkSize;
	
	/*const Texture::Name type = Texture::Name::SPACE_FRIGATE;*/

	const char* fileName = Tex_Chunk;

	eat(pModelFileName, ChunkType::TEXTURE_TYPE, Tex_Chunk, TexchunkBuff, chunkSize);



	delete[] Trunc_File_Name;
	delete[] Tex_Chunk;

	Texture::Add(TexchunkBuff, fileName, Name);
}


CustomModel::CustomModel(const char* const pModelFileName, const Texture::Name Name)
{
	//NEW CALLED
	char* Trunc_File_Name = new char[100];

	unsigned int i = 0;

	while (i <= strlen(pModelFileName) - 4)
	{
		Trunc_File_Name[i] = pModelFileName[i];

		if (i == strlen(pModelFileName) - 4)
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
	VBO_Vertex_vsn* p_2PyramidData = new VBO_Vertex_vsn[(unsigned int)modelHdr.numVerts];
	VBO_Trilist* pTriList = new VBO_Trilist[(unsigned int)modelHdr.numTriList];

	//// Read verts
	memcpy(&p_2PyramidData[0], &VertschunkBuff[sizeof(ModelFileInfo)], sizeof(VBO_Vertex_vsn) * this->numVerts);

	//// Read trilist
	memcpy(&pTriList[0], &VertschunkBuff[sizeof(ModelFileInfo)+ sizeof(VBO_Vertex_vsn) * this->numVerts],
		sizeof(VBO_Trilist) * this->numTris);

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

	// Find Bounding Volume - quick hack
	// Find Bounding Volume - quick hack
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
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(VBO_Vertex_vsn) * this->numVerts), p_2PyramidData, GL_STATIC_DRAW);

	// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
	void* offsetVert = (void*)((unsigned int)&p_2PyramidData[0].x - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex_vsn), offsetVert);
	glEnableVertexAttribArray(0);

	// Texture data is location: 1  (used in vertex shader)
	void* offsetTex = (void*)((unsigned int)&p_2PyramidData[0].s - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex_vsn), offsetTex);
	glEnableVertexAttribArray(1);

	// normals data in location 2 (used in vertex shader
	void* offsetNorm = (void*)((unsigned int)&p_2PyramidData[0].nx - (unsigned int)p_2PyramidData);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex_vsn), offsetNorm);
	glEnableVertexAttribArray(2);

	// Load the index data: ---------------------------------------------------------


		/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

	/* Copy the index data to our buffer */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(VBO_Trilist) * this->numTris), pTriList, GL_STATIC_DRAW);

	delete[] p_2PyramidData;
	delete[] pTriList;
	delete[] Trunc_File_Name;
	delete[]verts_Chunk;
	delete[]VertschunkBuff;
	delete[] pVerts;

	//if default texture nop need to load
	if (Name == Texture::Name::DEFAULT)
	{
		return;
	}

	LoadTextures(pModelFileName, Name);
}



CustomModel::~CustomModel()
{
}

void CustomModel::privCreateVAO(const char* const pModelFileName)
{
	assert(pModelFileName);
	this->privCreateVAO(pModelFileName);
}
