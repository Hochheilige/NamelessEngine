#pragma once

#include "btBulletDynamicsCommon.h"

class PhysicsModuleData
{
public:

    ~PhysicsModuleData();

    static PhysicsModuleData* GetInstance();

    void OnUpdate(float deltaTime);

    void AddCollisionShape(btCollisionShape* shape);
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