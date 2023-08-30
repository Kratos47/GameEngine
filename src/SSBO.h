//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SSBO_H
#define SSBO_H

#include <sb7.h>

class SSBOLink
{
public:
	SSBOLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~SSBOLink()
	{

	}
public:
	SSBOLink* next;
	SSBOLink* prev;
};

// Shader Storage Buffer Object
class SSBO :public SSBOLink
{
public:
	enum class Access
	{
		READ_ONLY  = 0x88B8,
		WRITE_ONLY = 0x88B9,
		READ_WRITE = 0x88BA
	};

	enum class Name
	{
		TEDDY_HIERARCHY,
		HUMAN_HIERARCHY,

		TEDDY_WALK_KEY,
		TEDDY_WALK_RESULT,

		TEDDY_IDLE_KEY,
		TEDDY_IDLE_RESULT,

		HUMAN_WALK_KEY,
		HUMAN_WALK_RESULT,

		HUMAN_RUN_KEY,
		HUMAN_RUN_RESULT,

		TEDDY_WORLD,
		HUMAN_WORLD
	};
	
public:
	//Big 4
	SSBO();
	SSBO(const SSBO &) = delete;
	const SSBO & operator = (const SSBO &) = delete;
	~SSBO();



	void Set(unsigned int elementCount, unsigned int size, void * inData);

	unsigned int GetLocation() const;
	void Bind(unsigned bindLocation);
	void BindRange(unsigned int bindLocation, int offset, int size);
	void *Map(SSBO::Access accessType);
	void Unmap();

public:
	GLuint ssbo;
	Name ssboName;
	unsigned int elementCount;
	unsigned int elementSize;
	void * pBuffer;
};

#endif

//--- End of File -------------------------------------------------------------
