#pragma once

#include "RigidBodyComponent.h"
#include "MonoObjects/MonoPhysicsComponent.h"

class RigidBodyCube : public RigidBodyComponent
{
public:

	RigidBodyCube();

	virtual void Init() override;

	virtual void OnRegister() override;

	ComponentType GetComponentType() override { return mType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }
private:

	ComponentType mType = ComponentType::RigidBodyCubeType;
	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;
};
