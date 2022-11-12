#include "DirectoryTree.h"

auto DirectoryTree::AddPath(const Path& path) -> void {

	DirectoryTreeNode* node = root;

	auto current = path.begin();
	auto end = path.end();

	////Clean path temp fix
	//Path tempPath = *current;
	//while (tempPath != rootPath) {
	//	tempPath /= *++current;
	//}
	//tempPath = *current;
	//while (current != end) {
	//	tempPath /= *++current;
	//}
	//path = tempPath;
	//current = path.begin();


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

	/*size_t start = root->name.length();
	DirectoryTreeNode* node = root;
	Path dir_name;*/

	////repeat untill a place for child found
	//while (true) {
	//	//get substring of a path
	//	size_t end = path.find_first_of('\\', start);
	//	if (end == static_cast<size_t>(-1)) {
	//		dir_name = path.substr(start);
	//		//find child with matching substring
	//		for (auto dir : node->children) {
	//			if (dir->name == dir_name)
	//				return;
	//		}
	//		node->AddChild(new DirectoryTreeNode(dir_name));
	//		return;
	//	}
	//	else {
	//		bool isFound = false;
	//		dir_name = path.substr(start, end - 1);
	//		start = end + 1;
	//		//find child with matching substring
	//		for (auto dir : node->children) {
	//			if (dir->name == dir_name) {
	//				node = dir;
	//				isFound = true;
	//				break;
	//			}		
	//		}
	//		if (!isFound) {
	//			node = node->AddChild(new DirectoryTreeNode(dir_name));
	//		}
	//	}
	//}
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
	while (current != end) {
		DirectoryTreeNode* n = nullptr;
		for (DirectoryTreeNode* dir : node->children) {
			if (dir->path == *current) {
				n = dir;
				break;
			}
		}
		if (n != nullptr)
		{
			node = n;
		}
		else
		{
			break;
		}
	}
    return nullptr;
}

auto DirectoryTree::GetPathFromRoot(DirectoryTreeNode* node) const -> Path
{
	Path path;

	while (node != nullptr)
	{
		path = node->path / path;
		node = node->parent;
	}

	return path;
}

