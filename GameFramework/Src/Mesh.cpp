#include "Mesh.h"

#include "Game.h"
#include "MeshProxy.h"

MeshProxy* ColoredMesh::CreateMeshProxy()
{
	MeshProxy* meshProxy = new MeshProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(),  sizeof(ColoredVertex),  &Indices[0], Indices.size());

	return meshProxy;
}

MeshProxy* TexturedMesh::CreateMeshProxy()
{
	MeshProxy* meshProxy = new MeshProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(), sizeof(TexturedVertex), &Indices[0], Indices.size());

	return meshProxy;
}

MeshProxy* BasicMesh::CreateMeshProxy()
{
	MeshProxy* meshProxy = new MeshProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(), sizeof(BasicVertex), &Indices[0], Indices.size());

	return meshProxy;
}
