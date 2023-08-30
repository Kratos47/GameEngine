#include "AnimController.h"
#include "eat.h"
#include "FrameMan.h"
#include "GameObjectMan.h"
#include "ModelMan.h"
#include "ClipMan.h"
#include "VectApp.h"
#include "PCSTreeForwardIterator.h"
#include "QuatApp.h"
#include "BoneWeight.h"
#include "BoneWeight.h"
#include "GraphicsObject_SkinLight.h"
#include"GameObjSkin.h"
#include <Trace.h>
#include "ShaderMan.h"
#include "ComputeBone.h"
#include "SSBO_MAN.h"
#include"HierarchyCompute.h"
void AnimController::baseSetAnimationHierarchy(GameObjectAnim** pBone,const char* const pModelFileName)
{
	//NEW CALLED
	char* Trunc_File_Name = new char[100];

	unsigned int i = 0;

	//Debugging varialble
	unsigned  int strSize = strlen(pModelFileName);

	i = 0;

	while (i < strSize - 4)
	{
		Trunc_File_Name[i] = pModelFileName[i];

		if (i == strSize - 5)
		{
			Trunc_File_Name[i + 1] = (char)0;
		}

		i++;
	}

	//NEW CALLED
	char* anim_Chunk = new char[100];
	strcpy(anim_Chunk, Trunc_File_Name);
	strcat(anim_Chunk, "_Skltn");

	unsigned char* skltnChunkBuff;
	unsigned int chunkSize;

	eat(pModelFileName, ChunkType::ANIM_TYPE, anim_Chunk, skltnChunkBuff, chunkSize);

	// create a header
	SKLTN modelHdr;

	memcpy(&modelHdr, &skltnChunkBuff[0], sizeof(SKLTN));

	////Set Bones number
	FrameMan* pAnimMan = FrameMan::privGetInstance();
	assert(pAnimMan);

	unsigned int numBones = modelHdr.NumBones;

	//Hrad coded for testing 
	pSktl_data = new SKLTN[numBones];

	//// Read verts
	memcpy(&pSktl_data[0], &skltnChunkBuff[sizeof(SKLTN)], sizeof(SKLTN) * modelHdr.NumBones);


	i = 0;

 	GameObjectRigid* pGameRigid = this->pSkeleton->pGameRigid;

	pBone[i]->SetIndex(pSktl_data[i].currNodeIndex);
	pBone[i]->SetName(pSktl_data[i].nodeName);
	pBone[i]->SetDrawEnable(false);  // Suppress visual on root bone skeleton
	GameObjectMan::Add(pBone[i], pGameRigid);

	i++;

	for (i = 1; i < modelHdr.NumBones; i++)
	{
		if (i == 1)
		{
			pBone[i]->SetIndex(pSktl_data[i].currNodeIndex);
			pBone[i]->SetName(pSktl_data[i].nodeName);
			pBone[i]->SetDrawEnable(false);  // Suppress visual on root bone skeleton
			GameObjectMan::Add(pBone[pSktl_data[i].currNodeIndex], pBone[pSktl_data[i].parentNodeIndex]);
		}
		else if (i > 1)
		{
			pBone[i]->SetIndex(pSktl_data[i].currNodeIndex);
			pBone[i]->SetName(pSktl_data[i].nodeName);
			//pBone[i]->SetDrawEnable(false);  // Suppress visual on root bone skeleton
			GameObjectMan::Add(pBone[pSktl_data[i].currNodeIndex], pBone[pSktl_data[i].parentNodeIndex]);
		}
	}

	// Note the root is set
	this->pSkeleton->pFirstBone = pBone[0];
	//this->pSkeleton->pTree->Print();

	delete skltnChunkBuff;
	delete[] anim_Chunk;
	delete[] Trunc_File_Name;

}

