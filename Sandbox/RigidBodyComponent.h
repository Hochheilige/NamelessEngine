#pragma once

#include "btBulletDynamicsCommon.h"
#include "PhysicsModule.h"
#include "SceneComponent.h"

class RigidBodyComponent : public SceneComponent
{
public:

	virtual ~RigidBodyComponent();

	btRigidBody* GetRigidBody();
	btCollisionShape* GetCollisionShape();
	btScalar GetMass();

	void Update(float DeltaTime);

	void MakeKinematic();
	//void MakeNonKinematic();

protected:
	btCollisionShape* Shape;
	btRigidBody* Body;
	btScalar Mass;
	btTransform PhysicsTransform;

public:
	bool is_kinematic = false;
};


