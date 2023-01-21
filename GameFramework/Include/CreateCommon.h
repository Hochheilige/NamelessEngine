#pragma once

#include <type_traits>

#include "Game.h"
#include <memory>

class Actor;
class Object;

template<class T>
auto CreateObject() -> std::shared_ptr<T>
{
	static_assert(std::is_base_of_v<Object, T>, "Use this only to create objects");
	std::shared_ptr<T> newObject = std::make_shared<T>();
	newObject->weakThis = newObject;

	return newObject;
}

template<class T>
auto CreateActor() -> std::shared_ptr<T>
{
	static_assert(std::is_base_of_v<Actor, T>, "Use this only to create actors");

	std::shared_ptr<T> newActor = CreateObject<T>();
	if (newActor)
		Game::GetInstance()->Actors.push_back(newActor);

	return newActor;
}
