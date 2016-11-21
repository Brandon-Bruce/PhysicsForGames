#ifndef FLUID_H
#define FLUID_H

#include <PxPhysics.h>

#include <PxPhysicsAPI.h>
#include <ParticleFluidEmitter.h>


using namespace physx;

class Fluid
{
public:
	static ParticleFluidEmitter* Create(PxPhysics* physics, PxScene* scene);
};

#endif