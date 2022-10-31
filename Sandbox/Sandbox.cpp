#include "Sandbox.h"

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

#include "CreateCommon.h"

Actor* Sandbox::CreateNonPhysicsBox(Transform transform) {
	Actor* box = CreateActor<Actor>();
	
	auto mesh_component = box->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(boxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);


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
	mesh_component->SetMeshProxy(boxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);

	return box;
}

Actor* Sandbox::CreateDynamicBox(Transform transform)
{
	Actor* box = CreateActor<Actor>();
	auto box_rb = box->AddComponent<RigidBodyCube>();
	box->SetTransform(transform);
	box_rb->SetMass(1);
	box_rb->Init();
	auto mesh_component = box->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(boxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);

	return box;
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

	return sphere;
}

void Sandbox::PrepareResources()
{
	Game::PrepareResources();
	mono = new MonoSystem();
	// create meshes
	boxMesh = new BoxMesh();
	boxMeshProxy = boxMesh->CreateMeshProxy();
	circleMesh = new CircleMesh();
	circleMeshProxy = circleMesh->CreateMeshProxy();
	sphereMesh = new SphereMesh();
	sphereMeshProxy = sphereMesh->CreateMeshProxy();

	auto obj = new ScriptObject("Cargo", mono);
	auto m = obj->GetTransform();
	std::cout << m.Position.x;
	obj->Update(2.0f);
	std::cout << m.Position.x;
	auto m2 = obj->GetTransform();

	// create shaders
	ShaderCompiler sc;
	sc.SetIsDebug(true);
	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	sc.SetPathToShader(L"../Shaders/MyVeryFirstShader.hlsl");

	vs = sc.CreateShader<SimpleVertexShader>();

	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");

	ps = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("PSPlainColor");
	psPlain = sc.CreateShader<PixelShader>();

	sc.SetPathToShader(L"../Shaders/BasicShader.hlsl");
	sc.SetEntryPoint("PSMain");
	PixelShader* basicPS = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	BasicVertexShader* basicVS = sc.CreateShader<BasicVertexShader>();

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
	vec = Vector4::Transform(vec, OrthoCamera->GetProjectionMatrix());

	Transform tr;
	tr.Position = Vector3(5, -0.5, 0);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(10, 0.5, 10);
	platform = CreateStaticBox(tr);

	//CreateSphereObject(3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1, 1, 1);
	tr.Position = Vector3(3.0f, 10.0f, 0.0f);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(1, 1, 1);
	auto sphere = CreateDynamicSphere(tr);
	sphere->UsePhysicsSimulation();

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			for (int k = 0; k < 10; ++k)
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


	//PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	//pc->SetPixelShader(basicPS);
	//pc->SetVertexShader(basicVS);


	//Create simple static box for testing
	tr.Position = Vector3(16, 3, 0);
	tr.Rotation.SetEulerAngles(0, 0, 0);
	tr.Scale = Vector3(2.0, 2.0, 10.0);
	staticBox = CreateNonPhysicsBox(tr);
	staticBox->SetTransform(tr);
	
}

void Sandbox::Update(float DeltaTime)
{
	//const auto mGame_Update = mono->GetVirtualMethod("Scripts", "Game", "OnUpdate()", csGameInstance);
	//mono->InvokeMethod(mGame_Update, csGameInstance, nullptr, nullptr);

	// Physics Simulation
	auto physics = PhysicsModuleData::GetInstance();
	physics->OnUpdate(DeltaTime);

	// TODO: base game class should do this
	for (auto actor : Actors)
	{
		actor->Update(DeltaTime);
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
			platform->UnUsePhysicsSimulation();
		}		
		if (input.GetKeyboard()->IsDown(KEY_TWO))
		{
			platform->UsePhysicsSimulation();
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


