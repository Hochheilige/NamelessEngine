#include "EditorContext.h"

#include "Component.h"

auto EditorContext::SetSelectedActor(Actor* InActor) -> void
{
	SelectedActor = InActor;
	SelectedComponent = nullptr;
}

auto EditorContext::SetSelectedComponent(Component* InComponent) -> void
{
	SelectedComponent = InComponent;
	if (SelectedComponent)
	{
		SelectedActor = SelectedComponent->GetOwner();
	}
}
