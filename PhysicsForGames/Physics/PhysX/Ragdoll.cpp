#include "Ragdoll.h"

const PxVec3 Ragdoll::X_AXIS = PxVec3(1, 0, 0);
const PxVec3 Ragdoll::Y_AXIS = PxVec3(0, 1, 0);
const PxVec3 Ragdoll::Z_AXIS = PxVec3(0, 0, 1);

Ragdoll::RagdollNode* Ragdoll::ragdollData[] =
{
	new RagdollNode(PxQuat(PxPi / 2.0f,			Z_AXIS), NO_PARENT,			1,	3,		1,	1,	"lower spine"),
	new RagdollNode(PxQuat(PxPi,				Z_AXIS), LOWER_SPINE,		1,	1,		-1,	1,	"left pelvis"),
	new RagdollNode(PxQuat(0,					Z_AXIS), LOWER_SPINE,		1,	1,		-1,	1,	"right pelvis"),
	new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f,	Z_AXIS), LEFT_PELVIS,		5,	2,		-1,	1,	"L upper leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f,	Z_AXIS), RIGHT_PELVIS,		5,	2,		-1,	1,	"R upper leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f,	Z_AXIS), LEFT_UPPER_LEG,	5,	1.75,	-1,	1,	"L lower leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f,	Z_AXIS), RIGHT_UPPER_LEG,	5,	1.75,	-1,	1,	"R lowerleg"),
	new RagdollNode(PxQuat(PxPi / 2.0f,			Z_AXIS), LOWER_SPINE,		1,	3,		1,	-1,	"upper spine"),
	new RagdollNode(PxQuat(PxPi,				Z_AXIS), UPPER_SPINE,		1,	1.5,	1,	1,	"left clavicle"),
	new RagdollNode(PxQuat(0,					Z_AXIS), UPPER_SPINE,		1,	1.5,	1,	1,	"right clavicle"),
	new RagdollNode(PxQuat(PxPi / 2.0f,			Z_AXIS), UPPER_SPINE,		1,	1,		1,	-1,	"neck"),
	new RagdollNode(PxQuat(PxPi / 2.0f,			Z_AXIS), NECK,				1,	3,		1,	-1,	"HEAD"),
	new RagdollNode(PxQuat(PxPi - 0.3f,			Z_AXIS), LEFT_CLAVICLE,		3,	1.5,	-1,	1,	"left upper arm"),
	new RagdollNode(PxQuat(0.3f,				Z_AXIS), RIGHT_CLAVICLE,	3,	1.5,	-1,	1,	"right upper arm"),
	new RagdollNode(PxQuat(PxPi - 0.3f,			Z_AXIS), LEFT_UPPER_ARM,	3,	1,		-1,	1,	"left lower arm"),
	new RagdollNode(PxQuat(0.3f,				Z_AXIS), RIGHT_UPPER_ARM,	3,	1,		-1,	1,	"right lower arm"),
	nullptr
};

Ragdoll::RagdollNode::RagdollNode(PxQuat _globalRotation, int _parentNodeIdx, float _halfLength, float _radius,
	float _parentLinkPos, float _childLinkPos, char* _name) : globalRotation(_globalRotation),
	parentNodeIdx(_parentNodeIdx), halfLength(_halfLength), radius(_radius), parentLinkPos(_parentLinkPos),
	childLinkPos(_childLinkPos), name(_name)
{
}

PxArticulation* Ragdoll::MakeRagDoll(PxPhysics* g_physics, RagdollNode** nodeArray, PxTransform worldPos,
	float scaleFactor, PxMaterial* ragDollMaterial)
{
	//create articulation for ragdoll
	PxArticulation* articulation = g_physics->createArticulation();
	RagdollNode** currentNode = nodeArray;

	//while there are more nodes to proccess
	while (*currentNode != nullptr)
	{
		//get pointer to current node
		RagdollNode* currentNodePtr = *currentNode;
		++currentNode;

		//pointer to hold parent node (if one exists)
		RagdollNode* parentNode = nullptr;
		//get scaled values for capsules
		float radius = currentNodePtr->radius*scaleFactor;
		float halfLength = currentNodePtr->halfLength*scaleFactor;
		float childHalfLength = radius + halfLength;
		float parentHalfLength = 0; //set later if a parent exists

		//get pointer to parent
		PxArticulationLink* parentLinkPtr = nullptr;
		currentNodePtr->scaledGlobalPos = worldPos.p;

		if (currentNodePtr->parentNodeIdx != -1)
		{
			//work out local position for link if parent exists
			parentNode = *(nodeArray + currentNodePtr->parentNodeIdx); //get pointer to parent node
			parentLinkPtr = parentNode->linkptr; //get pointer to parents link pointer
			parentHalfLength = (parentNode->radius + parentNode->halfLength) * scaleFactor;

			//calculate local position of node
			PxVec3 currentRelative = currentNodePtr->childLinkPos *
				currentNodePtr->globalRotation.rotate(PxVec3(childHalfLength, 0, 0));

			PxVec3 parentRelative = -currentNodePtr->parentLinkPos *
				parentNode->globalRotation.rotate(PxVec3(parentHalfLength, 0, 0));

			currentNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos -
				(parentRelative + currentRelative);
		}

		//link bone to parent
		//build transform for link
		PxTransform linkTransform = PxTransform(currentNodePtr->scaledGlobalPos,
			currentNodePtr->globalRotation);
		//create link in articulation
		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);
		//add link to ragdoll data for later use
		currentNodePtr->linkptr = link;
		float jointSpace = 0.01f; //gap between joints
		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + 0.01f;
		PxCapsuleGeometry capsule(radius, capsuleHalfLength);
		link->createShape(capsule, *ragDollMaterial); //adds capsule collider to link
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f); //adds mass

		//set up joints
		if (currentNodePtr->parentNodeIdx != -1)
		{
			//get pointer to joint from link
			PxArticulationJoint* joint = link->getInboundJoint();
			//get relative rotation of link
			PxQuat frameRotation = parentNode->globalRotation.getConjugate() *
				currentNodePtr->globalRotation;
			//set parent constraint frame
			PxTransform parentConstraintFrame = PxTransform(PxVec3(currentNodePtr->parentLinkPos *
				parentHalfLength, 0, 0), frameRotation);
			//set child constraint fram (frame of newly added link)
			PxTransform thisConstraintFrame = PxTransform(PxVec3(currentNodePtr->childLinkPos *
				childHalfLength, 0, 0));
			//set up poses for joint
			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisConstraintFrame);
			//set up contraints to stop it flopping around
			joint->setStiffness(20);
			joint->setDamping(20);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}

	}
	return articulation;
}
