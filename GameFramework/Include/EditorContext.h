#pragma once


class Actor;
class Component;

class EditorContext
{

public:

	auto SetSelectedActor(Actor* InActor) -> void;
	auto GetSelectedActor() const->Actor* { return SelectedActor; }

	auto SetSelectedComponent(Component* InComponent) -> void;
	auto GetSelectedComponent()->Component* const { return SelectedComponent; }

private:
	Actor* SelectedActor = nullptr;
	Component* SelectedComponent;

};

