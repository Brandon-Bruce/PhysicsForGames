#ifndef MYCOLLISIONCALLBACK_H
#define MYCOLLISIONCALLBACK_H

#include <PxSimulationEventCallback.h>
using namespace physx;

class MyCollisionCallback : public PxSimulationEventCallback
{
public:
	virtual void onContact(const PxContactPairHeader& pairHeader,
		const  PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs);
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32);
	virtual void onWake(PxActor**, PxU32);
	virtual void onSleep(PxActor**, PxU32);
};

struct FilterGroup
{
	enum Enum
	{
		GROUND = (1 << 0),
		BOX = (1 << 1),
		BULLET = (1 << 2)
	};

	static PxFilterFlags myFliterShader(PxFilterObjectAttributes attributes0, PxFilterData
		filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);

	static void setUpFiltering(PxRigidActor* actor, PxU32 filtergroup, PxU32 filterMask);
};

#endif