void AnimController::baseComputeHierarchy(SSBO::Name tableName, SSBO::Name worldResult, int numBones)
{
	

	// ------------------------------------------------
	//  Compute World Shader Test
	// ------------------------------------------------

	GameObjectAnim* anim = (GameObjectAnim*)this->pSkeleton->pGameRigid->GetChild();

	HierarchyCompute* hierarchy = new HierarchyCompute(pSktl_data,
		anim,
		numBones, anim->GetParent()->pName, this->hierarchyDeapth);
	


	delete[] pSktl_data;

	SSBO* pSBO_HierarchyTable = new SSBO();

	//set the name
	pSBO_HierarchyTable->ssboName = tableName;


	pSBO_HierarchyTable->Set(hierarchyDeapth*numBones, sizeof(int), hierarchy->dataHierarchy);


	// Block Waiting for stages to complete
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//pSBO_HierarchyTable->Set(hierarchyDeapth*numBones, sizeof(int), hierarchy);

	SSBOMan::Add(pSBO_HierarchyTable);

	this->currAnimBones = numBones;

	SSBO *result= new SSBO();

	// Result bone array
	Matrix* data = new Matrix[numBones]();
	assert(data);

	//set the name
	result->ssboName = worldResult;

	result->Set(numBones , sizeof(Matrix), &data);

	SSBOMan::Add(result);

	delete[]  data;
	delete[]  hierarchy->dataHierarchy;
	delete[]  hierarchy;
}

void AnimController::baseProcessAnimation(Clip* pClip, Bone* bResult, unsigned int NumBones,
	SSBO::Name nameKey, SSBO::Name nameResult)
{

	FrameMan* pFrameMan = FrameMan::privGetInstance();
	assert(pFrameMan);

	ClipMan* pClipMan = ClipMan::privGetInstance();
	assert(pClipMan);


	Frame_Bucket* pHead = pClipMan->active->pFrameHead;

	Frame_Bucket* pTmp = pHead->nextBucket;

	pClipMan->active = pClip;

	pHead = pClip->pFrameHead;
	pTmp = pHead->nextBucket;


	// Get the result bone array
	// Remember the firs Frame is the result
	bResult = pHead->pBone;

	// First one is the result, skip it
	pTmp = pHead->nextBucket;



	// Find which key frames
	int index = -2;
	while (tCurrent >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
	{
		index++;
		pTmp = pTmp->nextBucket;
	}

	// pTmp is the "B" key frame
	// pTmp->prev is the "A" key frame
	Frame_Bucket* pA = pTmp->prevBucket;
	Frame_Bucket* pB = pTmp;


	int indexA = index;
	int indexB = index + 1;

	// find the "S" of the time
	float tS = (tCurrent - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);


	// At this point We have indexA, indexB, tS, and result
	ALIGN_UNUSED_VAR(indexA);
	ALIGN_UNUSED_VAR(indexB);
	ALIGN_UNUSED_VAR(tS);
	ALIGN_UNUSED_VAR(bResult);

	Trace::out("\n");
	//tS = 1.00f;
	//Trace::out(" index %d  %d  ts:%f \n", indexA, indexB, tS);

	ShaderMan* inst = ShaderMan::privGetInstance();

	ShaderObject* pComputeShader = inst->Find(ShaderObject::Name::COMPUTE_MIXER);
	pComputeShader->SetActive();

	// Now bind the data (sbo's)
	// Since it is already on GPU... we can now just bind it

	SSBOMan* instSSBO = SSBOMan::privGetInstance();

	SSBO* pSBO_KeyFrameData = instSSBO->Find(nameKey);

	pSBO_KeyFrameData->BindRange(0, (int)(indexA * NumBones * sizeof(Bone)), NumBones  * sizeof(Bone));
	pSBO_KeyFrameData->BindRange(1, (int)(indexB * NumBones * sizeof(Bone)), NumBones  * sizeof(Bone));

	SSBO* pSBO_BoneResult = instSSBO->Find(nameResult);

	pSBO_BoneResult->Bind(2);



	int numBones = NumBones;
	glUniform1f(pComputeShader->GetLocation("ts"), tS);
	glUniform1i(pComputeShader->GetLocation("boneCount"), numBones);


	//detrmine dispatch count
	if (nameKey == SSBO::Name::TEDDY_IDLE_KEY|| nameKey == SSBO::Name::TEDDY_WALK_KEY)
	{
		// Kick it... start the compute 
		pComputeShader->Dispatch(1, 1, 1);
	}
	else if(nameKey == SSBO::Name::HUMAN_RUN_KEY|| nameKey == SSBO::Name::HUMAN_WALK_KEY)
	{
 	// Kick it... start the compute shader
		pComputeShader->Dispatch(4, 1, 1);
	}

	// Block Waiting for stages to complete
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);	
}

