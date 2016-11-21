#include "Fluid.h"

ParticleFluidEmitter* Fluid::Create(PxPhysics* physics, PxScene* scene)
{
	PxParticleFluid* pf;

	//Create particle system and set immutable properties
	PxU32 maxParticles = 4000;
	bool perParticleResetOffset = false;
	pf = physics->createParticleFluid(maxParticles, perParticleResetOffset);
	
	pf->setRestParticleDistance(0.3f);
	pf->setDynamicFriction(0.1f);
	pf->setStaticFriction(0.1f);
	pf->setDamping(0.1);
	pf->setParticleMass(0.1);
	pf->setRestitution(0);


	pf->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
	pf->setStiffness(100);

	if (pf)
	{
		scene->addActor(*pf);
		ParticleFluidEmitter* m_particleEmmiter = new ParticleFluidEmitter(maxParticles, PxVec3(25, 10, 0), pf, .1);
		m_particleEmmiter->setStartVelocityRange(-0.001f, -250.0f, -0.001f, 0.001f, -250.0f, 0.001f);

		return m_particleEmmiter;
	}
}
