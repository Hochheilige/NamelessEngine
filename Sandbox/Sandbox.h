#pragma once

#include "Game.h"
#include "GameComponent.h"
#include "MonoSystem.h"

#include "btBulletDynamicsCommon.h"

class Actor;
class RigidBodyComponent;

class Sandbox : public Game
{
public:

	virtual auto Initialize() -> void override;

	virtual void PrepareResources() override;

	virtual void Update(float DeltaTime) override;

	// Base objects with mesh and rigidbody components
	std::shared_ptr<Actor> CreateNonPhysicsBox(Transform transform);
	std::shared_ptr<Actor> CreatePlayahActor(Transform transform);
	std::shared_ptr<Actor> CreateStaticBox(Transform transform);
	std::shared_ptr<Actor> CreateDynamicBox(Transform transform);
	std::shared_ptr<Actor> CreateKinematicBox(Transform transform);
	std::shared_ptr<Actor> CreateStaticSphere(Transform transform);
	std::shared_ptr<Actor> CreateDynamicSphere(Transform transform);
	std::shared_ptr<Actor> CreateKinematicSphere(Transform transform);

	std::shared_ptr<Actor> CreateBunny(Transform transform);

	std::shared_ptr<Actor> CreateBun(Transform transform);

	auto CreateHierarcyTestActor() -> std::shared_ptr<Actor>;

private:
	

	

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

	std::weak_ptr<Actor> platform;
	std::weak_ptr<Actor> staticBox;


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
	virtual auto StartPlay() -> void override;
	virtual auto PausePlay() -> void override;
	virtual auto StopPlay() -> void override;
	virtual auto ResumePlay() -> void override;

	void ChangeGameState(bool isPlaying);
	

};