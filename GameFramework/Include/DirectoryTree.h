#pragma once
#include <string>
#include <vector>
#include <filesystem>

#include <iterator>

using Path = std::filesystem::path;


enum class DirectoryTreeNodeType
{
	// Root, //?
	Directory, // this has to be the first 
	AssetCollection, // this has to be the second
	File,
	CollectionAsset,
	Max // this has to be the last
};

class DirectoryTreeNode{
	friend class DirectoryTree;
private:
	Path name;
	DirectoryTreeNodeType nodeType = DirectoryTreeNodeType::File;

	DirectoryTreeNode* parent = nullptr;
	std::vector<DirectoryTreeNode*> children = {};

	size_t counts[static_cast<size_t>(DirectoryTreeNodeType::Max)];
private:
	auto AddChildNode(DirectoryTreeNode* node)->void;
	DirectoryTreeNode(Path p, DirectoryTreeNodeType nodeType);

public: 
	auto GetName() const -> const Path& { return name; }
	//auto GetDirectoryType() const -> DirectoryTreeNodeType { return nodeType; }
	auto AddFile(DirectoryTreeNode* leaf)->void;

public:
	
	auto GetChildren() const -> const std::vector<DirectoryTreeNode*>& { return children; }

	auto GetDirectChildByName(const Path& name) const -> DirectoryTreeNode*;

	auto IsDirectory() const -> bool { return nodeType == DirectoryTreeNodeType::Directory || nodeType == DirectoryTreeNodeType::AssetCollection; }
	auto IsAssetCollection() const -> bool { return nodeType == DirectoryTreeNodeType::AssetCollection; }
	auto IsAssetFromCollection() const -> bool { return nodeType == DirectoryTreeNodeType::CollectionAsset; }

	auto GetPathFromTreeRoot() const->Path;

	auto IsLeafNode() const -> bool { return GetNumDirectories() == 0; }

	auto GetNumDirectories() const -> size_t { return counts[static_cast<size_t>(DirectoryTreeNodeType::Directory)] + counts[static_cast<size_t>(DirectoryTreeNodeType::AssetCollection)]; }
};


class DirectoryTree 
{
	friend class AssetManager;
private:

	DirectoryTreeNode* root;
	auto AddNodeByPath(const Path& path, DirectoryTreeNodeType nodeType) -> DirectoryTreeNode*;
	
public:

	DirectoryTree(Path path) { root = new DirectoryTreeNode(path, DirectoryTreeNodeType::Directory); }
	auto GetRootNode() const -> DirectoryTreeNode* { return root; }

	auto GetDirectoryByPath(const Path& path)->DirectoryTreeNode*;

	// TODO:
	//class DirectoryOnlyIterator
	//{
	//	friend class DirectoryTree;
	//	DirectoryTreeNode* node;
	//	std::vector<DirectoryTreeNode*> stack;
	//	explicit DirectoryOnlyIterator(DirectoryTreeNode* _node = nullptr) : node(_node) { stack.push_back(node); }
	//public:
	//	auto operator++() -> DirectoryOnlyIterator&;
	//	//DirectoryOnlyIterator operator++(int) { DirectoryOnlyIterator retval = *this; ++(*this); return retval; }
	//	auto operator==(const DirectoryOnlyIterator& other) const -> bool { return node == other.node; }
	//	bool operator!=(const DirectoryOnlyIterator& other) const { return !(*this == other); }
	//	DirectoryTreeNode* operator*() const { return node; }

	//	auto Pop() -> void{ if (node) node = node->parent; }
	//};

	//auto GetDirectoryIterator() -> DirectoryOnlyIterator { return DirectoryOnlyIterator(GetRootNode()); }

	//auto end() -> DirectoryOnlyIterator { return DirectoryOnlyIterator(nullptr); }
};

