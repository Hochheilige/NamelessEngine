#include "ImGuiNodeEditorManager.h"
#include "MathInclude.h"
#include "ImGuiSubsystem.h"
#include "Game.h"

Node* FindNode(NodeEditorData& data, ned::NodeId id)
{
	for (auto& node : data.nodes)
		if (node.ID == id)
			return &node;

	return nullptr;
}

Node* FindRootNode(NodeEditorData& data)
{
	for (auto& node : data.nodes)
		if (node.Kind == NodeKind::Root)
			return &node;

	return nullptr;
}

Pin* FindPin(NodeEditorData& data, ned::PinId id)
{
	if (!id)
		return nullptr;

	for (Node& node : data.nodes)
	{
		for (auto& pin : node.Inputs)
			if (pin.ID == id)
				return &pin;

		for (auto& pin : node.Outputs)
			if (pin.ID == id)
				return &pin;
	}

	return nullptr;
}

Link* FindLink(NodeEditorData& data, ned::LinkId id)
{
	for (auto& link : data.links)
		if (link.ID == id)
			return &link;

	return nullptr;
}

std::vector<Link*> FindLinksByStartPinId(NodeEditorData& data, ned::PinId starPinId)
{
	std::vector<Link*> links;
	for (auto& link : data.links)
		if (link.StartPinID == starPinId)
			links.push_back(&link);

	return links;
}

static bool CanCreateLink(NodeEditorData& data, Pin* a, Pin* b)
{
	if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
		return false;

	return true;
}

ImColor GetIconColor(PinType type)
{
	switch (type)
	{
	default:
	case PinType::Flow:     return ImColor(255, 255, 255);
	}
};

void BuildNode(Node* node)
{
	for (auto& input : node->Inputs)
	{
		input.Node = node;
		input.Kind = PinKind::Input;
	}

	for (auto& output : node->Outputs)
	{
		output.Node = node;
		output.Kind = PinKind::Output;
	}
}

void BuildNodes(NodeEditorData& data)
{
	for (auto& node : data.nodes)
		BuildNode(&node);
}

auto ImGuiNodeEditorManager::OpenEditor(const Path& path) -> void
{
	// todo: initialize it somewhere else?
	taskTypes = Game::GetInstance()->GetTasksJson();

	std::unique_ptr<NodeEditorData>& dataPtr = openEditors.emplace_back();
	dataPtr.reset(new NodeEditorData());
	NodeEditorData& data = *dataPtr;

	data.config.SettingsFile = nullptr;
	data.config.UserPointer = dataPtr.get();

	data.windowClass.ClassId = GetImGuiSubsystem().GetNextWindowClassId();
	data.windowClass.DockingAllowUnclassed = false;
	data.windowClass.DockingAlwaysTabBar = true;
	// todo: call ned::DestroyEditor() somewhere
	data.context = ned::CreateEditor(&data.config);

	// temp
	SpawnRootNode(data);
}

auto ImGuiNodeEditorManager::DrawOpenEditors() -> void
{
	for (auto& data : openEditors)
	{
		DrawNodeEditorWindow(*data);

		// todo:
		/*ImGui::SetNextWindowClass(&behaviorTreeEditorClass);
		if (ImGui::Begin("Toolbar##NodeEditor"))
		{
			if (ImGui::Button("Save"))
			{

			}
		}
		ImGui::End();

		ImGui::SetNextWindowClass(&behaviorTreeEditorClass);
		if (ImGui::Begin("Details"))
		{

		}
		ImGui::End();*/
	}
}

auto ImGuiNodeEditorManager::GetImGuiSubsystem() -> ImGuiSubsystem&
{
	return *(ImGuiSubsystem::GetInstance());
}

auto ImGuiNodeEditorManager::SpawnRootNode(NodeEditorData& nodeEditorData) -> Node&
{
	Node& node = nodeEditorData.nodes.emplace_back(GetNextId(), "Root");
	node.Type = NodeType::Tree;
	node.Outputs.emplace_back(GetNextId(), "", PinType::Flow);
	node.Kind = NodeKind::Root;

	BuildNode(&node);

	return node;
}

