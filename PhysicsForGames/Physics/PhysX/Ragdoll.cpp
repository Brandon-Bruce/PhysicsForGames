#include "Ragdoll.h"

RagdollNode::RagdollNode(PxQuat _globalRotation, int _parentNodeIdx, float _halfLength, float _radius,
	float _parentLinkPos, float _childLinkPos, char* _name) : globalRotation(_globalRotation),
	parentNodeIdx(_parentNodeIdx), halfLength(_halfLength), radius(_radius), parentLinkPos(_parentLinkPos),
	childLinkPos(_childLinkPos), name(_name)
{
}

PxArticulation* Ragdoll::MakeRagDoll(PxPhysics * g_physics, RagdollNode ** nodeArray, PxTransform worldPos,
	float scaleFactor, PxMaterial * ragDollMaterial)
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
		}

	}
	return articulation;
}
