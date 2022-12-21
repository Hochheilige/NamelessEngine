#include "RigidBodyCube.h"

RigidBodyCube::RigidBodyCube()
{

}

void RigidBodyCube::Init()
{
	auto physics = PhysicsModuleData::GetInstance();

	const Transform& transform = GetTransform();

	Shape = new btBoxShape(btVector3(transform.Scale.x / 2, transform.Scale.y / 2, transform.Scale.z / 2));
	physics->AddCollisionShape(Shape);

	PhysicsTransform.setIdentity();
	PhysicsTransform.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));
	Quaternion quaternion = transform.Rotation.GetQuaterion();
	PhysicsTransform.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));

	btVector3 localInertia = btVector3(0, 0, 0);
	if (Mass != 0.0f)
		Shape->calculateLocalInertia(Mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(PhysicsTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(Mass, myMotionState, Shape, localInertia);
	Body = new btRigidBody(rbInfo);

	RegisterRigidBodyType();
}

void RigidBodyCube::OnRegister()
{
}

void RigidBodyCube::OnDeserializationCompleted()
{
	RigidBodyComponent::OnDeserializationCompleted();

	//spent 2 hours trying to figure out what's wrong
	// turned out Init() was called twice((
	//Init();
}


