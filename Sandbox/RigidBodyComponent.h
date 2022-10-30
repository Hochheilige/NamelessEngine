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

protected:
	btCollisionShape* Shape;
	btRigidBody* Body;
	btScalar Mass;
	btTransform PhysicsTransform;
};


