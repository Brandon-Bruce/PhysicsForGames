#include "DIYRigidBody.h"

#include <glm\ext.hpp>
#include <assert.h>

DIYRigidBody::DIYRigidBody(ShapeType shapeID, glm::vec4 colour,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float maxSpeed, float mass, bool staticObject) :
	PhysicsObject(shapeID, colour),
	position(position), rotation(rotation), velocity(velocity), maxSpeed(maxSpeed), mass(mass)
{
	this->staticObject = staticObject;
	if (staticObject == true)
	{
		velocity = glm::vec3(0,0,0);
	}
}

void DIYRigidBody::Update(glm::vec3 gravity, float deltaTime)
{
	if (staticObject == false)
	{
		velocity += acceleration * deltaTime;
		velocity += gravity * deltaTime;
		
		//Cap velocity to prevent tunelling
		if (velocity.length() > maxSpeed)
		{
			velocity = glm::normalize(velocity) * maxSpeed;
		}
		position += velocity * deltaTime;
		acceleration = glm::vec3(0);
	}
}

void DIYRigidBody::Debug()
{

}

void DIYRigidBody::MakeGizmo()
{

}

void DIYRigidBody::ApplyForce(glm::vec3 force)
{
	if (staticObject == false)
		acceleration += force / mass;
}

void DIYRigidBody::ApplyForceToActor(DIYRigidBody* actor2, glm::vec3 force)
{
	if (staticObject == false)
		ApplyForce(-force);
	actor2->ApplyForce(force);
}

void DIYRigidBody::ResetVolocity()
{
	velocity = glm::vec3(0, 0, 0);
}

void DIYRigidBody::Move(glm::vec3 pos)
{
	if (staticObject == false)
		position += pos;
}

DIYRigidBody::~DIYRigidBody()
{

}

float DIYRigidBody::GetMass()
{
	if (staticObject == false)
		return mass;
	else
		return 100000;
}

void DIYRigidBody::AddVelocity(glm::vec3 vel)
{
	if (staticObject == false)
		velocity = vel;
}
