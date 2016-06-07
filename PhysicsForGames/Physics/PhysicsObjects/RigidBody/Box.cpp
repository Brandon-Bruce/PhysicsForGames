#include "Box.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Box::Box(glm::vec4 color, glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float mass, glm::vec3 extents) :
	DIYRigidBody(ShapeType::BOX, color, position, rotation, velocity, mass),
	extents(extents)
{

}

void Box::MakeGizmo()
{
	const glm::mat4 rotationMat = glm::rotate(0.f, rotation);
	Gizmos::addAABBFilled(position, extents, color, &rotationMat);
}


Box::~Box()
{

}