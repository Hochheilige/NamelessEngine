#include "ImGuiSubsystem.h"

#include "ImGuiInclude.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include "imguizmo.h"

#include "Actor.h"
#include "Game.h"
#include "RenderingSystem.h"
#include "DisplayWin32.h"

#include <string>

#include "EngineContentRegistry.h"

#include "DirectoryTree.h"
#include "Serializator.h"

//temporary include
//#include "../External/bullet3/src/"

ImGuiSubsystem* ImGuiSubsystem::Instance = nullptr;

static const char* ActorDragDropSourceType = "ActorDragDropSourceType";

auto GetComponentTypeName(ComponentType type) -> std::string {
	std::string name = "";
	switch (type) {
	case MeshRendererType:
		name = "Mesh Renderer"; break;
	case RigidBodyCubeType:
		name = "Rigid Body Cube"; break;
	case RigidBodySphereType:
		name = "Rigid Body Sphere"; break;
	case LightPointType:
		name = "Point Light"; break;
	case SceneComponentType:
		name = "Scene Component"; break;
	}
	return name;
};

ImGuiSubsystem::ImGuiSubsystem()
	: mCurrentGizmoOperation(ImGuizmo::OPERATION::TRANSLATE)
	, mCurrentGizmoMode(ImGuizmo::MODE::WORLD)
	, useSnap(false)
	, MyGame(nullptr)
{
	Instance = this;
}

auto ImGuiSubsystem::Initialize(Game* const InGame) -> void
{
	MyGame = InGame;
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(MyGame->GetDisplay()->GetWindowHandle());
	ImGui_ImplDX11_Init(MyGame->GetD3DDevice().Get(), MyGame->GetD3DDeviceContext().Get());

	// TODO: get this from current camera and update it
	ImGuizmo::SetOrthographic(false);

	io.Fonts->AddFontDefault();
	mainFont = io.Fonts->AddFontFromFileTTF("../Assets/EngineContent/Fonts/Ubuntu-Light.ttf", 13.0f);
	IM_ASSERT(mainFont != NULL);

	io.ConfigWindowsMoveFromTitleBarOnly = true;

	InitStyle();

	GetEditorContext().SetSelectedDirectory("Assets");

	// Create imgui winodw classes
	// todo: use ImGui::GetID ?
	topLevelClass.ClassId = 1;
	topLevelClass.DockingAllowUnclassed = true;

	levelEditorClass.ClassId = 2;
	levelEditorClass.DockingAllowUnclassed = true;
	// do i want this?
	//levelEditorClass.DockingAlwaysTabBar = true;
}

auto ImGuiSubsystem::NewFrame() -> void
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

auto ImGuiSubsystem::Render() -> void
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

