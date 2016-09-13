#include "SpringJoint.h"
#include "DIYRigidBody.h"
#include <glm\ext.hpp>
#include <iostream>

int SpringJoint::layer = 1;

SpringJoint::SpringJoint(glm::vec4 colour,
	DIYRigidBody* connection1, DIYRigidBody* connection2, float springCoefficient, float damping) : 
	PhysicsObject(SPRING,colour),
	connections{ connection1,connection2 }, springCoefficient(springCoefficient), damping(damping)
{
	//put objects in same layer so they do not collide (using existing layer if already part of chain)
	if (connection1->layer != 0 || connection2->layer != 0)
	{
		int _layer = (connections[0]->layer > connections[1]->layer) ? connections[0]->layer : connections[1]->layer;
		connections[0]->layer = _layer; connections[1]->layer = _layer;
	}
	else
	{
		connections[0]->layer = layer; connections[1]->layer = layer++;
	}

	restlength = glm::length(connections[0]->GetPosition() - connections[1]->GetPosition());
}


SpringJoint::~SpringJoint()
{
}

void SpringJoint::Update(glm::vec3 gravity, float deltaTime)
{
	glm::vec3 connection = connections[0]->GetPosition() - connections[1]->GetPosition();
	float connectionLength = glm::length(connection);
	float force = springCoefficient * (connectionLength - restlength);
	connection /= connectionLength;
	connections[0]->ApplyForceToActor(connections[1], connection * force);
}

void SpringJoint::MakeGizmo()
{
}
