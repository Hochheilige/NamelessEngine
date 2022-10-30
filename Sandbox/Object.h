#pragma once

#include <string>

class Object
{
public:

	virtual void Update(float DeltaTime) = 0;

	virtual ~Object() = default;

	auto GetName() -> const std::string& { return Name; }

private:
	std::string Name;
};