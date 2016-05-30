#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
};

class PhysicsObject
{
public:
	PhysicsObject(ShapeType shapeID, glm::vec4 colour);
	virtual void Update(glm::vec3 gravity, float deltaTime) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};
	~PhysicsObject();

protected:
	ShapeType shapeID;
	glm::vec4 colour;
};

#endif