auto ImGuiSubsystem::EndFrame() -> void
{
	// needs to be called after imgui::Render
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

auto ImGuiSubsystem::DoLayout() -> void
{

	ImGui::PushFont(mainFont);

	LayOutMainMenuBar();

	// Add top-level dockspace
	ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_NoSplit, &topLevelClass);

	ImGui::SetNextWindowClass(&topLevelClass);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	// todo: put level name here
	const bool levelEditorOpen = ImGui::Begin("Level Editor");
	// add a level editor dockspace
	// Note: add this dockspace regardless of whether level editor window is open or not
	// otherwise the windows will get undocked
	ImGuiID dockspace_id = ImGui::GetID("LevelEditorDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None, &levelEditorClass);
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::SetNextWindowClass(&topLevelClass);
	static bool temp = true;
	ImGui::ShowDemoWindow(&temp);

	// level editor windows
	if (levelEditorOpen)
	{
		DrawToolbar();
		DrawViewport();
		DrawActorExplorer();
		DrawActorInspector();
		DrawBasicActorsWindow();
	}
	// common(unclassed) windows
	DrawAssetBrowser();
	DrawMessagesWindow();

	ImGui::PopFont();
}

auto ImGuiSubsystem::Shutdown() -> void
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

auto ImGuiSubsystem::AddMessageToDisplay(const std::string& Msg) -> void
{
	MessagesToDisplay.push_back(Msg);
}

ImGuiSubsystem::~ImGuiSubsystem()
{
	Shutdown();
}

auto ImGuiSubsystem::LayOutMainMenuBar() -> void
{
	ImGui::BeginMainMenuBar();
	ImGui::EndMainMenuBar();
}

auto ImGuiSubsystem::DrawDockspace() -> void
{
	
}

auto ImGuiSubsystem::DrawToolbar() -> void
{
	ImGui::SetNextWindowClass(&levelEditorClass);

	if (ImGui::Begin("Toolbar"))
	{
		const char playText[] = "Play";
		const char pauseText[] = "Pause";
		const char resumeText[] = "Resume";
		const char stopText[] = "Stop";

		switch (MyGame->GetPlayState())
		{
		case PlayState::Editor:
			if (ImGui::Button(playText))
			{
				MyGame->StartPlay();
			}
			break;
		case PlayState::Playing:
			if (ImGui::Button(pauseText))
			{
				MyGame->PausePlay();
			}
			/*ImGui::SameLine();
			if (ImGui::Button(stopText))
			{
				MyGame->StopPlay();
			}*/
			break;
		case PlayState::Paused:
			if (ImGui::Button(resumeText))
			{
				MyGame->ResumePlay();
			}
			/*ImGui::SameLine();
			if (ImGui::Button(stopText))
			{
				MyGame->StopPlay();
			}*/
			break;
		}

		ImGui::SameLine();

		if (ImGui::Button("Save"))
		{
			Serializator::SaveToFile("../Saves/game.json", Game::GetInstance());
		}
	}
	ImGui::End();
}

auto operator-(const ImVec2& l, const ImVec2& r) -> ImVec2
{
	return ImVec2{ l.x - r.x, l.y - r.y };
}

auto ImGuiSubsystem::DrawViewport() -> void
{
	ImGui::SetNextWindowClass(&levelEditorClass);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Viewport"))
	{
		ViewportStart = ImGui::GetCursorScreenPos();
		ViewportSize = ImGui::GetContentRegionAvail();
		ViewportMousePos = Vector2(ImGui::GetMousePos()) - ViewportStart;
		MyGame->MyRenderingSystem->HandleScreenResize({ ViewportSize.x, ViewportSize.y });
		ImGui::Image(MyGame->MyRenderingSystem->GetViewportTextureID(), ViewportSize);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ActorDragDropSourceType))
			{
				const std::string actorName = static_cast<const char*>(payload->Data);

				Transform t;
				t.Position = MyGame->MyRenderingSystem->GetWorldPositionUnerScreenPosition(ViewportMousePos);
				Actor* newActor = EngineContentRegistry::GetInstance()->CreateBasicActor(actorName, t);
				GetEditorContext().SetSelectedActor(newActor);
			}

			ImGui::EndDragDropTarget();
		}

		// Draw gizmos in the viewport
		{
			ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
			DrawGizmos();
		}
		// todo: move this somewhere more appropriate
		{
			if (ImGui::IsItemClicked() && !ImGuizmo::IsUsing())
			{
				GetEditorContext().SetSelectedActor(MyGame->MyRenderingSystem->GetActorUnderPosition(ViewportMousePos));
			}
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

auto ImGuiSubsystem::DrawActorExplorer() -> void
{
	ImGui::SetNextWindowClass(&levelEditorClass);
	if (ImGui::Begin("Actor Explorer"))
	{
		int i = 0;
		for (Actor* actor : MyGame->Actors)
		{
			const bool isSelectedActor = GetEditorContext().GetSelectedActor() == actor;

			if (actor->GetName() == "") {
				ImGui::Selectable((std::string("Actor") + std::to_string(i)).c_str(), isSelectedActor);
			}
			else {
				ImGui::Selectable(actor->GetName().c_str(), isSelectedActor);
			}

			if (ImGui::IsItemClicked())
			{
				GetEditorContext().SetSelectedActor(actor);
			}

			++i;
		}
	}

	ImGui::End();
}

auto ImGuiSubsystem::DrawComponentSelector(class Actor* actor) -> void {

	if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
		const bool isActorSelected = GetEditorContext().GetSelectedComponent() == nullptr;
		ImGui::AlignTextToFramePadding();
		if (ImGui::Selectable("Actor", isActorSelected))
		{
			GetEditorContext().SetSelectedComponent(nullptr);
		}

		// scene components:
		std::vector<SceneComponent*> stack;
		stack.push_back(actor->RootComponent);
		while (!stack.empty())
		{
			SceneComponent* comp = stack.back();
			stack.pop_back();

			if (comp == nullptr)
			{
				ImGui::TreePop();
				
				continue;
			}

			std::vector<SceneComponent*> ownChildren;
			ownChildren.reserve(comp->AttachedChildren.size());
			for (SceneComponent* child : comp->AttachedChildren)
			{
				// ignore attached components of other actors
				if (child->GetOwner() == actor)
				{
					ownChildren.push_back(child);
				}
			}

			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;
			const bool isLeaf = ownChildren.size() == 0;
			if (isLeaf)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;
			}
			const bool isSelected = comp == MyGame->MyEditorContext.GetSelectedComponent();
			if (isSelected)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
			}

			const bool isNodeOpen = ImGui::TreeNodeEx(GetComponentTypeName(comp->GetComponentType()).c_str(), nodeFlags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				GetEditorContext().SetSelectedComponent(comp);
			}
			
			if (isNodeOpen)
			{
				{
					stack.push_back(nullptr); // hack: use nullptr as a command to pop tree node
					stack.insert(stack.end(), ownChildren.begin(), ownChildren.end());
				}
			}
		}
		
		// TODO: Draw this only if the selected actor has non-scene components
		ImGui::Separator();
		
		// non scene components
		for (auto comp : actor->Components)
		{
			if (!dynamic_cast<SceneComponent*>(comp))
			{
				if (ImGui::Selectable("Non Scene Component", comp == MyGame->MyEditorContext.GetSelectedComponent()))
				{
					GetEditorContext().SetSelectedComponent(comp);
				}
			}
		}
	}
}

