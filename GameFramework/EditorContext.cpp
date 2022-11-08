#include "EditorContext.h"

#include "Component.h"

auto EditorContext::BindToOnSelectedComponentChanged(SelectedComponentChangedDelegate Delegate) -> DelegateHandle
{
	return OnSelectedComponentChanged.Add(std::move(Delegate));
}

auto EditorContext::UnbindFromSelectedComponentChanged(DelegateHandle& handle) -> void
{
	OnSelectedComponentChanged.Remove(handle);
}


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
