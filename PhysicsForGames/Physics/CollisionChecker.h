#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H

#define NUMBERSHAPE 3

class PhysicsObject;

//function pointer for array
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

class CollisionChecker
{
public:
	static glm::vec3 CalculateForceVector(PhysicsObject* obj1, PhysicsObject* obj2, float overLap, glm::vec3 normal);

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