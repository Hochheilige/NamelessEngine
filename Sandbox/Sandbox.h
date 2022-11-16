#pragma once

#include "Game.h"
#include "GameComponent.h"
#include "MonoSystem.h"

#include "btBulletDynamicsCommon.h"
#include "RigidBodyComponent.h"

class Actor;

class Sandbox : public Game
{
public:

	virtual auto Initialize() -> void override;

	virtual void PrepareResources() override;

	virtual void Update(float DeltaTime) override;

	// Base objects with mesh and rigidbody components
	Actor* CreateNonPhysicsBox(Transform transform);
	Actor* CreateStaticBox(Transform transform);
	Actor* CreateDynamicBox(Transform transform);
	Actor* CreateKinematicBox(Transform transform);
	Actor* CreateStaticSphere(Transform transform);
	Actor* CreateDynamicSphere(Transform transform);
	Actor* CreateKinematicSphere(Transform transform);

	Actor* CreateBunny(Transform transform);

	Actor* CreateBun(Transform transform);

	auto CreateHierarcyTestActor() ->Actor*;

private:
	void LoadGameFacade();

	class MonoSystem* mono;
	MonoObject* csGameInstance;

	class BoxMesh* boxMesh;
	class CircleMesh* circleMesh;
	class SphereMesh* sphereMesh;
	class VertexShader* vs;
	class PixelShader* ps;
	class PixelShader* psPlain;

	class RenderPrimitiveProxy* boxMeshProxy;
	class RenderPrimitiveProxy* circleMeshProxy;
	class RenderPrimitiveProxy* sphereMeshProxy;
	RenderPrimitiveProxy* bunnyMeshProxy;
	RenderPrimitiveProxy* texturedBoxMeshProxy;

	class Camera* PerspCamera;
	class Camera* OrthoCamera;
	class OrbitCameraController* OrbitCC;
	class CameraController* FPSCC;
	class GameComponent* CurrentCC = nullptr;

	class MeshRenderer* updateBox;

	std::vector<RigidBodyComponent*> rigidBodies;

	std::vector<class MeshRenderer*> box;
	class MeshRenderer* bunny;

	Actor* platform;
	Actor* sphere;
	Actor* staticBox;


	// burger bun
	RenderPrimitiveProxy* burgerMeshProxy;

	ComPtr<ID3D11Resource> burgerTexResource;
	ComPtr<ID3D11ShaderResourceView> burgerTexSRV;

	ComPtr<ID3D11Resource> burgerNormal;
	ComPtr<ID3D11ShaderResourceView> burgerNormalSRV;

	ComPtr<ID3D11Resource> burgerSpecular;
	ComPtr<ID3D11ShaderResourceView> burgerSpecSRV;

	PlayState prevPlayState = PlayState::Editor;


	virtual auto OnBeginPlay() -> void override;
};