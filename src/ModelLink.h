//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef MODEL_LINK_H
#define MODEL_LINK_H

class ModelLink
{
public:
	ModelLink()
	{
		this->next = nullptr;
		this->prev = nullptr;
	}

	virtual ~ModelLink()
	{

	}

	ModelLink *next;
	ModelLink *prev;
};

#endif

// --- End of File --------------------------------------------------
