//#include "SkeletonAnimation.h"
//
//// Root does not have a parent
//void Skeleton::startUp()
//{
//	Skeleton skel;
//
//	skel.modelTxArr[0] = skel.localTxArr[0];
//	skel.invBindTxArr[0] = inverse(skel.modelTxArr[0]);
//
//// Iterate the reest if the bones*/
//	for (int i = 1; i < BONE_COUNT; i++)
//	{
//		int& parentIndex = skel.parentArr[i];
//		float4x4& parentModelTx = skel.modelTxArr[parentIndex];
//
//		// modelTx changes the base from model space 
//		// to the base of the bone in the current pose.
//		skel.modelTxArr[i] = parentModelTx * skel.localTxArr[i];
//
//		// invBinxTx changees to base of the bone 
//		// IN BIND POSE to the model space.
//		skel.invBindTxArr[i] = inverse(skel.modelTxArr[i]);
//	};
//}
//
//void Skeleton::perFrame()
//{
//	Skeleton skel;
//
//	// Per Frame
//	// Updates localTxArr based on time
//	//interpolate(skel, time); // (kolla tidigare slides)
//
//	//updateFinals(skel);
//
//	// Uploades finalTxArr to the GPU
//	// uploadToMatrixBuffer(skel.finalTxArr, BONE_COUNT);
//
//	skel.modelTxArr[0] = skel.localTxArr[0];
//	skel.modelTxArr[0] = skel.modelTxArr[0] * skel.invBindTxArr[0];
//
//	// Iterate the rest of the bones
//	for (int i = 1; i < BONE_COUNT; i++)
//	{
//		int& parentIndex = skel.parentArr[i];
//		float4x4& parentModelTx = skel.modelTxArr[parentIndex];
//
//		// modelTx changes the base from modele space 
//		// to the base of the bone in the current pose
//		skel.modelTxArr[i] = parentModelTx * skel.localTxArr[i];
//
//		// finalTxArr transforms a vertex in a model space to the 
//		// currently animated pose, also in model space
//		skel.finalTxArr[i] = skel.modelTxArr[i] * skel.invBindTxArr[i];
//	}
//
//}
