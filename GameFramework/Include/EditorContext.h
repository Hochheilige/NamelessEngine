#pragma once

#include "Delegates.h"

class Actor;
class Component;


//DECLARE_MULTICAST_DELEGATE(SelectedActorChanged, Actor* /*New Selected Actor*/);
DECLARE_MULTICAST_DELEGATE(SelectedComponentChanged, Component* /*New Selected Component*/);

class EditorContext
{

public:
	SelectedComponentChanged OnSelectedComponentChanged;
	auto BindToOnSelectedComponentChanged(SelectedComponentChangedDelegate Delegate) -> DelegateHandle;
	auto UnbindFromSelectedComponentChanged(DelegateHandle& handle) -> void;

	auto SetSelectedActor(Actor* InActor) -> void;
	auto GetSelectedActor() const->Actor* { return SelectedActor; }

	auto SetSelectedComponent(Component* InComponent) -> void;
	auto GetSelectedComponent()->Component* const { return SelectedComponent; }

private:
	Actor* SelectedActor = nullptr;
	Component* SelectedComponent;


};

