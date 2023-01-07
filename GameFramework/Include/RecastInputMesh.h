#pragma once

#include <vector>

class DebugDrawer;

class RecastInputMesh
{
public:

	auto AddVertex(float x, float y, float z) -> void;

	auto AddFace(int a, int b, int c) -> void;

	auto DrawDebugWireFrame(DebugDrawer* debugDrawer) const -> void;

	auto GetNumFaces() const -> int { return faces.size() / 3; }

	auto GetNumVertices() const -> int { return vertices.size() / 3; }

protected:
	std::vector<float> vertices;

	std::vector<int> faces;
};