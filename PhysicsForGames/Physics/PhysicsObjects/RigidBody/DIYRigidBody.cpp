#include "DIYRigidBody.h"

DIYRigidBody::DIYRigidBody(ShapeType shapeID, glm::vec4 colour,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float mass, bool staticObject) :
	PhysicsObject(shapeID, colour),
	position(position), rotation(rotation), velocity(velocity), mass(mass), staticObject(staticObject)
{
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
