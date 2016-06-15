#ifndef SPHERE_H
#define SPHERE_H

#include "DIYRigidBody.h"

class Sphere : public DIYRigidBody
{
public:
	Sphere(glm::vec4 colour,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float mass, bool staticObject,
		float radius);
	virtual void MakeGizmo();
	~Sphere();

	float GetRadius() { return radius; }

private:
	float radius;
};

#endif