#pragma once

#include "MathInclude.h"
#include "ImGuiInclude.h"
#include "JsonInclude.h"
#include "ImGuiNodeEditorInclude.h"
#include "ImGuiNodeEditorManager.h"

#include <string>
#include <vector>
#include <memory>

#include <filesystem>
using Path = std::filesystem::path;

class Game;
class EditorContext;
class SceneComponent;
class DirectoryTreeNode;
class AssetManager;

enum CONTEXT_MENU_VALUES {
	NOTHING = -1,
	RENAME = 0,
	DUPLICATE_,
	DELETE_
};

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

	auto GetViewportSize() const -> const Vector2& { return ViewportSize; }

	auto GetIsViewportFocused() -> bool { return isViewportFocused; }

	auto OnSceneLoaded() -> void;

	auto GetTopLevelWindowClass() const -> const ImGuiWindowClass* { return &topLevelClass; }

	auto GetNextWindowClassId() -> int { return nextClassId++; }

private:
	auto LayOutMainMenuBar() -> void;
	auto DrawDockspace() -> void;
	auto DrawToolbar() -> void;
	auto DrawViewport() -> void;
	auto DrawActorExplorer() -> void;
	// begin inspector
	auto DrawComponentSelector(class Actor* actor) -> void;
	auto CanChangeGuizmo() -> bool;
	auto LayOutTransform() -> void;
	auto DrawActorInspector() -> void;
	auto DrawGeneralProperties(class Actor* actor) -> void;
	auto DrawRigidBodyProperties(Actor* actor) -> void;
	auto DrawStaticMeshProperties() -> void;
	auto DrawLightPointProperties(Actor* actor) -> void;
	// end instpector
	auto DrawGizmos() -> void;
	auto DrawMessagesWindow() -> void;
	auto DrawBasicActorsWindow() -> void;
	// asset browser
	auto DrawAssetBrowser() -> void;
	auto DrawAsset(const DirectoryTreeNode* file, const Vector2& itemSize = Vector2(80, 110)) -> void;
	// World settings
	auto DrawWorldSettings() -> void;
	auto DrawNavMeshSettings() -> void;
	// Node Editor
	auto DrawNodeEditor(ned::EditorContext* nodeEditorContext) -> void;

private:
	auto PollHotkeys() -> void;

private:
	auto GetEditorContext() const -> EditorContext&;
	auto GetSelectedSceneComponent() const->SceneComponent*;
	auto GetAssetManager() const->AssetManager*;

	auto InitStyle() -> void;

	auto ActorBrowserContextMenu(Actor* actor) const -> CONTEXT_MENU_VALUES;
	auto ActorBrowserContextMenu(Actor* actor, const char* str_id) const -> CONTEXT_MENU_VALUES;
	auto ActorBrowserContextMenuPopUp(Actor* actor) const -> CONTEXT_MENU_VALUES;

	// Helper style functions
	auto BoldHeader(const char* label, ImGuiTreeNodeFlags flags) const -> bool;
	auto BoldText(const char* label) const -> void;


	//Toolbar buttons
	auto ToolBarStopButton(const char stopText[]) -> void;


private:
	Game* MyGame;

	Vector2 ViewportMousePos;

	Vector2 ViewportStart;
	Vector2 ViewportSize;

	ImFont* mainFont;
	ImFont* headerFont;

	// Gizmo state
private:
	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;
	bool useSnap;
	Vector3 snap;



private:

	std::vector<std::string> MessagesToDisplay;

	static ImGuiSubsystem* Instance;


	int nextClassId = 1;
	ImGuiWindowClass topLevelClass;
	ImGuiWindowClass levelEditorClass;

	auto DrawFBXInspector(const Path& path) -> void;

	std::vector<Path> OpenedFbxInspectorWindows;

	Path currentLevel;
	char tempsavepath[512]{"../x64/gameTemp.json"};

	bool isViewportFocused;

	bool doDebug = false;

	bool isPhysicsEnabled;

	Vector3 dirLightRotation;

	ImGuiNodeEditorManager nodeEditorManager;
};
