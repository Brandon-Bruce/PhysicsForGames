#include "ControllerHitReport.h"

void ControllerHitReport::onShapeHit(const PxControllerShapeHit& hit)
{
	//get actor hit
	PxRigidActor* actor = hit.shape->getActor();
	//get normal of thing hit
	_playerContactNormal = hit.worldNormal;
	//cast to dynamic actor
	PxRigidDynamic* myActor = actor->is<PxRigidDynamic>();
	if (myActor)
	{
		//Add forces
	}
}
