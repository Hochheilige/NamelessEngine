#include "RigidBodySphere.h"

RigidBodySphere::RigidBodySphere(std::shared_ptr<Transform> transform)
{
	auto physics = PhysicsModuleData::GetInstance();

	mTransform = transform;

	Shape = new btSphereShape(transform->Scale.x);
	physics->AddCollisionShape(Shape);


	PhysicsTransform.setIdentity();
	PhysicsTransform.setOrigin(btVector3(transform->Position.x, transform->Position.y, transform->Position.z));
	Quaternion quaternion = transform->Rotation.GetQuaterion();
	PhysicsTransform.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
}

void RigidBodySphere::SetMass(float mass)
{
	Mass = mass;
}

void RigidBodySphere::Init()
{
	auto physics = PhysicsModuleData::GetInstance();
	btVector3 localInertia = btVector3(0, 0, 0);
	if (Mass != 0.0f)
		Shape->calculateLocalInertia(Mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(PhysicsTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(Mass, myMotionState, Shape, localInertia);
	Body = new btRigidBody(rbInfo);

	physics->GetDynamicsWorls()->addRigidBody(Body);
}

void RigidBodySphere::OnRegister()
{
}

void RigidBodySphere::Update(float DeltaTime)
{
	if (Body && Body->getMotionState())
	{
		Body->getMotionState()->getWorldTransform(PhysicsTransform);
	}

	mTransform->Position = Vector3(PhysicsTransform.getOrigin().x(), PhysicsTransform.getOrigin().y(), PhysicsTransform.getOrigin().z());
	mTransform->Rotation = Quaternion(PhysicsTransform.getRotation());
}
