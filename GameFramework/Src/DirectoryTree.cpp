#include "DirectoryTree.h"
#include <cassert>

auto DirectoryTree::AddDirectoryByPath(const Path& path) -> DirectoryTreeNode* {

	DirectoryTreeNode* node = root;

	auto current = path.begin();
	auto end = path.end();

	while (current != end) {
		//find child with matching directory name
		if (current->empty()) continue;
		bool isFound(false);
		for (auto dir : node->children) {
			if (dir->name == *current) {
				node = dir;
				isFound = true;
				break;
			}		
		}
		if (!isFound) {
			node = node->AddChildDirectory(new DirectoryTreeNode(*current));
		}
		++current;
	}

	return node;
}

auto DirectoryTree::AddFileByPath(const Path& path)->DirectoryTreeLeaf*
{
	Path dirPath = path;
	dirPath._Remove_filename_and_separator();
	DirectoryTreeNode* node = AddDirectoryByPath(dirPath);
	return node->AddFile(new DirectoryTreeLeaf(path));
}

auto DirectoryTree::GetDirectoryByPath(const Path& path) -> DirectoryTreeNode*
{
	DirectoryTreeNode* node = root;

	if (node && node->name == path)
	{
		return node;
	}

	auto current = path.begin();
	auto end = path.end();
	while (node != nullptr && ++current != end) {
		node = node->GetDirectChildByName(*(current));
	}
    return node;
}

auto DirectoryTreeLeaf::GetPathFromTreeRoot() const->Path
{
	Path path = name;
	DirectoryTreeLeaf* node = parent;

	while (node != nullptr)
	{
		path = node->name / path;
		node = node->parent;
	}

	return path;
}

DirectoryTreeNode::DirectoryTreeNode(Path p)
	: DirectoryTreeLeaf(p)
{
	// TODO: pass type as a parameter to constructor?
	directoryType = name.has_extension() ? DirectoryType::AssetCollection : DirectoryType::Directory;
}

auto DirectoryTreeNode::AddFile(DirectoryTreeLeaf* leaf)->DirectoryTreeLeaf*
{
	// todo: add unique by name?
	files.push_back(leaf);
	leaf->parent = this;

	return leaf;
}

auto DirectoryTreeNode::GetDirectChildByName(const Path& name) const -> DirectoryTreeNode*
{
	DirectoryTreeNode* node = nullptr;
	for (DirectoryTreeNode* dir : children) {
		if (dir->name == name) {
			node = dir;
			break;
		}
	}
	return node;
}

