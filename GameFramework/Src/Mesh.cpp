#include "Mesh.h"

#include "Game.h"
#include "RenderPrimitiveProxy.h"

RenderPrimitiveProxy* ColoredMesh::CreateRenderingPrimitiveProxy()
{
	RenderPrimitiveProxy* meshProxy = new RenderPrimitiveProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(),  static_cast<UINT>(sizeof(ColoredVertex)),  &Indices[0], Indices.size());

	return meshProxy;
}

RenderPrimitiveProxy* TexturedMesh::CreateRenderingPrimitiveProxy()
{
	RenderPrimitiveProxy* meshProxy = new RenderPrimitiveProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(), static_cast<UINT>(sizeof(TexturedVertex)), &Indices[0], Indices.size());

	return meshProxy;
}

RenderPrimitiveProxy* BasicMesh::CreateRenderingPrimitiveProxy()
{
	RenderPrimitiveProxy* meshProxy = new RenderPrimitiveProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(), static_cast<UINT>(sizeof(BasicVertex)), &Indices[0], Indices.size());

	return meshProxy;
}
