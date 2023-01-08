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
#include "RigidBodyComponent.h"
#include "windows.h"
#include "RenderingSystem.h"
#include "LightBase.h"
#include "EngineContentRegistry.h"
#include "AudioComponent.h"
#include "RigidBodyComponent.h"
#include "Component.h"
#include "ImGuiInclude.h"
#include "StaticMeshRenderer.h"
#include "AssetManager.h"


#include "CreateCommon.h"

Actor* Sandbox::CreateNonPhysicsBox(Transform transform) {
	Actor* box = CreateActor<Actor>();
	Path path = "../Assets/box.fbx/Cube";
	box->InitializeMonoActor("Scripts.Tests", "CustomActor");
	auto mesh_component = box->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetTexturePath("../Assets/white.png");
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());
	mesh_component->SetTransform(transform);

	return box;
}

Actor* Sandbox::CreatePlayahActor(Transform transform) {
	Actor* box = CreateActor<Actor>();
	Path path = "../Assets/box.fbx/Cube";
	box->InitializeMonoActor("Scripts.Tests", "PlayahActor");
	auto mesh_component = box->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetTexturePath("../Assets/white.png");
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());
	box->GetRoot()->SetTransform(transform);

	return box;
}

Actor* Sandbox::CreateStaticBox(Transform transform)
{
	Actor* box = CreateActor<Actor>();
	auto box_rb = box->AddComponent<RigidBodyComponent>();
	box->SetTransform(transform);
	box_rb->SetRigidBodyType(RigidBodyType::STATIC);
	box_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
	box_rb->SetCollisionShapeType(CollisionShapeType::BOX);
	box_rb->SetMass(0);
	box_rb->Init();
	box_rb->EnablePhysicsSimulation();
	Path path = "../Assets/box.fbx/Cube";
	auto mesh_component = box->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	auto audio = box->AddComponent<AudioComponent>();
	audio->Init();
	audio->LoadSound("../Assets/test.wav", true, true);
	audio->SetVolume(0.1);
	//audio->Play("../Assets/test.wav");

	return box;
}

Actor* Sandbox::CreateDynamicBox(Transform transform)
{
	Actor* box = CreateActor<Actor>();
	//box->InitializeMonoActor("CustomActor");
	auto box_rb = box->AddComponent<RigidBodyComponent>();
	box->SetTransform(transform);
	box_rb->SetRigidBodyType(RigidBodyType::DYNAMIC);
	box_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
	box_rb->SetCollisionShapeType(CollisionShapeType::BOX);
	box_rb->SetMass(0.1f);
	box_rb->Init();
	box_rb->EnablePhysicsSimulation();
	Path path = "../Assets/box.fbx/Cube";
	auto mesh_component = box->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return box;
}

Actor* Sandbox::CreateKinematicBox(Transform transform)
{
	Actor* box = CreateActor<Actor>();
	auto box_rb = box->AddComponent<RigidBodyComponent>();
	box->SetTransform(transform);
	box_rb->SetRigidBodyType(RigidBodyType::KINEMATIC);
	box_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
	box_rb->SetCollisionShapeType(CollisionShapeType::BOX);
	box_rb->SetMass(0.1f);
	box_rb->Init();
	box_rb->EnablePhysicsSimulation();
	Path path = "../Assets/box.fbx/Cube";
	auto mesh_component = box->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return box;
}

Actor* Sandbox::CreateStaticSphere(Transform transform)
{
	Actor* sphere = CreateActor<Actor>();
	auto sphere_rb = sphere->AddComponent<RigidBodyComponent>();
	sphere->SetTransform(transform);
	sphere_rb->SetRigidBodyType(RigidBodyType::STATIC);	
	sphere_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
	sphere_rb->SetCollisionShapeType(CollisionShapeType::SPHERE);
	sphere_rb->SetMass(0);
	sphere_rb->Init();
	sphere_rb->EnablePhysicsSimulation();
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
	auto sphere_rb = sphere->AddComponent<RigidBodyComponent>();
	sphere->SetTransform(transform);
	sphere_rb->SetRigidBodyType(RigidBodyType::DYNAMIC);
	sphere_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS_AND_PHYSICS);
	sphere_rb->SetCollisionShapeType(CollisionShapeType::SPHERE);
	sphere_rb->SetMass(1);
	sphere_rb->Init();
	sphere_rb->EnablePhysicsSimulation();
	auto mesh_component = sphere->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(sphereMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return sphere;
}

