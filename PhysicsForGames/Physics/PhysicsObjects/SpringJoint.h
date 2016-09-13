#ifndef SPRINGJOINT_H
#define SPRINGJOINT_H

#include "PhysicsObject.h"

class DIYRigidBody;

class SpringJoint : public PhysicsObject
{
public:
	SpringJoint(glm::vec4 colour,
		DIYRigidBody* connection1, DIYRigidBody* connection2, float springCoefficient, float damping);
	~SpringJoint();

	virtual void Update(glm::vec3 gravity, float deltaTime);
	virtual void Debug() {}
	virtual void MakeGizmo();

	float GetMass() { return 100000; }
	glm::vec3 GetVelocity() { return glm::vec3(0, 0, 0); }
	void AddVelocity(glm::vec3 vel) {}
	void Move(glm::vec3 pos) {}

private:
	DIYRigidBody* connections[2];
	float damping;
	float restlength;
	float springCoefficient;

	static int layer;
};

#endif