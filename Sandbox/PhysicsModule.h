#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include <vector>

class PhysicsModuleData
{
public:

    ~PhysicsModuleData();

    static PhysicsModuleData* GetInstance();

    void OnUpdate(float deltaTime);

    void AddCollisionShape(btCollisionShape* shape);
    btDiscreteDynamicsWorld* GetDynamicsWorld();

    void AddGhostObject(btGhostObject* obj);
    std::vector<btGhostObject*> GetGhostObjects();
	void RemoveGhostObject(btGhostObject* obj);

private:
    PhysicsModuleData();

    static PhysicsModuleData* instance;

    btCollisionConfiguration* CollisionConfiguration;
    btDispatcher* Dispatcher;
    btBroadphaseInterface* Broadphase;
    btConstraintSolver* Solver;
    btDiscreteDynamicsWorld* World;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    std::vector<btGhostObject*> ghostObjects;
};