//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef  CUSTOM_MODEL_H
#define  CUSTOM_MODEL_H

#include "Model.h"
#include "Texture.h"
class CustomModel : public Model
{
public:
	CustomModel(const char* const pModelFileName, const Texture::Name Name = Texture::Name::DEFAULT);
	CustomModel() = delete;
	CustomModel(const CustomModel&) = delete;
	CustomModel& operator = (CustomModel&) = delete;
	
	virtual ~CustomModel();

	// Data
	virtual void privCreateVAO(const char* const pModelFileName) override;
};

#endif

// --- End of File ---------
