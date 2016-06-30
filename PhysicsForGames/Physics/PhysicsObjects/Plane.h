#ifndef PLANE_H
#define PLANE_H

#include "PhysicsObject.h"

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <assert.h>

class FlyCamera;

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec4 colour, glm::vec3 normal, float distance, FlyCamera* camera);
	virtual void Update(glm::vec3, float deltaTime);
	virtual void Debug();
	virtual void MakeGizmo();
	~Plane();

	glm::vec3 GetNormal() { return glm::vec3(normal.x, normal.y, normal.z); }
	float GetDistance() { return normal.w; }
	float GetMass() { return 0; }
	glm::vec3 GetVelocity() { return glm::vec3(0, 0, 0); }
	void AddVelocity(glm::vec3 vel) { assert(vel.length() > 0); }
	void Move(glm::vec3 pos) { assert(pos.length() > 0); }

private:

	glm::vec4 normal;
	FlyCamera* camera;
};

#endif