void redudndant()
{



	///////HHHARD coding
	//this->hierarchyDeapth = 9;

	//int *hierarchy= new int[180];

	//for (int i = 0; i < 180; i++) 
	//{
	//	hierarchy[i] = 20;
	//}

	////1
	//hierarchy[0] = 0;
	//hierarchy[1] = 20;
	//hierarchy[2] = 20;
	//hierarchy[3] = 20;
	//hierarchy[4] = 20;
	//hierarchy[5] = 20;
	//hierarchy[6] = 20;
	//hierarchy[7] = 20;
	//hierarchy[8] = 20;

	////2
	//hierarchy[9] = 0;
	//hierarchy[10] = 1;
	//hierarchy[11] = 20;
	//hierarchy[12] = 20;
	//hierarchy[13] = 20;
	//hierarchy[14] = 20;
	//hierarchy[15] = 20;
	//hierarchy[16] = 20;
	//hierarchy[17] = 20;

	////3
	//hierarchy[18] = 0;
	//hierarchy[19] = 1;
	//hierarchy[20] = 2;
	//hierarchy[21] = 20;
	//hierarchy[22] = 20;
	//hierarchy[23] = 20;
	//hierarchy[24] = 20;
	//hierarchy[25] = 20;
	//hierarchy[26] = 20;

	////4
	//hierarchy[27] = 0;
	//hierarchy[28] = 1;
	//hierarchy[29] = 2;
	//hierarchy[30] = 3;
	//hierarchy[31] = 20;
	//hierarchy[32] = 20;
	//hierarchy[33] = 20;
	//hierarchy[34] = 20;
	//hierarchy[35] = 20;

	////5
	//hierarchy[36] = 0;
	//hierarchy[37] = 1;
	//hierarchy[38] = 2;
	//hierarchy[39] = 3;
	//hierarchy[40] = 4;
	//hierarchy[41] = 20;
	//hierarchy[42] = 20;
	//hierarchy[43] = 20;
	//hierarchy[44] = 20;

	////6
	//hierarchy[45] = 0;
	//hierarchy[46] = 1;
	//hierarchy[47] = 2;
	//hierarchy[48] = 3;
	//hierarchy[49] = 4;
	//hierarchy[50] = 5;
	//hierarchy[51] = 20;
	//hierarchy[52] = 20;
	//hierarchy[53] = 20;
	//
	////7
	//hierarchy[54] = 0;
	//hierarchy[55] = 1;
	//hierarchy[56] = 2;
	//hierarchy[57] = 6;
	//hierarchy[58] = 20;
	//hierarchy[59] = 20;
	//hierarchy[60] = 20;
	//hierarchy[61] = 20;
	//hierarchy[62] = 20;
	//
	////8
	//hierarchy[63] = 0;
	//hierarchy[64] = 1;
	//hierarchy[65] = 2;
	//hierarchy[66] = 6;
	//hierarchy[67] = 7;
	//hierarchy[68] = 20;
	//hierarchy[69] = 20;
	//hierarchy[70] = 20;
	//hierarchy[71] = 20;
	//
	////9
	//hierarchy[72] = 0;
	//hierarchy[73] = 1;
	//hierarchy[74] = 2;
	//hierarchy[75] = 6;
	//hierarchy[76] = 7;
	//hierarchy[77] = 8;
	//hierarchy[78] = 20;
	//hierarchy[79] = 20;
	//hierarchy[80] = 20;
	//
	////10
	//hierarchy[81] = 0;
	//hierarchy[82] = 1;
	//hierarchy[83] = 2;
	//hierarchy[84] = 9;
	//hierarchy[85] = 20;
	//hierarchy[86] = 20;
	//hierarchy[87] = 20;
	//hierarchy[88] = 20;
	//hierarchy[89] = 20;
	//
	////11
	//hierarchy[90] = 0;
	//hierarchy[91] = 1;
	//hierarchy[92] = 2;
	//hierarchy[93] = 9;
	//hierarchy[94] = 20;
	//hierarchy[95] = 20;
	//hierarchy[96] = 20;
	//hierarchy[97] = 20;
	//hierarchy[98] = 20;
	//
	////12
	//hierarchy[99]  = 0;
	//hierarchy[100] = 1;
	//hierarchy[101] = 2;
	//hierarchy[102] = 9;
	//hierarchy[103] = 10;
	//hierarchy[104] = 20;
	//hierarchy[105] = 20;
	//hierarchy[106] = 20;
	//hierarchy[107] = 20;

	////13
	//hierarchy[108] = 0;
	//hierarchy[109] = 1;
	//hierarchy[110] = 2;
	//hierarchy[111] = 9;
	//hierarchy[112] = 10;
	//hierarchy[113] = 11;
	//hierarchy[114] = 20;
	//hierarchy[115] = 20;
	//hierarchy[116] = 20;

	////14
	//hierarchy[117] = 0;
	//hierarchy[118] = 1;
	//hierarchy[119] = 2;
	//hierarchy[120] = 9;
	//hierarchy[121] = 10;
	//hierarchy[122] = 11;
	//hierarchy[123] = 12;
	//hierarchy[124] = 20;
	//hierarchy[125] = 20;
	//
	////15
	//hierarchy[126] = 0;
	//hierarchy[127] = 1;
	//hierarchy[128] = 2;
	//hierarchy[129] = 9;
	//hierarchy[130] = 10;
	//hierarchy[131] = 11;
	//hierarchy[132] = 12;
	//hierarchy[133] = 13;
	//hierarchy[134] = 20;
	//
	////16
	//hierarchy[135] = 0;
	//hierarchy[136] = 1;
	//hierarchy[137] = 2;
	//hierarchy[138] = 9;
	//hierarchy[139] = 10;
	//hierarchy[140] = 11;
	//hierarchy[141] = 12;
	//hierarchy[142] = 13;
	//hierarchy[143] = 14;
	//
	////17
	//hierarchy[144] = 0;
	//hierarchy[145] = 1;
	//hierarchy[146] = 2;
	//hierarchy[147] = 9;
	//hierarchy[148] = 10;
	//hierarchy[149] = 15;
	//hierarchy[150] = 20;
	//hierarchy[151] = 20;
	//hierarchy[152] = 20;
	//
	////18
	//hierarchy[153] = 0;
	//hierarchy[154] = 1;
	//hierarchy[155] = 2;
	//hierarchy[156] = 9;
	//hierarchy[157] = 10;
	//hierarchy[158] = 15;
	//hierarchy[159] = 16;
	//hierarchy[160] = 20;
	//hierarchy[161] = 20;
	//
	////19
	//hierarchy[162] = 0;
	//hierarchy[163] = 1;
	//hierarchy[164] = 2;
	//hierarchy[165] = 9;
	//hierarchy[166] = 10;
	//hierarchy[167] = 15;
	//hierarchy[168] = 16;
	//hierarchy[169] = 17;
	//hierarchy[170] = 20;
	//
	////20
	//hierarchy[171] = 0;
	//hierarchy[172] = 1;
	//hierarchy[173] = 2; 
	//hierarchy[174] = 9;
	//hierarchy[175] = 10;
	//hierarchy[176] = 15;
	//hierarchy[177] = 16;
	//hierarchy[178] = 17;
	//hierarchy[179] = 18;

	///////HHHARD coding
	//// Just for debugging purposes....
//// let's look at the data

//Bone* pResultBones = (Bone*)pSBO_BoneResult->Map(SSBO::Access::READ_ONLY);

//// Copy the data back to CPU
////    ---> BAD this is just a hack... don't do this
////         ONLY to prove the shader

//for (int i = 0; i < NumBones; i++)
//{
//	bResult[i] = pResultBones[i];
//}


//pSBO_BoneResult->Unmap();






//UNUSED_VAR(nameKey);
//UNUSED_VAR(nameResult);

//FrameMan* pAnimMan = FrameMan::privGetInstance();
//assert(pAnimMan);

//ClipMan* pClipMan = ClipMan::privGetInstance();
//assert(pClipMan);

//Frame_Bucket* pHead = pClipMan->active->pFrameHead;

//Frame_Bucket* pTmp = pHead->nextBucket;

//pClipMan->active = pClip;

//pHead = pClip->pFrameHead;
//pTmp = pHead->nextBucket;


//// Get the result bone array
//// Remember the firs Frame is the result
//bResult = pHead->pBone;

//// First one is the result, skip it
//pTmp = pHead->nextBucket;

//// Find which key frames
//while (tCurrent >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
//{
//	pTmp = pTmp->nextBucket;
//}

//// pTmp is the "B" key frame
//// pTmp->prev is the "A" key frame
//Frame_Bucket* pA = pTmp->prevBucket;
//Frame_Bucket* pB = pTmp;

//// find the "S" of the time
//float tS = (tCurrent - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

//// interpolate to 
//Bone* bA = pA->pBone;
//Bone* bB = pB->pBone;

//// Interpolate to tS time, for all bones
//for (unsigned int i = 0; i < NumBones; i++)
//{
//	// interpolate ahoy!
//	VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
//	QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
//	VectApp::Lerp(bResult->S, bA->S, bB->S, tS);

//	// advance the pointer
//	bA++;
//	bB++;
//	bResult++;
//}









//bResult = FrameBucketMan::GetActive()->poBone;

////// interpolate to 
//Bone* bA = pA->poBone;
//Bone* bB = pB->poBone;

//// Interpolate to tS time, for all bones
//for (int i = 0; i < NUM_BONES; i++)
//{
//	// interpolate ahoy!
//	VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
//	QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
//	VectApp::Lerp(bResult->S, bA->S, bB->S, tS);

//	// advance the pointer
//	bA++;
//	bB++;
//	bResult++;
//}

//bResult = FrameBucketMan::GetActive()->poBone;
//for (int i = 0; i < NUM_BONES; i++)
//{ 
//	Trace::out("\n");
//	Trace::out("i:%d ---------------\n", i);

//	Trace::out("\n");
//	Trace::out("cpuResult:\n");
//	Trace::out("      T: %f  %f  %f  %f \n", bResult->T[x], bResult->T[y], bResult->T[z], bResult->T[w]);
//	Trace::out("      Q: %f  %f  %f  %f \n", bResult->Q[x], bResult->Q[y], bResult->Q[z], bResult->Q[w]);
//	Trace::out("      S: %f  %f  %f  %f \n", bResult->S[x], bResult->S[y], bResult->S[z], bResult->S[w]);

//	Trace::out("\n");
//	Trace::out("computeResult:\n");
//	Trace::out("      T: %f  %f  %f  %f \n", pResultBones->T[x], pResultBones->T[y], pResultBones->T[z], pResultBones->T[w]);
//	Trace::out("      Q: %f  %f  %f  %f \n", pResultBones->Q[x], pResultBones->Q[y], pResultBones->Q[z], pResultBones->Q[w]);
//	Trace::out("      S: %f  %f  %f  %f \n", pResultBones->S[x], pResultBones->S[y], pResultBones->S[z], pResultBones->S[w]);

//	bResult++;
//	pResultBones++;
//}
}


