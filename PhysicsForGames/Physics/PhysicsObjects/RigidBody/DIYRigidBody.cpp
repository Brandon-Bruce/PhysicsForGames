#include "DIYRigidBody.h"

DIYRigidBody::DIYRigidBody(ShapeType shapeID, glm::vec4 colour,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float mass) :
	PhysicsObject(shapeID, colour),
	position(position), rotation(rotation), velocity(velocity), mass(mass)
{

}

void DIYRigidBody::Update(glm::vec3 gravity, float deltaTime)
{
	position += velocity * deltaTime;
	ApplyForce(gravity * deltaTime);
}

void DIYRigidBody::Debug()
{

}

void DIYRigidBody::MakeGizmo()
{

}

void DIYRigidBody::ApplyForce(glm::vec3 force)
{
	velocity += force / mass;
}

void DIYRigidBody::ApplyForceToActor(DIYRigidBody* actor2, glm::vec3 force)
{
	ApplyForce(-force);
	actor2->ApplyForce(force);
}

DIYRigidBody::~DIYRigidBody()
{

}
