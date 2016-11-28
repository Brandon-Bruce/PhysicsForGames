#ifndef CHARCATERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include <PxPhysicsAPI.h>
#include <glm\mat4x4.hpp>
using namespace physx;

class CharacterController
{
public:
	CharacterController();
	~CharacterController();
	void Update();
	void MakeGizmo();

private:
	PxTransform position;
};

#endif