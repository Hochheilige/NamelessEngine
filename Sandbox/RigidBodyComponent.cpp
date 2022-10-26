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

btRigidBody* RigidBodyComponent::GetRigidBody()
{
    return Body;
}

btCollisionShape* RigidBodyComponent::GetCollisionShape()
{
    return Shape;
}

btScalar RigidBodyComponent::GetMass()
{
    return Mass;
}

//btTransform RigidBodyComponent::Update()
//{
//    if (Body && Body->getMotionState())
//    {
//        Body->getMotionState()->getWorldTransform(Transform);
//    }
//
//    return Transform;
//}
