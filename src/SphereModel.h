//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

#include "Model.h"

class SphereModel : public Model
{
public:
	SphereModel(const char * const pModelFileName);

	SphereModel() = delete;
	SphereModel(const SphereModel &) = delete;
	SphereModel & operator = (SphereModel &) = delete;
	virtual ~SphereModel();

private:
	virtual void privCreateVAO(const char * const pModelFileName) override;

	// Data

};

#endif

// --- End of File ---------
