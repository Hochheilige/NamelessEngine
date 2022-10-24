#pragma once

class Component
{
public:
	virtual void Init() = 0;
	virtual void OnRegister() = 0;
	virtual void Update(float DeltaTime) = 0;
};