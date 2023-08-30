//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef _2PYRAMID_MODEL_H
#define _2PYRAMID_MODEL_H

#include "Model.h"

class _2PyramidModel : public Model
{
public:
	_2PyramidModel(const char * const pModelFileName);

	_2PyramidModel() = delete;
	_2PyramidModel(const _2PyramidModel &) = delete;
	_2PyramidModel & operator = (_2PyramidModel &) = delete;
	virtual ~_2PyramidModel();

private:
	virtual void privCreateVAO(const char * const pModelFileName) override;

	// Data

};

#endif

// --- End of File ---------

