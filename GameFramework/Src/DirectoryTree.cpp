#include "DirectoryTree.h"
#include <cassert>


auto DirectoryTreeNode::GetPathFromTreeRoot() const->Path
{
	Path path = name;
	DirectoryTreeNode* node = parent;

	while (node != nullptr)
	{
		path = node->name / path;
		node = node->parent;
	}

	return path;
}

DirectoryTreeNode::~DirectoryTreeNode()
{
	for (DirectoryTreeNode* child : children)
	{
		delete child;
	}
}

DirectoryTreeNode::DirectoryTreeNode(Path p, DirectoryTreeNodeType nodeType)
	: name(p)
	, nodeType(nodeType)
{

}

auto DirectoryTreeNode::GetDirectChildByName(const Path& name) const -> DirectoryTreeNode*
{
	// todo: save name hash to make this method faster?
	DirectoryTreeNode* node = nullptr;
	for (DirectoryTreeNode* dir : children) {
		if (dir->name == name) {
			node = dir;
			break;
		}
	}
	return node;
}

auto DirectoryTreeNode::AddChildNode(DirectoryTreeNode* node)->void
{
	children.push_back(node);
	size_t i = children.size() - 1;
	while (i > 0 && children[i-1]->nodeType > children[i]->nodeType)
	{
		std::swap(children[i - 1], children[i]);
		--i;
	}
	++counts[static_cast<size_t>(node->nodeType)];
	node->parent = this;
}

auto DirectoryTree::AddNodeByPath(const Path& path, DirectoryTreeNodeType nodeType, AssetType assetType) -> DirectoryTreeNode* {

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
			auto temp = current;
			++temp;
			DirectoryTreeNodeType nt = temp == end ? nodeType : DirectoryTreeNodeType::Directory;
			DirectoryTreeNode* newNode = new DirectoryTreeNode(*current, nt);
			node->AddChildNode(newNode);
			node = newNode;
			if (nt == DirectoryTreeNodeType::File)
			{
				newNode->assetType = assetType;
			}
		}
		++current;
	}

	return node;
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

DirectoryTree::~DirectoryTree()
{
	delete root;
}

//auto DirectoryTree::DirectoryOnlyIterator::operator++()->DirectoryOnlyIterator&
//{
//	return *this;
//}