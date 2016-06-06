#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H

#define NUMBERSHAPE 2

class PhysicsObject;

//function pointer for array
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

class CollisionChecker
{
public:
	static bool Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);

	static bool Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);

	static fn CollisionFunctionArray[];
};

#endif