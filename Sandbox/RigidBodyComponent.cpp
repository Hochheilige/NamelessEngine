#include "RigidBodyComponent.h"
#include "Game.h"

RigidBodyComponent::RigidBodyComponent()
{
    
}

RigidBodyComponent::~RigidBodyComponent()
{
    auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
	if (rigidBody.Collision != nullptr)
	{
		PhysicsModuleData::GetInstance()->RemoveGhostObject(rigidBody.Collision);
		world->removeCollisionObject(rigidBody.Collision);
		delete rigidBody.Collision;
	}
	if (rigidBody.Body != nullptr)
	{
		world->removeRigidBody(rigidBody.Body);
		delete rigidBody.Body->getCollisionShape();
		delete rigidBody.Body->getMotionState();
		delete rigidBody.Body;
	}
}

void RigidBodyComponent::Init()
{
    auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
    const Transform& transform = GetTransform();

    PhysicsTransform.setIdentity();
    PhysicsTransform.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));
    Quaternion quaternion = transform.Rotation.GetQuaterion();
    PhysicsTransform.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));

    switch (Usage)
    {
    case RigidBodyUsage::PHYSICS:
    {
        Shape = new btEmptyShape();
        //physics->AddCollisionShape(Shape);

        btVector3 localInertia = btVector3(0, 0, 0);

        btDefaultMotionState* myMotionState = new btDefaultMotionState(PhysicsTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(Mass, myMotionState, Shape, localInertia);
        rigidBody.Body = new btRigidBody(rbInfo);
        rigidBody.Body->setUserPointer(this);

        RegisterRigidBodyType();

        break;
    }
    case RigidBodyUsage::COLLISIONS:
    {
        rigidBody.Collision = new btGhostObject();
        CreateShape(transform.Scale);

        rigidBody.Collision->setCollisionShape(Shape);
        rigidBody.Collision->setWorldTransform(PhysicsTransform);
        rigidBody.Collision->setUserPointer(this);
        world->addCollisionObject(rigidBody.Collision);
        world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback);

        PhysicsModuleData::GetInstance()->AddGhostObject(rigidBody.Collision);

        break;
    }
    case RigidBodyUsage::COLLISIONS_AND_PHYSICS:
    {
        CreateShape(transform.Scale);
        //physics->AddCollisionShape(Shape);

        btVector3 localInertia = btVector3(0, 0, 0);
        if (Mass != 0.0f)
            Shape->calculateLocalInertia(Mass, localInertia);

        btDefaultMotionState* myMotionState = new btDefaultMotionState(PhysicsTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(Mass, myMotionState, Shape, localInertia);
        rigidBody.Body = new btRigidBody(rbInfo);
        rigidBody.Body->setUserPointer(this);

        RegisterRigidBodyType();
        break;
    }
    default:
        break;
    }
}

btRigidBody* RigidBodyComponent::GetRigidBody()
{
    return rigidBody.Body;
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
    if (rigidBody.Body)
    {
        auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
        if (isPhysicsSimulationEnabled)
            world->removeRigidBody(rigidBody.Body);
        btVector3 inertia;
        rigidBody.Body->getCollisionShape()->calculateLocalInertia(mass, inertia);
        rigidBody.Body->setActivationState(DISABLE_DEACTIVATION);
        rigidBody.Body->setMassProps(mass, inertia);
        rigidBody.Body->setLinearFactor(btVector3(1, 1, 1));
        rigidBody.Body->setAngularFactor(btVector3(1, 1, 1));
        rigidBody.Body->updateInertiaTensor();
        rigidBody.Body->clearForces();
        if (isPhysicsSimulationEnabled)
            world->addRigidBody(rigidBody.Body);
    }
    //mMonoComponent->SetMass(mass);
}

void RigidBodyComponent::SetGravity(float gravity)
{
    if (rigidBody.Body)
    {
        auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
        if (isPhysicsSimulationEnabled)
            world->removeRigidBody(rigidBody.Body);
        rigidBody.Body->setGravity(btVector3(0, -gravity, 0));
        rigidBody.Body->setFlags(rigidBody.Body->getFlags() | BT_DISABLE_WORLD_GRAVITY);
        if (isPhysicsSimulationEnabled)
            world->addRigidBody(rigidBody.Body);
    }
}

