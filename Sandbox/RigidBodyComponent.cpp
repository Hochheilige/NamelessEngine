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

RigidBodyType RigidBodyComponent::GetType()
{
    return rbType;
}

void RigidBodyComponent::SetMass(float mass)
{
    Mass = mass;
    //mMonoComponent->SetMass(mass);
}

void RigidBodyComponent::UpdateMass(float mass)
{
    Mass = mass;
    auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorls();
    world->removeRigidBody(Body);
    btVector3 inertia;
    Body->getCollisionShape()->calculateLocalInertia(mass, inertia);
    Body->setActivationState(DISABLE_DEACTIVATION);
    Body->setMassProps(mass, inertia);
    Body->setLinearFactor(btVector3(1, 1, 1));
    Body->setAngularFactor(btVector3(1, 1, 1));
    Body->updateInertiaTensor();
    Body->clearForces();
    world->addRigidBody(Body);
}

void RigidBodyComponent::SetRigidBodyType(RigidBodyType type)
{
    rbType = type;
}

void RigidBodyComponent::SetPhysicsTransform(Transform transform)
{
    MakeKinematic();
    PhysicsTransform.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));
    btQuaternion quat = btQuaternion(transform.Rotation.GetQuaterion().x, transform.Rotation.GetQuaterion().y, 
        transform.Rotation.GetQuaterion().z, transform.Rotation.GetQuaterion().w);
    PhysicsTransform.setRotation(quat);
    Body->setWorldTransform(PhysicsTransform);
    MakeDynamic();
}

void RigidBodyComponent::MakeKinematic()
{
    Body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    rbType = RigidBodyType::KINEMATIC;
}

void RigidBodyComponent::MakeDynamic()
{
    Body->setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT);
    rbType = RigidBodyType::DYNAMIC;
}

void RigidBodyComponent::RegisterRigidBodyType()
{
    switch (rbType)
    {
    case RigidBodyType::STATIC:
        Body->setCollisionFlags(this->Body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        break;
    case RigidBodyType::DYNAMIC:
        Body->setCollisionFlags(this->Body->getCollisionFlags() | btCollisionObject::CF_DYNAMIC_OBJECT);
        break;
    case RigidBodyType::KINEMATIC:
        Body->setCollisionFlags(this->Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        break;
    default:
        break;
    }

    Body->forceActivationState(DISABLE_DEACTIVATION);
}

void RigidBodyComponent::Update(float DeltaTime)
{
    Transform t = GetTransform();
    switch (rbType)
    {
    case RigidBodyType::STATIC:
    {
        break;
    }
    case RigidBodyType::DYNAMIC:
    {
        if (Body && Body->getMotionState())
        {
            Body->getMotionState()->getWorldTransform(PhysicsTransform);
        }
        t.Position = Vector3(PhysicsTransform.getOrigin().x(), PhysicsTransform.getOrigin().y(), PhysicsTransform.getOrigin().z());
        t.Rotation = Quaternion(PhysicsTransform.getRotation());
        break;
    }
    case RigidBodyType::KINEMATIC:
    {
        PhysicsTransform.setOrigin(btVector3(t.Position.x, t.Position.y, t.Position.z));
        btQuaternion quat = btQuaternion(t.Rotation.GetQuaterion().x, t.Rotation.GetQuaterion().y, t.Rotation.GetQuaterion().z, t.Rotation.GetQuaterion().w);
        PhysicsTransform.setRotation(quat);
        Body->setWorldTransform(PhysicsTransform);
        break;
    }
    default:
        break;
    }

    SetTransform(t);

}
