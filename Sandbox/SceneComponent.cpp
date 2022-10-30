#include "SceneComponent.h"

auto SceneComponent::SetTransform(const Transform& InTransform) -> void
{
	// Calculate relative transform
	if (SceneComponent* sceneComp = GetAttachmentParent())
	{
		mTransform = InTransform.GetTransformMatrix() * sceneComp->GetTransform().GetInverseTransformMatrix();
	}
	else
	{
		mTransform = InTransform;
	}
}

auto SceneComponent::GetTransform() const -> const Transform&
{
	if (mAttachmentParent == nullptr)
	{
		return mTransform;
	}

	return mTransform.TransformToWorld(mAttachmentParent->GetTransform());
}

auto SceneComponent::SetAttachmentParent(SceneComponent* InAttachmentParent, std::string InAttachSocketName) -> void
{
	if (InAttachmentParent == this)
	{
		return;
	}

	// todo: Check for loops when attaching

	if (mAttachmentParent != nullptr)
	{
		mAttachmentParent->AttachedChildren.erase(
			remove(mAttachmentParent->AttachedChildren.begin(), mAttachmentParent->AttachedChildren.end(), this), mAttachmentParent->AttachedChildren.end());
	}

	mAttachmentParent = InAttachmentParent;

	if (mAttachmentParent != nullptr)
	{
		mAttachmentParent->AttachedChildren.push_back(this);
	}
}

auto SceneComponent::GetAttahcmentRoot() -> SceneComponent*
{
	SceneComponent* sceneComp;
	for (sceneComp = this; sceneComp && sceneComp->GetAttachmentParent(); sceneComp = sceneComp->GetAttachmentParent());
	return sceneComp;
}
