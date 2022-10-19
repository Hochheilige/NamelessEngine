#include "RigidBodyComponent.h"

RigidBodyComponent::~RigidBodyComponent()
{
    auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorls();
    world->removeRigidBody(Body);
    delete Body->getCollisionShape();
    delete Body->getMotionState();
    delete Body;
    //delete Shape;
}

btTransform RigidBodyComponent::Update()
{
    if (Body && Body->getMotionState())
    {
        Body->getMotionState()->getWorldTransform(Transform);
    }

    return Transform;
}
