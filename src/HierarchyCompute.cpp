#include "HierarchyCompute.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectAnim.h"
#include "GraphicsObject_Null.h"
#include "PCSTree.h"
#include "AnimStructs.h"
#include "Framework.h"
#include "CubeModel.h"
#include "ModelMan.h"
#include "ShaderMan.h"

//BY BONE INDEX

//HierarchyCompute::HierarchyCompute(SKLTN* hierarchyRoot, GameObjectAnim* anim,
//									int numBones, char* name, int& hierarchyDeapth)
//{
//	int maxDepth = findLevels(hierarchyRoot, numBones);
//
//	hierarchyDeapth = maxDepth;
//
//	//Find the Hierarchy//Find the Hierarchy//Find the Hierarchy//Find the Hierarchy//Find the Hierarchy 
//	PCSTreeForwardIterator pIter(anim);
//
//	PCSNode* currNode;
//
//	//int* hierarchy = new int[numBones];
//	int i = 0;
//	int nodeNum = 0;
//	int prev = 0;
//	//Create Magic Table
//
//	this->dataHierarchy = new int[numBones * maxDepth];
//
//	for (int i = 0; i < numBones * maxDepth; i++) 
//	{
//		this->dataHierarchy[i] = 0x0;
//	}
//
//	while (!pIter.IsDone())
//	{
//		//Trace::out("\n CURRENT TABLE STATE \n");
//
//		int currDeapth = 0;
//
//		currNode = pIter.CurrentItem();
//
//		GameObjectAnim* pGameObj = (GameObjectAnim*)currNode;
//
//		GameObjectAnim* tmp;
//
//		tmp = (GameObjectAnim*)currNode;
//
//		//get depth of the current node
//		currDeapth = getDepth(tmp,name);
//
//
//		tmp = (GameObjectAnim*)currNode;
//
//		//set the first row in the magic table
//		if (tmp->indexBoneArray <= 0)
//		{
//			setRow0(maxDepth, numBones);
//		}
//		else
//		{
//			//set rows after the 0th row
//			tmp = (GameObjectAnim*)currNode;
//
//			int tmpIndex = currDeapth;
//
//			while (tmp->pName != name)
//			{
//				dataHierarchy[tmpIndex + (maxDepth * nodeNum)] = tmp->indexBoneArray;
//				
//				tmp = (GameObjectAnim*)tmp->GetParent();
//
//				tmpIndex--;
//			}
//
//			for (int l = currDeapth + 1; l < maxDepth; l++) 
//			{
//				dataHierarchy[l + (maxDepth * nodeNum)] = numBones;
//			}
//		}
//
//		for (int i = 0; i < numBones * maxDepth; i++)
//		{		
//			if (i % maxDepth == 0)
//			{
//				//if(i != 0)
//				//Trace::out("\n");
//			}
//
//			//Trace::out("%d \t", dataHierarchy[i]);
//		}
//		
//		//Trace::out("\n CURRENT TABLE STATE \n");
//		//Trace::out("\n");
//		//Trace::out("\n");
//
//		nodeNum++;
//
//		pIter.Next();
//
//	}
//}



