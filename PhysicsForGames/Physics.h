#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"

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
	PxScene* CreateDefaultScene();
	void SetUpIntroToPhysX();
	void MakeRagDoll();
	virtual void shutdown();
    virtual bool update();
	void GunFire();
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
	PxSimulationFilterShader m_defaultFilterShader;

	PxMaterial* m_physicsMaterial;
	PxMaterial* m_boxMaterial;
	PxCooking* m_physicsCooker;
	PxControllerManager* m_controllerManager;

	std::vector<PxArticulation*> ragdolls;
	std::queue<PxRigidDynamic*> bullets;

	//Gun
	float cooldown;
};



#endif //CAM_PROJ_H_
