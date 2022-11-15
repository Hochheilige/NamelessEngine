#include "StaticMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Game.h"

auto StaticMesh::CreateRenderData() -> void
{
	Assimp::Importer importer;
	// todo: check if the asset is part of a collection first and move such functionality to Asset class
	Path pathToCollection = GetFullPath();
	pathToCollection._Remove_filename_and_separator();
	const aiScene* scene = importer.ReadFile(pathToCollection.string(), aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (scene == nullptr)
	{
		return;
	}

	const Path name = GetFullPath().filename();

	std::vector<const aiNode*> stack;
	stack.push_back(scene->mRootNode);
	while (!stack.empty())
	{
		const aiNode* node = stack.back();
		stack.pop_back();

		stack.insert(stack.end(), node->mChildren, node->mChildren + node->mNumChildren);

		if (node->mNumMeshes > 0 && name.string()._Equal(node->mName.C_Str()))
		{
			std::vector<TexturedVertex> vertices;
			std::vector<UINT> indices;

			renderData.reset(new StaticMeshRenderData());
			renderData->vertexSize = sizeof(vertices[0]);

			for (size_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
			{
				const aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIndex]];

				for (size_t i = 0; i < mesh->mNumVertices; ++i) {
					TexturedVertex v;

					v.Position.x = mesh->mVertices[i].x;
					v.Position.y = mesh->mVertices[i].y;
					v.Position.z = mesh->mVertices[i].z;

					v.Normal.x = mesh->mNormals[i].x;
					v.Normal.y = mesh->mNormals[i].y;
					v.Normal.z = mesh->mNormals[i].z;

					v.Binormal.x = mesh->mBitangents[i].x;
					v.Binormal.y = mesh->mBitangents[i].y;
					v.Binormal.z = mesh->mBitangents[i].z;

					v.Tangent.x = mesh->mTangents[i].x;
					v.Tangent.y = mesh->mTangents[i].y;
					v.Tangent.z = mesh->mTangents[i].z;

					vertices.push_back(v);
				}

				if (mesh->mTextureCoords != nullptr) {
					for (size_t i = 0; i < mesh->mNumVertices; ++i) {
						vertices[i].TexCoord.x = mesh->mTextureCoords[0][i].x;
						vertices[i].TexCoord.y = mesh->mTextureCoords[0][i].y;
					}
				}

				StaticMeshSection& section = renderData->sections.emplace_back();
				section.materialIndex = mesh->mMaterialIndex;
				section.indicesStart = indices.size();
				section.numIndices = mesh->mNumFaces * 3;

				for (size_t i = 0; i < mesh->mNumFaces; ++i) {
					indices.push_back(mesh->mFaces[i].mIndices[0]);
					indices.push_back(mesh->mFaces[i].mIndices[1]);
					indices.push_back(mesh->mFaces[i].mIndices[2]);
				}
			}

			D3D11_BUFFER_DESC vertexBufDesc = {};
			vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufDesc.CPUAccessFlags = 0;
			vertexBufDesc.MiscFlags = 0;
			vertexBufDesc.StructureByteStride = 0;
			vertexBufDesc.ByteWidth = vertices.size() * renderData->vertexSize;

			D3D11_SUBRESOURCE_DATA vertexData = {};
			vertexData.pSysMem = vertices.data();
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
			device->CreateBuffer(&vertexBufDesc, &vertexData, renderData->vertexBuffer.GetAddressOf());

			D3D11_BUFFER_DESC indexBufDesc = {};
			indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufDesc.CPUAccessFlags = 0;
			indexBufDesc.MiscFlags = 0;
			indexBufDesc.StructureByteStride = 0;
			indexBufDesc.ByteWidth = sizeof(indices[0]) * static_cast<UINT>(indices.size());

			D3D11_SUBRESOURCE_DATA indexData = {};
			indexData.pSysMem = indices.data();
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			device->CreateBuffer(&indexBufDesc, &indexData, renderData->indexBuffer.GetAddressOf());

			break;
		}
	}

}
