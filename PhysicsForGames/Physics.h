#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"

#include <vector>
#include <PxPhysicsAPI.h>
#include <PxScene.h>

class PhysicsObject;

using namespace physx;
class Physics : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void renderGizmos(PxScene* physics_scene);
	void CheckForCollisions();

    Renderer* m_renderer;
    FlyCamera m_camera;
    float m_delta_time;

	std::vector<PhysicsObject*> actors;
	glm::vec3 gravity;
};



#endif //CAM_PROJ_H_
