#pragma once

#include "Game.h"
#include "GameComponent.h"

class KatamariGame : public Game
{
public:
	virtual void PrepareResources() override;

	virtual void Update(float DeltaTime) override;

protected:
	class KatamariComponent* Player = nullptr;

	ComPtr<ID3D11Resource> cupTexResource;
	ComPtr<ID3D11ShaderResourceView> cupTexSRV;

	ComPtr<ID3D11Resource> pepsiTexResource;
	ComPtr<ID3D11ShaderResourceView> pepsiTexSRV;

	ComPtr<ID3D11Resource> pizzaBoxTexResource;
	ComPtr<ID3D11ShaderResourceView> pizzaBoxTexSRV;

	ComPtr<ID3D11Resource> burgerTexResource;
	ComPtr<ID3D11ShaderResourceView> burgerTexSRV;

	ComPtr<ID3D11Resource> burgerNormal;
	ComPtr<ID3D11ShaderResourceView> burgerNormalSRV;

	ComPtr<ID3D11Resource> burgerSpecular;
	ComPtr<ID3D11ShaderResourceView> burgerSpecSRV;


	class PixelShader* basicPS;
	class BasicVertexShader* basicVS;

	class PixelShader* texturedPS;
	class TexturedVertexShader* texturedVS;

	class MeshProxy* canMeshProxy;

	class KatamariComponent* CreateKatamariComponent(
		class MeshProxy* InMesh, 
		ComPtr<ID3D11ShaderResourceView> TexSRV, 
		Vector3 Position, 
		float Radius,
		const LitMaterial& mat,
		Vector3 MeshScale, 
		Vector3 MeshLocalPos = Vector3::Zero);

	class KatamariComponent* CreateCanComponent(Vector3 Position, float Radius, Vector3 MeshScale);
};

class KatamariComponent : public GameComponent
{
public:
	KatamariComponent();

	void SetRadius(float NewRadius);

	void InitLineSphere(class PixelShader* InPixelShader, class VertexShader* InVertexShader, Color InColor);

	class SphereCollider* GetSphereCollider() { return mSphereCollider; }

	float GetRadius() const { return mRadius; }

	void DestroyShpereCollider();

protected:

	float mRadius = 0.5f;
	
	class SphereCollider* mSphereCollider = nullptr;
	class LineSphere* mLineSphere = nullptr;
};