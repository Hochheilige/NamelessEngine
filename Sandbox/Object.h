#pragma once

#include <string>
#include <memory>

class Object
{
public:


	virtual void Update(float DeltaTime) = 0;

	virtual ~Object() = default;

	auto GetName() const -> const std::string& { return Name; }
	auto SetName(std::string s) -> void { Name = s; }

	auto AsSharedPtr() const -> std::shared_ptr<Object> { return weakThis.lock(); }
	auto AsWeakPtr() const -> std::weak_ptr<Object> { return weakThis; }

protected:
	std::string Name = "";

private:
	template<class T> friend auto CreateObject() -> std::shared_ptr<T>;
	std::weak_ptr<Object> weakThis;
};