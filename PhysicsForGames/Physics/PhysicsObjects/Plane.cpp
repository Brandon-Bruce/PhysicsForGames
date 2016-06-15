#include "Plane.h"

#include <Camera.h>

#include <Gizmos.h>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

Plane::Plane(glm::vec4 colour, glm::vec3 normal, float distance, FlyCamera* camera) :
	PhysicsObject(ShapeType::PLANE, colour),
	camera(camera)
{
	this->normal = glm::vec4(glm::normalize(normal), distance);
}

void Plane::Update(glm::vec3, float deltaTime)
{

}

void Plane::Debug()
{

}

void Plane::MakeGizmo()
{
	//float lineSegmentLength = 300;
	//glm::vec2 centrePoint = glm::vec2(normal) * normal.w;
	//glm::vec2 parallel = glm::vec2(normal.y, -normal.x); // easy to rotate normal
	//
	//glm::vec2 start = centrePoint + (parallel * lineSegmentLength);
	//glm::vec2 end = centrePoint - (parallel * lineSegmentLength);
	//Gizmos::add2DLine(start, end, color);

	float lineSegmentLength = 300;
	glm::vec3 centrePoint = glm::vec3(normal.x, normal.y, normal.z) * normal.w;
	glm::vec3 parallel1 = glm::vec3(normal.y, -normal.x, normal.z); // easy to rotate normal
	glm::vec3 parallel2 = glm::vec3(normal.x, normal.z, -normal.y); // easy to rotate normal
	
	glm::vec3 start = centrePoint + (parallel1 * lineSegmentLength);
	glm::vec3 end = centrePoint - (parallel1 * lineSegmentLength);
	Gizmos::addLine(start, end, color);

	glm::vec3 start = centrePoint + (parallel2 * lineSegmentLength);
	glm::vec3 end = centrePoint - (parallel2 * lineSegmentLength);
	Gizmos::addLine(start, end, color);
}

Plane::~Plane()
{

}
