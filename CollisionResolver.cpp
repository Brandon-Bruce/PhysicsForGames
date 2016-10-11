#include "CollisionResolver.h"
#include "PhysicsShapeEnum.h"

#include "PhysicsActor.h"
#include "PhysicsShape.h"
#include "Rigidbody.h"

#include <algorithm>

using CollisionFunction = std::function<CollisionResolver::CollisionData(class PhysicsActor*, class PhysicsActor*)>;
CollisionFunction CollisionResolver::m_collisionFunctions[] = {
	CheckSphereSphere, CheckSphereAABB, CheckSpherePlane,
	CheckAABBSphere, CheckAABBAABB, CheckAABBPlane,
	CheckPlaneSphere, CheckPlaneAABB, CheckPlanePlane
};

void CollisionResolver::CheckCollision(class PhysicsActor* pActorA, class PhysicsActor* pActorB)
{
	unsigned int uiIndex = (int)(pActorA->GetPhysicsShape()->GetShapeType()) *
		(int)PhysicsShapeEnum::NUM_SHAPES +
		(int)(pActorB->GetPhysicsShape()->GetShapeType());
	
	CollisionData cData = m_collisionFunctions[uiIndex](pActorA, pActorB);
	
	if (cData.didCollide)
	{
		SeperateActors(pActorA, pActorB, cData.normal, cData.interceptDistance);
		UpdateVelocities(pActorA, pActorB, cData.normal);
	}
}

CollisionResolver::CollisionData CollisionResolver::CheckSphereSphere(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData;
	cData.didCollide = false;

	Sphere* pA = A->GetPhysicsShapeAs<Sphere>();
	Sphere* pB = B->GetPhysicsShapeAs<Sphere>();

	glm::vec3 offset = B->GetPosition() - A->GetPosition();
	float distance = glm::length(offset);

	float interceptDistance = (pA->GetRadius() + pB->GetRadius()) - distance;
	if (interceptDistance > 0)
	{
		cData.didCollide = true;
		cData.normal = glm::normalize(offset);
		cData.interceptDistance = interceptDistance;
	}

	return cData;
}

CollisionResolver::CollisionData CollisionResolver::CheckSphereAABB(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData;
	cData.didCollide = false;

	Sphere* pA = A->GetPhysicsShapeAs<Sphere>();
	AABB*	pB = B->GetPhysicsShapeAs<AABB>();

	glm::vec3 offset = A->GetPosition() - B->GetPosition();
	if(std::abs(offset.x) > 0) offset.x = std::min(std::abs(offset.x), pB->GetExtents().x) * (std::abs(offset.x) / offset.x);
	if(std::abs(offset.y) > 0) offset.y = std::min(std::abs(offset.y), pB->GetExtents().y) * (std::abs(offset.y) / offset.y);
	if(std::abs(offset.z) > 0) offset.z = std::min(std::abs(offset.z), pB->GetExtents().z) * (std::abs(offset.z) / offset.z);

	glm::vec3 boxPoint = B->GetPosition() + offset;

	offset = boxPoint - A->GetPosition();
	float distance = glm::length(offset);

	float interceptDistance = pA->GetRadius() - distance;
	if (interceptDistance > 0)
	{
		cData.didCollide = true;
		cData.normal = glm::normalize(offset);
		cData.interceptDistance = interceptDistance;
	}
	return cData;
}

CollisionResolver::CollisionData CollisionResolver::CheckSpherePlane(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData;
	cData.didCollide = false;

	Sphere* pA = A->GetPhysicsShapeAs<Sphere>();
	Plane*	pB = B->GetPhysicsShapeAs<Plane>();

	float distance = glm::dot(A->GetPosition(), pB->GetNormal()) - pB->GetDistanceFromOrigin();
	float interceptDistance = pA->GetRadius() - distance;

	if (interceptDistance > 0)
	{
		cData.didCollide = true;
		cData.normal = -pB->GetNormal();
		cData.interceptDistance = interceptDistance;
	}

	return cData;
}

CollisionResolver::CollisionData CollisionResolver::CheckAABBSphere(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData = CheckSphereAABB(B, A);
	if (cData.didCollide) cData.normal = -cData.normal;
	return cData;
}

