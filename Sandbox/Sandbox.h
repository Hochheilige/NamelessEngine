#pragma once

#include "Game.h"
#include "GameComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

class Sandbox : public Game
{
public:

	virtual void PrepareResources() override;

	virtual void Update(float DeltaTime) override;

	class MeshRenderer* CreateObject(float trans_x, float trans_y, float trans_z,
		float rot_x, float rot_y, float rot_z,
		float scale_x, float scale_y, float scale_z,
		const char* mesh_path);	
	class MeshRenderer* CreateCubeObject(float trans_x, float trans_y, float trans_z,
		float rot_x, float rot_y, float rot_z,
		float scale_x, float scale_y, float scale_z);
	class MeshRenderer* CreateSphereObject(float trans_x, float trans_y, float trans_z,
		float rot_x, float rot_y, float rot_z,
		float scale_x, float scale_y, float scale_z);

private:
	class BoxMesh* boxMesh;
	class CircleMesh* circleMesh;
	class SphereMesh* sphereMesh;
	class SimpleVertexShader* vs;
	class PixelShader* ps;
	class PixelShader* psPlain;

	class MeshProxy* boxMeshProxy;
	class MeshProxy* circleMeshProxy;
	class MeshProxy* sphereMeshProxy;

	class Camera* PerspCamera;
	class Camera* OrthoCamera;
	class OrbitCameraController* OrbitCC;
	class CameraController* FPSCC;
	class GameComponent* CurrentCC = nullptr;

	ComPtr<ID3D11Resource> cupTexResource;
	ComPtr<ID3D11ShaderResourceView> cupTexSRV;

};