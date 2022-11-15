#pragma once

#include "Asset.h"
#include "Mesh.h"

#include <memory>
#include <vector>

struct StaticMeshSection
{
	int32_t materialIndex;
	uint32_t indicesStart;
	uint32_t vertexStart;
	uint32_t numIndices;
};

// Data needed to render a static mesh
struct StaticMeshRenderData
{
	std::vector<StaticMeshSection> sections;

	uint32_t vertexSize;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
};

/*
* A Mesh backed up by a file on disk
*/
class StaticMesh : public Asset
{
	friend class AssetManager;
public:
	
	auto GetRenderData() const -> const StaticMeshRenderData* { return renderData.get(); }

	virtual auto Load() -> bool override { CreateRenderData(); return renderData.get() != nullptr; }

	// import-related variables

	// serialize/deserialize functions

protected:
	auto CreateRenderData() -> void;

	std::unique_ptr<StaticMeshRenderData> renderData;

};