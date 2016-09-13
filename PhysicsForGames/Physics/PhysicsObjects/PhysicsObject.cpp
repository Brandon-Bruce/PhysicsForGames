#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(ShapeType shapeID, glm::vec4 color) :
	shapeID(shapeID), color(color)
{
	layer = 0;
}

PhysicsObject::~PhysicsObject()
{

}
