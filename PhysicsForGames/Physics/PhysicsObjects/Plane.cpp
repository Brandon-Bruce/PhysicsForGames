#include "Plane.h"

#include <Camera.h>

#include <Gizmos.h>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

Plane::Plane(glm::vec4 colour, glm::vec3 normal, float distance) :
	PhysicsObject(ShapeType::PLANE, colour)
{
	this->normal = glm::vec4(glm::normalize(normal), distance);
	this->staticObject = true;
}

void Plane::Update(glm::vec3, float deltaTime)
{

}

void Plane::Debug()
{

}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec3 planeNormal(normal.x, normal.y, normal.z);
	glm::vec3 parallel1(0, 0, 0);
	glm::vec3 parallel2(0, 0, 0);


	if (planeNormal == glm::vec3(0, 0, 1))
		parallel1 = glm::vec3(0, 1, 0);
	else
		parallel1 = glm::vec3(0, 0, 1);

	//get vectors that are perpendecular to the plan
	parallel1 = glm::cross(planeNormal, parallel1);
	parallel2 = glm::cross(planeNormal, parallel1);

	glm::vec3 centrePoint = planeNormal * normal.w;
	parallel1 = glm::normalize(parallel1);
	parallel2 = glm::normalize(parallel2);

	glm::vec3 start = centrePoint + parallel1 * lineSegmentLength;
	glm::vec3 end = centrePoint - parallel1 * lineSegmentLength;
	Gizmos::addLine(start, end, color);

	start = centrePoint + parallel2 * lineSegmentLength;
	end = centrePoint - parallel2 * lineSegmentLength;
	Gizmos::addLine(start, end, color);
}

Plane::~Plane()
{

}
