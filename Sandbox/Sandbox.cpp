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


Actor* Sandbox::CreateStaticBox(Transform transform)
{
	Actor* box = new Actor(transform);
	auto mesh_component = box->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(boxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	auto box_rb = box->AddComponent<RigidBodyCube>();
	box_rb->SetMass(0);
	box_rb->Init();

	return box;
}

Actor* Sandbox::CreateDynamicBox(Transform transform)
{
	Actor* box = new Actor(transform);
	auto mesh_component = box->AddComponent<MeshRenderer>();
	mesh_component->SetMeshProxy(boxMeshProxy);
	mesh_component->SetPixelShader(ps);
	mesh_component->SetVertexShader(vs);
	auto box_rb = box->AddComponent<RigidBodyCube>();
	box_rb->SetMass(1);
	box_rb->Init();

	return box;
}

Actor* Sandbox::CreateSphere(Transform transform)
{
	return nullptr;
}

void Sandbox::PrepareResources()
{
	Game::PrepareResources();
	// create meshes
	boxMesh = new BoxMesh();
	boxMeshProxy = boxMesh->CreateMeshProxy();
	circleMesh = new CircleMesh();
	circleMeshProxy = circleMesh->CreateMeshProxy();
	sphereMesh = new SphereMesh();
	sphereMeshProxy = sphereMesh->CreateMeshProxy();


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
	PerspCamera->Transform.Position = PerspCamera->Transform.Rotation.GetForwardVector() * -5.0f;

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
	auto platform = CreateStaticBox(tr);
	Actors.push_back(platform);

	//CreateSphereObject(3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1, 1, 1);

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			for (int k = 0; k < 10; ++k)
			{
				tr.Position = Vector3(i, 40.0f + j, k);
				tr.Rotation.SetEulerAngles(45.0f, 45.0f, 0.0f);
				tr.Scale = Vector3(1.0f, 1.0f, 1.0f);
				auto box = CreateDynamicBox(tr);
				box->UsePhysicsSimulation();
				Actors.push_back(box);
			}

	FPSCC = CreateGameComponent<CameraController>();
	FPSCC->SetCameraToControl(PerspCamera);


	//PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	//pc->SetPixelShader(basicPS);
	//pc->SetVertexShader(basicVS);

}

void Sandbox::Update(float DeltaTime)
{
	// Physics Simulation
	auto physics = PhysicsModuleData::GetInstance();
	physics->OnUpdate(DeltaTime);

	for (auto actor : Actors)
	{
		actor->Update(DeltaTime);
	}

	InputDevice& input = *Game::GetInstance()->GetInputDevice();

	static float pressTime = GetTotalElapsedTime();

	constexpr float cooldown = 0.2f;

	if (GetTotalElapsedTime() - pressTime > cooldown)
	{
		if (input.IsKeyDown(79))
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
		if (input.IsKeyDown(49))
		{
			Actors[0]->UnUsePhysicsSimulation();
		}		
		if (input.IsKeyDown(50))
		{
			Actors[0]->UsePhysicsSimulation();
		}
	}

	if (input.IsKeyDown(84))
	{
		CurrentCamera = OrthoCamera;
	}
	else
	{
		CurrentCamera = PerspCamera;
	}
}




