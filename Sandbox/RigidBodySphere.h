#pragma once

#include "RigidBodyComponent.h"
#include "MonoObjects/MonoPhysicsComponent.h"

class RigidBodySphere : public RigidBodyComponent
{
public:

	RigidBodySphere();

	virtual void Init() override;

	virtual void OnRegister() override;

	ComponentType GetComponentType() override { return mType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }
private:

	ComponentType mType = ComponentType::RigidBodySphereType;
	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;
};
