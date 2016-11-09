#include "MyCollisionCallback.h"
#include <iostream>
#include <PxRigidActor.h>

using std::cout;

void MyCollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		const PxContactPair& cp = pairs[i];
		//only interested in touches foung and lost
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			cout << "Collisoon detected between: ";
			cout << pairHeader.actors[0]->getName() << " ";
			cout << pairHeader.actors[1]->getName() << std::endl;
		}
	}
}

void MyCollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		PxTriggerPair* pair = pairs + i;
		PxActor* triggerActor = pair->triggerActor;
		PxActor* otherActor = pair->otherActor;
		cout << otherActor->getName();
		cout << " Entered Trigger ";
		cout << triggerActor->getName() << std::endl;
	}
}

void MyCollisionCallback::onConstraintBreak(PxConstraintInfo*, PxU32)
{
}

void MyCollisionCallback::onWake(PxActor**, PxU32)
{
}

void MyCollisionCallback::onSleep(PxActor**, PxU32)
{
}

PxFilterFlags FilterGroup::myFliterShader(PxFilterObjectAttributes attributes0, PxFilterData
	filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) ||
		PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) ||
		(filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND |
		PxPairFlag::eNOTIFY_TOUCH_LOST;
	return PxFilterFlag::eDEFAULT;
}

void FilterGroup::setUpFiltering(PxRigidActor* actor, PxU32 filtergroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filtergroup; //world0 = own ID
	filterData.word1 = filterMask; //world1 = ID mask to filter pairs that trigger contaxt callback

	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; ++i)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	_aligned_free(shapes);
}