// Keenan(27) - rework using iterator
void AnimController::baseSetAnimationPose(const float BoneWidth)
{
	//// Keenan(31)

	//PCSTreeForwardIterator pIter(this->pSkeleton->pFirstBone);
	//PCSNode* pNode = pIter.First();
	//GameObjectAnim* pGameObj = 0;

	////int ctr_i = 0;

	//// walks the anim node does the pose for everything that
	//while (pNode != 0)
	//{
	//	assert(pNode);
	//	// Update the game object
	//	pGameObj = (GameObjectAnim*)pNode;

	//	AnimController::baseSetBonePose(pGameObj, BoneWidth);

	//	pNode = pIter.Next();
	//
	//}
}
void AnimController::baseComputeKeyFrame(int NumBones, SSBO::Name nameKey, SSBO::Name nameResult)
{

	// ------------------------------------------------
	//  Compute MIxer Shader Test
	// ------------------------------------------------

	// Quick HACK - copy the bone data into ONE SBO array

	ClipMan* inst = ClipMan::privGetInstance();

	ComputeBone* pComputeKeyFrameBone = new ComputeBone(inst->active->pClipFrames, NumBones);
	assert(pComputeKeyFrameBone);

	// Result bone array
	Bone* pComputeBoneResult = new Bone[NumBones]();
	assert(pComputeBoneResult);

	// Transfer data (Shader Buffer Object) to GPU

	SSBO* pSBO_KeyFrameData = new SSBO();

	//set the name
	pSBO_KeyFrameData->ssboName = nameKey;

	pSBO_KeyFrameData->Set(pComputeKeyFrameBone->boneCount * pComputeKeyFrameBone->numFrames,
		sizeof(Bone),
		pComputeKeyFrameBone->pBoneData);
	SSBOMan::Add(pSBO_KeyFrameData);

	SSBOMan* instSSBOMan = SSBOMan::privGetInstance();

	SSBO* pSBO_BoneResult = new SSBO();

	//set the name
	pSBO_BoneResult->ssboName = nameResult;

	pSBO_BoneResult->Set(NumBones , sizeof(Bone), pComputeBoneResult);

	SSBOMan::Add(pSBO_BoneResult);


	delete pComputeKeyFrameBone;
	delete[] pComputeBoneResult;
}

