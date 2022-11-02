#pragma once

#include "RigidBodyComponent.h"
#include "MonoObjects/MonoPhysicsComponent.h"

class RigidBodyCube : public RigidBodyComponent
{
public:

	RigidBodyCube();

	void SetMass(float mass);

	virtual void Init() override;

	virtual void OnRegister() override;

	//virtual void Update(float DeltaTime) override;

	ComponentType GetComponentType() override { return mType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }
private:

	ComponentType mType = ComponentType::RigidBodyCubeType;
	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;
};
