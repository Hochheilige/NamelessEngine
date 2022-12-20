#pragma once

#include "btBulletDynamicsCommon.h"
#include "PhysicsModule.h"
#include "SceneComponent.h"


enum class RigidBodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC
};

class RigidBodyComponent : public SceneComponent
{
public:

	friend class ImGuiSubsystem;

	RigidBodyComponent();

	virtual ~RigidBodyComponent();

	btRigidBody* GetRigidBody();

	btCollisionShape* GetCollisionShape();

	btScalar GetMass();

	RigidBodyType GetType();

	void SetMass(float mass);

	void SetGravity(float gravity);

	void SetRigidBodyType(RigidBodyType type);

	void SetLinearVelocity(btVector3 velocity);

	void RegisterRigidBodyType();

	virtual void Update(float DeltaTime) override;

	void MakeKinematic();

	void MakeDynamic();

	void MakeStatic();

	virtual auto SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void;

	ComponentType GetComponentType() override;

	json Serialize() const override;
	void Deserialize(const json* in) override;

	auto EnablePhysicsSimulation() -> void;
	auto DisablePhysicsSimulation() -> void;

protected:
	btCollisionShape* Shape;
	btRigidBody* Body;
	btScalar Mass;
	btTransform PhysicsTransform;

	// todo: do we need this? - we can query type using Body->isKinematicObject(), Body->isStaticObject()
	RigidBodyType rbType;
	RigidBodyType OriginType;

	bool isPhysicsSimulationEnabled = false;
	bool simulationNeedsEnabling = false;

public:
	// todo: do we need this?
	bool is_kinematic = false;


	auto SetPhysicsSimulation() -> void;
};


