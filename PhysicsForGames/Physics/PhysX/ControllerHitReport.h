#ifndef ControllerHitReport_H
#define ControllerHitReport_H

#include <PxPhysicsAPI.h>
using namespace physx;

class ControllerHitReport : public PxUserControllerHitReport
{
public:
	virtual void onShapeHit(const PxControllerShapeHit& hit);
	virtual void onControllerHit(const PxControllersHit& hit){};
	virtual void onObstacleHit(const PxControllerObstacleHit& hit){};
	ControllerHitReport() : PxUserControllerHitReport(){};
	PxVec3 getPlayerContactNormal(){ return _playerContactNormal; };
	void clearPlayerContactNormal(){ _playerContactNormal = PxVec3(0, 0, 0); };
	PxVec3 _playerContactNormal;
};

#endif