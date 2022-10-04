#include "MeshLoader.h"

#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

using namespace std;


MeshLoader::MeshLoader(std::string filepath)
{
	OpenFile(filepath);
}

TexturedMesh MeshLoader::GetMesh(size_t meshIndex)
{
	const aiScene* scene = importer.GetScene();

	TexturedMesh res;

	aiMesh* mesh = scene->mMeshes[meshIndex];

	

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

		res.AddVertex(v);
	}
	if (mesh->mTextureCoords != nullptr) {
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			res.GetVertex(i).TexCoord.x = mesh->mTextureCoords[0][i].x;
			res.GetVertex(i).TexCoord.y = 1.0f - mesh->mTextureCoords[0][i].y;
		}
	}

	// todo: load texture for the mesh too using scene->mMaterials[mesh->mMaterialIndex]->GetTexture()?

	for (size_t i = 0; i < mesh->mNumFaces; ++i) {
		res.AddIndex(mesh->mFaces[i].mIndices[0]);
		res.AddIndex(mesh->mFaces[i].mIndices[1]);
		res.AddIndex(mesh->mFaces[i].mIndices[2]);
	}

	return res;
}

void MeshLoader::OpenFile(std::string filepath)
{
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
	}
}