void RigidBodyComponent::SetLinearVelocity(btVector3 velocity)
{
    if (rigidBody.Body)
    {
        rigidBody.Body->activate(true);
        rigidBody.Body->setLinearVelocity(velocity);
    }
}

void RigidBodyComponent::SetRigidBodyType(RigidBodyType type)
{
    rbType = type;
}

void RigidBodyComponent::SetRigidBodyUsage(RigidBodyUsage usage)
{
    Usage = usage;
}

void RigidBodyComponent::SetCollisionShapeType(CollisionShapeType type)
{
    ShapeType = type;
}

void RigidBodyComponent::CreateShape(Vector3 scale)
{
    switch (ShapeType)
    {
    case CollisionShapeType::BOX:
    {
        Shape = new btBoxShape(btVector3(scale.x / 2, scale.y / 2, scale.z / 2));
        break;
    }
        
    case CollisionShapeType::SPHERE:
    {
        Shape = new btSphereShape(scale.x);
        break;
    }
    case CollisionShapeType::CAPSULE:
    {
        Shape = new btCapsuleShape(scale.x, scale.y);
        break;
    }
    default:
        Shape = new btEmptyShape();
        break;
    }
}

void RigidBodyComponent::SetCollisionShape(CollisionShapeType type, Vector3 scale)
{
    if (rigidBody.Body && Shape)
    {
        if (Usage == RigidBodyUsage::PHYSICS)
            Usage = RigidBodyUsage::COLLISIONS_AND_PHYSICS;

        ShapeType = type;
        CreateShape(scale);

        auto world = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
        if (isPhysicsSimulationEnabled)
            world->removeRigidBody(rigidBody.Body);
        btVector3 inertia;
        rigidBody.Body->setCollisionShape(Shape);
        rigidBody.Body->getCollisionShape()->calculateLocalInertia(Mass, inertia);
        rigidBody.Body->setActivationState(DISABLE_DEACTIVATION);
        rigidBody.Body->setMassProps(Mass, inertia);
        rigidBody.Body->setLinearFactor(btVector3(1, 1, 1));
        rigidBody.Body->setAngularFactor(btVector3(1, 1, 1));
        rigidBody.Body->updateInertiaTensor();
        rigidBody.Body->clearForces();

        if (isPhysicsSimulationEnabled)
            world->addRigidBody(rigidBody.Body);
    }
}

void RigidBodyComponent::MakeKinematic()
{
    rigidBody.Body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    rbType = RigidBodyType::KINEMATIC;
    rigidBody.Body->forceActivationState(DISABLE_DEACTIVATION);
}

void RigidBodyComponent::MakeDynamic()
{
    rigidBody.Body->setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT);
    rbType = RigidBodyType::DYNAMIC;
    rigidBody.Body->forceActivationState(DISABLE_DEACTIVATION);
}

void RigidBodyComponent::MakeStatic()
{
    rigidBody.Body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    rbType = RigidBodyType::STATIC;
    SetMass(0.0f);
    rigidBody.Body->forceActivationState(DISABLE_DEACTIVATION);
}

