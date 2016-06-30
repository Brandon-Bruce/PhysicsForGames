#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H

#include <glm\vec3.hpp>

#define NUMBERSHAPE 3

class PhysicsObject;

//function pointer for array
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

class CollisionChecker
{
public:
	static void Seperate(PhysicsObject* obj1, PhysicsObject* obj2, float overlap, glm::vec3 normal);
	static void CalculateResponse(PhysicsObject* obj1, PhysicsObject* obj2, float overLap, glm::vec3 normal);

	static bool Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2);

	static bool Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2);

	static bool Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Box(PhysicsObject* obj1, PhysicsObject* obj2);

	static fn CollisionFunctionArray[];
};

#endif