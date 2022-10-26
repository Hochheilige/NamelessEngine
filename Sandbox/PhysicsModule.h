#pragma once

#include "btBulletDynamicsCommon.h"

class PhysicsModuleData
{
public:

    static PhysicsModuleData* GetInstance();

    void OnUpdate(float deltaTime);

    btAlignedObjectArray<btCollisionShape*> GetCollisionShapes();
    btDiscreteDynamicsWorld* GetDynamicsWorls();

private:
    PhysicsModuleData();

    static PhysicsModuleData* instance;

    btCollisionConfiguration* CollisionConfiguration;
    btDispatcher* Dispatcher;
    btBroadphaseInterface* Broadphase;
    btConstraintSolver* Solver;
    btDiscreteDynamicsWorld* World;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;
};