Actor* Sandbox::CreateKinematicSphere(Transform transform)
{
	Actor* sphere = CreateActor<Actor>();
	//sphere->InitializeMonoActor("Scripts.Tests", "CustomActor");
	auto sphere_rb = sphere->AddComponent<RigidBodyComponent>();
	sphere->SetTransform(transform);
	sphere_rb->SetRigidBodyType(RigidBodyType::KINEMATIC);
	sphere_rb->SetRigidBodyUsage(RigidBodyUsage::COLLISIONS);
	sphere_rb->SetCollisionShapeType(CollisionShapeType::SPHERE);
	sphere_rb->SetMass(1);
	sphere_rb->Init();
	//sphere_rb->EnablePhysicsSimulation();
	auto mesh_component = sphere->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(sphereMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	auto audio = sphere->AddComponent<AudioComponent>();
	audio->LoadSound("../Assets/fall.ogg");
	audio->SetVolume(0.5);

	return sphere;
}

Actor* Sandbox::CreateBunny(Transform transform)
{
	Actor* bunny = CreateActor<Actor>();
	Path path = "../Assets/stanford-bunny.fbx/Stanford Bunny-mesh";
	auto mesh_component = bunny->AddComponent<StaticMeshRenderer>();
	transform.Scale = Vector3::One * 0.015f;
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	return bunny;
}

Actor* Sandbox::CreateBun(Transform transform)
{
	Path path = "../Assets/tjciddjqx_LOD0.fbx/Aset_edible_baked_goods_S_tjciddjqx_LOD0";
	Actor* actor = CreateActor<Actor>();
	auto mesh_component = actor->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	actor->SetTransform(transform);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetTexturePath("../Assets/tjciddjqx_2K_Albedo.jpg");
	mesh_component->SetNormalPath("../Assets/tjciddjqx_2K_Normal_LOD0.jpg");
	mesh_component->SetSpecularSRV(burgerSpecSRV);

	return actor;
}

auto Sandbox::CreateHierarcyTestActor() -> Actor*
{
	Actor* actor = CreateActor<Actor>();
	actor->AddComponent<SceneComponent>();
	Path path = "../Assets/tjciddjqx_LOD0.fbx/Aset_edible_baked_goods_S_tjciddjqx_LOD0";
	

	Transform tr;
	tr.Scale = Vector3::One * 0.4f;
	actor->SetTransform(tr);
	auto mesh_component = actor->AddComponent<StaticMeshRenderer>();
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetTexturePath("../Assets/tjciddjqx_2K_Albedo.jpg");
	mesh_component->SetNormalPath("../Assets/tjciddjqx_2K_Normal_LOD0.jpg");
	mesh_component->SetSpecularSRV(burgerSpecSRV);

	SceneComponent* sceneComp = actor->AddComponent<SceneComponent>();

	path = "../Assets/stanford-bunny.fbx/Stanford Bunny-mesh";
	mesh_component = actor->AddComponent<StaticMeshRenderer>();
	mesh_component->SetAttachmentParent(sceneComp);
	tr.Scale = Vector3::One * 0.015f;
	tr.Position = { 0, 3.463, 0 };
	mesh_component->SetRelativeTransform(tr);
	mesh_component->SetStaticMesh(GetAssetManager()->LoadStaticMesh(path));
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	mesh_component->SetAlbedoSRV(EngineContentRegistry::GetInstance()->GetWhiteTexSRV());
	mesh_component->SetNormalSRV(EngineContentRegistry::GetInstance()->GetBasicNormalTexSRV());

	/*actor->AddComponent<Component>();
	actor->AddComponent<Component>();*/

	return nullptr;
}

void Sandbox::LoadGameFacade() {
	auto mono = MonoSystem::GetInstance();
	const auto mLoader_Boot = mono->GetMethod("Scripts.Internal", "Loader", "Boot()");
	csGameInstance = mono_gchandle_new(mono->InvokeStaticMethod(mLoader_Boot, nullptr, nullptr), TRUE);
	
	//TODO
	/*const auto mGame_OnSpecifyEngineSettings = mono->GetVirtualMethod("Scripts", "Game", "OnSpecifyEngineSettings(Scripts.EngineSettings.Builder)", csGameInstance);
	mono->InvokeMethod(mGame_OnSpecifyEngineSettings, csGameInstance, nullptr, nullptr);*/

	const auto mGame_Load = mono->GetVirtualMethod("Scripts", "Game", "OnLoad()", csGameInstance);
	mono->InvokeInstanceMethod(mGame_Load, csGameInstance, nullptr, nullptr);
}

void Sandbox::PrepareResources()
{
	Game::PrepareResources();

	// create meshes
	boxMesh = new BoxMesh();
	boxMeshProxy = boxMesh->CreateRenderingPrimitiveProxy();
	circleMesh = new CircleMesh();
	circleMeshProxy = circleMesh->CreateRenderingPrimitiveProxy();
	sphereMesh = new SphereMesh();
	sphereMeshProxy = sphereMesh->CreateRenderingPrimitiveProxy();

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
		bunnyMeshProxy = mesh.CreateRenderingPrimitiveProxy();
	}

	{
		MeshLoader ml = MeshLoader("../Assets/box.fbx");
		TexturedMesh mesh = ml.GetMesh(0);
		texturedBoxMeshProxy = mesh.CreateRenderingPrimitiveProxy();

		ml.OpenFile("../Assets/tjciddjqx_LOD0.fbx");
		TexturedMesh burgerMesh = ml.GetMesh(0);
		burgerMeshProxy = burgerMesh.CreateRenderingPrimitiveProxy();
	}

	CreateWICTextureFromFile(GetD3DDevice().Get(), L"../Assets/tjciddjqx_2K_Albedo.jpg", burgerTexResource.GetAddressOf(), burgerTexSRV.GetAddressOf());
	CreateNormalMapTextureFromFile(L"../Assets/tjciddjqx_2K_Normal_LOD0.jpg", burgerNormal.GetAddressOf(), burgerNormalSRV.GetAddressOf());
	CreateNormalMapTextureFromFile(L"../Assets/tjciddjqx_2K_Roughness.jpg", burgerSpecular.GetAddressOf(), burgerSpecSRV.GetAddressOf());


	// Setup editor camera
	EditorPOV.UpdateProjectionMatrixPerspective(45.0f, Display->GetAspectRatio(), 0.01f, 1000.0f);
	EditorPOV.Transform.Position = {-3.0f, 12.0f, 9.0f};
	FPSCC = CreateGameComponent<CameraController>();
	FPSCC->SetCameraToControl(&EditorPOV);
	FPSCC->SetPitchYaw(-45.0f, -45.0f);
	

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

	//CreateSphereObject(3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1, 1, 1);
	tr.Position = Vector3(0.0f, 0.8f, 0.0f);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(1, 1, 1);
	
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			for (int k = 0; k < 1; ++k)
			{
				tr.Position = Vector3(i, 40.0f + j, k);
				tr.Rotation.SetEulerAngles(45.0f, 45.0f, 0.0f);
				tr.Scale = Vector3(1.0f, 1.0f, 1.0f);
				auto box = CreateDynamicBox(tr);
			}

	



	/*OrbitCC = CreateGameComponent<OrbitCameraController>();
	OrbitCC->SCToOrbit = sphere->GetRoot();
	OrbitCC->OrbitRadius = 10.0f;*/


	//PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	//pc->SetPixelShader(basicPS);
	//pc->SetVertexShader(basicVS);





	/*QuadRenderer* qr = new QuadRenderer();
	qr->SetVertexShader(vs);
	qr->SetPixelShader(ps);
	al->SetRenderer(qr);*/


	//dr->SetRenderer(qr);

	//Create simple static box for testing
	tr.Position = Vector3(16, 3, 0);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(4.0, 4.0, 20.0);
	staticBox = CreateNonPhysicsBox(tr);
	//staticBox->SetTransform(tr);
	
	tr.Scale = Vector3::One * 0.015f;
	tr.Position = { 11.638, 3.463, -7.674 };

	EngineContentRegistry::GetInstance()->CreatePointLight(tr);

	tr.Scale = Vector3::One * 0.4f;
	tr.Position = { 11.436f, 0.843f, -2.502f };
	CreateBun(tr);

	//Playah actor
	tr.Position = Vector3(0, 3, 0);
	tr.Scale = { 1.0f, 1.0f, 1.0f};
	tr.Rotation.SetEulerAngles(0, 0, 0);
	Actor* playah = CreatePlayahActor(tr);

	CreateHierarcyTestActor();
}

