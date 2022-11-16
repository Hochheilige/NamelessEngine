#include "RigidBodyComponent.h"
#include "Game.h"

RigidBodyComponent::RigidBodyComponent()
{
    auto game = Game::GetInstance();
    game->MyEditorContext.OnSelectedComponentChanged.AddRaw(this, &RigidBodyComponent::HandleSelectedComponentChanded);
}

RigidBodyComponent::~RigidBodyComponent()
{
    auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorls();
    world->removeRigidBody(Body);
    delete Body->getCollisionShape();
    delete Body->getMotionState();
    delete Body;

    auto game = Game::GetInstance();
    //game->MyEditorContext.OnSelectedComponentChanged.RemoveObject(this);
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
    return OriginType;
}

void RigidBodyComponent::SetMass(float mass)
{
    Mass = mass;
    if (Body)
    {
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
    //mMonoComponent->SetMass(mass);
}

void RigidBodyComponent::SetGravity(float gravity)
{
    if (Body)
    {
        auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorls();
        world->removeRigidBody(Body);
        Body->setGravity(btVector3(0, gravity, 0));
        Body->setFlags(Body->getFlags() | BT_DISABLE_WORLD_GRAVITY);
        world->addRigidBody(Body);
    }
}

void RigidBodyComponent::SetRigidBodyType(RigidBodyType type)
{
    rbType = type;
    OriginType = type;
}

void RigidBodyComponent::SetPhysicsTransform(Transform transform)
{
    if (OriginType != RigidBodyType::KINEMATIC)
        MakeKinematic();

    PhysicsTransform.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));
    btQuaternion quat = btQuaternion(transform.Rotation.GetQuaterion().x, transform.Rotation.GetQuaterion().y, 
        transform.Rotation.GetQuaterion().z, transform.Rotation.GetQuaterion().w);
    PhysicsTransform.setRotation(quat);
    Body->setWorldTransform(PhysicsTransform);

    switch (OriginType)
    {
    case RigidBodyType::STATIC:
        MakeStatic();
        break;
    case RigidBodyType::DYNAMIC:
        MakeDynamic();
        break;
    default:
        break;
    }

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

void RigidBodyComponent::MakeStatic()
{
    Body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    rbType = RigidBodyType::STATIC;
}

void RigidBodyComponent::RegisterRigidBodyType()
{
    switch (OriginType)
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
        PhysicsTransform.setOrigin(btVector3(t.Position.x, t.Position.y, t.Position.z));
        btQuaternion quat = btQuaternion(t.Rotation.GetQuaterion().x, t.Rotation.GetQuaterion().y, t.Rotation.GetQuaterion().z, t.Rotation.GetQuaterion().w);
        PhysicsTransform.setRotation(quat);
        Body->setWorldTransform(PhysicsTransform);
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
        Body->getMotionState()->setWorldTransform(PhysicsTransform);
        break;
    }
    default:
        break;
    }

    SetTransform(t);

}

void RigidBodyComponent::HandleSelectedComponentChanded(Component* newSelectedComponent)
{
    if (OriginType == RigidBodyType::DYNAMIC)
    {
        if (this == dynamic_cast<RigidBodyComponent*>(newSelectedComponent))
        {

            MakeKinematic();
        }
        else
        {
            MakeDynamic();
        }
    }
}
