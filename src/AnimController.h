 #ifndef ANIM_CONTROLLER
#define ANIM_CONTROLLER


#include "GameObjectRigid.h"
#include "GraphicsObject_Null.h"
#include "NullModel.h"
#include "GameObjectAnim.h"
#include "GraphicsObject_TextureLight.h"
#include "PCSNode.h"
#include "PCSTree.h"
#include "PyramidModel.h"
#include"AnimStructs.h"
#include "Clip.h"
#include "FrameBucket.h"
#include "SSBO.h"
//#include "GraphicsObject_SkinColor.h"
//#include "GameObjSkin.h"

class GameObjectSkin;
class GraphicsObject_SkinLight;
class Time;

class AnimController
{
public:
	AnimController() = default;
	AnimController(AnimController& copyModel) = delete;
	AnimController& operator = (AnimController& copyModel) = delete;
	~AnimController() = default;

	enum class InstancenName
	{
		TEDDY_01,
		TEDDY_02,
		HUMAN_01,
		HUMAN_02,
		DEFAULT
	};

	class Skeleton
	{
	public:
		PCSTree* pTree;
		PCSNode* root;
		GameObjectAnim* pFirstBone;
		GameObjectRigid* pGameRigid;
		NullModel* pNullModel;
		GraphicsObject_Null* pGameObjNull;
	};
	
	virtual void SetAnimationHierarchy(const char* const pModelFileName) = 0;

	virtual void ProcessAnimation(Clip* pClip, SSBO::Name nameKey, SSBO::Name nameResult) = 0;

	virtual void SetBonePose(GameObjectAnim* node)= 0;

	virtual void SetAnimationPose()= 0;

	virtual void SetAnimationData(const char* const pModelFileName, const char* clipName)= 0;

	virtual void SetInvPose(const char* const pModelFileName, const char* chunkName) = 0;
	
	virtual void ComputeKeyFrame(SSBO::Name nameKey, SSBO::Name nameResult) = 0;
	
	virtual void ComputeHierarchyTable(SSBO::Name tableName, SSBO::Name worldResult) = 0;



	void baseSetAnimationHierarchy(GameObjectAnim** pBone, const char* const pModelFileName);
	
	void baseProcessAnimation(Clip* pClip, Bone* bResult, unsigned int NumBones, SSBO::Name nameKey, SSBO::Name nameResult);
	void baseSetAnimationPose(const float BoneWidth);
	void baseSetBonePose(GameObjectAnim* node, const float BoneWidth);
	void baseSetAnimationData(const char* const pModelFileName, const char* clipName, Frame_Bucket* pFrameHead, unsigned int NumBones);
	void baseSetInvPose(const char* const pModelFileName, const char* chunkName, int NumBones);
	
	void baseComputeKeyFrame(int NumBones, SSBO::Name nameKey, SSBO::Name nameResult);
	
	void baseComputeHierarchy(SSBO::Name tableName, SSBO::Name worldResult, int numBones);

	AnimController::InstancenName charName2Enum(const char* name);
	void enumToStr(GameObjectRigid* pGameRigid, AnimController::InstancenName name);
	char* enumToStr(AnimController::InstancenName name);

	

	////Data
	// Note: these static should be tied into an animation system
	Time tCurrent;
	Skeleton* pSkeleton;	
	AnimController* pNext;
	AnimController* pPrev;
	InstancenName pName;
	Matrix* bonePoseInv;
	Matrix* currPose;
	GraphicsObject_SkinLight* pGraphicsObject_SkinLight;
	GameObjectSkin* pGameObjSkin;

	//Data For Magic table
	SKLTN* pSktl_data;
	int currAnimBones;
	int hierarchyDeapth;
};
#endif