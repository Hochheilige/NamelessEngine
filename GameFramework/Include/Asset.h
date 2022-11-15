#pragma once

#include "FileSystem.h"

class Asset // : public Object?
{
	friend class AssetManager;
public:

	auto GetFullPath() const -> const Path& { return fullPath; }

	virtual auto Load() -> bool { return false; }

	virtual ~Asset() = default;

private:
	Path fullPath;
};

class TextureAsset : public Asset
{

};

class AnimationAsset : public Asset
{

};

class MaterialAsset : public Asset
{

};

class MaterialInstanceAsset : public Asset
{

};