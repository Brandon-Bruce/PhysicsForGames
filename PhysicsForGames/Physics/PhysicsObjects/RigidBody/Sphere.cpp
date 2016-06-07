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
	Gizmos::addSphere(position, radius, (int)radius * 100, (int)radius * 100, color);
}

Sphere::~Sphere()
{

}
