#pragma once

#include "MeshLoader.h"

#include <filesystem>
#include <memory>
#include <unordered_map>

class Asset;
class DirectoryTree;

using Path = std::filesystem::path;

class AssetManager
{
public:

	auto Initialize() -> void;

	template<class K, class V>
	using AssetMapType = std::unordered_map<K, V>;

	Asset* LoadAsset(Path AssetPath) {}

	template<class T>
	T* LoadAsset(Path AssetPath)
	{
		return dynamic_cast<T*>(LoadAsset(AssetPath));
	}

	auto GetAssetDirectoryTree() -> DirectoryTree* { return directoryTree.get(); };

private:

	std::unique_ptr<DirectoryTree> directoryTree;

	MeshLoader MyMeshLoader;

	AssetMapType<std::filesystem::path, Asset*> LoadedAssetsMap;

private:
	Path assetsPath = Path("../Assets");
	Path projectPath = Path("../");

	auto FillDirectoryTree() -> void;

	auto IsAssetCollectionExtension(const Path& extension) const -> bool;
};