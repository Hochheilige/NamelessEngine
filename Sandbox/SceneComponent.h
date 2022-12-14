#pragma once

#include "Component.h"
#include "Transform.h"
#include "JsonInclude.h"

#include <string>

#include "MonoObjects/MonoSceneComponent.h"

enum class TeleportType
{
	// todo: add a flag to move a physics body with a sweep
	TeleportPhysics,
	ResetPhysics
};

class SceneComponent : public Component
{
public:

	friend class ImGuiSubsystem;
	
	// todo: add funcions to set world transform of scene components
	// Transform get/update funcions
	virtual auto SetTransform(const Transform& InTransform, TeleportType InTeleportType = TeleportType::TeleportPhysics) -> void;
	auto GetTransform() const -> const Transform&;

	//auto SetPosition(const Vector3& InPosition) -> void;
	auto GetPosition() const -> const Vector3& { return GetTransform().Position; }

	//auto SetScale(const Vector3& InScale) -> void;
	auto GetScale() const -> const Vector3& { return GetTransform().Scale; }

	//auto SetEulerDegrees(const Vector3& InEulerDegrees) -> void;
	//auto SetEulerDegrees(float InYaw, float InPitch, float InRoll) -> void;
	auto GetEulerDegrees() const -> const Vector3& { return GetTransform().Rotation.GetEulerDegrees(); }

	
public: // Relative transform get/update funcions
	auto GetRelativeTransform() const -> const Transform& { return mTransform; }
	auto SetRelativeTransform(const Transform& InTransform) -> void { mTransform = InTransform; }

	auto SetRelativePosition(const Vector3& InPosition) -> void { mTransform.Position = InPosition; }
	auto GetRelativePosition() const -> const Vector3& { return mTransform.Position; }

	auto SetRelativeScale(const Vector3& InScale) -> void { mTransform.Scale = InScale; }
	auto GetRelativeScale() const -> const Vector3& { return mTransform.Scale; }

	auto SetRelativeEulerDegrees(const Vector3& InEulerDegrees) -> void { mTransform.Rotation = Rotator(InEulerDegrees); }
	auto SetRelativeEulerDegrees(float InYaw, float InPitch, float InRoll) -> void { mTransform.Rotation = Rotator({ InYaw, InPitch, InRoll }); }
	auto GetRelativeEulerDegrees() const -> const Vector3& { return mTransform.Rotation.GetEulerDegrees(); }

	
public: // Attachment related fucntions
	auto SetAttachmentParent(SceneComponent* InAttachmentParent, std::string InAttachSocketName = "") -> void;
	auto GetAttachmentParent() const -> SceneComponent* { return mAttachmentParent; }

	auto GetAttachedChildren() const -> const std::vector<SceneComponent*>& { return AttachedChildren; }

	ComponentType GetComponentType() override { return mType; }
	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	//Serialization Part
	json Serialize() const override;
	void Deserialize(const json* in) override;

	static Component* Create()
	{
		return new SceneComponent();
	}
private:

	ComponentType mType = SceneComponentType;
	MonoSceneComponent* mMonoComponent = new MonoSceneComponent;
private:
	auto GetAttahcmentRoot() -> SceneComponent*;

private:
	Transform mTransform;

	SceneComponent* mAttachmentParent = nullptr;
	std::vector<SceneComponent*> AttachedChildren;
};

