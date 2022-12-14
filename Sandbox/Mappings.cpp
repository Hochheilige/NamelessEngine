#include "Mappings.h"
#include "Sandbox.h"

Component* Mappings::CreateComponent(Actor* actor, int compType)
{
	//return actor->AddComponent<RigidBodyComponent>();
	auto box_rb = actor->AddComponent<RigidBodyComponent>();
	Transform tr;
	tr.Position = Vector3(16, 3, 0);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(4.0, 4.0, 20.0);
	actor->SetTransform(tr);
	box_rb->SetRigidBodyType(RigidBodyType::STATIC);
	box_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
	box_rb->SetCollisionShapeType(CollisionShapeType::BOX);
	box_rb->SetMass(0);
	box_rb->Init();

	return box_rb;
}
