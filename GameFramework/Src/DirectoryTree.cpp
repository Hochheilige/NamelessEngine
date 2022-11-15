#include "DirectoryTree.h"
#include <cassert>

auto DirectoryTree::AddPath(const Path& path) -> void {

	DirectoryTreeNode* node = root;

	auto current = path.begin();
	auto end = path.end();

	while (current != end) {
		//find child with matching directory name
		if (current->empty()) continue;
		bool isFound(false);
		for (auto dir : node->children) {
			if (dir->path == *current) {
				node = dir;
				isFound = true;
				break;
			}		
		}
		if (!isFound) {
			node = node->AddChild(new DirectoryTreeNode(*current));
		}
		++current;
	}
}

auto DirectoryTree::GetNode(const Path& path) -> DirectoryTreeNode*
{
	DirectoryTreeNode* node = root;

	if (node && node->path == path)
	{
		return node;
	}

	auto current = path.begin();
	auto end = path.end();
	while (node != nullptr && current != end) {
		node = node->GetChildWithName(*current);
	}
    return node;
}

auto DirectoryTree::GetPathFromRoot(DirectoryTreeNode* node) const -> Path
{
	Path path = node->path;
	assert(node != nullptr);
	node = node->parent;

	while (node != nullptr)
	{
		path = node->path / path;
		node = node->parent;
	}

	return path;
}

auto DirectoryTreeNode::GetChildWithName(const Path& name) const -> DirectoryTreeNode*
{
	DirectoryTreeNode* node = nullptr;
	for (DirectoryTreeNode* dir : children) {
		if (dir->path == name) {
			node = dir;
			break;
		}
	}
	return node;
}
