#ifndef DIYRIGIDBODY_H
#define DIYRIGIDBODY_H

#include "PhysicsObject.h"

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

class DIYRigidBody : public PhysicsObject
{
public:
	DIYRigidBody(ShapeType shapeID, glm::vec4 colour,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float mass);
	virtual void Update(glm::vec3 gravity, float deltaTime);
	virtual void Debug();
	virtual void MakeGizmo();
	void ApplyForce(glm::vec3 force);
	void ApplyForceToActor(DIYRigidBody* actor2, glm::vec3 force);
	~DIYRigidBody();

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;

	float mass;
};

#endif