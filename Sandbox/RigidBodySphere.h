#pragma once

#include "RigidBodyComponent.h"
#include "MonoObjects/MonoPhysicsComponent.h"

class RigidBodySphere : public RigidBodyComponent
{
public:

	RigidBodySphere();

	virtual void Init() override;

	virtual void OnRegister() override;

	ComponentType GetComponentType() override { return RigidBodySphereType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	void OnDeserializationCompleted() override;

	static Component* Create()
	{
		return new RigidBodySphere();
	}
private:
	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;
};
