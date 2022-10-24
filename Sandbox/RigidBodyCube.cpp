#include "RigidBodyCube.h"

RigidBodyCube::RigidBodyCube(std::shared_ptr<Transform> transform)
{
	auto physics = PhysicsModuleData::GetInstance();

	mTransform = transform;

	Shape = new btBoxShape(btVector3(transform->Scale.x / 2, transform->Scale.y / 2, transform->Scale.z / 2));
	physics->GetCollisionShapes().push_back(Shape);
	
	
	PhysicsTransform.setIdentity();
	PhysicsTransform.setOrigin(btVector3(transform->Position.x, transform->Position.y, transform->Position.z));
	Quaternion quaternion = transform->Rotation.GetQuaterion();
	PhysicsTransform.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));	
}

void RigidBodyCube::SetMass(float mass)
{
	Mass = mass;
}

void RigidBodyCube::Init()
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

void RigidBodyCube::OnRegister()
{
}

void RigidBodyCube::Update(float DeltaTime)
{
	if (Body && Body->getMotionState())
    {
        Body->getMotionState()->getWorldTransform(PhysicsTransform);
    }

	mTransform->Position = Vector3(PhysicsTransform.getOrigin().x(), PhysicsTransform.getOrigin().y(), PhysicsTransform.getOrigin().z());
	mTransform->Rotation = Quaternion(PhysicsTransform.getRotation());
}
