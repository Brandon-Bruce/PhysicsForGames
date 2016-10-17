#include "CollisionChecker.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

#include <glm\ext.hpp>
#include <algorithm>

fn CollisionChecker::CollisionFunctionArray[] =
{
	//Plane							//Sphere							//Box							//Spring
	nullptr,						CollisionChecker::Plane2Sphere,		CollisionChecker::Plane2Box,	nullptr, //Plane
	CollisionChecker::Sphere2Plane,	CollisionChecker::Sphere2Sphere,	CollisionChecker::Sphere2Box,	nullptr, //Sphere
	CollisionChecker::Box2Plane,	CollisionChecker::Box2Sphere,		CollisionChecker::Box2Box,		nullptr, //Box
	nullptr,						nullptr,							nullptr,						nullptr //Spring
};

void CollisionChecker::Seperate(PhysicsObject* obj1, PhysicsObject* obj2, float overlap, glm::vec3 normal)
{
	assert(obj1->staticObject == false || obj2->staticObject == false);
	float totalMass = obj1->GetMass() + obj2->GetMass();
	float massRatio1 = obj1->GetMass() / totalMass;
	float massRatio2 = obj2->GetMass() / totalMass;

	//correct ratio issues if object1 is static
	if (obj1->staticObject == true)
	{
		massRatio1 = 1.f;
		massRatio2 = 0;
	}
	//correct ratio issues if object2 is static
	else if (obj2->staticObject == true)
	{
		massRatio2 = 1.f;
		massRatio1 = 0;
	}

	//seperate relative to mass of objects
	glm::vec3 separationVec = normal * overlap;
	obj1->Move(-separationVec * massRatio2);
	obj2->Move(separationVec * massRatio1);
}

void CollisionChecker::CalculateResponse(PhysicsObject* obj1, PhysicsObject* obj2, float overLap, glm::vec3 normal)
{
	Seperate(obj1, obj2, overLap, normal);

	const float coefficientOfRestitution = 0.5f;

	float object1Mass = obj1->GetMass();
	float object2Mass = obj2->GetMass();
	glm::vec3 relativeVel = obj2->GetVelocity() - obj1->GetVelocity();
	float velocityAlongNormal = glm::dot(relativeVel, normal);
	float impulseAmount = -(1 + coefficientOfRestitution) * velocityAlongNormal;
	impulseAmount /= 1 / object1Mass + 1 / object2Mass;


	glm::vec3 impulse = impulseAmount * normal;
	obj1->AddVelocity(1 / object1Mass * -impulse);
	obj2->AddVelocity(1 / object2Mass * +impulse);
}

bool CollisionChecker::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Plane(obj2, obj1);
}

bool CollisionChecker::Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	if (box != nullptr && plane != nullptr)
	{
		glm::vec3 min = box->GetPosition() - box->GetExtents();
		glm::vec3 max = box->GetPosition() + box->GetExtents();
		glm::vec3 planeNormal = plane->GetNormal();

		//float distance = glm::dot(box->GetPosition(), planeNormal);
		//if (distance < 0)
		//{
		//	planeNormal *= -1;
		//}

		float minDistance = glm::dot(min, planeNormal);
		float maxDistance = glm::dot(max, planeNormal);
		float intercept = -std::min(minDistance, maxDistance) - plane->GetDistance();

		if (intercept > 0)
		{
			CalculateResponse(obj1, obj2, intercept, plane->GetNormal());
			return true;
		}
	}
	return false;
}

bool CollisionChecker::Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast to sphere and plane
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if successful check for collision
	if (sphere != nullptr && plane != nullptr)
	{
		//get planes normal
		glm::vec3 planeNormal = plane->GetNormal();
		float sphereToPlane = glm::dot(sphere->GetPosition(), planeNormal) - plane->GetDistance();
		//flip normal if behind plane
		if (sphereToPlane < 0)
		{
			planeNormal *= -1;
			sphereToPlane *= -1;
		}


		//check if sphere radius is bigger then distance to plane
		float intersection = sphere->GetRadius() - sphereToPlane;
		if (intersection > 0)
		{
			CalculateResponse(obj2, obj1, intersection, planeNormal);
			return true;
		}
	}
	return false;
}

