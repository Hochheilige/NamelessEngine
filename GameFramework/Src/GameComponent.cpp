#include "GameComponent.h"
#include <algorithm>

using namespace std;

Transform GameComponent::GetWorldTransform() const
{
	// todo: cache world transform for a frame?
	if (Parent == nullptr)
	{
		return mTransform;
	}

    return mTransform.TransformToWorld(Parent->GetWorldTransform());
}

void GameComponent::SetParent(GameComponent* InParent)
{
	if (InParent == this)
	{
		return;
	}

	// Remove self from the old parent's children
	if (Parent != nullptr)
	{
		Parent->Children.erase(remove(Parent->Children.begin(), Parent->Children.end(), this), Parent->Children.end());
	}

	Parent = InParent;

	// Add self to the new parent's children
	if (Parent != nullptr)
	{
		Parent->Children.push_back(this);
	}
}
