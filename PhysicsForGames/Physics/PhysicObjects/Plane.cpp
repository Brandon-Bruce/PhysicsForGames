#include "Plane.h"

#include <Gizmos.h>

Plane::Plane(glm::vec4 colour, glm::vec2 normal, float distance) :
	PhysicsObject(ShapeType::PLANE, colour),
	normal(normal), distance(distance), camera(camera)
{

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
	glm::vec2 centrePoint = normal * distance;
	glm::vec2 parallel = glm::vec2(normal.y, -normal.x); // easy to rotate normal
													//through 90 degrees around z
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centrePoint + (parallel * lineSegmentLength);
	glm::vec2 end = centrePoint - (parallel * lineSegmentLength);
	Gizmos::add2DLine(start, end, colour);
}

Plane::~Plane()
{

}
