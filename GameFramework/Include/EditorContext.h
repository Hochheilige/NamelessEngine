#pragma once

#include "Delegates.h"
#include <memory>

class Actor;
class Component;

#include <filesystem>

using Path = std::filesystem::path;

//DECLARE_MULTICAST_DELEGATE(SelectedActorChanged, Actor* /*New Selected Actor*/);
DECLARE_MULTICAST_DELEGATE(SelectedComponentChanged, Component* /*New Selected Component*/);

class EditorContext
{

public:
	SelectedComponentChanged OnSelectedComponentChanged;
	auto BindToOnSelectedComponentChanged(SelectedComponentChangedDelegate Delegate) -> DelegateHandle;
	auto UnbindFromSelectedComponentChanged(DelegateHandle& handle) -> void;

	auto SetSelectedActor(std::weak_ptr<Actor> IActor) -> void;
	auto GetSelectedActor() const->std::weak_ptr<Actor> { return SelectedActor; }

	auto SetSelectedComponent(Component* InComponent) -> void;
	auto GetSelectedComponent() const ->Component*  { return SelectedComponent; }

	auto SetSelectedDirectory(const Path& path) -> void { selectedDirectory = path; }
	auto GetSelectedDirectory() const -> const Path& { return selectedDirectory; }

private:
	std::weak_ptr<Actor> SelectedActor;
	Component* SelectedComponent;

	Path selectedDirectory;

};

