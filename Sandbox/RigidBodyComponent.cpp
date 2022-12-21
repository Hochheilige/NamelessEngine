#include "RigidBodyComponent.h"
#include "Game.h"

RigidBodyComponent::RigidBodyComponent()
{
    
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
        Body->setGravity(btVector3(0, -gravity, 0));
        Body->setFlags(Body->getFlags() | BT_DISABLE_WORLD_GRAVITY);
        world->addRigidBody(Body);
    }
}

void RigidBodyComponent::SetLinearVelocity(btVector3 velocity)
{
    if (Body)
    {
        Body->activate(true);
        Body->setLinearVelocity(velocity);
    }
}

void RigidBodyComponent::SetRigidBodyType(RigidBodyType type)
{
    rbType = type;
    OriginType = type;
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
	// static object should not be moved, especially on update
	/* case RigidBodyType::STATIC:
    {
        PhysicsTransform.setOrigin(btVector3(t.Position.x, t.Position.y, t.Position.z));
        btQuaternion quat = btQuaternion(t.Rotation.GetQuaterion().x, t.Rotation.GetQuaterion().y, t.Rotation.GetQuaterion().z, t.Rotation.GetQuaterion().w);
        PhysicsTransform.setRotation(quat);
        Body->setWorldTransform(PhysicsTransform);
        break;
    }*/
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
	// there's probalbly no need to update this every frame
    /*case RigidBodyType::KINEMATIC:
    {
        PhysicsTransform.setOrigin(btVector3(t.Position.x, t.Position.y, t.Position.z));
        btQuaternion quat = btQuaternion(t.Rotation.GetQuaterion().x, t.Rotation.GetQuaterion().y, t.Rotation.GetQuaterion().z, t.Rotation.GetQuaterion().w);
        PhysicsTransform.setRotation(quat);
        Body->getMotionState()->setWorldTransform(PhysicsTransform);
        break;
    }*/
	default:
		break;
	}

   SceneComponent::SetTransform(t);
}

auto RigidBodyComponent::SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void
{
	SceneComponent::SetTransform(InTransform, InTeleportType);

	PhysicsTransform.setOrigin(btVector3(InTransform.Position.x, InTransform.Position.y, InTransform.Position.z));
	btQuaternion quat = btQuaternion(InTransform.Rotation.GetQuaterion().x, InTransform.Rotation.GetQuaterion().y,
		InTransform.Rotation.GetQuaterion().z, InTransform.Rotation.GetQuaterion().w);
	PhysicsTransform.setRotation(quat);
	// Kinimatic bodies take their transform from motion state, so we need to update it too
	if (Body->isKinematicObject() && Body->getMotionState())
	{
		Body->getMotionState()->setWorldTransform(PhysicsTransform);
	}
	else
	{
		// this funcion seems to be too strong when using with kinematic bodies, resulting in visible overlap
		// probably beacause right now physics is not updated every frame
		Body->setCenterOfMassTransform(PhysicsTransform);
	}
	
	if (InTeleportType == TeleportType::ResetPhysics)
	{
		Body->clearForces();
		Body->clearGravity();
		Body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		Body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
		// todo: make sure everything is cleared
	}
}

ComponentType RigidBodyComponent::GetComponentType()
{
	return Undefined;
}

json RigidBodyComponent::Serialize() const
{
    auto json = SceneComponent::Serialize();
    assert(json.is_object());

    json["mass"] = Mass;
    json["type"] = rbType;
    json["physics_simulation"] = isPhysicsSimulationEnabled;
    json["need_physics"] = simulationNeedsEnabling;

    return json;
}

void RigidBodyComponent::Deserialize(const json* in)
{
    SetMass(in->at("mass").get<float>());
    SetRigidBodyType(in->at("type").get<RigidBodyType>());

	SceneComponent::Deserialize(in);

    Init();

    simulationNeedsEnabling = in->at("need_physics");
    isPhysicsSimulationEnabled = in->at("physics_simulation");

    
     SetPhysicsSimulation();
    
    
}

auto RigidBodyComponent::EnablePhysicsSimulation() -> void
{   

    if (!Body) Init();

    {
        PhysicsModuleData::GetInstance()->GetDynamicsWorls()->addRigidBody(Body);
        isPhysicsSimulationEnabled = true;
    }
}

auto RigidBodyComponent::DisablePhysicsSimulation() -> void
{   
    if (isPhysicsSimulationEnabled) {
        PhysicsModuleData::GetInstance()->GetDynamicsWorls()->removeRigidBody(Body);
        isPhysicsSimulationEnabled = false;
    }
}

auto RigidBodyComponent::SetPhysicsSimulation() -> void
{
    if (isPhysicsSimulationEnabled) {
        EnablePhysicsSimulation();
    }
        
    else
        DisablePhysicsSimulation();
}
