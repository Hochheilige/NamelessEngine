#include "ImGuiNodeEditorManager.h"
#include "MathInclude.h"

auto ImGuiNodeEditorManager::DrawOpenEditors() -> void
{
	for (NodeEditorData& data : openEditors)
	{
		DrawNodeEditorWindow(data);
	}
}

auto ImGuiNodeEditorManager::DrawNodeEditorWindow(NodeEditorData& nodeEditorData) -> void
{
	//
	//ImGui::SetNextWindowClass(&topLevelClass);
	bool editorWindowOpen = ImGui::Begin("BT Editor");
	ImGuiID btEditorDockspaceId = ImGui::GetID("BehaviorTreeEditorDockspace");
	//ImGui::DockSpace(btEditorDockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None, &behaviorTreeEditorClass);
	ImGui::End();

	if (!editorWindowOpen)
	{
		return;
	}

	// Should I have a different class for each window?
	// probably yes, becuase otherwise i would be able to move a window from one editor window to another
	//ImGui::SetNextWindowClass(&behaviorTreeEditorClass);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	const bool drawingNodeCanvas = ImGui::Begin("Node Canvas");
	ImGui::PopStyleVar();
	if (drawingNodeCanvas)
	{

		ned::SetCurrentEditor(nodeEditorData.context);
		ned::Begin("BT Edtior");

		auto cursorTopLeft = ImGui::GetCursorScreenPos();

		// draw this editor's nodes

		// draw links

		// draw stuff to create new links

		// draw stuff to delete stuff - should possibly

			// link delete

			// node delete

		ImGui::SetCursorScreenPos(cursorTopLeft);

		ned::Suspend();
		// poll context menus
		ned::Resume();

		ned::Suspend();
		// draw opened context menus
		ned::Resume();

		ned::End();
		ned::SetCurrentEditor(nullptr);
	}
	ImGui::End();
}

auto ImGuiNodeEditorManager::DrawNode(Node& node, Pin*& newLinkPin) -> void
{
	const float rounding = 5.0f;
	const float padding = 12.0f;

	const auto pinBackground = ned::GetStyle().Colors[ned::StyleColor_NodeBg];

	ned::PushStyleColor(ned::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
	ned::PushStyleColor(ned::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
	ned::PushStyleColor(ned::StyleColor_PinRect, ImColor(60, 180, 255, 150));
	ned::PushStyleColor(ned::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

	ned::PushStyleVar(ned::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
	ned::PushStyleVar(ned::StyleVar_NodeRounding, rounding);
	ned::PushStyleVar(ned::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
	ned::PushStyleVar(ned::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
	ned::PushStyleVar(ned::StyleVar_LinkStrength, 0.0f);
	ned::PushStyleVar(ned::StyleVar_PinBorderWidth, 1.0f);
	ned::PushStyleVar(ned::StyleVar_PinRadius, 5.0f);

	ned::BeginNode(node.ID);

	ImGui::BeginVertical(node.ID.AsPointer());
	ImGui::BeginHorizontal("inputs");
	ImGui::Spring(0, padding * 2);

	ImRect inputsRect;
	int inputAlpha = 200;
	if (!node.Inputs.empty())
	{
		auto& pin = node.Inputs[0];
		ImGui::Dummy(ImVec2(0, padding));
		ImGui::Spring(1, 0);
		inputsRect = ImGui_GetItemRect();

		ned::PushStyleVar(ned::StyleVar_PinArrowSize, 10.0f);
		ned::PushStyleVar(ned::StyleVar_PinArrowWidth, 10.0f);
#if IMGUI_VERSION_NUM > 18101
		ned::PushStyleVar(ned::StyleVar_PinCorners, ImDrawFlags_RoundCornersBottom);
#else
		ned::PushStyleVar(ned::StyleVar_PinCorners, 12);
#endif
		ned::BeginPin(pin.ID, ned::PinKind::Input);
		ned::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
		ned::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
		ned::EndPin();
		ned::PopStyleVar(3);

		if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
			inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
	}
	else
		ImGui::Dummy(ImVec2(0, padding));

	ImGui::Spring(0, padding * 2);
	ImGui::EndHorizontal();

	ImGui::BeginHorizontal("content_frame");
	ImGui::Spring(1, padding);

	ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
	ImGui::Dummy(ImVec2(160, 0));
	ImGui::Spring(1);
	ImGui::TextUnformatted(node.Name.c_str());
	ImGui::Spring(1);
	ImGui::EndVertical();
	auto contentRect = ImGui_GetItemRect();

	ImGui::Spring(1, padding);
	ImGui::EndHorizontal();

	ImGui::BeginHorizontal("outputs");
	ImGui::Spring(0, padding * 2);

	ImRect outputsRect;
	int outputAlpha = 200;
	if (!node.Outputs.empty())
	{
		auto& pin = node.Outputs[0];
		ImGui::Dummy(ImVec2(0, padding));
		ImGui::Spring(1, 0);
		outputsRect = ImGui_GetItemRect();

#if IMGUI_VERSION_NUM > 18101
		ned::PushStyleVar(ned::StyleVar_PinCorners, ImDrawFlags_RoundCornersTop);
#else
		ned::PushStyleVar(ned::StyleVar_PinCorners, 3);
#endif
		ned::BeginPin(pin.ID, ned::PinKind::Output);
		ned::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
		ned::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
		ned::EndPin();
		ned::PopStyleVar();

		if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
			outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
	}
	else
		ImGui::Dummy(ImVec2(0, padding));

	ImGui::Spring(0, padding * 2);
	ImGui::EndHorizontal();

	ImGui::EndVertical();

	ned::EndNode();
	ned::PopStyleVar(7);
	ned::PopStyleColor(4);

	auto drawList = ned::GetNodeBackgroundDrawList(node.ID);

	//const auto fringeScale = ImGui::GetStyle().AntiAliasFringeScale;
	//const auto unitSize    = 1.0f / fringeScale;

	//const auto ImDrawList_AddRect = [](ImDrawList* drawList, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
	//{
	//    if ((col >> 24) == 0)
	//        return;
	//    drawList->PathRect(a, b, rounding, rounding_corners);
	//    drawList->PathStroke(col, true, thickness);
	//};

#if IMGUI_VERSION_NUM > 18101
	const auto    topRoundCornersFlags = ImDrawFlags_RoundCornersTop;
	const auto bottomRoundCornersFlags = ImDrawFlags_RoundCornersBottom;
#else
	const auto    topRoundCornersFlags = 1 | 2;
	const auto bottomRoundCornersFlags = 4 | 8;
#endif

	drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
		IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
	//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
	drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
		IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
	//ImGui::PopStyleVar();
	drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
		IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
	//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
	drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
		IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
	//ImGui::PopStyleVar();
	drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
	drawList->AddRect(
		contentRect.GetTL(),
		contentRect.GetBR(),
		IM_COL32(48, 128, 255, 100), 0.0f);
	//ImGui::PopStyleVar();
}
