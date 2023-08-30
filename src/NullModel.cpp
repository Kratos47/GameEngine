//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <sb7.h>
#include <assert.h>

#include "NullModel.h"


NullModel::NullModel(const char * const modelFileName)
	: Model()
{
	UNUSED_VAR(modelFileName);
}

NullModel::~NullModel()
{
	// remove anything dynamic here
}

void NullModel::privCreateVAO(const char * const modelFileName)
{
	UNUSED_VAR(modelFileName);
}

//--- End of File ------------------------------------------------------------


