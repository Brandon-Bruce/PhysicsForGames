#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"
#include "ParticleFluidEmitter.h"
#include "ControllerHitReport.h"

#include <vector>
#include <queue>
#include <PxPhysicsAPI.h>
#include <PxScene.h>

class PhysicsObject;

using namespace physx;
class Physics : public Application
{
public:
	virtual bool startup();
	void SetUpPhysX();
	void CreateDefaultScene();
	void SetUpIntroToPhysX();
	void SetUpController();
	void MakeRagDoll();
	void SetShapeAsTrigger(PxRigidActor* actor);
	virtual void shutdown();
    virtual bool update();
	void GunFire();
	void CharacterController();
    virtual void draw();

	void renderGizmos(PxScene* physics_scene);
	void CheckForCollisions();

    Renderer* m_renderer;
    FlyCamera m_camera;
    float m_delta_time;

	std::vector<PhysicsObject*> actors;
	glm::vec3 gravity;

	/**************
	**PhysX stuff**
	**************/
	PxFoundation* m_physicsFoundation;
	PxPhysics* m_physics;
	PxScene* m_physicsScene;

	PxDefaultErrorCallback m_defaultErrorCallback;
	PxDefaultAllocator m_defaultAllocator;

	PxMaterial* m_physicsMaterial;
	PxMaterial* m_boxMaterial;
	PxCooking* m_physicsCooker;
	PxControllerManager* m_controllerManager;

	std::vector<PxArticulation*> ragdolls;
	std::queue<PxRigidDynamic*> bullets;

	ParticleFluidEmitter* m_particleEmmiter;

	//Gun
	float cooldown;

	//ontroller Stuff
	ControllerHitReport* myHitReport;
	PxController* playerController;
	float _characterYVelocity;
	float _playerGravity;
	PxReal _characterRotation;
};



#endif //CAM_PROJ_H_
