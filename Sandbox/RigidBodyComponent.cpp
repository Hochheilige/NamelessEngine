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

void RigidBodyComponent::MakeKinematic()
{
    Body->setCollisionFlags(this->Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    Body->forceActivationState(DISABLE_DEACTIVATION);
    is_kinematic = true;
}

//void RigidBodyComponent::MakeNonKinematic()
//{
//    //btCollisionObject::CollisionFlags flag;
//
//    //if (Mass == 0)
//    //{
//    //    flag = btCollisionObject::CF_STATIC_OBJECT;
//    //}
//    //else
//    //{
//    //    flag = btCollisionObject::CF_DYNAMIC_OBJECT;
//    //}
//    //Body->setCollisionFlags(this->Body->getCollisionFlags() | flag);
//    //Body->forceActivationState(DISABLE_DEACTIVATION);
//    is_kinematic = false;
//}

void RigidBodyComponent::Update(float DeltaTime)
{
    Transform t = GetTransform();
    if (is_kinematic)
    {
        PhysicsTransform.setOrigin(btVector3(t.Position.x, t.Position.y, t.Position.z));
        Body->getMotionState()->setWorldTransform(PhysicsTransform);
    }
    else
    {
        if (Body && Body->getMotionState())
        {
            Body->getMotionState()->getWorldTransform(PhysicsTransform);
        }
        t.Position = Vector3(PhysicsTransform.getOrigin().x(), PhysicsTransform.getOrigin().y(), PhysicsTransform.getOrigin().z());
        t.Rotation = Quaternion(PhysicsTransform.getRotation());
    }
    SetTransform(t);

}
