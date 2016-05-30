#include "PhysicsScene.h"
#include "Sphere.h"

PhysicsScene::PhysicsScene(glm::vec3 gravity) : gravity(gravity)
{

}

void PhysicsScene::Update(float deltaTime)
{
	// update actors
	for (unsigned int i = 0; i < actors.size(); ++i)
		actors[i]->Update(gravity, deltaTime);
}

void PhysicsScene::DebugScene()
{

}

void PhysicsScene::AddGizmos()
{
	// add gizmos for actors to scene
	for (unsigned int i = 0; i < actors.size(); ++i)
		actors[i]->MakeGizmo();
}

PhysicsScene::~PhysicsScene()
{
	// clean up actors
	for (unsigned int i = 0; i < actors.size(); ++i)
		delete actors[i];
	actors.clear();
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	// add actor to back of vector
	actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	// find actor to remove and remove ("erase") it
	std::vector<PhysicsObject*>::iterator toRemove =
		std::find(actors.begin(), actors.end(), actor);
	actors.erase(toRemove);
}
