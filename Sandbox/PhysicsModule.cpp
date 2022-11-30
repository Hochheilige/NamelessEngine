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

PhysicsModuleData::~PhysicsModuleData()
{
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    delete World;
    delete Solver;
    delete Broadphase;
    delete Dispatcher;
    delete CollisionConfiguration;
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
    if (collisionShapes.size() != 0)
    {
        World->stepSimulation(deltaTime);
    }
}

void PhysicsModuleData::AddCollisionShape(btCollisionShape* shape)
{
    collisionShapes.push_back(shape);
}

btDiscreteDynamicsWorld* PhysicsModuleData::GetDynamicsWorls()
{
    return World;
}
