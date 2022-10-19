#include "PhysicsModule.h"

PhysicsModuleData* PhysicsModuleData::instance = nullptr;

PhysicsModuleData::PhysicsModuleData()
{
    CollisionConfiguration = new btDefaultCollisionConfiguration();
    Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
    Broadphase = new btDbvtBroadphase();
    Solver = new btSequentialImpulseConstraintSolver();
    Solver->reset(); // reset solver seed
    World = new btDiscreteDynamicsWorld(
        Dispatcher, Broadphase, Solver, CollisionConfiguration
    );

    World->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

PhysicsModuleData* PhysicsModuleData::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new PhysicsModuleData();
    }
    return instance;
}

void PhysicsModuleData::OnUpdate(float deltaTime)
{
    World->stepSimulation(deltaTime);
}

btAlignedObjectArray<btCollisionShape*> PhysicsModuleData::GetCollisionShapes()
{
    return collisionShapes;
}

btDiscreteDynamicsWorld* PhysicsModuleData::GetDynamicsWorls()
{
    return World;
}