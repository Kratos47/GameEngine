//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef RITTER_SPHERE_MODEL_H
#define RITTER_SPHERE_MODEL_H

#include "Model.h"

class Ritter_SphereModel : public Model
{
public:
	Ritter_SphereModel( const char * const modelFileName );
	~Ritter_SphereModel();

	// tells the compiler do not create or allow it to be used, c++11
	Ritter_SphereModel(const Ritter_SphereModel&) = delete;
	Ritter_SphereModel& operator=(const Ritter_SphereModel& other) = delete;
private:
	void privCreateVAO( const char * const modelFileName  ) override;



};

#endif