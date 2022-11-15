#include "EngineContentRegistry.h"

#include "Actor.h"
#include "CreateCommon.h"
#include "Game.h"
#include "LightBase.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "MeshLoader.h"
#include "RigidBodyCube.h"
#include "RigidBodySphere.h"
#include "ShaderCompiler.h"
#include "StaticMeshRenderer.h"
#include "AssetManager.h"

#include <WICTextureLoader.h>

EngineContentRegistry::EngineContentRegistry(Game* InGame)
	: MyGame(InGame)
{
#pragma region Create Textures
	DirectX::CreateWICTextureFromFile(MyGame->GetD3DDevice().Get(), L"../Assets/white.png", WhiteTex.GetAddressOf(), WhiteTexSRV.GetAddressOf());
	DirectX::CreateWICTextureFromFile(MyGame->GetD3DDevice().Get(), L"../Assets/folder_thumb.png", &FolderTex, &FolderTexSRV, 256);
	DirectX::CreateWICTextureFromFile(MyGame->GetD3DDevice().Get(), L"../Assets/generic_file_thumb.png", &GenericFileTex, &GenericFileTexSRV, 256);
	CreateNormalMapTextureFromFile(L"../Assets/basicNormal.png", BasicNormalTex.GetAddressOf(), BasicNormalTexSRV.GetAddressOf());
#pragma endregion Create Textures


#pragma region Create Meshes
	MeshLoader ml = MeshLoader("../Assets/box.fbx");
	TexturedMesh mesh = ml.GetMesh(0);
	TexturedBoxMeshProxy = mesh.CreateRenderingPrimitiveProxy();

	SphereMesh sphereMesh;
	SphereMeshProxy = sphereMesh.CreateRenderingPrimitiveProxy();
#pragma endregion Create Meshes


#pragma region Create Shaders
	ShaderCompiler sc;
	sc.SetIsDebug(true);
	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	sc.SetPathToShader(L"../Shaders/TexturedShader.hlsl");

	DefaultVertexShader = sc.CreateShader<TexturedVertexShader>();

	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");

	DefaultPixelShader = sc.CreateShader<PixelShader>();
#pragma endregion Create Shaders


#pragma region Save Create Funcs
	CreateActionsMap.emplace(std::string("Box Actor"), &EngineContentRegistry::CreateBox);
	CreateActionsMap.emplace(std::string("Point Light Actor"), &EngineContentRegistry::CreatePointLight);
	CreateActionsMap.emplace(std::string("Sphere Actor"), &EngineContentRegistry::CreateSphere);
#pragma endregion Save Create Funcs


	for (auto pair : CreateActionsMap)
	{
		BasicActorNames.push_back(pair.first);
	}
}

EngineContentRegistry::~EngineContentRegistry()
{
	delete TexturedBoxMeshProxy;
	delete DefaultPixelShader;
	delete DefaultVertexShader;
}

auto EngineContentRegistry::CreateBasicActor(const std::string& BaseActorName, const Transform& transform)->Actor*
{
	auto findRes = CreateActionsMap.find(BaseActorName);
	if (findRes != CreateActionsMap.end())
	{
		CreateFuncType f = findRes->second;
		return (this->*f)(transform);
	}
	return nullptr;
}

auto EngineContentRegistry::CreateBox(const Transform& transform) -> Actor*
{
	Actor* box = CreateActor<Actor>();
	RigidBodyCube* box_rb = box->AddComponent<RigidBodyCube>();
	box->SetTransform(transform);
	box_rb->SetMass(1);
	box_rb->Init();
	StaticMeshRenderer* mesh_component = box->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(MyGame->GetAssetManager()->LoadStaticMesh(Path("../Assets/box.fbx/Cube")));
	mesh_component->SetPixelShader(DefaultPixelShader);
	mesh_component->SetVertexShader(DefaultVertexShader);
	mesh_component->SetAlbedoSRV(WhiteTexSRV);
	mesh_component->SetNormalSRV(BasicNormalTexSRV);
	box->UsePhysicsSimulation();
	return box;
}

auto EngineContentRegistry::CreatePointLight(const Transform& transform)->Actor*
{
	Actor* actor = CreateActor<Actor>();

	PointLight* pl = actor->AddComponent<PointLight>();
	pl->SetRelativePosition(transform.Position);
	MyGame->MyRenderingSystem->RegisterLight(pl);

	// TODO: create renderer in light ?
	MeshRenderer* mr = new MeshRenderer(false);
	mr->SetMeshProxy(TexturedBoxMeshProxy);
	mr->SetVertexShader(DefaultVertexShader);
	mr->SetPixelShader(DefaultPixelShader);
	// todo: calculate size based on intensity
	mr->SetRelativeScale(Vector3::One * 50.0f);
	mr->SetAttachmentParent(pl);

	pl->SetRenderer(mr);
	//pl->color = Color(0.5f, 1.0f, 1.0f);

	return actor;
}

auto EngineContentRegistry::CreateSphere(const Transform& transform)->Actor*
{
	Actor* sphere = CreateActor<Actor>();
	auto sphere_rb = sphere->AddComponent<RigidBodySphere>();
	sphere->SetTransform(transform);
	sphere_rb->SetMass(1);
	sphere_rb->Init();
	auto mesh_component = sphere->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(SphereMeshProxy);
	mesh_component->SetPixelShader(DefaultPixelShader);
	mesh_component->SetVertexShader(DefaultVertexShader);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());
	return sphere;
}


auto EngineContentRegistry::CreateNormalMapTextureFromFile(const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView) -> void
{
	DirectX::CreateWICTextureFromFileEx(
		MyGame->GetD3DDevice().Get(),
		fileName,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0, 0,
		DirectX::WIC_LOADER_IGNORE_SRGB,
		texture,
		textureView);
}
