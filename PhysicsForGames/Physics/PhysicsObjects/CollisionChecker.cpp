#include "CollisionChecker.h"
#include "Sphere.h"

#include <glm\ext.hpp>

fn CollisionChecker::CollisionFunctionArray[] =
{
	nullptr, CollisionChecker::Plane2Sphere,
	CollisionChecker::Sphere2Plane, CollisionChecker::Sphere2Sphere,
};

bool CollisionChecker::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool CollisionChecker::Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
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
		float distanceApart = glm::length(sphere1->GetPosition() - sphere2->GetPosition());
		float totalRadius = sphere1->GetRadius() + sphere2->GetRadius();
		if (distanceApart < totalRadius)
		{
			sphere1->ResetVolocity();
			sphere2->ResetVolocity();
			return true;
		}
	}
	return false;
}
