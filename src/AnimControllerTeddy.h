#ifndef ANIM_CONTROLLER_TEDDY
#define ANIM_CONTROLLER_TEDDY

#include "AnimController.h" 
#include "ClipMan.h"
#include "BoneWeight.h"
#include "SSBO.h"
class SkinModel;

class AnimControllerTeddy : public AnimController 
{
public:
	AnimControllerTeddy() = delete;
	AnimControllerTeddy(AnimControllerTeddy& copyModel) = delete;
	AnimControllerTeddy& operator = (AnimControllerTeddy& copyModel) = delete;
	~AnimControllerTeddy() = default;
	AnimControllerTeddy(PyramidModel* pPyramidModel, SkinModel* pSkinModel, ShaderObject* pShaderObject_textureLight, 
		ShaderObject* pShaderObject_SkinColor, InstancenName name);

	void ProcessAnimation(Clip* pClip, SSBO::Name nameKey, SSBO::Name nameResult) override;

	void SetAnimationPose() override;

	void SetBonePose(GameObjectAnim* node) override;

	void SetAnimationHierarchy(const char* const pModelFileName) override;

	void SetAnimationData(const char* const pModelFileName, const char* clipName) override;

	void SetInvPose(const char* const pModelFileName, const char* chunkName)override;
	
	void ComputeKeyFrame(SSBO::Name nameKey, SSBO::Name nameResult) override;
	
	void ComputeHierarchyTable(SSBO::Name tableName, SSBO::Name worldResult) override;

	//Data
	static const unsigned int NumBones = 20;
	static const unsigned int BoneWidth = 10;

	GraphicsObject_TextureLight* pGraphics_TextureLight[NumBones];
	GameObjectAnim* pBone[NumBones];
	Frame_Bucket* pFrameHead;
	Bone* bResult;
};

#endif