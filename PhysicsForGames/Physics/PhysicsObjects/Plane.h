#ifndef PLANE_H
#define PLANE_H

#include "PhysicsObject.h"

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

class FlyCamera;

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec4 colour, glm::vec3 normal, float distance, FlyCamera* camera);
	virtual void Update(glm::vec3, float deltaTime);
	virtual void Debug();
	virtual void MakeGizmo();
	~Plane();

	glm::vec3 GetNormal() { return glm::vec3(normal); }
	float GetDistance() { return normal.w; }

private:
	void CalulateDrawArea();

	glm::vec4 normal;
	FlyCamera* camera;

	////for drawing
	//glm::vec3 topLeft;
	//glm::vec3 topRight;
	//glm::vec3 bottomLeft;
	//glm::vec3 bottomRight;

	////direction to move when drawing grid
	//glm::vec3 topDirection;
	//glm::vec3 bottomDirection;
	//glm::vec3 leftDirection;
	//glm::vec3 rightDirection;
};

#endif