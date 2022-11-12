#pragma once
#include <string>
#include <vector>
#include <filesystem>

using Path = std::filesystem::path;

class DirectoryTreeNode {
	friend class DirectoryTree;
private:
	DirectoryTreeNode* parent = nullptr;
	std::vector<DirectoryTreeNode*> children = {};
	DirectoryTreeNode* sibling = nullptr;
	Path path;

public:
	auto AddChild(DirectoryTreeNode* node) -> DirectoryTreeNode* { 
		children.push_back(node); 
		node->parent = this;
		return node;
	}
	auto SetParent(DirectoryTreeNode* node) -> void { parent = node; }
	DirectoryTreeNode(Path p) { path = p; }
	auto GetPath() const -> const Path& { return path; }
	auto GetChildren() const -> const std::vector<DirectoryTreeNode*>& {
		return children;
	}
};


class DirectoryTree {
private:

	DirectoryTreeNode* root;
	
public:

	auto AddPath(const Path& path) -> void;
	DirectoryTree(Path path) {
		root = new DirectoryTreeNode(path);
	}
	auto GetRootNode() const -> DirectoryTreeNode* { return root; }

	auto GetNode(const Path& path)->DirectoryTreeNode*;

	auto GetPathFromRoot(DirectoryTreeNode* node) const->Path;

};