void RigidBodyComponent::RegisterRigidBodyType()
{
    switch (rbType)
    {
    case RigidBodyType::STATIC:
        rigidBody.Body->setCollisionFlags(this->rigidBody.Body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        break;
    case RigidBodyType::DYNAMIC:
        rigidBody.Body->setCollisionFlags(this->rigidBody.Body->getCollisionFlags() | btCollisionObject::CF_DYNAMIC_OBJECT);
        break;
    case RigidBodyType::KINEMATIC:
        rigidBody.Body->setCollisionFlags(this->rigidBody.Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        break;
    default:
        break;
    }

    rigidBody.Body->forceActivationState(DISABLE_DEACTIVATION);
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
        rigidBody.Body->setWorldTransform(PhysicsTransform);
        break;
    }*/
	case RigidBodyType::DYNAMIC:
	{
	if (rigidBody.Body && rigidBody.Body->getMotionState())
        {
            rigidBody.Body->getMotionState()->getWorldTransform(PhysicsTransform);
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
        rigidBody.Body->getMotionState()->setWorldTransform(PhysicsTransform);
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

    if (rigidBody.Body) {
        // Kinimatic bodies take their transform from motion state, so we need to update it too
        if (rigidBody.Body->isKinematicObject() && rigidBody.Body->getMotionState())
        {
            rigidBody.Body->getMotionState()->setWorldTransform(PhysicsTransform);
        }
        else
        {
            // this funcion seems to be too strong when using with kinematic bodies, resulting in visible overlap
            // probably beacause right now physics is not updated every frame
            rigidBody.Body->setCenterOfMassTransform(PhysicsTransform);
        }

		if (ShapeType == CollisionShapeType::BOX || ShapeType == CollisionShapeType::SPHERE || ShapeType == CollisionShapeType::CAPSULE)
		{
			btConvexInternalShape* castedShape = static_cast<btConvexInternalShape*>(Shape);
			Vector3 scale = InTransform.Scale;
			if (ShapeType == CollisionShapeType::BOX)
				scale /= 2;
			castedShape->setImplicitShapeDimensions(btVector3(scale.x, scale.y, scale.z));
		}

        if (InTeleportType == TeleportType::ResetPhysics)
        {
            rigidBody.Body->clearForces();
            rigidBody.Body->clearGravity();
            rigidBody.Body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
            rigidBody.Body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
            // todo: make sure everything is cleared
        }
    }
    else if (rigidBody.Collision)
    {
        rigidBody.Collision->setWorldTransform(PhysicsTransform);
    }
}

ComponentType RigidBodyComponent::GetComponentType()
{
    switch (ShapeType)
    {
    case CollisionShapeType::BOX:
        return RigidBodyCubeType;
        break;
    case CollisionShapeType::SPHERE:
        return RigidBodySphereType;
        break;
    case CollisionShapeType::CAPSULE:
        return Undefined;
        break;
    default:
        break;
    }
	
}

json RigidBodyComponent::Serialize() const
{
    auto json = SceneComponent::Serialize();
    assert(json.is_object());

    json["mass"] = Mass;
    json["type"] = rbType;
    json["physics_simulation"] = isPhysicsSimulationEnabled;
    json["need_physics"] = simulationNeedsEnabling;
    json["usage"] = Usage;
    json["shape_type"] = ShapeType;

    return json;
}

void RigidBodyComponent::Deserialize(const json* in)
{
    SetMass(in->at("mass").get<float>());
    SetRigidBodyType(in->at("type").get<RigidBodyType>());
    SetRigidBodyUsage(in->at("usage").get<RigidBodyUsage>());
    SetCollisionShapeType(in->at("shape_type").get<CollisionShapeType>());

	SceneComponent::Deserialize(in);

    Init();

    simulationNeedsEnabling = in->at("need_physics");
    isPhysicsSimulationEnabled = in->at("physics_simulation");

    
     SetPhysicsSimulation();
    
    
}

auto RigidBodyComponent::EnablePhysicsSimulation() -> void
{   

    if (!rigidBody.Body) Init();

    {
        int before = PhysicsModuleData::GetInstance()->GetDynamicsWorld()->getNumCollisionObjects();
        PhysicsModuleData::GetInstance()->GetDynamicsWorld()->addRigidBody(rigidBody.Body);
        isPhysicsSimulationEnabled = true;
        int after = PhysicsModuleData::GetInstance()->GetDynamicsWorld()->getNumCollisionObjects();
        bool WasAddedOnce = after == before + 1;
    }
}

auto RigidBodyComponent::DisablePhysicsSimulation() -> void
{   
    if (isPhysicsSimulationEnabled) {

        int before = PhysicsModuleData::GetInstance()->GetDynamicsWorld()->getNumCollisionObjects();

        PhysicsModuleData::GetInstance()->GetDynamicsWorld()->removeRigidBody(rigidBody.Body);
        isPhysicsSimulationEnabled = false;

        int after = PhysicsModuleData::GetInstance()->GetDynamicsWorld()->getNumCollisionObjects();
        bool WasNOTDeleted = after == before;
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