auto ImGuiNodeEditorManager::SpawnSequenceNode(NodeEditorData& nodeEditorData) -> Node&
{
	Node& node = nodeEditorData.nodes.emplace_back(GetNextId(), "Sequence");
	node.Type = NodeType::Tree;
	node.Inputs.emplace_back(GetNextId(), "", PinType::Flow);
	node.Outputs.emplace_back(GetNextId(), "", PinType::Flow);
	node.Kind = NodeKind::Sequence;

	BuildNode(&node);

	return node;
}

auto ImGuiNodeEditorManager::SpawnSelectorNode(NodeEditorData& nodeEditorData) -> Node&
{
	Node& node = nodeEditorData.nodes.emplace_back(GetNextId(), "Selector");
	node.Type = NodeType::Tree;
	node.Inputs.emplace_back(GetNextId(), "", PinType::Flow);
	node.Outputs.emplace_back(GetNextId(), "", PinType::Flow);
	node.Kind = NodeKind::Selector;

	BuildNode(&node);

	return node;
}

auto ImGuiNodeEditorManager::SpawnTaskNode(NodeEditorData& nodeEditorData, json& taskType) -> Node&
{
	Node& node = nodeEditorData.nodes.emplace_back(GetNextId(), taskType["Name"].get<std::string>().c_str(), ImColor(255, 0, 255));
	node.Type = NodeType::Tree;
	node.Inputs.emplace_back(GetNextId(), "", PinType::Flow);
	node.Kind = NodeKind::Task;

	node.taskData = taskType;

	BuildNode(&node);

	return node;
}

