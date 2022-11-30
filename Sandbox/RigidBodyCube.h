#pragma once

#include "RigidBodyComponent.h"
#include "MonoObjects/MonoPhysicsComponent.h"

class RigidBodyCube : public RigidBodyComponent
{
public:

	RigidBodyCube();

	virtual void Init() override;

	virtual void OnRegister() override;

	ComponentType GetComponentType() override { return RigidBodyCubeType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	void OnDeserializationCompleted() override;

	static Component* Create()
	{
		return new RigidBodyCube();
	}
private:
	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;
};
