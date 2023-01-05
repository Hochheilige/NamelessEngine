#pragma once

#include "MeshLoader.h"

#include "FileSystem.h"
#include <memory>
#include <unordered_map>

class Asset;
class DirectoryTree;
class StaticMesh;
class AlbedoTexture;
class NormalTexture;

class AssetManager
{
public:

	auto Initialize() -> void;

	template<class K, class V>
	using AssetMapType = std::unordered_map<K, V>;

	auto LoadStaticMesh(const Path& path)->StaticMesh*;
	auto LoadAlbedoTexture(const Path& path)->AlbedoTexture*;
	auto LoadNormalTexture(const Path& path) -> NormalTexture*;
	

	/*Asset* LoadAsset(const Path& AssetPath) {}

	template<class T>
	T* LoadAsset(Path AssetPath)
	{
		return dynamic_cast<T*>(LoadAsset(AssetPath));
	}*/

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