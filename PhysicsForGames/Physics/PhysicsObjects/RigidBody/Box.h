#ifndef BOX_H
#define BOX_H

#include "DIYRigidBody.h"

class Box :	public DIYRigidBody
{
public:
	Box(glm::vec4 color, glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, float mass, glm::vec3 extents);
	virtual void MakeGizmo();
	~Box();

	glm::vec3 GetExtents() { return extents; }

private:
	glm::vec3 extents;
};

#endif