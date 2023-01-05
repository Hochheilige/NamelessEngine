#pragma once

#include <string>

class Object
{
public:


	virtual void Update(float DeltaTime) = 0;

	virtual ~Object() = default;

	auto GetName() const -> const std::string& { return Name; }
	auto SetName(std::string s) -> void { Name = s; }

protected:
	std::string Name = "";
};