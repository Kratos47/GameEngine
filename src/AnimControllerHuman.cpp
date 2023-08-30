#include "AnimControllerHuman.h"
#include "ClipMan.h"
#include "GameObject.h"
#include "GameObjectMan.h"
#include "ModelMan.h"
#include "FrameMan.h" 
#include"MathApp.h"
#include "PCSTreeForwardIterator.h"
#include "eat.h"
#include "ShaderMan.h"
#include"SkinModel.h"
#include "GraphicsObject_SkinLight.h"
#include"GameObjSkin.h"
AnimControllerHuman::AnimControllerHuman(PyramidModel* pPyramidModel,SkinModel* pSkinModel, ShaderObject* pShaderObject_textureLight, ShaderObject* pShaderObject_SkinColor, InstancenName name)
{
	pSkeleton = new Skeleton();

	this->pSkeleton->pTree = GameObjectMan::GetPCSTree();
	this->pSkeleton->root = this->pSkeleton->pTree->GetRoot();

	// Null object
	
	this->pSkeleton->pNullModel = new NullModel(0);
	this->pSkeleton->pGameObjNull = new GraphicsObject_Null(this->pSkeleton->pNullModel, pShaderObject_textureLight);
	this->pSkeleton->pGameRigid = new GameObjectRigid(this->pSkeleton->pGameObjNull);
	enumToStr(this->pSkeleton->pGameRigid,name);
	this->pName = name;
	GameObjectMan::Add(this->pSkeleton->pGameRigid, GameObjectMan::GetRoot());
	ModelMan::Add(this->pSkeleton->pNullModel);

	
	pGraphicsObject_SkinLight= new GraphicsObject_SkinLight(pSkinModel, pShaderObject_SkinColor, NumBones, this->pName);
	pGameObjSkin = new GameObjectSkin(pGraphicsObject_SkinLight);
	GameObjectMan::Add(pGameObjSkin, GameObjectMan::GetRoot());

	// Create GameObject
	Vect color;
	Vect pos(1, 1, 1);

	// Humanoid - from H_000.txt
	color.set(1.0f, 0.0f, 0.0f, 1.0f);
	for (unsigned int i = 0; i < NumBones; i++) 
	{
		this->pGraphics_TextureLight[i] = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
		pBone[i] = new GameObjectAnim(this->pGraphics_TextureLight[i]);
	}


}

void AnimControllerHuman::SetAnimationHierarchy(const char* const pModelFileName)
{
	baseSetAnimationHierarchy(pBone, pModelFileName);
}


void AnimControllerHuman::ProcessAnimation(Clip* pClip, SSBO::Name nameKey, SSBO::Name nameResult)
{
	baseProcessAnimation(pClip, this->bResult, NumBones, nameKey, nameResult);
}
// Keenan(27) - rework using iterator
void AnimControllerHuman::SetAnimationPose()
{
	baseSetAnimationPose(BoneWidth);
}
//
void AnimControllerHuman::SetBonePose(GameObjectAnim* node)
{
	baseSetBonePose(node, BoneWidth);
}

void AnimControllerHuman::SetAnimationData(const char* const pModelFileName, const char* clipName)
{
	baseSetAnimationData(pModelFileName, clipName, this->pFrameHead, NumBones);
}

void AnimControllerHuman::SetInvPose(const char* const pModelFileName, const char* chunkName)
{
	UNUSED_VAR(pModelFileName);
	UNUSED_VAR(chunkName);
	baseSetInvPose(pModelFileName, chunkName, NumBones);
}

void AnimControllerHuman::ComputeKeyFrame(SSBO::Name nameKey, SSBO::Name nameResult)
{
	baseComputeKeyFrame(NumBones,nameKey, nameResult);
}
void AnimControllerHuman::ComputeHierarchyTable(SSBO::Name tableName,SSBO::Name worldResult)
{
	baseComputeHierarchy(tableName, worldResult, NumBones);
}