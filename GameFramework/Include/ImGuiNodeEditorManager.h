#pragma once

#include <vector>
#include <filesystem>
using Path = std::filesystem::path;

#include "ImGuiInclude.h"
#include "ImGuiNodeEditorInclude.h"
#include "imgui_internal.h"

#include <memory>

class ImGuiSubsystem;

enum class PinType
{
	Flow
};

enum class PinKind
{
	Output,
	Input
};

enum class NodeType
{
	Tree,
	Comment
};

enum class NodeKind
{
	Root,
	Sequence,
	Selector,
	Task
};

struct Node;

struct Pin
{
	ned::PinId ID;
	std::string Name;
	PinType     Type;
	PinKind     Kind;
	Node* Node;

	Pin(int id, const char* name, PinType type) :
		ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
	{
	}
};

struct Node {
	ned::NodeId ID;
	std::string Name;
	ImColor Color;
	NodeType Type;
	NodeKind Kind;
	ImVec2 Size;
	int Ordinal = -1;
	// need to add stuff that will allow to find out what type of node it is : root, sequence, selector, task (for task we need to remember what task it is
	// mb just a json object?

	std::vector<Pin> Inputs;
	std::vector<Pin> Outputs;

	Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) :
		ID(id), Name(name), Color(color), Type(NodeType::Tree), Size(0, 0), Kind(NodeKind::Selector), Ordinal(-1)
	{
	}
};

struct Link
{
	ned::LinkId ID;

	ned::PinId StartPinID;
	ned::PinId EndPinID;

	ImColor Color;

	Link(ned::LinkId id, ned::PinId startPinId, ned::PinId endPinId) :
		ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
	{
	}
};

struct NodeEditorData
{
	Path path;
	ned::EditorContext* context;

	std::vector<Node> nodes;
	std::vector<Link> links;

	ImGuiWindowClass windowClass;

	ned::Config config;

	// todo: should the following be here? - prolly yes
	bool createNewNode = false;
	Pin* newNodeLinkPin = nullptr;
	Pin* newLinkPin = nullptr;
	ned::NodeId contextNodeId = 0;
	ned::LinkId contextLinkId = 0;
	ned::PinId  contextPinId = 0;

};

static inline ImRect ImGui_GetItemRect()
{
	return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

class ImGuiNodeEditorManager
{
public:
	
	// Should the path be here or
	auto OpenEditor(const Path& path) -> void;

	

	auto DrawOpenEditors() -> void;

private:

	std::vector<std::unique_ptr<NodeEditorData>> openEditors;

	// todo: should each node editor have its own id generator? To combat possible overflow?
	int nextId = 1;

protected:

	auto GetImGuiSubsystem() -> ImGuiSubsystem&;

protected:

	auto SpawnRootNode(NodeEditorData& nodeEditorData) -> Node&;
	auto SpawnSequenceNode(NodeEditorData& nodeEditorData) -> Node&;
	auto SpawnTaskNode(NodeEditorData& nodeEditorData) -> Node&;
	auto SpawnSelectorNode(NodeEditorData& nodeEditorData) -> Node&;
protected:

	auto GetNextId() -> int { return nextId++; }

	auto DrawNodeEditorWindow(NodeEditorData& nodeEditorData) -> void;

	auto DrawNode(NodeEditorData& nodeEditorData, Node& node) -> void;

	auto TryCreateNewLink(NodeEditorData& nodeEditorData) -> void;

	auto TryDeleteNodesAndLinks(NodeEditorData& nodeEditorData) -> void;

	auto DrawNodeContextMenuPopup(NodeEditorData& nodeEditorData) -> void;
	auto DrawPinContextMenuPopup(NodeEditorData& nodeEditorData) -> void;
	auto DrawLinkContextMenuPopup(NodeEditorData& nodeEditorData) -> void;
	auto DrawCreateNodeContextMenuPopup(NodeEditorData& nodeEditorData, const ImVec2& newNodePosition) -> void;

	auto UpdateNodeOrdinals(NodeEditorData& data) -> void;
	auto UpdateNodeOrdinals_Exec(NodeEditorData& data, Node* curNode, int& curOrdinal) -> void;
};