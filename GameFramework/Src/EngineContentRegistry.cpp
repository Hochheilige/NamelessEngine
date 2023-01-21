#include "EngineContentRegistry.h"

#include "Actor.h"
#include "CreateCommon.h"
#include "Game.h"
#include "LightBase.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "MeshLoader.h"
#include "RigidBodyComponent.h"
#include "ShaderCompiler.h"
#include "StaticMeshRenderer.h"
#include "AssetManager.h"

#include <WICTextureLoader.h>

EngineContentRegistry::EngineContentRegistry(Game* InGame)
	: MyGame(InGame)
{
#pragma region Create Textures
	DirectX::CreateWICTextureFromFile(MyGame->GetD3DDevice().Get(), L"../Assets/white.png", WhiteTex.GetAddressOf(), WhiteTexSRV.GetAddressOf());
	DirectX::CreateWICTextureFromFile(MyGame->GetD3DDevice().Get(), L"../Assets/EngineContent/Textures/folder_thumb_v2.png", &FolderTex, &FolderTexSRV, 256);
	DirectX::CreateWICTextureFromFile(MyGame->GetD3DDevice().Get(), L"../Assets/EngineContent/Textures/generic_file_thumb_v2.png", &GenericFileTex, &GenericFileTexSRV, 256);
	DirectX::CreateWICTextureFromFile(MyGame->GetD3DDevice().Get(), L"../Assets/EngineContent/Textures/collection_folder_thumb.png", &AssetColTex, &AssetColTexSRV, 256);
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

	sc.SetPathToShader(L"../Shaders/MyVeryFirstShader.hlsl");
	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");

	PosColorVertexShader = sc.CreateShader<SimpleVertexShader>();

	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");

	PosColorPixelShader = sc.CreateShader<PixelShader>();

#pragma endregion Create Shaders


#pragma region Save Create Funcs
	CreateActionsMap.emplace(std::string("Box Actor"), &EngineContentRegistry::CreateBox);
	CreateActionsMap.emplace(std::string("Point Light Actor"), &EngineContentRegistry::CreatePointLight);
	CreateActionsMap.emplace(std::string("Sphere Actor"), &EngineContentRegistry::CreateSphere);
#pragma endregion Save Create Funcs


#pragma region Light Renderers
	quadRenderer = new QuadRenderer();
	quadRenderer->SetVertexShader(GetDefaultVertexShader());
	quadRenderer->SetPixelShader(GetDefaultPixelShader());

	boxRenderer = new MeshRenderer(false);
	boxRenderer->SetMeshProxy(TexturedBoxMeshProxy);
	boxRenderer->SetVertexShader(DefaultVertexShader);
	boxRenderer->SetPixelShader(DefaultPixelShader);
#pragma endregion Light Renderers

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
	delete quadRenderer;
	delete boxRenderer;
}

auto EngineContentRegistry::CreateBasicActor(const std::string& BaseActorName, const Transform& transform) -> std::shared_ptr<Actor>
{
	auto findRes = CreateActionsMap.find(BaseActorName);
	if (findRes != CreateActionsMap.end())
	{
		CreateFuncType f = findRes->second;
		return (this->*f)(transform);
	}
	return std::shared_ptr<Actor>();
}

auto EngineContentRegistry::CreateBox(const Transform& transform) -> std::shared_ptr<Actor>
{
	std::shared_ptr<Actor> box = CreateActor<Actor>();
	auto box_rb = box->AddComponent<RigidBodyComponent>();
	box->SetTransform(transform);
	box_rb->SetRigidBodyType(RigidBodyType::DYNAMIC);
	box_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
	box_rb->SetMass(1);
	box_rb->Init();
	StaticMeshRenderer* mesh_component = box->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(MyGame->GetAssetManager()->LoadStaticMesh(Path("../Assets/box.fbx/Cube")));
	mesh_component->SetPixelShader(DefaultPixelShader);
	mesh_component->SetVertexShader(DefaultVertexShader);
	mesh_component->SetAlbedoSRV(WhiteTexSRV);
	mesh_component->SetNormalSRV(BasicNormalTexSRV);
	box_rb->EnablePhysicsSimulation();
	return box;
}

auto EngineContentRegistry::CreatePointLight(const Transform& transform) -> std::shared_ptr<Actor>
{
	std::shared_ptr<Actor> actor = CreateActor<Actor>();

	PointLight* pl = actor->AddComponent<PointLight>();
	pl->SetRelativePosition(transform.Position);

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

auto EngineContentRegistry::CreateSphere(const Transform& transform) -> std::shared_ptr<Actor>
{
	std::shared_ptr<Actor> sphere = CreateActor<Actor>();
	auto sphere_rb = sphere->AddComponent<RigidBodyComponent>();
	sphere->SetTransform(transform);
	sphere_rb->SetRigidBodyType(RigidBodyType::DYNAMIC);
	sphere_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
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

auto EngineContentRegistry::GetBoxLightRenderer() const -> Renderer*
{
	return boxRenderer;
}
