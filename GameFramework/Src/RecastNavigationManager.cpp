#include "RecastNavigationManager.h"

#include "PhysicsModule.h"

#include "RecastInputMesh.h"

#include "Game.h"
#include "RenderingSystem.h"

auto RecastNavigationManager::GenerateNavMesh() -> void
{
	GenerateRecastInputMesh();
}

auto RecastNavigationManager::DrawDebugInputMesh() const -> void
{
	if (meshFromPhysicsWorld.get() == nullptr)
	{
		return;
	}

	DebugDrawer* dd = Game::GetInstance()->MyRenderingSystem->GetDebugDrawer();
	meshFromPhysicsWorld->DrawDebugWireFrame(dd);
}

auto RecastNavigationManager::GenerateRecastInputMesh() -> void
{
	meshFromPhysicsWorld.reset(new RecastInputMesh());

	btDiscreteDynamicsWorld* physicsWorld = PhysicsModuleData::GetInstance()->GetDynamicsWorld();

	btCollisionObjectArray& collisionObjects = physicsWorld->getCollisionObjectArray();
	for (int i = 0; i < collisionObjects.size(); ++i)
	{
		btCollisionObject* collisionObject = collisionObjects[i];
		if (collisionObject->getInternalType() != btCollisionObject::CO_RIGID_BODY)
		{
			continue;
		}

		// Generate nav mesh only for static collisions
		if ((collisionObject->getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT) == 0)
		{
			continue;
		}

		btCollisionShape* shape = collisionObject->getCollisionShape();

		// Hack: use local transform aabb to generate the mesh; a better aproach would be to create a mesh for each type of shape
		btVector3 aabbMin;
		btVector3 aabbMax;
		shape->getAabb(btTransform::getIdentity(), aabbMin, aabbMax);

		btVector3 localBox[] =
		{
			{aabbMin.x(), aabbMin.y(), aabbMax.z()}, // v0
			{aabbMax.x(), aabbMin.y(), aabbMax.z()}, // v1
			{aabbMin.x(), aabbMax.y(), aabbMax.z()}, // v2
			{aabbMax.x(), aabbMax.y(), aabbMax.z()}, // v3
			{aabbMin.x(), aabbMin.y(), aabbMin.z()}, // v4
			{aabbMax.x(), aabbMin.y(), aabbMin.z()}, // v5
			{aabbMin.x(), aabbMax.y(), aabbMin.z()}, // v6
			{aabbMax.x(), aabbMax.y(), aabbMin.z()}, // v7
		};

		const int vertIndexOffset = meshFromPhysicsWorld->GetNumVertices();

		const btTransform& t = collisionObject->getWorldTransform();
		for (btVector3& vert : localBox)
		{
			t.getRotation()* btVector3();
			const btVector3 worldVert = t * vert;
			meshFromPhysicsWorld->AddVertex(worldVert.x(), worldVert.y(), worldVert.z());
		}

		meshFromPhysicsWorld->AddFace(vertIndexOffset + 0, vertIndexOffset + 2, vertIndexOffset + 1); // front
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 1, vertIndexOffset + 2, vertIndexOffset + 3); // front
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 4, vertIndexOffset + 6, vertIndexOffset + 5); // back
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 5, vertIndexOffset + 6, vertIndexOffset + 7); // back
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 0, vertIndexOffset + 4, vertIndexOffset + 2); // left
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 4, vertIndexOffset + 6, vertIndexOffset + 2); // left
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 1, vertIndexOffset + 5, vertIndexOffset + 3); // right
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 5, vertIndexOffset + 7, vertIndexOffset + 3); // right
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 0, vertIndexOffset + 1, vertIndexOffset + 4); // bottom
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 1, vertIndexOffset + 5, vertIndexOffset + 4); // bottom
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 2, vertIndexOffset + 3, vertIndexOffset + 6); // top
		meshFromPhysicsWorld->AddFace(vertIndexOffset + 3, vertIndexOffset + 7, vertIndexOffset + 6); // top
	}
}
