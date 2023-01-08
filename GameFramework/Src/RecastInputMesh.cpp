#include "RecastInputMesh.h"

#include "DebugDrawer.h"

#include "LinearMath/btVector3.h"

auto RecastInputMesh::AddVertex(float x, float y, float z) -> void
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

auto RecastInputMesh::AddFace(int a, int b, int c) -> void
{
	faces.push_back(a);
	faces.push_back(b);
	faces.push_back(c);

	float* v0 = &(vertices[a * 3]);
	float* v1 = &(vertices[b * 3]);
	float* v2 = &(vertices[c * 3]);

	btVector3 vec0{ v0[0], v0[1], v0[2] };
	btVector3 vec1{ v1[0], v1[1], v1[2] };
	btVector3 vec2{ v2[0], v2[1], v2[2] };

	btVector3 normal = (vec1 - vec0).cross(vec2 - vec0);
	normals.push_back(normal.x());
	normals.push_back(normal.y());
	normals.push_back(normal.z());
}

auto RecastInputMesh::DrawDebugWireFrame(DebugDrawer* debugDrawer) const -> void
{
	for (int i = 0; i < faces.size(); i += 3)
	{
		btVector3 v0 = { vertices[faces[i + 0] * 3 + 0], vertices[faces[i + 0] * 3 + 1], vertices[faces[i + 0] * 3 + 2] };
		btVector3 v1 = { vertices[faces[i + 1] * 3 + 0], vertices[faces[i + 1] * 3 + 1], vertices[faces[i + 1] * 3 + 2] };
		btVector3 v2 = { vertices[faces[i + 2] * 3 + 0], vertices[faces[i + 2] * 3 + 1], vertices[faces[i + 2] * 3 + 2] };
		btVector3 color = { 0.0f, 1.0f, 1.0f };
		debugDrawer->drawLine(v0, v1, color);
		debugDrawer->drawLine(v1, v2, color);
		debugDrawer->drawLine(v2, v0, color);
	}
}
