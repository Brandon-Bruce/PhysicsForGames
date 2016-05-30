#include "Sphere.h"
#include "Gizmos.h"

Sphere::Sphere(glm::vec4 colour,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float mass,
	float radius) :
	DIYRigidBody(ShapeType::SPHERE, colour, position, rotation, velocity, mass),
	radius(radius)
{

}

void Sphere::MakeGizmo()
{
	Gizmos::addSphere(position, radius, radius * 10, radius * 10, colour);
}

Sphere::~Sphere()
{

}