auto ImGuiSubsystem::LayOutTransform() -> void
{
	SceneComponent* ssc = GetSelectedSceneComponent();
	if (ssc == nullptr)
	{
		return;
	}

	if (!ImGui::CollapsingHeader("Transform")) {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 130), true, window_flags);

		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		Transform t = mCurrentGizmoMode == ImGuizmo::MODE::LOCAL ? ssc->GetRelativeTransform() : ssc->GetTransform();
		float rot[] = { t.Rotation.GetEulerDegrees().x, t.Rotation.GetEulerDegrees().y,
			t.Rotation.GetEulerDegrees().z };

		bool transformUpdated = false;
		transformUpdated |= ImGui::DragFloat3("\tPosition", static_cast<float*>(&t.Position.x), 0.5f);
		transformUpdated |= ImGui::DragFloat3("\tScale", static_cast<float*>(&t.Scale.x), 0.1f);
		if (ImGui::DragFloat3("\tRotation", rot, 1.0f))
		{
			t.Rotation.SetEulerAngles(rot[0], rot[1], rot[2]);
			transformUpdated = true;
		}

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		if (transformUpdated)
		{
			mCurrentGizmoMode == ImGuizmo::MODE::LOCAL ? ssc->SetRelativeTransform(t) : ssc->SetTransform(t);
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
}

