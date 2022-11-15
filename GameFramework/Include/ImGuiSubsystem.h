#pragma once

#include "MathInclude.h"
#include "ImGuiInclude.h"

#include <string>
#include <vector>

class Game;
class EditorContext;
class SceneComponent;

namespace ImGuizmo
{
	enum OPERATION;
	enum MODE;
}

class ImGuiSubsystem
{
public:
	ImGuiSubsystem();

	auto Initialize(Game* const InGame) -> void;

	auto NewFrame() -> void;
	auto Render() -> void;
	auto EndFrame() -> void;

	auto DoLayout() -> void;

	auto Shutdown() -> void;

	auto AddMessageToDisplay(const std::string& Msg) -> void;

	~ImGuiSubsystem();

	static auto GetInstance() { return Instance; }
private:
	auto LayOutMainMenuBar() -> void;
	auto DrawDockspace() -> void;
	auto DrawToolbar() -> void;
	auto DrawViewport() -> void;
	auto DrawActorExplorer() -> void;
	// begin inspector
	auto DrawComponentSelector(class Actor* actor) -> void;
	auto LayOutTransform() -> void;
	auto DrawActorInspector() -> void;
	auto DrawGeneralProperties(class Actor* actor) -> void;
	auto DrawRigidBodyProperties(Actor* actor) -> void;
	// end instpector
	auto DrawGizmos() -> void;
	auto DrawMessagesWindow() -> void;
	auto DrawBasicActorsWindow() -> void;
	// asset browser
	auto DrawAssetBrowser() -> void;

private:
	auto GetEditorContext() const -> EditorContext&;
	auto GetSelectedSceneComponent() const->SceneComponent*;

	auto InitStyle() -> void;
private:
	Game* MyGame;

	Vector2 ViewportMousePos;

	Vector2 ViewportStart;
	Vector2 ViewportSize;

	ImFont* mainFont;

	// Gizmo state
private:
	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;
	bool useSnap;
	Vector3 snap;
private:

	std::vector<std::string> MessagesToDisplay;

	static ImGuiSubsystem* Instance;

	ImGuiWindowClass topLevelClass;
	ImGuiWindowClass levelEditorClass;
};