void AnimController::baseSetBonePose(GameObjectAnim* node, const float BoneWidth)
{
	//// Now get the world matrices
	//// getting the parent from current node
	//GameObjectAnim* childNode = (GameObjectAnim*)node;
	////GameObjectAnim *parentNode = (GameObjectAnim *)node->GetParent();

	//GameObjectAnim* parentNode;
	//if (node->GetParent()->GetParent()->GetParent() != nullptr)
	//	parentNode = (GameObjectAnim*)node->GetParent();
	//else
	//	parentNode = nullptr;

	//if (parentNode == this->pSkeleton->root)
	//{
	//	return;
	//}

	//if (parentNode != 0 && childNode != 0)
	//{
	//	// starting point
	//	Vect start(0.0f, 0.0f, 0.0f);

	//	//  At this point, Find the two bones initial positions in world space
	//	//  Now get the length and directions

	//	Vect ptA = start * *parentNode->GetWorld();
	//	Vect ptB = start * *childNode->GetWorld();

	//	// direction between the anchor points of the respective bones
	//	Vect dir = (ptA - ptB);

	//	// length of the bone 0
	//	float mag = dir.mag();

	//	Matrix S(Matrix::Scale::XYZ, BoneWidth, BoneWidth, mag);
	//	Quat Q(Quat::Orient::LocalToWorld, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
	//	Matrix T(Matrix::Trans::XYZ, ptB);

	//	Matrix BoneOrient = S * Q * T;

	//	childNode->SetBoneOrientation(BoneOrient);
	//}
}

