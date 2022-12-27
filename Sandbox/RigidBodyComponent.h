#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "PhysicsModule.h"
#include "SceneComponent.h"


#include "MonoObjects/MonoPhysicsComponent.h"

enum class RigidBodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC
};

enum class RigidBodyUsage
{
	PHYSICS,
	COLLISIONS,
	COLLISIONS_AND_PHYSICS
};

enum class CollisionShapeType
{
	BOX,
	SPHERE,
	CAPSULE
};

class RigidBodyComponent : public SceneComponent
{
public:

	friend class ImGuiSubsystem;

	RigidBodyComponent();

	virtual void Init() override;

	virtual ~RigidBodyComponent();

	btRigidBody* GetRigidBody();

	btCollisionShape* GetCollisionShape();

	btScalar GetMass();

	RigidBodyType GetType();

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	void SetMass(float mass);

	void SetGravity(float gravity);

	void SetRigidBodyType(RigidBodyType type);

	void SetRigidBodyUsage(RigidBodyUsage usage);

	void SetCollisionShapeType(CollisionShapeType type);

	void SetCollisionShape(CollisionShapeType type, Vector3 scale);

	void SetLinearVelocity(btVector3 velocity);

	void RegisterRigidBodyType();

	virtual void Update(float DeltaTime) override;

	void MakeKinematic();

	void MakeDynamic();

	void MakeStatic();

	void CreateShape(Vector3 scale);

	virtual auto SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void;

	ComponentType GetComponentType() override;

	json Serialize() const override;
	void Deserialize(const json* in) override;

	auto EnablePhysicsSimulation() -> void;
	auto DisablePhysicsSimulation() -> void;

	static Component* Create()
	{
		return new RigidBodyComponent();
	}

protected:
	btCollisionShape* Shape;
	btScalar Mass;
	btTransform PhysicsTransform;

	struct RigidBody
	{
		btRigidBody* Body;
		btGhostObject* Collision;
	} rigidBody;
	
	RigidBodyUsage Usage;
	CollisionShapeType ShapeType;

	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;

	// todo: do we need this? - we can query type using Body->isKinematicObject(), Body->isStaticObject()
	RigidBodyType rbType;

	bool isPhysicsSimulationEnabled = false;
	bool simulationNeedsEnabling = false;

public:
	// todo: do we need this?
	bool is_kinematic = false;


	auto SetPhysicsSimulation() -> void;
	auto applyCentralImpulse(const Vector3& impulse) -> void {
		rigidBody.Body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
	}
};


