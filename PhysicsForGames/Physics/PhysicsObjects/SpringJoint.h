#ifndef SPRINGJOINT_H
#define SPRINGJOINT_H

#include "PhysicsObject.h"

class DIYRigidBody;

class SpringJoint : public PhysicsObject
{
public:
	SpringJoint(DIYRigidBody* connection1);
	~SpringJoint();
};

#endif