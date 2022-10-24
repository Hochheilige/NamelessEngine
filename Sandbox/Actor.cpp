#include "Actor.h"

Actor::Actor(Transform transform) : mTransform(transform)
{}

Transform Actor::GetTransform() const
{
	return mTransform;
}

Transform Actor::GetWorldTransform() const
{
	if (Parent == nullptr)
	{
		return mTransform;
	}

	return mTransform.TransformToWorld(Parent->GetWorldTransform());
}

Actor* Actor::GetParent() const
{
	return Parent;
}

void Actor::SetParent(Actor* InParent)
{
	if (InParent == this)
	{
		return;
	}

	if (Parent != nullptr)
	{
		Parent->Children.erase(
			remove(Parent->Children.begin(), Parent->Children.end(), this), Parent->Children.end());
	}

	Parent = InParent;

	if (Parent != nullptr)
	{
		Parent->Children.push_back(this);
	}
}

Actor* Actor::GetChild(Actor* Child) const
{
	auto foundChild = find(Children.begin(), Children.end(), Child);
	if (foundChild == Children.end())
	{
		return nullptr;
	}

	return *foundChild;
}

void Actor::AddChild(Actor* Child)
{
	Children.push_back(Child);
}


void Actor::Update(float DeltaTime)
{
	for (auto component : Components)
	{
		component->Update(DeltaTime);
	}
}

void Actor::RemoveChild(Actor* Child)
{
	Children.erase(remove(Children.begin(), Children.end(), Child), Children.end());
}