void AnimController::baseSetAnimationData(const char* const pModelFileName, const char* clipName, Frame_Bucket* pFrameHead,unsigned int NumBones)
{
	FrameMan* pAnimMan = FrameMan::privGetInstance();
	assert(pAnimMan);

	//NEW CALLED
	char* Trunc_File_Name = new char[100];

	unsigned int i = 0;

	//Debugging varialble
	unsigned  int strSize = strlen(pModelFileName);

	i = 0;

	while (i < strSize - 4)
	{
		Trunc_File_Name[i] = pModelFileName[i];

		if (i == strSize - 5)
		{
			Trunc_File_Name[i + 1] = (char)0;
		}

		i++;
	}

	//NEW CALLED
	char* clip_Chunk = new char[100];

	memset(clip_Chunk, 0x0,100);

	if (strcmp(pModelFileName, "H_R_F.spu") != 0)
	{
		strcpy(clip_Chunk, Trunc_File_Name);
		strcat(clip_Chunk, "_");
	}
	strcat(clip_Chunk, clipName);
	strcat(clip_Chunk, "_Clip");

	unsigned char* clipChunkBuff;
	unsigned int chunkSize;

	eat(pModelFileName, ChunkType::ANIM_TYPE, clip_Chunk, clipChunkBuff, chunkSize);

	ChunkHeader    chnkHdr;
	memcpy(&chnkHdr, &clipChunkBuff[0], sizeof(ChunkHeader));

	// create a header
	ClipHdr modelHdr;

	memcpy(&modelHdr, &clipChunkBuff[0], sizeof(ClipHdr));

	unsigned int clipSize = modelHdr.clipEndIndex - modelHdr.clipStartIndex;

	//Hrad coded for testing 
	ClipData* pClip_data = new ClipData[clipSize];

	//// Read verts
	memcpy(&pClip_data[0], &clipChunkBuff[sizeof(ClipHdr)], sizeof(ClipData) * clipSize);


	// --------  Result Frame  ----------------------

	pFrameHead = new Frame_Bucket();
	pFrameHead->prevBucket = 0;
	pFrameHead->nextBucket = 0;
	pFrameHead->KeyTime = Time(Time::ZERO);
	pFrameHead->pBone = new Bone[NumBones];

	// --------  Frame 0  ----------------------------

	const unsigned int numFrames = modelHdr.frameCount;

	Frame_Bucket* pTmp = new Frame_Bucket[numFrames];

	i = 0;

	unsigned int pClip_data_ctr = 0;
	//pTmp[i] = new Frame_Bucket();
	pTmp[i].prevBucket = pFrameHead;
	pTmp[i].nextBucket = 0;
	pTmp[i].KeyTime = 0 * Time(Time::NTSC_30_FRAME);
	pTmp[i].pBone = new Bone[NumBones];
	pFrameHead->nextBucket = &pTmp[i];


	for (unsigned int j = 0; j < NumBones; j++)
	{
		pTmp[i].pBone[j].T = pClip_data[pClip_data_ctr].trans;
		pTmp[i].pBone[j].Q = pClip_data[pClip_data_ctr].rot;
		pTmp[i].pBone[j].S = pClip_data[pClip_data_ctr].scale;
		pClip_data_ctr++;
	}

	int pClip_frame_ctr = 0;

	i++;

	for (i = 1; i < numFrames; i++)
	{
		//pTmp[i] = new Frame_Bucket();
		pTmp[i].prevBucket = &pTmp[i - 1];
		pTmp[i].nextBucket = 0;
		pTmp[i].KeyTime = pClip_frame_ctr * Time(Time::NTSC_30_FRAME);
		pTmp[i].pBone = new Bone[NumBones];
		pTmp[i - 1].nextBucket = &pTmp[i];

		pClip_frame_ctr++;

		for (unsigned int j = 0; j < NumBones; j++)
		{
			pTmp[i].pBone[j].T = pClip_data[pClip_data_ctr].trans;
			pTmp[i].pBone[j].Q = pClip_data[pClip_data_ctr].rot;
			pTmp[i].pBone[j].S = pClip_data[pClip_data_ctr].scale;
			pClip_data_ctr++;
		}
	}

	delete[] Trunc_File_Name;
	delete[] clip_Chunk;
	delete[] clipChunkBuff;
	delete[] pClip_data;

	ClipMan* pClipMan = ClipMan::privGetInstance();
	assert(pClipMan);

	Time maxTime = (float)(modelHdr.totalTime - 33) * Time(Time::ONE_MILLISECOND);

	///Create Clip and add to manager
	Clip* pClip = new Clip(pFrameHead, pTmp, modelHdr.clipName, maxTime);

	ClipMan::Add(pClip);

	////ForDeletion most probably a hack
	pClipMan->active->pFrameHead = pFrameHead;
}