auto ImGuiSubsystem::DrawRigidBodyProperties(Actor* actor) -> void 
{
	if (!ImGui::CollapsingHeader("RigidBody Component")) {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("RB", ImVec2(0, 35), true, window_flags);

		bool is_p_enabled = actor->is_physics_enabled;
		bool is_p_enabled_old = actor->is_physics_enabled;
		ImGui::Checkbox("Simulate Physics", &is_p_enabled);

		if (is_p_enabled != is_p_enabled_old)
		{
			is_p_enabled_old = is_p_enabled;
			if (actor->is_physics_enabled) {
				actor->UnUsePhysicsSimulation();
			}
			else {
				actor->UsePhysicsSimulation();
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();

		/*ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("Kinematic", ImVec2(0, 130), true, window_flags);

		auto rb_comp = actor->GetComponentOfClass<RigidBodyComponent>();
		bool is_kinematic = rb_comp->is_kinematic;
		bool is_kinematic_old = rb_comp->is_kinematic;
		ImGui::Checkbox("Simulate Physics", &is_kinematic);

		if (is_kinematic != is_kinematic_old)
		{
			is_kinematic_old = is_kinematic;
			if (rb_comp->is_kinematic) {
				rb_comp->MakeNonKinematic();
			}
			else {
				rb_comp->MakeKinematic();
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();*/
	}
}

auto ImGuiSubsystem::DrawActorInspector() -> void
{
	ImGui::SetNextWindowClass(&levelEditorClass);
	if (ImGui::Begin("Actor Inspector"))
	{

		if (Actor* actor = GetEditorContext().GetSelectedActor())
		{
			//General properties
			DrawGeneralProperties(actor);
			DrawComponentSelector(actor);
			LayOutTransform();

			switch (GetSelectedSceneComponent()->GetComponentType()) {
			case MeshRendererType:
				break;
			case RigidBodyCubeType:
			case RigidBodySphereType:
				DrawRigidBodyProperties(actor);
				break;
			}

		}
		else
		{
			ImGui::Text("No actor selected");
			ImGui::Text("Mouse position in viewport:");
			ImGui::Text(("x = " + std::to_string(ViewportMousePos.x) + "\ty = " + std::to_string(ViewportMousePos.y)).c_str());
		}
	}

	ImGui::End();
}

auto ImGuiSubsystem::DrawGeneralProperties(Actor* actor) -> void
{
	static char tempName[128];
	memcpy(tempName, actor->GetName().c_str(), actor->GetName().length() + 1);

	if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildGeneral", ImVec2(0, 35), true, window_flags);

		ImGui::InputText("\tName", tempName, 20);

		actor->SetName(tempName);

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
}

auto ImGuiSubsystem::DrawGizmos() -> void
{
	SceneComponent* selectedSceneComponent = GetSelectedSceneComponent();

	if (selectedSceneComponent)
	{
		ImGuizmo::SetRect(ViewportStart.x, ViewportStart.y, ViewportSize.x, ViewportSize.y);

		Camera* camera = Game::GetInstance()->GetCurrentCamera();

		Matrix mView = camera->GetViewMatrix();
		Matrix mProj = camera->GetProjectionMatrix();

		Transform t = selectedSceneComponent->GetTransform();
		Matrix tMatrix = t.GetTransformMatrix();
		if (ImGuizmo::Manipulate(&mView._11, &(mProj._11), mCurrentGizmoOperation, mCurrentGizmoMode, &tMatrix._11, NULL, useSnap ? &snap.x : NULL))
		{
			t.SetFromMatrix(tMatrix);
			selectedSceneComponent->SetTransform(t);
		}
	}
}

auto ImGuiSubsystem::DrawMessagesWindow() -> void
{
	if (ImGui::Begin("Msgs"))
	{
		for (const std::string& str : MessagesToDisplay)
		{
			ImGui::Text(str.c_str());
		}
		MessagesToDisplay.clear();
	}
	ImGui::End();
}

auto ImGuiSubsystem::DrawBasicActorsWindow() -> void
{
	ImGui::SetNextWindowClass(&levelEditorClass);
	EngineContentRegistry::GetInstance();
	if (ImGui::Begin("Basic Actors"))
	{
		for (const std::string& name : EngineContentRegistry::GetInstance()->GetBasicActorNames())
		{
			ImGui::Selectable(name.c_str());
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Drag to place in scene");

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload(ActorDragDropSourceType, name.c_str(), name.size() + 1);

				ImGui::Text(name.c_str());

				ImGui::EndDragDropSource();
			}
		}
	}

	ImGui::End();
}

auto ImGuiSubsystem::GetEditorContext() const -> EditorContext&
{
	return MyGame->MyEditorContext;
}

auto ImGuiSubsystem::GetSelectedSceneComponent() const -> SceneComponent*
{
	Component* comp = GetEditorContext().GetSelectedComponent();
	SceneComponent* selectedSceneComponent = dynamic_cast<SceneComponent*>(comp);
	// Don't check selected actor if selected component is a non scene component
	if (comp && !selectedSceneComponent)
	{
		return nullptr;
	}

	if (selectedSceneComponent == nullptr)
	{
		Actor* actor = GetEditorContext().GetSelectedActor();
		if (actor)
		{
			selectedSceneComponent = actor->RootComponent;
		}
	}

	return selectedSceneComponent;
}

auto ImGuiSubsystem::InitStyle() -> void
{
	ImGuiStyle& imguiStyle = ImGui::GetStyle();

	imguiStyle.TabRounding = 0.0f;
	imguiStyle.ScrollbarRounding = 0.0f;
	imguiStyle.IndentSpacing = 10.0f;

	// Set Colors
	// TODO: move to a separate func
	ImVec4* colors = imguiStyle.Colors;
	colors[ImGuiCol_Text]					= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]			= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]				= ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_ChildBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]				= ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border]					= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]				= ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_FrameBgActive]			= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_TitleBg]				= ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgActive]			= ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]				= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]			= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab]				= ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]		= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button]					= ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	colors[ImGuiCol_ButtonHovered]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive]			= ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header]					= ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_HeaderHovered]			= ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator]				= colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive]		= ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip]				= ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]		= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab]					= ImVec4(0.153f, 0.153f, 0.153f, 1.000f);
	colors[ImGuiCol_TabHovered]				= colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive]				= ImVec4(0.136f, 0.320f, 0.549f, 1.000f);
	colors[ImGuiCol_TabUnfocused]			= ImVec4(0.153f, 0.153f, 0.153f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.216f, 0.216f, 0.216f, 1.000f);
	colors[ImGuiCol_DockingPreview]			= colors[ImGuiCol_HeaderActive] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]		= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableBorderLight]		= ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]			= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		imguiStyle.WindowRounding = 0.0f;
		imguiStyle.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGuizmo::Style& imguizmoStyle = ImGuizmo::GetStyle();
	imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_X] = Vector4(0.549f, 0.136f, 0.136f, 1.0f);
	imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Y] = Vector4(0.136f, 0.549f, 0.136f, 1.0f);
	imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Z] = Vector4(0.136f, 0.136f, 0.549f, 1.000f);
	imguizmoStyle.Colors[ImGuizmo::COLOR::PLANE_X] = Vector4::Lerp(imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Y], imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Z], 0.5f);
	imguizmoStyle.Colors[ImGuizmo::COLOR::PLANE_Y] = Vector4::Lerp(imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_X], imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Z], 0.5f);
	imguizmoStyle.Colors[ImGuizmo::COLOR::PLANE_Z] = Vector4::Lerp(imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Y], imguizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_X], 0.5f);

	//imguizmoStyle.TranslationLineArrowSize = 10.0f;
	//imguizmoStyle.TranslationLineThickness = 6.0f;
}

