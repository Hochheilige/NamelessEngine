#include "RigidBodyCube.h"

// TODO: Right now our relationship with physics is one-directional:
// physics updates our transform, but we cannot change transform in the physics world

RigidBodyCube::RigidBodyCube()
{

}

void RigidBodyCube::SetMass(float mass)
{
	Mass = mass;
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

	//physics->GetDynamicsWorls()->addRigidBody(Body);
}

void RigidBodyCube::OnRegister()
{
}

void RigidBodyCube::Update(float DeltaTime)
{
	// TODO: this is literally the same code as in the other RigidBody's update -> this can be moved to the parent class
	if (Body && Body->getMotionState())
    {
        Body->getMotionState()->getWorldTransform(PhysicsTransform);
    }

	Transform t = GetTransform();
	t.Position = Vector3(PhysicsTransform.getOrigin().x(), PhysicsTransform.getOrigin().y(), PhysicsTransform.getOrigin().z());
	t.Rotation = Quaternion(PhysicsTransform.getRotation());
	SetTransform(t);
}