void AnimController::baseSetInvPose(const char* const pModelFileName, const char* chunkName, int NumBones)
{
	//NEW CALLED
	char* Trunc_File_Name = new char[100];

	unsigned int i = 0;

	//Debugging varialble
	unsigned  int strSize = strlen(pModelFileName);

	i = 0;

	while (i < strSize - 4)
	{
		Trunc_File_Name[i] = pModelFileName[i];

		if (i == strSize - 5)
		{
			Trunc_File_Name[i + 1] = (char)0;
		}

		i++;
	}




	//NEW CALLED
	char* clip_Chunk = new char[100];
	strcpy(clip_Chunk, chunkName);
	strcat(clip_Chunk, "_inv");


	unsigned char* poseInvChunkBuff;
	unsigned int chunkSize;

	eat(pModelFileName, ChunkType::ANIM_TYPE, clip_Chunk, poseInvChunkBuff, chunkSize);

	ChunkHeader    chnkHdr;
	memcpy(&chnkHdr, &poseInvChunkBuff[0], sizeof(ChunkHeader));

	// create a header
	InvBoneP modelHdr;

	memcpy(&modelHdr, &poseInvChunkBuff[0], sizeof(InvBoneP));


	//Hrad coded for testing 
	bonePoseInv = new Matrix[NumBones];


	//initialize current pose
	currPose = new Matrix[NumBones];

	//// Read verts
	memcpy(&bonePoseInv[0], &poseInvChunkBuff[sizeof(InvBoneP)], sizeof(Matrix) * NumBones);


	this->pGraphicsObject_SkinLight->InvBonePose = bonePoseInv;

	delete[] Trunc_File_Name;
	delete[] poseInvChunkBuff;
	delete[] clip_Chunk;
}



AnimController::InstancenName AnimController::charName2Enum(const char* name)
{
	if (strcmp(name, "Human_01") == 0)
	{
		return AnimController::InstancenName::HUMAN_01;
	}
	else if (strcmp(name, "Teddy_01") == 0)
	{
		return AnimController::InstancenName::TEDDY_01;
	}
	return  AnimController::InstancenName::DEFAULT;
}

void AnimController::enumToStr(GameObjectRigid* pGameRigid, AnimController::InstancenName name)
{
	if (name == AnimController::InstancenName::HUMAN_01)
	{
		pGameRigid->SetName("Human_01");
	}
	else if (name == AnimController::InstancenName::TEDDY_01)
	{
		pGameRigid->SetName("Teddy_01");
	}
}

char* AnimController::enumToStr(AnimController::InstancenName name)
{
	if (name == AnimController::InstancenName::HUMAN_01)
	{
		return "Human_01";
	}
	else if (name == AnimController::InstancenName::TEDDY_01)
	{
		return "Teddy_01";
	}
	return 0;
}
