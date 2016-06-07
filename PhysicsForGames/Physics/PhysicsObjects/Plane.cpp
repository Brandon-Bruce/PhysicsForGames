#include "Plane.h"

#include <Camera.h>

#include <Gizmos.h>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

Plane::Plane(glm::vec4 colour, glm::vec3 normal, float distance, FlyCamera* camera) :
	PhysicsObject(ShapeType::PLANE, colour),
	camera(camera)
{
	this->normal = glm::vec4(glm::normalize(normal), 0);
	//CalulateDrawArea();
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
	glm::vec2 centrePoint = glm::vec2(normal) * normal.w;
	glm::vec2 parallel = glm::vec2(normal.y, -normal.x); // easy to rotate normal
													//through 90 degrees around z
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centrePoint + (parallel * lineSegmentLength);
	glm::vec2 end = centrePoint - (parallel * lineSegmentLength);
	Gizmos::add2DLine(start, end, colour);

	//starting positions for drawing grid
	//glm::vec3 topPosition = topLeft;
	//glm::vec3 bottomPosition = bottomLeft;
	//glm::vec3 leftPosition = topLeft;
	//glm::vec3 rightPosition = topRight;

	//for (int x = 0; x <= 5; ++x)
	//{
	//	for (int y = 0; y <= 5; ++y)
	//	{
	//		Gizmos::addLine(topPosition, bottomPosition, color);
	//		Gizmos::addLine(leftPosition, rightPosition, color);

	//		topPosition += topDirection;
	//		bottomPosition += bottomDirection;
	//		leftPosition -= leftDirection;
	//		rightPosition -= rightDirection;
	//	}
	//}

	//Gizmos::addLine(topPosition, bottomPosition, color);
	//Gizmos::addLine(leftPosition, rightPosition, color);
}

Plane::~Plane()
{

}

void Plane::CalulateDrawArea()
{
	//GLFWwindow* currWindow = glfwGetCurrentContext();
	//int width = 0, height = 0;
	//glfwGetWindowSize(currWindow, &width, &height);

	////find positions of corners
	//topLeft = camera->pickAgainstPlane((float)0, (float)0, normal);
	//topRight = camera->pickAgainstPlane((float)width, (float)0, normal);
	//bottomLeft = camera->pickAgainstPlane((float)0, (float)height, normal);
	//bottomRight = camera->pickAgainstPlane((float)width, (float)height, normal);

	////find centre of corners
	//glm::vec3 centre = (((bottomRight + topLeft) / 2) + ((bottomLeft + topRight) / 2)) / 2;

	////adjust corner sizes using centre
	//topLeft = centre + glm::normalize(centre - topLeft) * 5;
	//topRight = centre + glm::normalize(centre - topRight) * 5;
	//bottomLeft = centre + glm::normalize(centre - bottomLeft) * 5;
	//bottomRight = centre + glm::normalize(centre - bottomRight) * 5;

	////directions to move when drawing grid
	//topDirection = glm::normalize(topLeft - topRight);
	//bottomDirection = glm::normalize(bottomLeft - bottomRight);
	//leftDirection = glm::normalize(topLeft - bottomLeft);
	//rightDirection = glm::normalize(topRight - bottomRight);
}
