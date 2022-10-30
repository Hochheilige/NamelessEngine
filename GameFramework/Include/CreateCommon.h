#pragma once

#include <type_traits>

#include "Game.h"

class Actor;

template<class T>
auto CreateActor() -> T*
{
	static_assert(std::is_base_of_v<Actor, T>, "Use this only to create actors");

	T* newActor = new T();

	if (newActor)
		Game::GetInstance()->Actors.push_back(newActor);

	return newActor;
}