auto Sandbox::Initialize() -> void
{
	auto mono = MonoSystem::GetInstance();
	// copy imgui context and allocator functions to imgui in c#
	const auto mGame_InitImGui = mono->GetMethod("Scripts", "Game", "InitImGui");
	ImGuiContext* ctx = ImGui::GetCurrentContext();
	ImGuiMemAllocFunc allocFunc;
	ImGuiMemFreeFunc freeFunc;
	void* userData;
	ImGui::GetAllocatorFunctions(&allocFunc, &freeFunc, &userData);
	void* params[] = { &ctx, &allocFunc, &freeFunc, &userData };
	MonoObject* exception;
	mono->InvokeInstanceMethod(mGame_InitImGui, csGameInstance, params, nullptr);
}

void Sandbox::Update(float DeltaTime)
{
	auto mono = MonoSystem::GetInstance();

	const auto mGame_OnGUI = mono->GetVirtualMethod("Scripts", "Game", "OnGUI", csGameInstance);
	mono->InvokeInstanceMethod(mGame_OnGUI, csGameInstance, nullptr, nullptr);

	/*const auto mGame_GetInheritors = mono->GetVirtualMethod("Scripts", "Game", "GetActorInheritors()", csGameInstance);
	auto res = mono->InvokeMethod(mGame_GetInheritors, csGameInstance, nullptr, nullptr);
	auto str = mono_string_to_utf8(mono_object_to_string(res, nullptr));*/
	
	const auto mGame_Update = mono->GetVirtualMethod("Scripts", "Game", "OnUpdate()", csGameInstance);
	mono->InvokeInstanceMethod(mGame_Update, csGameInstance, nullptr, nullptr);

	// TODO: base game class should do this
	if (GetPlayState() == PlayState::Playing)
	{
		// Temporary block just to check how sound works
		if (prevPlayState == PlayState::Editor || prevPlayState == PlayState::Paused)
		{
			// platform->GetComponentOfClass<AudioComponent>()->Play();
		}

		// Physics Simulation
		auto physics = PhysicsModuleData::GetInstance();
		physics->OnUpdate(DeltaTime);
		// create a copy of the Actors array to avoid iterator invalidation
		std::vector<Actor*> actors = Actors;
		for (auto actor : actors)
		{
			actor->Update(DeltaTime);
		}
	}
	else
	{
		auto audio = AudioModule::GetInstance();
		audio->StopAllChannels();
	}

	prevPlayState = GetPlayState();

	InputDevice& input = *Game::GetInstance()->GetInputDevice();

	static float pressTime = GetTotalElapsedTime();

	constexpr float cooldown = 0.2f;

	auto keyboard = input.GetKeyboard();

	if (GetTotalElapsedTime() - pressTime > cooldown)
	{
		if (keyboard->IsDown(KEY_O) && false) // temp prevent from using orbit camera
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
			//sphere->GetComponentOfClass<AudioComponent>()->Play();
		}		
		if (input.GetKeyboard()->IsDown(KEY_TWO))
		{
			//sphere->GetComponentOfClass<AudioComponent>()->StopChannel();
		}
	}
}

void Sandbox::StartPlay()
{
	Game::StartPlay();
	ChangeGameState(true);
}

void Sandbox::PausePlay()
{
	Game::PausePlay();
	ChangeGameState(false);
}

void Sandbox::StopPlay()
{
	Game::StopPlay();
	ChangeGameState(false);
}

void Sandbox::ResumePlay()
{
	Game::ResumePlay();
	ChangeGameState(true);
}

auto Sandbox::OnBeginPlay() -> void
{
	Game::OnBeginPlay();
	ChangeGameState(true);
	for (auto actor : Actors)
	{
		actor->OnBeginPlay();
	}
}

void Sandbox::ChangeGameState(bool isPlaying)
{
	auto mono = MonoSystem::GetInstance();
	void *args [1];
	bool result = isPlaying;
	args[0] = &result;
	const auto changeState = mono->GetVirtualMethod("Scripts", "Game", "ChangeState", csGameInstance);
	mono->InvokeInstanceMethod(changeState, csGameInstance, args, nullptr);
}

