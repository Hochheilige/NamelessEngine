#include "Sandbox.h"

#include "Actor.h"
#include "ShaderCompiler.h"
#include "Shader.h"
#include "Mesh.h"
#include "DisplayWin32.h"
#include "MeshRenderer.h"
#include "CameraController.h"
#include "OrbitCameraController.h"
#include "InputDevice.h"
#include "PlaneComponent.h"
#include "MeshLoader.h"
#include "LightBase.h"
#include "WICTextureLoader.h"
#include "RigidBodyCube.h"
#include "RigidBodySphere.h"
#include "Mappings.h"
#include "AudioComponent.h"
#include "ScriptObject.h"
#include "windows.h"
#include "mono/metadata/debug-helpers.h"
#include "RenderingSystem.h"
#include "LightBase.h"
#include "EngineContentRegistry.h"

#include "ImGuiInclude.h"

#include "CreateCommon.h"

Actor* Sandbox::CreateNonPhysicsBox(Transform transform) {
	Actor* box = CreateActor<Actor>();
	
	auto mesh_component = box->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(texturedBoxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());
	mesh_component->SetRelativeTransform(transform);

	return box;
}

Actor* Sandbox::CreateStaticBox(Transform transform)
{
	Actor* box = CreateActor<Actor>();
	auto box_rb = box->AddComponent<RigidBodyCube>();
	box->SetTransform(transform);
	box_rb->SetMass(0);
	box_rb->Init();
	auto mesh_component = box->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(texturedBoxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return box;
}

Actor* Sandbox::CreateDynamicBox(Transform transform)
{
	Actor* box = CreateActor<Actor>();
	box->InitializeMonoActor("CustomActor");
	auto box_rb = box->AddComponent<RigidBodyCube>();
	box->SetTransform(transform);
	box_rb->SetMass(1);
	box_rb->Init();
	auto mesh_component = box->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(texturedBoxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return box;
}

Actor* Sandbox::CreateBunny(Transform transform)
{
	Actor* bunny = CreateActor<Actor>();
	auto mesh_component = bunny->AddComponent<MeshRenderer>();
	bunny->SetTransform(transform);
	mesh_component->SetMeshProxy(bunnyMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return bunny;
}

Actor* Sandbox::CreateBun(Transform transform)
{
	Actor* actor = CreateActor<Actor>();
	auto mesh_component = actor->AddComponent<MeshRenderer>();
	actor->SetTransform(transform);
	mesh_component->SetMeshProxy(burgerMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(burgerTexSRV);
	mesh_component->SetNormalSRV(burgerNormalSRV);
	mesh_component->SetSpecularSRV(burgerSpecSRV);

	return actor;
}

auto Sandbox::CreateHierarcyTestActor() -> Actor*
{
	Actor* actor = CreateActor<Actor>();
	actor->AddComponent<SceneComponent>();
	auto mesh_component = actor->AddComponent<MeshRenderer>();

	Transform tr;
	tr.Scale = Vector3::One * 0.4f;
	actor->SetTransform(tr);
	mesh_component->SetMeshProxy(burgerMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(burgerTexSRV);
	mesh_component->SetNormalSRV(burgerNormalSRV);
	mesh_component->SetSpecularSRV(burgerSpecSRV);

	SceneComponent* sceneComp = actor->AddComponent<SceneComponent>();

	mesh_component = actor->AddComponent<MeshRenderer>();
	mesh_component->SetAttachmentParent(sceneComp);
	tr.Scale = Vector3::One * 0.015f;
	tr.Position = { 0, 3.463, 0 };
	mesh_component->SetRelativeTransform(tr);
	mesh_component->SetMeshProxy(bunnyMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	/*actor->AddComponent<Component>();
	actor->AddComponent<Component>();*/

	return nullptr;
}

void Sandbox::LoadGameFacade() {
	const auto mLoader_Boot = mono->GetMethod("Scripts.Internal", "Loader", "Boot()");
	csGameInstance = mono->InvokeMethod(mLoader_Boot, nullptr, nullptr, nullptr);
	
	//TODO
	/*const auto mGame_OnSpecifyEngineSettings = mono->GetVirtualMethod("Scripts", "Game", "OnSpecifyEngineSettings(Scripts.EngineSettings.Builder)", csGameInstance);
	mono->InvokeMethod(mGame_OnSpecifyEngineSettings, csGameInstance, nullptr, nullptr);*/

	const auto mGame_Load = mono->GetVirtualMethod("Scripts", "Game", "OnLoad()", csGameInstance);
	mono->InvokeMethod(mGame_Load, csGameInstance, nullptr, nullptr);
}

Actor* Sandbox::CreateStaticSphere(Transform transform)
{
	Actor* sphere = CreateActor<Actor>();
	auto sphere_rb = sphere->AddComponent<RigidBodySphere>();
	sphere->SetTransform(transform);
	sphere_rb->SetMass(0);
	sphere_rb->Init();
	auto mesh_component = sphere->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(sphereMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return sphere;
}

Actor* Sandbox::CreateDynamicSphere(Transform transform)
{
	Actor* sphere = CreateActor<Actor>();
	auto sphere_rb = sphere->AddComponent<RigidBodySphere>();
	sphere->SetTransform(transform);
	sphere_rb->SetMass(1);
	sphere_rb->Init();
	auto mesh_component = sphere->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(sphereMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return sphere;
}

void Sandbox::PrepareResources()
{
	Game::PrepareResources();


	mono = MonoSystem::GetInstance();
	// create meshes
	boxMesh = new BoxMesh();
	boxMeshProxy = boxMesh->CreateMeshProxy();
	circleMesh = new CircleMesh();
	circleMeshProxy = circleMesh->CreateMeshProxy();
	sphereMesh = new SphereMesh();
	sphereMeshProxy = sphereMesh->CreateMeshProxy();

	LoadGameFacade();

	/*auto obj = new ScriptObject("Cargo", mono);
	auto m = obj->GetTransform();
	std::cout << m.Position.x;
	obj->Update(2.0f);
	std::cout << m.Position.x;
	auto m2 = obj->GetTransform();*/

	// create shaders
	ShaderCompiler sc;
	sc.SetIsDebug(true);
	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	sc.SetPathToShader(L"../Shaders/TexturedShader.hlsl");

	vs = sc.CreateShader<TexturedVertexShader>();

	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");

	ps = sc.CreateShader<PixelShader>();

	sc.SetPathToShader(L"../Shaders/MyVeryFirstShader.hlsl");
	sc.SetEntryPoint("PSPlainColor");
	psPlain = sc.CreateShader<PixelShader>();

	sc.SetPathToShader(L"../Shaders/BasicShader.hlsl");
	sc.SetEntryPoint("PSMain");
	PixelShader* basicPS = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	BasicVertexShader* basicVS = sc.CreateShader<BasicVertexShader>();

	{
		MeshLoader ml = MeshLoader("../Assets/stanford-bunny.fbx");
		TexturedMesh mesh = ml.GetMesh(0);
		bunnyMeshProxy = mesh.CreateMeshProxy();
	}

	{
		MeshLoader ml = MeshLoader("../Assets/box.fbx");
		TexturedMesh mesh = ml.GetMesh(0);
		texturedBoxMeshProxy = mesh.CreateMeshProxy();

		ml.OpenFile("../Assets/tjciddjqx_LOD0.fbx");
		TexturedMesh burgerMesh = ml.GetMesh(0);
		burgerMeshProxy = burgerMesh.CreateMeshProxy();
	}

	CreateWICTextureFromFile(GetD3DDevice().Get(), L"../Assets/tjciddjqx_2K_Albedo.jpg", burgerTexResource.GetAddressOf(), burgerTexSRV.GetAddressOf());
	CreateNormalMapTextureFromFile(L"../Assets/tjciddjqx_2K_Normal_LOD0.jpg", burgerNormal.GetAddressOf(), burgerNormalSRV.GetAddressOf());
	CreateNormalMapTextureFromFile(L"../Assets/tjciddjqx_2K_Roughness.jpg", burgerSpecular.GetAddressOf(), burgerSpecSRV.GetAddressOf());


	// Setup PerspCamera
	PerspCamera = new Camera();
	CurrentCamera = PerspCamera;
	PerspCamera->UpdateProjectionMatrixPerspective(45.0f, Display->GetAspectRatio(), 0.01f, 1000.0f);
	PerspCamera->Transform.Position = {-3.0f, 12.0f, 9.0f};
	

	OrthoCamera = new Camera();
	const float height = 16.0f;
	OrthoCamera->UpdateProjectionMatrixOrthographic(Display->GetAspectRatio() * height, height, -0.1f, -10.0f);
	OrthoCamera->Transform.Position = Vector3(0.0f, 5.0f, -4.0f);
	OrthoCamera->Transform.Rotation.SetEulerAngles(90.0f, 0.0f, 0.0f);

	Vector4 vec(0.0f, 0.0f, 1.0f, 1.0f);
	vec = Vector4::Transform(vec, OrthoCamera->GetProjectionMatrixTransposed());

	Transform tr;
	tr.Position = Vector3(5, -0.5, 0);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(20, 0.5, 20);
	platform = CreateStaticBox(tr);
	platform->UsePhysicsSimulation();

	//CreateSphereObject(3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1, 1, 1);
	tr.Position = Vector3(3.0f, 0.8f, 0.0f);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(1, 1, 1);
	sphere = CreateDynamicSphere(tr);
	sphere->UsePhysicsSimulation();
	sphere->GetComponentOfClass<RigidBodyComponent>()->MakeKinematic();

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			for (int k = 0; k < 1; ++k)
			{
				tr.Position = Vector3(i, 40.0f + j, k);
				tr.Rotation.SetEulerAngles(45.0f, 45.0f, 0.0f);
				tr.Scale = Vector3(1.0f, 1.0f, 1.0f);
				auto box = CreateDynamicBox(tr);
				box->UsePhysicsSimulation();
			}

	FPSCC = CreateGameComponent<CameraController>();
	FPSCC->SetCameraToControl(PerspCamera);
	FPSCC->SetPitchYaw(-45.0f, -45.0f);

	OrbitCC = CreateGameComponent<OrbitCameraController>();
	OrbitCC->SCToOrbit = sphere->GetRoot();
	OrbitCC->OrbitRadius = 10.0f;


	//PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	//pc->SetPixelShader(basicPS);
	//pc->SetVertexShader(basicVS);

	LightCam.Transform.Rotation = Vector3(-70.0f, 0.0f, 0.0f);
	LightCam.Transform.Position = Vector3(0.0f, 20.0f, 0.0f);
	LightCam.UpdateProjectionMatrixOrthographic(40.0f, 40.0f, 0.0f, 100.0f);
	DirectiLight.direction = LightCam.Transform.Rotation.GetForwardVector();

	AmbientLight* al = new AmbientLight();
	MyRenderingSystem->RegisterLight(al);
	QuadRenderer* qr = new QuadRenderer();
	qr->SetVertexShader(vs);
	qr->SetPixelShader(ps);
	al->SetRenderer(qr);

	DirectionalLight* dr = new DirectionalLight();
	MyRenderingSystem->RegisterLight(dr);

	dr->SetRenderer(qr);

	//Create simple static box for testing
	tr.Position = Vector3(16, 3, 0);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(4.0, 4.0, 20.0);
	staticBox = CreateNonPhysicsBox(tr);
	staticBox->SetTransform(tr);
	
	tr.Scale = Vector3::One * 0.015f;
	tr.Position = { 11.638, 3.463, -7.674 };
	CreateBunny(tr);

	EngineContentRegistry::GetInstance()->CreatePointLight(tr);

	tr.Scale = Vector3::One * 0.4f;
	tr.Position = { 11.436f, 0.843f, -2.502f };
	CreateBun(tr);

	CreateHierarcyTestActor();
}

auto Sandbox::Initialize() -> void
{
	// copy imgui context and allocator functions to imgui in c#
	const auto mGame_InitImGui = mono->GetMethod("Scripts", "Game", "InitImGui");
	ImGuiContext* ctx = ImGui::GetCurrentContext();
	ImGuiMemAllocFunc allocFunc;
	ImGuiMemFreeFunc freeFunc;
	void* userData;
	ImGui::GetAllocatorFunctions(&allocFunc, &freeFunc, &userData);
	void* params[] = { &ctx, &allocFunc, &freeFunc, &userData };
	MonoObject* exception;
	mono->InvokeMethod(mGame_InitImGui, csGameInstance, params, nullptr);
}

void Sandbox::Update(float DeltaTime)
{
	const auto mGame_OnGUI = mono->GetVirtualMethod("Scripts", "Game", "OnGUI", csGameInstance);
	mono->InvokeMethod(mGame_OnGUI, csGameInstance, nullptr, nullptr);

	/*const auto mGame_GetInheritors = mono->GetVirtualMethod("Scripts", "Game", "GetActorInheritors()", csGameInstance);
	auto res = mono->InvokeMethod(mGame_GetInheritors, csGameInstance, nullptr, nullptr);
	auto str = mono_string_to_utf8(mono_object_to_string(res, nullptr));*/
	
	const auto mGame_Update = mono->GetVirtualMethod("Scripts", "Game", "OnUpdate()", csGameInstance);
	mono->InvokeMethod(mGame_Update, csGameInstance, nullptr, nullptr);

	// TODO: base game class should do this
	if (GetPlayState() == PlayState::Playing)
	{
		// Physics Simulation
		auto physics = PhysicsModuleData::GetInstance();
		physics->OnUpdate(DeltaTime);
		for (auto actor : Actors)
		{
			actor->Update(DeltaTime);
		}
	}

	InputDevice& input = *Game::GetInstance()->GetInputDevice();

	static float pressTime = GetTotalElapsedTime();

	constexpr float cooldown = 0.2f;

	auto keyboard = input.GetKeyboard();

	if (GetTotalElapsedTime() - pressTime > cooldown)
	{
		if (keyboard->IsDown(KEY_O))
		{
			// switch PerspCamera controller
			if (CurrentCC == FPSCC)
			{
				CurrentCC = OrbitCC;
				FPSCC->SetCameraToControl(nullptr);
				OrbitCC->SetCameraToControl(PerspCamera);
				FPSCC->bShouldUpdate = false;
				OrbitCC->bShouldUpdate = true;
			}
			else
			{
				CurrentCC = FPSCC;
				OrbitCC->SetCameraToControl(nullptr);
				FPSCC->SetCameraToControl(PerspCamera);
				Vector3 euler = PerspCamera->Transform.Rotation.GetEulerDegrees();
				FPSCC->SetPitchYaw(euler.x, euler.y);
				FPSCC->bShouldUpdate = true;
				OrbitCC->bShouldUpdate = false;

			}
			pressTime = GetTotalElapsedTime();
		}
		if (keyboard->IsDown(KEY_ONE))
		{
			sphere->UnUsePhysicsSimulation();
		}		
		if (input.GetKeyboard()->IsDown(KEY_TWO))
		{
			sphere->UsePhysicsSimulation();
		}
	}

	if (keyboard->IsDown(KEY_T))
	{
		CurrentCamera = OrthoCamera;
	}
	else
	{
		CurrentCamera = PerspCamera;
	}
}

auto Sandbox::OnBeginPlay() -> void {
	CurrentCC = OrbitCC;
	FPSCC->SetCameraToControl(nullptr);
	OrbitCC->SetCameraToControl(PerspCamera);
	FPSCC->bShouldUpdate = false;
	OrbitCC->bShouldUpdate = true;

	for (auto actor : Actors)
	{
		actor->OnBeginPlay();
	}
}

