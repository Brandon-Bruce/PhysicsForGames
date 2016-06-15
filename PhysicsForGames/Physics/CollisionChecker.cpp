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

glm::vec3 CollisionChecker::CalculateForceVector(PhysicsObject* obj1, PhysicsObject* obj2, float overLap, glm::vec3 normal)
{
	const float coefficientOfRestitution = 0.5f;
	glm::vec3 relative = obj1 - velocity1;
	float velocityAlongNormal = glm::dot

	return glm::vec3();
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
		float sphereToPlane = glm::dot(sphere->GetPosition(), planeNormal) - plane->GetDistance();

		// if behind plane flip normal
		if (sphereToPlane < 0)
		{
			sphereToPlane *= -1;
			collisionNormal *= -1;
		}

		//check if sphere radius is bigger then distance to plane
		float intersection = sphere->GetRadius() - sphereToPlane;
		if (intersection > 0)
		{
			// find point where collision occured

			//flip plane normal if we are behind
			if (sphereToPlane < 0)
				planeNormal *= -1;

			glm::vec3 forceVector = -1 * sphere->GetMass() * planeNormal *
				(glm::dot(planeNormal, sphere->GetVelocity()));
			sphere->ApplyForce(2 * forceVector);
			sphere->Move(collisionNormal * intersection *0.5f);
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
			glm::vec3 relativeVelocity = sphere1->GetVelocity() - sphere2->GetVelocity();
			glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
			glm::vec3 forceVector = collisionVector * 1.f / (1 / sphere1->GetMass() + 1 / sphere2->GetMass());
			// use newtons third law to apply collision forces to colliding bodies
			//sphere1->ApplyForceToActor(sphere2, 2 * forceVector);
			sphere1->SetVelocity(-forceVector);
			sphere2->SetVelocity(forceVector);

			//move spheres out of collision
			glm::vec3 seperationVector = collisionNormal * intersection * 0.5f;
			sphere1->Move(-seperationVector);
			sphere2->Move(seperationVector);
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
