#ifndef ANIM_CONTROLLER_HUMAN
#define ANIM_CONTROLLER_HUMAN

#include "AnimController.h" 
#include "ClipMan.h"
#include "BoneWeight.h"

class SkinModel;

class AnimControllerHuman : public AnimController
{
public:
	AnimControllerHuman() = delete;
	AnimControllerHuman(AnimControllerHuman& copyModel) = delete;
	AnimControllerHuman& operator = (AnimControllerHuman& copyModel) = delete;
	~AnimControllerHuman() = default;
	AnimControllerHuman(PyramidModel* pPyramidModel, SkinModel* pSkinModel, ShaderObject* pShaderObject_textureLight, 
		ShaderObject* pShaderObject_SkinColor, InstancenName name);

	void ProcessAnimation(Clip* pClip, SSBO::Name nameKey, SSBO::Name nameResult) override;

	void SetAnimationPose() override;

	void SetBonePose(GameObjectAnim* node)override;

	void SetAnimationHierarchy(const char* const pModelFileName)override;

	void SetAnimationData(const char* const pModelFileName, const char* clipName)override;

	void SetInvPose(const char* const pModelFileName, const char* chunkName)override;
	
	void ComputeKeyFrame(SSBO::Name nameKey, SSBO::Name nameResult);

	void ComputeHierarchyTable(SSBO::Name tableName, SSBO::Name worldResult) override;

	//Data
	static const unsigned int NumBones = 80;
	static const unsigned int BoneWidth = 10;

	GameObjectAnim* pBone[NumBones];
	GraphicsObject_TextureLight* pGraphics_TextureLight[NumBones];
	Frame_Bucket* pFrameHead;
	Bone* bResult;
};

#endif