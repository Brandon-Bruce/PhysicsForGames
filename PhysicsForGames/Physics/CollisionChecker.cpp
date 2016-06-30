#include "CollisionChecker.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

#include <glm\ext.hpp>

fn CollisionChecker::CollisionFunctionArray[] =
{
	nullptr, CollisionChecker::Plane2Sphere, CollisionChecker::Plane2Box,
	CollisionChecker::Sphere2Plane, CollisionChecker::Sphere2Sphere, CollisionChecker::Sphere2Box,
	CollisionChecker::Box2Plane, CollisionChecker::Box2Sphere, CollisionChecker::Box2Box
};

void CollisionChecker::Seperate(PhysicsObject* obj1, PhysicsObject* obj2, float overlap, glm::vec3 normal)
{
	float totalMass = obj1->GetMass() + obj2->GetMass();
	float massRatio1 = obj1->GetMass() / totalMass;
	float massRatio2 = obj2->GetMass() / totalMass;

	//seperate relative to mass of objects
	glm::vec3 separationVec = normal * overlap;
	obj1->Move(-separationVec * massRatio2);
	obj2->Move(separationVec * massRatio1);
}

void CollisionChecker::CalculateResponse(PhysicsObject* obj1, PhysicsObject* obj2, float overLap, glm::vec3 normal)
{
	Seperate(obj1, obj2, overLap, normal);

	const float coefficientOfRestitution = 0.5f;

	glm::vec3 relativeVel = obj2->GetVelocity() - obj1->GetVelocity();
	float velocityAlongNormal = glm::dot(relativeVel, normal);
	float impulseAmount = -(1 - coefficientOfRestitution) * velocityAlongNormal;
	impulseAmount /= 1 / obj1->GetMass() + 1 / obj2->GetMass();

	glm::vec3 impulse = impulseAmount * normal;
	obj1->AddVelocity(1 / obj1->GetMass() * -impulse);
	obj2->AddVelocity(1 / obj2->GetMass() * +impulse);
}

bool CollisionChecker::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Plane(obj2, obj1);
}

bool CollisionChecker::Plane2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);
	if (box != nullptr && plane != nullptr)
	{
		//detect collision
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
		glm::vec3 planeNormal = plane->GetNormal();
		glm::vec3 collisionNormal = planeNormal;
		float sphereToPlane = glm::dot(sphere->GetPosition(), planeNormal);

		// if behind plane flip normal
		if (sphereToPlane < 0)
		{
			sphereToPlane *= -1;
			collisionNormal *= -1;
		}

		//check if sphere radius is bigger then distance to plane
		float intersection = (sphere->GetRadius() + plane->GetDistance()) - sphereToPlane;
		if (intersection > 0)
		{
			CalculateResponse(obj1, obj2, intersection, planeNormal);
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
			CalculateResponse(obj1, obj2, intersection, glm::normalize(delta));
			return true;
		}
	}
	return false;
}

bool CollisionChecker::Sphere2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool CollisionChecker::Box2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool CollisionChecker::Box2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool CollisionChecker::Box2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}
