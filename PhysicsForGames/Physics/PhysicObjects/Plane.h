#ifndef PLANE_H
#define PLANE_H

#include "PhysicsObject.h"

#include <glm\vec2.hpp>

class Camera;

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec4 colour, glm::vec2 normal, float distance);
	virtual void Update(glm::vec3, float deltaTime);
	virtual void Debug();
	virtual void MakeGizmo();
	~Plane();

private:
	glm::vec2 normal;
	float distance;
	Camera* camera;
};

#endif