//
//BY Keenan Example
HierarchyCompute::HierarchyCompute(SKLTN* hierarchyRoot, GameObjectAnim* anim,
	int numBones, char* name, int& hierarchyDeapth)
{

	SKLTN* hierarchyFiller = new SKLTN[numBones]();

	for (int i = 0; i < numBones; i++) 
	{
		hierarchyFiller[i] = hierarchyRoot[i];
	}

	int maxDepth = findLevels(hierarchyRoot, numBones);

	hierarchyDeapth = maxDepth;

	//Find the Hierarchy//Find the Hierarchy//Find the Hierarchy//Find the Hierarchy//Find the Hierarchy 
	PCSTreeForwardIterator pIter(anim);

	PCSNode* currNode;

	//int* hierarchy = new int[numBones];
	int i = 0;
	int nodeNum = 0;
	int Index = 0;
	//Create Magic Table

	this->dataHierarchy = new int[numBones * maxDepth];

	for (int i = 0; i < numBones * maxDepth; i++)
	{
		this->dataHierarchy[i] = 0x0;
	}

	i = 0;
	while (!pIter.IsDone())
	{
		Trace::out("\n CURRENT TABLE STATE \n");

		int currDeapth = 0;

		currNode = pIter.CurrentItem();

		GameObjectAnim* pGameObj = (GameObjectAnim*)currNode;

		GameObjectAnim* tmp;

		tmp = (GameObjectAnim*)currNode;

		//get depth of the current node
		currDeapth = getDepth(tmp, name);


		tmp = (GameObjectAnim*)currNode;

		//set the first row in the magic table
		if (tmp->indexBoneArray <= 0)
		{
			setRow0(maxDepth, numBones);
		}
		else
		{
			//set rows after the 0th row
			tmp = (GameObjectAnim*)currNode;

			//int tmpIndex = currDeapth;
			int parentIndex = 0;
			//int tmpIndex = 0;

			int curr = i;
			while (currDeapth != 0)
			{
				currDeapth = getDepth(hierarchyFiller,hierarchyFiller[i].currNodeIndex);

				parentIndex = hierarchyFiller[hierarchyFiller[i].currNodeIndex].parentNodeIndex;

				dataHierarchy[currDeapth + (maxDepth * nodeNum)] = hierarchyFiller[curr].currNodeIndex;

				curr = hierarchyFiller[curr].parentNodeIndex;

				//tmp = (GameObjectAnim*)tmp->GetParent();
				hierarchyFiller[i].currNodeIndex = hierarchyFiller[hierarchyFiller[i].currNodeIndex].parentNodeIndex;

				parentIndex--;

				//tmpIndex--;
			}

			for (int m = 0; m < numBones; m++)
			{
				hierarchyFiller[m] = hierarchyRoot[m];
			}

			currDeapth = getDepth(hierarchyFiller, hierarchyFiller[i].currNodeIndex);

			for (int l = currDeapth + 1; l < maxDepth; l++)
			{
				dataHierarchy[l + (maxDepth * nodeNum)] = numBones;
			}
		}

		for (int i = 0; i < numBones * maxDepth; i++)
		{
			if (i % maxDepth == 0)
			{
				if (i != 0)
					Trace::out("\n");
			}

			Trace::out("%d \t", dataHierarchy[i]);
		}

		Trace::out("\n CURRENT TABLE STATE \n");
		Trace::out("\n");
		Trace::out("\n");

		nodeNum++;

		i++;

		pIter.Next();
	}

	delete[] hierarchyFiller;
}

int HierarchyCompute::findLevels(SKLTN* hierarchyRoot, int numBones) 
{
	//LEVELS //LEVELS //LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS
		// Create the tree
	PCSTree* poRootTree = new PCSTree();
	assert(poRootTree);

	PCSNode* nodes = new PCSNode[numBones];

	for (int i = 1; i < numBones; i++)
	{
		poRootTree->Insert(&nodes[hierarchyRoot[i].currNodeIndex],
			&nodes[hierarchyRoot[i].parentNodeIndex]);
	}


	PCSTree::Info info;
	poRootTree->GetInfo(info);

	int maxDepth = info.maxNumLevels - 1;

	delete poRootTree;
	delete[] nodes;

	return maxDepth;

	//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS//LEVELS s
}

void HierarchyCompute::setRow0(int maxDepth, int numBones)
{
	dataHierarchy[0] = 0;
	for (int i = 1; i < maxDepth; i++)
	{
		dataHierarchy[i] = numBones;
	}
}

int HierarchyCompute::getDepth(GameObjectAnim* tmp,char* name)
{
	int currDeapth = -1;

	while (tmp->pName != name)
	{
		currDeapth++;

		tmp = (GameObjectAnim*)tmp->GetParent();
	}

	return currDeapth;
}

int HierarchyCompute::getDepth(SKLTN* skltn, int cuurIndex )
{
	int currDeapth = cuurIndex;
	int realDeapth =  0;

	while (currDeapth != 0)
	{
		currDeapth = skltn[cuurIndex].parentNodeIndex;
		cuurIndex = skltn[cuurIndex].parentNodeIndex;
		realDeapth++;
	}

	return realDeapth;
}