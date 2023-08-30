#ifndef HIERARCHY_COMPUTE
#define HIERARCHY_COMPUTE

#include "Align16.h"
#include"PCSTree.h"
class SKLTN;
class GameObjectAnim;

class HierarchyCompute : public Align16
{
	public:
	HierarchyCompute() = delete;
	HierarchyCompute(HierarchyCompute& copyModel) = delete;
	HierarchyCompute& operator = (HierarchyCompute& copyModel) = delete;
	~HierarchyCompute() = default;

	HierarchyCompute(SKLTN* hierarchyRoot, GameObjectAnim* anim, int numBones, char* name , int &hierarchyDeapth);

	int findLevels(SKLTN* hierarchyRoot, int numbones);

	void setRow0(int maxDepth, int numBones);

	int getDepth(GameObjectAnim* tmp, char* name);

	int getDepth(SKLTN* skltn, int cuurIndex);

	//Data
	int* dataHierarchy;
};
#endif
