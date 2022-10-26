#pragma once

#include "btBulletDynamicsCommon.h"
#include "PhysicsModule.h"
#include "Component.h"

class RigidBodyComponent : public Component
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


