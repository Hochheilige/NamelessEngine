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

	void SetPhysicsTransform(Transform transform);

	void RegisterRigidBodyType();

	void Update(float DeltaTime);

	void MakeKinematic();

	void MakeDynamic();

	void MakeStatic();

	auto HandleSelectedComponentChanded(Component* newSelectedComponent) -> void;

protected:
	btCollisionShape* Shape;
	btRigidBody* Body;
	btScalar Mass;
	btTransform PhysicsTransform;

	RigidBodyType rbType;
	RigidBodyType OriginType;

public:
	bool is_kinematic = false;
};


