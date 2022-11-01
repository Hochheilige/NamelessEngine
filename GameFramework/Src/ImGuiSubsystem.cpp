#include "ImGuiSubsystem.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include "imguizmo.h"


#include "Game.h"
#include "DisplayWin32.h"

#include <string>

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

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(MyGame->GetDisplay()->GetWindowHandle());
	ImGui_ImplDX11_Init(MyGame->GetD3DDevice().Get(), MyGame->GetD3DDeviceContext().Get());

	// TODO: get this from current camera and update it
	ImGuizmo::SetOrthographic(false);
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
	DrawDockspace();
	static bool temp = true;
	ImGui::ShowDemoWindow(&temp);
	DrawActorExplorer();
	DrawActorInspector();
	// need to draw this after gizmos to have a relevant ImGuizmo::IsUsing
	DrawViewport();
}

auto ImGuiSubsystem::Shutdown() -> void
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ImGuiSubsystem::~ImGuiSubsystem()
{
	Shutdown();
}

auto ImGuiSubsystem::DrawDockspace() -> void
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
	// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	// In this specific demo, we are not using DockSpaceOverViewport() because:
	// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	// - we allow the host window to have padding (when opt_padding == true)
	// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	// TL;DR; this demo is more complicated than what you would normally use.
	// If we removed all the options we are showcasing, this demo would become:
	//     void ShowExampleAppDockSpace()
	//     {
	//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	//     }

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static bool dockspaceOpen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, dockspaceOpen != NULL))
				dockspaceOpen = false;
			ImGui::EndMenu();
		}


		ImGui::EndMenuBar();
	}



	ImGui::End();

}

auto operator-(const ImVec2& l, const ImVec2& r) -> ImVec2
{
	return ImVec2{ l.x - r.x, l.y - r.y };
}

auto ImGuiSubsystem::DrawViewport() -> void
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");
	ViewportStart = ImGui::GetCursorScreenPos();
	ViewportSize = ImGui::GetContentRegionAvail();
	const ImVec2 mousePos = ImGui::GetMousePos() - ViewportStart;
	ViewportMousePos = {mousePos.x, mousePos.y};
	MyGame->MyRenderingSystem->HandleScreenResize({ ViewportSize.x, ViewportSize.y });
	ImGui::Image(MyGame->MyRenderingSystem->GetViewportTextureID(), ViewportSize);
	// todo: move this somewhere more appropriate
	{
		if (ImGui::IsItemClicked() && !ImGuizmo::IsUsing())
		{
			MyGame->MyEditorContext.SelectedActor = MyGame->MyRenderingSystem->GetActorUnderPosition(ViewportMousePos);
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

auto ImGuiSubsystem::DrawActorExplorer() -> void
{
	ImGui::Begin("Actor Explorer");
	//ImGui::BeginListBox("Actors");
	int i = 0;
	for (Actor* actor : MyGame->Actors)
	{
		const bool isSelectedActor = MyGame->MyEditorContext.SelectedActor == actor;
		if (isSelectedActor)
		{
			ImGui::PushStyleColor(0, {0.5f, 0.5, 0.5f, 1.0f});
		}
		ImGui::Button((std::string("Actor") + std::to_string(i)).c_str());
		if (ImGui::IsItemClicked())
		{
			MyGame->MyEditorContext.SelectedActor = actor;
		}
		if (isSelectedActor)
		{
			ImGui::PopStyleColor();
		}
		++i;
	}
	
	ImGui::End();
}

auto ImGuiSubsystem::DrawActorInspector() -> void
{
	ImGui::Begin("Actor Inspector");

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	static bool useSnap(false);
	Vector3 snap;

	Camera* camera = Game::GetInstance()->GetCurrentCamera();

	Matrix mView = camera->GetViewMatrix();
	Matrix mProj = camera->GetProjectionMatrix();


	if (!ImGui::CollapsingHeader("Transform")) {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 110), true, window_flags);

		

		if (Actor* actor = MyGame->MyEditorContext.SelectedActor)
		{


			if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
				mCurrentGizmoOperation = ImGuizmo::SCALE;

			Transform t = actor->GetTransform();
			float rot[] = {t.Rotation.GetEulerDegrees().x, t.Rotation.GetEulerDegrees().y,
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

			ImGuizmo::SetRect(ViewportStart.x, ViewportStart.y, ViewportSize.x, ViewportSize.y);
			Matrix identityMatrix = Matrix::Identity;

			Matrix tMatrix = t.GetTransformMatrix();
			if (ImGuizmo::Manipulate(&mView._11, &(mProj._11), mCurrentGizmoOperation, mCurrentGizmoMode, &tMatrix._11, NULL, useSnap ? &snap.x : NULL))
			{
				transformUpdated = true;
				t.SetFromMatrix(tMatrix);
			}
			
			if (transformUpdated)
			{
				actor->SetTransform(t);
			}
		}
		else
		{
			ImGui::Text("No actor selected");
			ImGui::Text(("x = " + std::to_string(ViewportMousePos.x) + "\ty = " + std::to_string(ViewportMousePos.y)).c_str());
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
	
	ImGui::End();
}