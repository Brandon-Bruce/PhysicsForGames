#include "Box.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Box::Box(glm::vec4 color, glm::vec3 position, glm::vec3 velocity, float maxSpeed, float mass, bool staticObject, glm::vec3 extents) :
	DIYRigidBody(ShapeType::BOX, color, position, glm::vec3(0,1,0), velocity, maxSpeed, mass, staticObject),
	extents(extents)
{

}

void Box::MakeGizmo()
{
	Gizmos::addAABBFilled(position, extents, color);
}


Box::~Box()
{

}
