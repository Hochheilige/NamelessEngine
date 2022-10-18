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
#include "Mappings.h"
#include "AudioComponent.h"
#include "windows.h"
#include "mono/metadata/debug-helpers.h"

MeshRenderer* Sandbox::CreateObject(float trans_x, float trans_y, float trans_z,
	float rot_x, float rot_y, float rot_z,
	float scale_x, float scale_y, float scale_z,
	const char* mesh_path)
{
	MeshLoader ml = MeshLoader(mesh_path);
	TexturedMesh mesh = ml.GetMesh(0);
	MeshProxy* meshProxy = mesh.CreateMeshProxy();

	MeshRenderer* object = Game::GetInstance()->CreateGameComponent<MeshRenderer>();
	object->SetMeshProxy(meshProxy);
	object->SetPixelShader(ps);
	object->SetVertexShader(vs);
	object->mTransform.Position = Vector3(trans_x, trans_y, trans_z);
	object->mTransform.Rotation.SetEulerAngles(rot_x, rot_y, rot_z);
	object->mTransform.Scale = Vector3(scale_x, scale_y, scale_z);

	return object;
}

MeshRenderer* Sandbox::CreateCubeObject(float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z, float scale_x, float scale_y, float scale_z)
{
	boxMesh = new BoxMesh();
	boxMeshProxy = boxMesh->CreateMeshProxy();

	MeshRenderer* object = Game::GetInstance()->CreateGameComponent<MeshRenderer>();
	object->SetMeshProxy(boxMeshProxy);
	object->SetPixelShader(ps);
	object->SetVertexShader(vs);
	object->mTransform.Position = Vector3(trans_x, trans_y, trans_z);
	object->mTransform.Rotation.SetEulerAngles(rot_x, rot_y, rot_z);
	object->mTransform.Scale = Vector3(scale_x, scale_y, scale_z);

	return object;
}

MeshRenderer* Sandbox::CreateSphereObject(float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z, float scale_x, float scale_y, float scale_z)
{
	sphereMesh = new SphereMesh();
	sphereMeshProxy = sphereMesh->CreateMeshProxy();

	MeshRenderer* object = Game::GetInstance()->CreateGameComponent<MeshRenderer>();
	object->SetMeshProxy(sphereMeshProxy);
	object->SetPixelShader(ps);
	object->SetVertexShader(vs);
	object->mTransform.Position = Vector3(trans_x, trans_y, trans_z);
	object->mTransform.Rotation.SetEulerAngles(rot_x, rot_y, rot_z);
	object->mTransform.Scale = Vector3(scale_x, scale_y, scale_z);

	return object;
}

void Sandbox::LoadGameFacade() {
	auto method = mono->GetMethod("Scripts.Internal", "Loader", "Boot()");
	mono->InvokeMethod(method, nullptr, nullptr, nullptr);
	//TODO uncomment 
}

void Sandbox::PrepareResources()
{
	Game::PrepareResources();
	// create meshes
	mono = new MonoSystem();

	LoadGameFacade();

	boxMesh = new BoxMesh();
	boxMeshProxy = boxMesh->CreateMeshProxy();
	circleMesh = new CircleMesh();
	circleMeshProxy = circleMesh->CreateMeshProxy();
	sphereMesh = new SphereMesh();
	sphereMeshProxy = sphereMesh->CreateMeshProxy();

	auto audioComponent = new AudioComponent(mono->GetImage());
	audioComponent->OnRegister();

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

	CreateSphereObject(3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 30.0f, 1, 1, 1);

	//CreateCubeObject(2.0f, 2.0f, 0.0f, 45.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	/*const char* bunny_path = "..\\Assets\\stanford-bunny.fbx";
	CreateObject(0, 1, 0, 0, 0, 0, 0.005, 0.005, 0.005, bunny_path);*/

	FPSCC = CreateGameComponent<CameraController>();
	FPSCC->SetCameraToControl(PerspCamera);


	PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	pc->SetPixelShader(basicPS);
	pc->SetVertexShader(basicVS);

}

void Sandbox::Update(float DeltaTime)
{
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
		if (input.IsKeyDown(73))
		{
			const char* bunny_path = "..\\Assets\\stanford-bunny.fbx";
			CreateObject(0, 1, 0, 0, 0, 0, 0.005, 0.005, 0.005, bunny_path);
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


