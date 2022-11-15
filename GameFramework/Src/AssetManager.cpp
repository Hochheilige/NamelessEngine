#include "AssetManager.h"

#include "DirectoryTree.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "StaticMesh.h"


auto AssetManager::Initialize() -> void
{
	directoryTree.reset(new DirectoryTree(Path("Assets")));
	FillDirectoryTree();
}

auto AssetManager::FillDirectoryTree() -> void
{
	for (auto dir_entry : std::filesystem::recursive_directory_iterator(assetsPath)) {
		const Path path = dir_entry.path().lexically_relative(assetsPath);
		const bool isDirectory = dir_entry.is_directory() || dir_entry.is_regular_file() && dir_entry.path().extension() == Path(".fbx");
		if (dir_entry.is_directory()) {
			directoryTree->AddNodeByPath(path, DirectoryTreeNodeType::Directory);
		}
		else if (dir_entry.is_regular_file())
		{
			
			if (IsAssetCollectionExtension(path.extension()))
			{
				Assimp::Importer importer;
				const aiScene* scene = importer.ReadFile(dir_entry.path().string(), aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

				directoryTree->AddNodeByPath(path, DirectoryTreeNodeType::AssetCollection);
				
				// todo: think of how to make it better
				// at least save this data into meta files
				std::vector<const aiNode*> stack;
				stack.push_back(scene->mRootNode);
				while (!stack.empty())
				{
					const aiNode* node = stack.back();
					stack.pop_back();

					stack.insert(stack.end(), node->mChildren, node->mChildren + node->mNumChildren);

					if (node->mNumMeshes > 0)
					{
						for (size_t i = 0; i < node->mNumMeshes; ++i)
						{
							const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
							directoryTree->AddNodeByPath(path/Path(mesh->mName.C_Str()), DirectoryTreeNodeType::CollectionAsset);
						}
					}
				}
			}
			else
			{
				directoryTree->AddNodeByPath(path, DirectoryTreeNodeType::File);
			}
		}
	}
}

auto AssetManager::IsAssetCollectionExtension(const Path& extension) const -> bool
{
	return extension == Path(".fbx") || extension == Path(".obj");
}

auto AssetManager::LoadStaticMesh(const Path& path)->StaticMesh*
{
	auto res = LoadedAssetsMap.find(path);
	if (res != LoadedAssetsMap.end())
	{
		return dynamic_cast<StaticMesh*>(res->second);
	}

	StaticMesh* sm = new StaticMesh();

	sm->fullPath = path;

	if (!sm->Load())
	{
		return nullptr;
	}

	LoadedAssetsMap.insert({ path, sm });

	return sm;
}