auto ImGuiSubsystem::DrawAssetBrowser() -> void 
{
	if (ImGui::Begin("Content browser", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV;

		Game* game = Game::GetInstance();

		if (ImGui::BeginTable("Asset browser", 2, tableFlags)) {

			std::string path = "../Assets";

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			// DirectoryTree control buttons

			ImGui::Button("Add +");

			ImGui::Separator();


			// Directory tree

			DirectoryTree* dt = game->GetDirectoryTree();

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

			if (ImGui::BeginChild("File browser", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, window_flags)) {

				DirectoryTreeNode* dt_node = dt->GetRootNode();

				std::vector<DirectoryTreeNode*> stack;
				stack.push_back(dt_node);

				while (!stack.empty()) {
					dt_node = stack.back();
					stack.pop_back();
					if (dt_node == nullptr) {
						ImGui::TreePop();
						continue;
					}
					ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
					const bool isLeaf = dt_node->GetChildren().size() == 0;
					if (isLeaf)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Leaf;
					}
					const Path currentPathFromRoot = dt->GetPathFromRoot(dt_node);
					Path currentSelectedDirectory = GetEditorContext().GetSelectedDirectory();
					const bool isSelected = GetEditorContext().GetSelectedDirectory() == currentPathFromRoot;
					if (isSelected)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
					}
					const bool isOpen = ImGui::TreeNodeEx(dt_node->GetPath().string().c_str(), nodeFlags);
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					{
						GetEditorContext().SetSelectedDirectory(currentPathFromRoot);
					}
					if (isOpen) {
						stack.push_back(nullptr);
						stack.insert(stack.end(), dt_node->GetChildren().begin(), dt_node->GetChildren().end());
					}
				}

				ImGui::EndChild();
			}

			ImGui::TableNextColumn();

			//control buttons

			ImGui::ArrowButton("DirectoryUP", ImGuiDir_Up);
			Path currentSelectedDirectory = GetEditorContext().GetSelectedDirectory();

			// TODO disabble the button properly
			if (currentSelectedDirectory != "Assets") {
				
			}

			if (ImGui::IsItemClicked() && currentSelectedDirectory != "Assets") {
				currentSelectedDirectory._Remove_filename_and_separator();
				GetEditorContext().SetSelectedDirectory(currentSelectedDirectory);
			}
			ImGui::SameLine();
			ImGui::Text(GetEditorContext().GetSelectedDirectory().string().c_str());
			ImGui::Separator();

			// Assets

			if (ImGui::BeginChild("Asset browser", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, window_flags)) {

				Path path = game->assetsPath;
				path._Remove_filename_and_separator();
				path = path / GetEditorContext().GetSelectedDirectory();

				ImVec2 itemSize(80, 110);

				ImGuiStyle& style = ImGui::GetStyle();
				float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
				for (auto entry : std::filesystem::directory_iterator(path)) 
				{
				
					ImGui::BeginGroup();
					const ImVec2 selectableCursorPos = ImGui::GetCursorPos() + style.ItemSpacing;
					ImGui::SetCursorPos(selectableCursorPos);
					std::string pathAsString = entry.path().filename().string();
					ImGuiSelectableFlags flags =  0;
					if (entry.is_directory())
						flags = ImGuiSelectableFlags_AllowDoubleClick;
					ImGui::Selectable(("##" + pathAsString).c_str(), false, flags, itemSize);
					// double-clicking to choose directories
					if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered() && entry.is_directory()) {
						GetEditorContext().SetSelectedDirectory(entry.path().lexically_relative(".."));
						ImGui::EndGroup();
						break;
					}
					
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(pathAsString.c_str());
					ImGui::SetItemAllowOverlap();

					const ImVec2 imageCursorPosition = selectableCursorPos + ImVec2{ 0.0f, style.ItemSpacing.y };
					ImGui::SetCursorPos(imageCursorPosition);
					// todo: replace with a proper image
					ImTextureID imageId = nullptr;
					if (entry.is_directory())
					{
						imageId = EngineContentRegistry::GetInstance()->GetFolderTexSRV().Get();
					}
					if (imageId == nullptr)
					{
						imageId = EngineContentRegistry::GetInstance()->GetGenericFileTexSRV().Get();
					}
					ImGui::Image(imageId, ImVec2(itemSize.x, itemSize.x));
					std::string str = entry.path().filename().string();
					// todo properly habdle text not fully fitting
					if (str.length() > 15)
						str = str.substr(0, 12) + "...";
					ImGui::SetCursorPos(ImGui::GetCursorPos() + style.ItemSpacing);
					ImGui::Text(str.c_str());
					ImGui::EndGroup();

					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + itemSize.x; // Expected position if next button was on same line
					if (next_button_x2 < window_visible_x2)
						ImGui::SameLine(0.0f, 0.0f);
				}

				ImGui::EndChild();
			}

			ImGui::EndTable();
		}
	}
	ImGui::End();
}

