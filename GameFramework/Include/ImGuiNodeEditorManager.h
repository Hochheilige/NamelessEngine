#pragma once

#include <vector>
#include <filesystem>
using Path = std::filesystem::path;

#include "ImGuiInclude.h"
#include "ImGuiNodeEditorInclude.h"
#include "imgui_internal.h"

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
	ImVec2 Size;
	// need to add stuff that will allow to find out what type of node it is : root, sequence, selector, task (for task we need to remember what task it is
	// mb just a json object?

	auto AddInputPin(const Pin& pin) -> void;
	auto AddOutputPin(const Pin& pin) -> void;

//protected:
	std::vector<Pin> Inputs;
	std::vector<Pin> Outputs;
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

static inline ImRect ImGui_GetItemRect()
{
	return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

static bool CanCreateLink(Pin* a, Pin* b)
{
	if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
		return false;

	return true;
}

class ImGuiNodeEditorManager
{
public:
	
	// Should the path be here or
	auto OpenEditor(const Path& path) -> void;

	struct NodeEditorData
	{
		Path path;
		ned::EditorContext* context;

		std::vector<Node> nodes;
		std::vector<Link> links;
	};

	std::vector<NodeEditorData> openEditors;

	int nextId = 0;

	auto DrawOpenEditors() -> void;

protected:

	auto DrawNodeEditorWindow(NodeEditorData& nodeEditorData) -> void;

	auto DrawNode(Node& node, Pin*& newLinkPin) -> void;
};