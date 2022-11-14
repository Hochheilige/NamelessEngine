#pragma once
#include <string>
#include <vector>
#include <filesystem>

using Path = std::filesystem::path;

enum class DirectoryType
{
	Directory,
	AssetCollection
};

// todo: remove leaf?
class DirectoryTreeLeaf
{
	friend class DirectoryTree;
	friend class DirectoryTreeNode;
protected:
	Path name;
	DirectoryTreeLeaf(Path p) : name(p) { }
	class DirectoryTreeNode* parent = nullptr;
public:
	auto GetName() const -> const Path& { return name; }

	virtual auto IsDirectory() const -> bool { return false; }

	auto GetPathFromTreeRoot() const->Path;

};

class DirectoryTreeNode : public DirectoryTreeLeaf {
	friend class DirectoryTree;
private:
	std::vector<DirectoryTreeNode*> children = {};
	std::vector<DirectoryTreeLeaf*> files;

	DirectoryType directoryType = DirectoryType::Directory;
public:
	auto AddChildDirectory(DirectoryTreeNode* node) -> DirectoryTreeNode* { 
		children.push_back(node); 
		node->parent = this;
		return node;
	}
	auto AddFile(DirectoryTreeLeaf* leaf)->DirectoryTreeLeaf*;
	DirectoryTreeNode(Path p);
	auto GetChildDirectories() const -> const std::vector<DirectoryTreeNode*>& { return children; }
	auto GetChildFiles() const -> const std::vector<DirectoryTreeLeaf*>& { return files; }

	auto GetDirectChildByName(const Path& name) const -> DirectoryTreeNode*;

	auto GetDirectoryType() const -> DirectoryType { return directoryType; }

	virtual auto IsDirectory() const -> bool override { return true; }

};


class DirectoryTree {
private:

	DirectoryTreeNode* root;
	
public:

	auto AddDirectoryByPath(const Path& path) -> DirectoryTreeNode*;
	auto AddFileByPath(const Path& path)->DirectoryTreeLeaf*;
	DirectoryTree(Path path) {
		root = new DirectoryTreeNode(path);
	}
	auto GetRootNode() const -> DirectoryTreeNode* { return root; }

	auto GetDirectoryByPath(const Path& path)->DirectoryTreeNode*;

};

