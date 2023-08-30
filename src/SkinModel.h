//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef  SKIN_MODEL_H
#define  SKIN_MODEL_H

#include "Model.h"
#include "Texture.h"
class SkinModel : public Model
{
public:
	SkinModel(const char* const pModelFileName, const char* const pWeightsFileName, Texture::Name Name = Texture::Name::DEFAULT);
	SkinModel() = delete;
	SkinModel(const SkinModel&) = delete;
	SkinModel& operator = (SkinModel&) = delete;

	virtual ~SkinModel();

	// Data
	virtual void privCreateVAO(const char* const pModelFileName) override;
};

#endif

// --- End of File ---------