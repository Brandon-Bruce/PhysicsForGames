#ifndef RAGDOLL_H
#define RAGDOLL_H

#include <PxPhysicsAPI.h>
using namespace physx;





class Ragdoll
{
public:
	enum RagDollParts
	{
		NO_PARENT = -1,
		LOWER_SPINE,
		LEFT_PELVIS,
		RIGHT_PELVIS,
		LEFT_UPPER_LEG,
		RIGHT_UPPER_LEG,
		LEFT_LOWER_LEG,
		RIGHT_LOWER_LEG,
		UPPER_SPINE,
		LEFT_CLAVICLE,
		RIGHT_CLAVICLE,
		NECK,
		HEAD,
		LEFT_UPPER_ARM,
		RIGHT_UPPER_ARM,
		LEFT_LOWER_ARM,
		RIGHT_LOWER_ARM,
	};


	//Axis for use with quaternions
	static const PxVec3 X_AXIS;
	static const PxVec3 Y_AXIS;
	static const PxVec3 Z_AXIS;


	struct RagdollNode
	{
		PxQuat globalRotation; //rotation of link in model space
		PxVec3 scaledGlobalPos; //position of link in world space
		int parentNodeIdx; //index of parent node
		float halfLength; //half length of capsule for node
		float radius; //radious of capsule for node
		float parentLinkPos; //relative position of link centre in parent ro this node
		float childLinkPos; //relative position of link in child
		char* name; //name of link
		PxArticulationLink* linkptr;
		//constructor
		RagdollNode(PxQuat _globalRotation, int _parentNodeIdx, float _halfLength, float _radius,
			float _parentLinkPos, float _childLinkPos, char* _name);
	};

	//male ragdoll
	static PxArticulation* MakeRagDoll(PxPhysics* g_physics, RagdollNode** nodeArray, PxTransform worldPos,
		float scaleFactor, PxMaterial* ragDollMaterial);

	//complex humanoid ragdoll example
	static RagdollNode* ragdollData[];
};

#endif