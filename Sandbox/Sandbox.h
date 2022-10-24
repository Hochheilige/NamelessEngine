#pragma once

#include "Game.h"
#include "GameComponent.h"

#include "btBulletDynamicsCommon.h"
#include "RigidBodyComponent.h"
#include "Actor.h"

class Sandbox : public Game
{
public:

	virtual void PrepareResources() override;

	virtual void Update(float DeltaTime) override;

	// Base objects with mesh and rigidbody components
	Actor* CreateBox(Transform transform);
	Actor* CreateSphere(Transform transform);

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

	class MeshRenderer* updateBox;

	std::vector<RigidBodyComponent*> rigidBodies;

	std::vector<class MeshRenderer*> box;
	class MeshRenderer* bunny;

};