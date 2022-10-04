#pragma once

#include <string>

#include <assimp/Importer.hpp>

class MeshLoader
{
public:
	MeshLoader(std::string filepath);
	MeshLoader() = default;
	~MeshLoader() = default;

	class TexturedMesh GetMesh(size_t meshIndex);

	void OpenFile(std::string filepath);

private:


	Assimp::Importer importer;
};