CollisionResolver::CollisionData CollisionResolver::CheckAABBAABB(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData;
	cData.didCollide = false;

	AABB*	pA = A->GetPhysicsShapeAs<AABB>();
	AABB*	pB = B->GetPhysicsShapeAs<AABB>();
	
	glm::vec3 AMin = A->GetPosition() - pA->GetExtents();
	glm::vec3 AMax = A->GetPosition() + pA->GetExtents();
	glm::vec3 BMin = B->GetPosition() - pB->GetExtents();
	glm::vec3 BMax = B->GetPosition() + pB->GetExtents();

	glm::vec3 minOffset = AMax - BMin;
	glm::vec3 maxOffset = AMin - BMax;

	if (minOffset.x < 0 || maxOffset.x > 0 ||
		minOffset.y < 0 || maxOffset.y > 0 ||
		minOffset.z < 0 || maxOffset.z > 0)
	{
		//No Collision
		return cData;
	}
	else
	{
		glm::vec3 offset = glm::vec3(
			std::abs(minOffset.x) < std::abs(maxOffset.x) ? minOffset.x : maxOffset.x,
			std::abs(minOffset.y) < std::abs(maxOffset.y) ? minOffset.y : maxOffset.y,
			std::abs(minOffset.z) < std::abs(maxOffset.z) ? minOffset.z : maxOffset.z);

		offset.x = std::abs(offset.x) < std::abs(offset.y) ? offset.x : 0.0f;
		offset.y = std::abs(offset.y) < std::abs(offset.z) ? offset.y : 0.0f;
		offset.z = std::abs(offset.z) < std::abs(offset.x) ? offset.x : 0.0f;

		cData.didCollide = true;
		cData.normal = glm::normalize(offset);
		cData.interceptDistance = glm::length(offset);
		return cData;
	}

}

CollisionResolver::CollisionData CollisionResolver::CheckAABBPlane(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData;
	cData.didCollide = false;

	AABB*	pA = A->GetPhysicsShapeAs<AABB>();
	Plane*	pB = B->GetPhysicsShapeAs<Plane>();

	glm::vec3 min = A->GetPosition() - pA->GetExtents();
	glm::vec3 max = A->GetPosition() + pA->GetExtents();

	float minDistance = glm::dot(min, pB->GetNormal()) - pB->GetDistanceFromOrigin();
	float maxDistance = glm::dot(max, pB->GetNormal()) - pB->GetDistanceFromOrigin();
	float interceptDistance = -std::min(minDistance, maxDistance);

	if (interceptDistance > 0)
	{
		cData.didCollide = true;
		cData.normal = -pB->GetNormal();
		cData.interceptDistance = interceptDistance;
	}

	return cData;
}

CollisionResolver::CollisionData CollisionResolver::CheckPlaneSphere(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData = CheckSpherePlane(B, A);
	if(cData.didCollide) cData.normal = -cData.normal;
	return cData;
}

CollisionResolver::CollisionData CollisionResolver::CheckPlaneAABB(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData = CheckAABBPlane(B, A);
	if (cData.didCollide) cData.normal = -cData.normal;
	return cData;
}

CollisionResolver::CollisionData CollisionResolver::CheckPlanePlane(class PhysicsActor* A, class PhysicsActor* B)
{
	CollisionData cData;
	cData.didCollide = false;
	return cData;
}

void CollisionResolver::SeperateActors(class PhysicsActor* pActorA, class PhysicsActor* pActorB, glm::vec3 normal, float interceptDistance)
{
	Rigidbody* pRigidA = pActorA->GetRigidbody();
	Rigidbody* pRigidB = pActorB->GetRigidbody();

	if (pRigidA && pRigidB)
	{
		float totalMass = pRigidA->GetMass() + pRigidB->GetMass();
		float ratioA = pRigidB->GetMass() / totalMass;
		float ratioB = pRigidA->GetMass() / totalMass;

		pActorA->SetPosition(pActorA->GetPosition() - (normal * interceptDistance * ratioA));
		pActorB->SetPosition(pActorB->GetPosition() + (normal * interceptDistance * ratioB));
	}
	else if (pRigidA)
	{
		pActorA->SetPosition(pActorA->GetPosition() - (normal * interceptDistance));
	}
	else
	{
		pActorB->SetPosition(pActorB->GetPosition() + (normal * interceptDistance));
	}
}

void CollisionResolver::UpdateVelocities(class PhysicsActor* pActorA, class PhysicsActor* pActorB, glm::vec3 normal)
{
	glm::vec3 relativeVelocity = glm::vec3(0);
	
	Rigidbody* pRigidA = pActorA->GetRigidbody();
	Rigidbody* pRigidB = pActorB->GetRigidbody();

	float avRestitution = 1.0f;
	float rMassA = 0.0f;
	float rMassB = 0.0f;

	if (pRigidB)
	{
		relativeVelocity = pRigidB->GetVelocity();
		avRestitution = pRigidB->GetRestitution();
		rMassB = 1.0f / pRigidB->GetMass();
	}
	if (pRigidA)
	{
		relativeVelocity -= pRigidA->GetVelocity();
		avRestitution = std::min(avRestitution, pRigidA->GetRestitution());
		rMassA = 1.0f / pRigidA->GetMass();
	}

	float jTop = -(1 + avRestitution) * glm::dot(relativeVelocity, normal);
	float jBottom = rMassA + rMassB;
	float j = jTop / jBottom;

	if (pRigidA)
	{
		glm::vec3 velChange = j * rMassA * normal;
		pRigidA->SetVelocity(pRigidA->GetVelocity() - velChange);
	}

	if (pRigidB)
	{
		glm::vec3 velChange = j * rMassB * normal;
		pRigidB->SetVelocity(pRigidB->GetVelocity() + velChange);
	}


}
