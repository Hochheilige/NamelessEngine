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

	void Update(float DeltaTime);

	void MakeKinematic();

	void MakeDynamic();

	void MakeStatic();

	virtual auto SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void;

protected:
	btCollisionShape* Shape;
	btRigidBody* Body;
	btScalar Mass;
	btTransform PhysicsTransform;

	// todo: do we need this? - we can query type using Body->isKinematicObject(), Body->isStaticObject()
	RigidBodyType rbType;
	RigidBodyType OriginType;

public:
	// todo: do we need this?
	bool is_kinematic = false;
};