bool CollisionChecker::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	// if successful check for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		glm::vec3 sphere1Pos = sphere1->GetPosition();
		glm::vec3 sphere2Pos = sphere2->GetPosition();
		glm::vec3 delta = sphere2Pos - sphere1Pos;

		float distanceApart = glm::length(delta);
		float totalRadius = sphere1->GetRadius() + sphere2->GetRadius();
		float intersection = totalRadius - distanceApart;
		if (intersection > 0)
		{
			glm::vec3 collisionNormal = glm::normalize(delta);
			CalculateResponse(obj1, obj2, intersection, collisionNormal);
			return true;
		}
	}
	return false;
}

bool CollisionChecker::Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	if (sphere != nullptr && box != nullptr)
	{
		glm::vec3 offset = sphere->GetPosition() - box->GetPosition();
		if (std::abs(offset.x) > 0)
		{
			offset.x = std::min(std::abs(offset.x), box->GetExtents().x) * (std::abs(offset.x) / offset.x);
		}
		if (std::abs(offset.y) > 0)
		{
			offset.y = std::min(std::abs(offset.y), box->GetExtents().y) * (std::abs(offset.y) / offset.y);
		}
		if (std::abs(offset.z) > 0)
		{
			offset.z = std::min(std::abs(offset.z), box->GetExtents().z) * (std::abs(offset.z) / offset.z);
		}

		glm::vec3 boxPoint = box->GetPosition() + offset;

		offset = boxPoint - sphere->GetPosition();
		float distance = glm::length(offset);

		float interceptDistance = sphere->GetRadius() - distance;
		if (interceptDistance > 0)
		{
			CalculateResponse(obj1, obj2, interceptDistance, glm::normalize(offset));
			return true;
		}
	}
	return false;
}

bool CollisionChecker::Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Plane2Box(obj2, obj1);
}

bool CollisionChecker::Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Box(obj2, obj1);
}

bool CollisionChecker::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec3 box1Min = box1->GetPosition() - box1->GetExtents();
		glm::vec3 box2Min = box2->GetPosition() - box2->GetExtents();

		glm::vec3 box1Max = box1->GetPosition() + box1->GetExtents();
		glm::vec3 box2Max = box2->GetPosition() + box2->GetExtents();

		glm::vec3 intersectionMax = box1Min - box2Max;
		glm::vec3 intersectionMin = box1Max - box2Min;

		if (!(intersectionMax.x > 0 ||
			intersectionMax.y > 0 ||
			intersectionMax.z > 0 ||
			intersectionMin.x < 0 ||
			intersectionMin.y < 0 ||
			intersectionMin.z < 0))
		{
			glm::vec3 intersection = glm::vec3(
				(std::abs(intersectionMax.x) < std::abs(intersectionMin.x) ? intersectionMax.x : intersectionMin.x),
				(std::abs(intersectionMax.y) < std::abs(intersectionMin.y) ? intersectionMax.y : intersectionMin.y),
				(std::abs(intersectionMax.z) < std::abs(intersectionMin.z) ? intersectionMax.z : intersectionMin.z));

			intersection.x = std::abs(intersection.x) < std::abs(intersection.y) ? intersection.x : 0;
			intersection.y = std::abs(intersection.y) < std::abs(intersection.z) ? intersection.y : 0;
			intersection.z = std::abs(intersection.z) < std::abs(intersection.x) ? intersection.z : 0;

			float overlap = glm::length(intersection);

			CalculateResponse(obj1, obj2, overlap, glm::normalize(intersection));
			return true;
		}
	}
	return false;
}