auto ImGuiNodeEditorManager::DrawNodeEditorWindow(NodeEditorData& nodeEditorData) -> void
{
	ImGui::SetNextWindowClass(GetImGuiSubsystem().GetTopLevelWindowClass());
	bool editorWindowOpen = ImGui::Begin("BT Editor");
	// todo: need a unique dockspace id
	ImGuiID btEditorDockspaceId = ImGui::GetID(("BehaviorTreeEditorDockspace" + std::to_string(nodeEditorData.windowClass.ClassId)).c_str());
	//ImGuiID btEditorDockspaceId = nodeEditorData.windowClass.ClassId;
	ImGui::DockSpace(btEditorDockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None, &nodeEditorData.windowClass);
	ImGui::End();

	if (!editorWindowOpen)
	{
		return;
	}

	// Should I have a different class for each window?
	// probably yes, becuase otherwise i would be able to move a window from one editor window to another
	ImGui::SetNextWindowClass(&nodeEditorData.windowClass);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	// todo: need unique window id's
	const bool drawingNodeCanvas = ImGui::Begin("Node Canvas");
	ImGui::PopStyleVar();
	if (drawingNodeCanvas)
	{

		ned::SetCurrentEditor(nodeEditorData.context);
		ned::Begin("BT Edtior");

		auto cursorTopLeft = ImGui::GetCursorScreenPos();

		// draw this editor's nodes
		for (Node& node : nodeEditorData.nodes)
		{
			DrawNode(nodeEditorData, node);
		}

		// draw links
		for (Link& link : nodeEditorData.links)
		{
			ned::Link(link.ID, link.StartPinID, link.EndPinID, link.Color, 2.0f);
		}

		if (!nodeEditorData.createNewNode)
		{
			// draw stuff to create new links
			TryCreateNewLink(nodeEditorData);

			// draw stuff to delete
			TryDeleteNodesAndLinks(nodeEditorData);
		}

		ImGui::SetCursorScreenPos(cursorTopLeft);


		auto openPopupPosition = ImGui::GetMousePos();
		ned::Suspend();
		// poll context menus
		if (ned::ShowNodeContextMenu(&nodeEditorData.contextNodeId))
			ImGui::OpenPopup("Node Context Menu");
		else if (ned::ShowPinContextMenu(&nodeEditorData.contextPinId))
			ImGui::OpenPopup("Pin Context Menu");
		else if (ned::ShowLinkContextMenu(&nodeEditorData.contextLinkId))
			ImGui::OpenPopup("Link Context Menu");
		else if (ned::ShowBackgroundContextMenu())
		{
			ImGui::OpenPopup("Create New Node");
			nodeEditorData.newNodeLinkPin = nullptr;
		}
		ned::Resume();

		ned::Suspend();
		// draw opened context menus
		DrawNodeContextMenuPopup(nodeEditorData);
		DrawPinContextMenuPopup(nodeEditorData);
		DrawLinkContextMenuPopup(nodeEditorData);
		DrawCreateNodeContextMenuPopup(nodeEditorData, openPopupPosition);
		ned::Resume();

		ned::End();

		auto editorMin = ImGui::GetItemRectMin();
		auto editorMax = ImGui::GetItemRectMax();

		if (true)
		{
			UpdateNodeOrdinals(nodeEditorData);

			auto drawList = ImGui::GetWindowDrawList();
			drawList->PushClipRect(editorMin, editorMax);

			for (auto& node : nodeEditorData.nodes)
			{
				if (node.Kind == NodeKind::Root)
				{
					continue;
				}
				ned::NodeId nodeId = node.ID;
				auto p0 = ned::GetNodePosition(nodeId);
				auto p1 = p0 + ned::GetNodeSize(nodeId);
				p0 = ned::CanvasToScreen(p0);
				p1 = ned::CanvasToScreen(p1);


				ImGuiTextBuffer builder;
				builder.appendf("#%d", node.Ordinal);

				auto textSize = ImGui::CalcTextSize(builder.c_str());
				auto padding = ImVec2(2.0f, 2.0f);
				auto widgetSize = textSize + padding * 2;

				auto widgetPosition = ImVec2(p1.x, p0.y) + ImVec2(0.0f, -widgetSize.y);

				drawList->AddRectFilled(widgetPosition, widgetPosition + widgetSize, IM_COL32(100, 80, 80, 190), 3.0f, ImDrawFlags_RoundCornersAll);
				drawList->AddRect(widgetPosition, widgetPosition + widgetSize, IM_COL32(200, 160, 160, 190), 3.0f, ImDrawFlags_RoundCornersAll);
				drawList->AddText(widgetPosition + padding, IM_COL32(255, 255, 255, 255), builder.c_str());
			}

			drawList->PopClipRect();
		}
		ned::SetCurrentEditor(nullptr);
	}
	ImGui::End();
}

auto ImGuiNodeEditorManager::DrawNode(NodeEditorData& nodeEditorData, Node& node) -> void
{
	if (node.Type != NodeType::Tree)
		return;

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

		if (nodeEditorData.newLinkPin && !CanCreateLink(nodeEditorData, nodeEditorData.newLinkPin, &pin) && &pin != nodeEditorData.newLinkPin)
			inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
	}
	else
	{
		ImGui::Dummy(ImVec2(0, padding));
	}

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

		if (nodeEditorData.newLinkPin && !CanCreateLink(nodeEditorData, nodeEditorData.newLinkPin, &pin) && &pin != nodeEditorData.newLinkPin)
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

	if (!node.Inputs.empty()) {
		drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
		//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
		drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
		//ImGui::PopStyleVar();
	}
	if (!node.Outputs.empty())
	{
		drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
		//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
		drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
		//ImGui::PopStyleVar();
	}
	drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
	drawList->AddRect(
		contentRect.GetTL(),
		contentRect.GetBR(),
		IM_COL32(48, 128, 255, 100), 0.0f);
	//ImGui::PopStyleVar();
}

