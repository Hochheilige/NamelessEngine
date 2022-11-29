#include "SceneComponent.h"
#include "Serializer.h"

auto SceneComponent::SetTransform(const Transform& InTransform, TeleportType InTeleportType/* = ETeleportType::TeleportPhysics*/) -> void
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

json SceneComponent::Serialize() const
{
	json out = json::object();
	out["transform"] = mTransform;
	return out;
}

void SceneComponent::Deserialize(const json* in)
{
	assert(in->is_object());

	mTransform = in->at("transform");
}

auto SceneComponent::GetAttahcmentRoot() -> SceneComponent*
{
	SceneComponent* sceneComp;
	for (sceneComp = this; sceneComp && sceneComp->GetAttachmentParent(); sceneComp = sceneComp->GetAttachmentParent());
	return sceneComp;
}
