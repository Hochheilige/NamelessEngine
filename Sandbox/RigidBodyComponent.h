#pragma once

#include "btBulletDynamicsCommon.h"
#include "PhysicsModule.h"

class RigidBodyComponent
{
public:
	RigidBodyComponent(){}

	~RigidBodyComponent();

	void Init();
	void OnRegister();

	friend RigidBodyComponent* CreateCubeComponent(
		float trans_x, float trans_y, float trans_z,
		float rot_x, float rot_y, float rot_z,
		float scale_x, float scale_y, float scale_z,
		float mass
	);

	friend RigidBodyComponent* CreateSphereComponent(
		float trans_x, float trans_y, float trans_z,
		float rot_x, float rot_y, float rot_z,
		float scale_x, float scale_y, float scale_z,
		float mass
	);

	// TODO temp logic
	btTransform Update();

private:
	btCollisionShape* Shape;
	btRigidBody* Body;
	btTransform Transform;
	btScalar Mass;
};


