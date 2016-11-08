#ifndef MYCOLLISIONCALLBACK_H
#define MYCOLLISIONCALLBACK_H

#include <PxSimulationEventCallback.h>

class MyCollisionCallback : public PxSimulationEventCallback
{
public:
	virtual void onContact(const PxContactPairHeader& pairHeader,
		const  PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger();
};

#endif