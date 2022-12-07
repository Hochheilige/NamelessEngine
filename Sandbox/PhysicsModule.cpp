#include "PhysicsModule.h"

PhysicsModuleData* PhysicsModuleData::instance = nullptr;

void callback(btDynamicsWorld* world, btScalar timeSleep)
{
    auto ghostObjects = PhysicsModuleData::GetInstance()->GetGhostObjects();
    for (const auto ghost : ghostObjects)
    {
        if (ghost->getNumOverlappingObjects())
        {
            for (int i = 0; i < ghost->getNumOverlappingObjects(); ++i)
            {
                // We can get object that this object overlapp with
                // I think that we should find somehow Actors of this objects
                // and do something that we need on this callback
                btCollisionObject* rb = ghost->getOverlappingObject(i);

            }
        }
    }
}

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

    World->setInternalTickCallback(callback, this, true);
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

    World->stepSimulation(deltaTime);

}

void PhysicsModuleData::AddCollisionShape(btCollisionShape* shape)
{
    collisionShapes.push_back(shape);
}

btDiscreteDynamicsWorld* PhysicsModuleData::GetDynamicsWorld()
{
    return World;
}

void PhysicsModuleData::AddGhostObject(btGhostObject* obj)
{
    ghostObjects.push_back(obj);
}

std::vector<btGhostObject*> PhysicsModuleData::GetGhostObjects()
{
    return ghostObjects;
}