auto ImGuiNodeEditorManager::TryCreateNewLink(NodeEditorData& nodeEditorData) -> void
{
	if (ned::BeginCreate(ImColor(255, 255, 255), 2.0f))
	{
		auto showLabel = [](const char* label, ImColor color)
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
			auto size = ImGui::CalcTextSize(label);

			auto padding = ImGui::GetStyle().FramePadding;
			auto spacing = ImGui::GetStyle().ItemSpacing;

			ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

			auto rectMin = ImGui::GetCursorScreenPos() - padding;
			auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

			auto drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
			ImGui::TextUnformatted(label);
		};

		ned::PinId startPinId = 0, endPinId = 0;
		if (ned::QueryNewLink(&startPinId, &endPinId))
		{
			auto startPin = FindPin(nodeEditorData, startPinId);
			auto endPin = FindPin(nodeEditorData, endPinId);

			nodeEditorData.newLinkPin = startPin ? startPin : endPin;

			if (startPin->Kind == PinKind::Input)
			{
				std::swap(startPin, endPin);
				std::swap(startPinId, endPinId);
			}

			// todo: check for graph cycles? - can there be cycles in the tree (if only one input is allowed)?

			if (startPin && endPin)
			{
				auto iterToExistingEndIdLink = std::find_if(nodeEditorData.links.begin(), nodeEditorData.links.end(), [endPin](const Link& link) {return link.EndPinID == endPin->ID; });
				auto iterToExistingStartIdLink = std::find_if(nodeEditorData.links.begin(), nodeEditorData.links.end(), [startPin](const Link& link) {return link.StartPinID == startPin->ID; });

				if (endPin == startPin)
				{
					ned::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (endPin->Kind == startPin->Kind)
				{
					showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
					ned::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				//else if (endPin->Node == startPin->Node)
				//{
				//    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
				//    ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
				//}
				else if (endPin->Type != startPin->Type)
				{
					showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
					ned::RejectNewItem(ImColor(255, 128, 128), 1.0f);
				}
				else 
				{
					if (iterToExistingEndIdLink != nodeEditorData.links.end() || iterToExistingStartIdLink != nodeEditorData.links.end())
					{
						showLabel("+ Replace Link", ImColor(32, 45, 32, 180));
					}
					else
					{
						showLabel("+ Create Link", ImColor(32, 45, 32, 180));
					}
					if (ned::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
					{
						if (iterToExistingEndIdLink != nodeEditorData.links.end())
						{
							nodeEditorData.links.erase(iterToExistingEndIdLink);
						}
						if (startPin->Node && startPin->Node->Kind == NodeKind::Root)
						{
							iterToExistingStartIdLink = std::find_if(nodeEditorData.links.begin(), nodeEditorData.links.end(), [startPin](const Link& link) {return link.StartPinID == startPin->ID; });
							if (iterToExistingStartIdLink != nodeEditorData.links.end()) nodeEditorData.links.erase(iterToExistingStartIdLink);
						}
						Link& link = nodeEditorData.links.emplace_back(Link(GetNextId(), startPinId, endPinId));
						link.Color = GetIconColor(startPin->Type);
					}
				}
			}
		}

		ned::PinId pinId = 0;
		if (ned::QueryNewNode(&pinId))
		{
			nodeEditorData.newLinkPin = FindPin(nodeEditorData, pinId);
			if (nodeEditorData.newLinkPin)
				showLabel("+ Create Node", ImColor(32, 45, 32, 180));

			if (ned::AcceptNewItem())
			{
				nodeEditorData.createNewNode = true;
				nodeEditorData.newNodeLinkPin = FindPin(nodeEditorData, pinId);
				nodeEditorData.newLinkPin = nullptr;
				ned::Suspend();
				ImGui::OpenPopup("Create New Node");
				ned::Resume();
			}
		}
	}
	else
	{
		nodeEditorData.newLinkPin = nullptr;
	}

	ned::EndCreate();
}

auto ImGuiNodeEditorManager::TryDeleteNodesAndLinks(NodeEditorData& nodeEditorData) -> void
{
	if (ned::BeginDelete())
	{
		ned::NodeId nodeId = 0;
		while (ned::QueryDeletedNode(&nodeId))
		{
			if (FindNode(nodeEditorData, nodeId)->Kind == NodeKind::Root)
			{
				ned::RejectDeletedItem();
			}
			else if (ned::AcceptDeletedItem())
			{
				std::vector<Node>& nodes = nodeEditorData.nodes;
				auto iter = std::find_if(nodes.begin(), nodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
				if (iter != nodes.end())
					nodes.erase(iter);
			}
		}

		ned::LinkId linkId = 0;
		while (ned::QueryDeletedLink(&linkId))
		{
			if (ned::AcceptDeletedItem())
			{
				std::vector<Link>& links = nodeEditorData.links;
				auto iter = std::find_if(links.begin(), links.end(), [linkId](auto& link) { return link.ID == linkId; });
				if (iter != links.end())
					links.erase(iter);
			}
		}
	}
	ned::EndDelete();
}

auto ImGuiNodeEditorManager::DrawNodeContextMenuPopup(NodeEditorData& nodeEditorData) -> void
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("Node Context Menu"))
	{
		auto node = FindNode(nodeEditorData, nodeEditorData.contextNodeId);

		ImGui::TextUnformatted("Node Context Menu");
		ImGui::Separator();
		if (node)
		{
			ImGui::Text("ID: %p", node->ID.AsPointer());
			ImGui::Text("Type: %s", (node->Type == NodeType::Tree ? "Tree" : "Comment"));
			ImGui::Text("Inputs: %d", (int)node->Inputs.size());
			ImGui::Text("Outputs: %d", (int)node->Outputs.size());
		}
		else
			ImGui::Text("Unknown node: %p", nodeEditorData.contextNodeId.AsPointer());
		ImGui::Separator();
		if (ImGui::MenuItem("Delete", nullptr, nullptr, node->Kind != NodeKind::Root))
			ned::DeleteNode(nodeEditorData.contextNodeId);
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

auto ImGuiNodeEditorManager::DrawPinContextMenuPopup(NodeEditorData& nodeEditorData) -> void
{
	if (ImGui::BeginPopup("Pin Context Menu"))
	{
		auto pin = FindPin(nodeEditorData, nodeEditorData.contextPinId);

		ImGui::TextUnformatted("Pin Context Menu");
		ImGui::Separator();
		if (pin)
		{
			ImGui::Text("ID: %p", pin->ID.AsPointer());
			if (pin->Node)
				ImGui::Text("Node: %p", pin->Node->ID.AsPointer());
			else
				ImGui::Text("Node: %s", "<none>");
		}
		else
			ImGui::Text("Unknown pin: %p", nodeEditorData.contextPinId.AsPointer());

		ImGui::EndPopup();
	}
}

auto ImGuiNodeEditorManager::DrawLinkContextMenuPopup(NodeEditorData& nodeEditorData) -> void
{
	if (ImGui::BeginPopup("Link Context Menu"))
	{
		auto link = FindLink(nodeEditorData, nodeEditorData.contextLinkId);

		ImGui::TextUnformatted("Link Context Menu");
		ImGui::Separator();
		if (link)
		{
			ImGui::Text("ID: %p", link->ID.AsPointer());
			ImGui::Text("From: %p", link->StartPinID.AsPointer());
			ImGui::Text("To: %p", link->EndPinID.AsPointer());
		}
		else
			ImGui::Text("Unknown link: %p", nodeEditorData.contextLinkId.AsPointer());
		ImGui::Separator();
		if (ImGui::MenuItem("Delete"))
			ned::DeleteLink(nodeEditorData.contextLinkId);
		ImGui::EndPopup();
	}
}

auto ImGuiNodeEditorManager::DrawCreateNodeContextMenuPopup(NodeEditorData& nodeEditorData, const ImVec2& newNodePosition) -> void
{
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	if (ImGui::BeginPopup("Create New Node"))
	{
		Node* node = nullptr;
		if (ImGui::CollapsingHeader("Composites"))
		{
			if (ImGui::MenuItem("Sequence"))
			{
				node = &SpawnSequenceNode(nodeEditorData);
			}
			if (ImGui::MenuItem("Selector"))
			{
				node = &SpawnSelectorNode(nodeEditorData);
			}
		}
		if (ImGui::CollapsingHeader("Tasks"))
		{
			for (json& j : taskTypes)
			{
				if (j["Name"].get<std::string>() == "BTTask")
				{
					continue;
				}
				if (ImGui::MenuItem(j["Name"].get<std::string>().c_str()))
				{
					node = &SpawnTaskNode(nodeEditorData, j);
				}
			}
		}

		if (node)
		{
			// we need this because adding a new node to the vector might move the nodes, which will break pointers in pins
			BuildNodes(nodeEditorData);

			nodeEditorData.createNewNode = false;

			ned::SetNodePosition(node->ID, newNodePosition);

			if (auto startPin = nodeEditorData.newNodeLinkPin)
			{
				auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

				for (auto& pin : pins)
				{
					if (CanCreateLink(nodeEditorData, startPin, &pin))
					{
						auto endPin = &pin;
						if (startPin->Kind == PinKind::Input)
							std::swap(startPin, endPin);

						// todo: reuse this code?
						////////////////////////////
						auto iterToExistingEndIdLink = std::find_if(nodeEditorData.links.begin(), nodeEditorData.links.end(), [endPin](const Link& link) {return link.EndPinID == endPin->ID; });
						if (iterToExistingEndIdLink != nodeEditorData.links.end())
						{
							nodeEditorData.links.erase(iterToExistingEndIdLink);
						}
						if (startPin->Node && startPin->Node->Kind == NodeKind::Root)
						{
							auto iterToExistingStartIdLink = std::find_if(nodeEditorData.links.begin(), nodeEditorData.links.end(), [startPin](const Link& link) {return link.StartPinID == startPin->ID; });
							if (iterToExistingStartIdLink != nodeEditorData.links.end()) nodeEditorData.links.erase(iterToExistingStartIdLink);
						}
						////////////////////////////

						Link newLink = nodeEditorData.links.emplace_back(Link(GetNextId(), startPin->ID, endPin->ID));
						newLink.Color = GetIconColor(startPin->Type);

						break;
					}
				}
			}
		}

		ImGui::EndPopup();
	}
	else
	{
		nodeEditorData.createNewNode = false;
	}
}

auto ImGuiNodeEditorManager::UpdateNodeOrdinals(NodeEditorData& data) -> void
{
	// todo: this would be faster if I didn't store everything as id, but as a smart pointer
	Node* root = FindRootNode(data);

	// reset ordinals (this is for unconnected nodes)
	for (Node& node : data.nodes)
	{
		node.Ordinal = -1;
	}

	assert(root != nullptr && "Root node was not found");

	int curOrdinal = -1;
	UpdateNodeOrdinals_Exec(data, root, curOrdinal);
}

auto ImGuiNodeEditorManager::UpdateNodeOrdinals_Exec(NodeEditorData& data, Node* curNode, int& curOrdinal) -> void
{
	curNode->Ordinal = curOrdinal++;
	
	if (curNode->Outputs.size() == 0)
	{
		return;
	}
	std::vector<Link*> outputLinks = FindLinksByStartPinId(data, curNode->Outputs[0].ID);

	std::vector<Node*> directChildNodes;
	directChildNodes.reserve(outputLinks.size());
	for (Link* link : outputLinks)
	{
		Node* node = FindPin(data, link->EndPinID)->Node;
		assert(node != nullptr && "A pin doesn't have a valid node pointer");
		directChildNodes.push_back(node);
	}

	// todo: this should probably be called inside ned::begin() ned::end()
	std::sort(directChildNodes.begin(), directChildNodes.end(),
		[](Node* const & l, Node* const& r) { return ned::GetNodePosition(l->ID).x < ned::GetNodePosition(r->ID).x; });

	for (Node* node : directChildNodes)
	{
		UpdateNodeOrdinals_Exec(data, node, curOrdinal);
	}
}
