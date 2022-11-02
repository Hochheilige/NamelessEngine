#pragma once

#include "RigidBodyComponent.h"

class RigidBodySphere : public RigidBodyComponent
{
public:

	RigidBodySphere();

	void SetMass(float mass);

	virtual void Init() override;

	virtual void OnRegister() override;

	//virtual void Update(float DeltaTime) override;

private:

};
