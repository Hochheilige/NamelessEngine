#include "HW2Game.h"

#include "ShaderCompiler.h"
#include "Shader.h"
#include "Mesh.h"
#include "DisplayWin32.h"
#include "MeshRenderer.h"
#include "CameraController.h"
#include "OrbitCameraController.h"
#include "InputDevice.h"
#include "PlaneComponent.h"

void HW2Game::PrepareResources()
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
	sc.SetPathToShader(L"../../Shaders/MyVeryFirstShader.hlsl");

	vs = sc.CreateShader<SimpleVertexShader>();

	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");

	ps = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("PSPlainColor");
	psPlain = sc.CreateShader<PixelShader>();

	sc.SetPathToShader(L"../../Shaders/BasicShader.hlsl");
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


	MeshRenderer* box = CreateGameComponent<MeshRenderer>();
	box->SetMeshProxy(boxMeshProxy);
	box->SetVertexShader(vs);
	box->SetPixelShader(ps);
	box->mTransform.Rotation.SetEulerAngles(0.0f, 0.0f, 30.0f);
	box->mTransform.Position.x = 3.0f;

	MeshRenderer* box2 = CreateGameComponent<MeshRenderer>();
	box2->SetMeshProxy(boxMeshProxy);
	box2->SetVertexShader(vs);
	box2->SetPixelShader(ps);
	RotateAroundActorComponent* raac = box2->AddChildComponent<RotateAroundActorComponent>();
	raac->GCToRotateAround = box;
	raac->OrbitRadius = 3.0f;
	raac->Scale = Vector3::One * 0.5f;

	MeshRenderer* box3 = CreateGameComponent<MeshRenderer>();
	box3->SetMeshProxy(boxMeshProxy);
	box3->SetVertexShader(vs);
	box3->SetPixelShader(ps);
	raac = box3->AddChildComponent<RotateAroundActorComponent>();
	raac->GCToRotateAround = box2;
	raac->Scale = Vector3::One * 0.5f;

	MeshRenderer* box4 = CreateGameComponent<MeshRenderer>();
	box4->SetMeshProxy(boxMeshProxy);
	box4->SetVertexShader(vs);
	box4->SetPixelShader(ps);
	box4->mTransform.Rotation.SetEulerAngles(0.0f, 0.0f, -30.0f);
	box4->mTransform.Position.x = -3.0f;
	box4->AddChildComponent<RotatingComponent>();

	MeshRenderer* box5 = box4->AddChildComponent<MeshRenderer>();
	box5->SetMeshProxy(boxMeshProxy);
	box5->SetVertexShader(vs);
	box5->SetPixelShader(ps);
	box5->mTransform.Position.z = 2.0f;
	box5->mTransform.Scale = Vector3::One * 0.5f;
	box5->AddChildComponent<RotatingComponent>();

	MeshRenderer* box6 = box5->AddChildComponent<MeshRenderer>();
	box6->SetMeshProxy(boxMeshProxy);
	box6->SetVertexShader(vs);
	box6->SetPixelShader(ps);
	box6->mTransform.Position.z = 2.0f;
	box6->mTransform.Scale = Vector3::One * 0.5f;
	box6->AddChildComponent<RotatingComponent>();

	MeshRenderer* box7 = box6->AddChildComponent<MeshRenderer>();
	box7->SetMeshProxy(boxMeshProxy);
	box7->SetVertexShader(vs);
	box7->SetPixelShader(ps);
	box7->mTransform.Position.z = 2.0f;
	box7->mTransform.Scale = Vector3::One * 0.5f;
	box7->AddChildComponent<RotatingComponent>();


	// todo add debug circles for orbits

	MeshRenderer* sphere1 = CreateGameComponent<MeshRenderer>();
	sphere1->SetMeshProxy(sphereMeshProxy);
	sphere1->SetVertexShader(vs);
	sphere1->SetPixelShader(ps);
	sphere1->mTransform.Position.y = 2.0f;

	// Create sphere /*circle*/ grid
	const int numSpheres = 10;
	const float circleRadius = (numSpheres - 1) / 2.0f;
	const float circleRadiusSq = circleRadius * circleRadius;
	const float verDist = 2.5f;
	const float horDist = 2.5f;
	GameComponent* grid = CreateGameComponent<GameComponent>();
	for (int i = 0; i < numSpheres; ++i)
	{
		for (int j = 0; j < numSpheres; ++j)
		{
			/*if (Vector2(i - circleRadius, j - circleRadius).LengthSquared() >= circleRadiusSq)
			{
				continue;
			}*/

			GameComponent* gc = grid->AddChildComponent<GameComponent>();
			gc->mTransform.Position = Vector3(i * horDist, 0.0f, -j * verDist);

			MeshRenderer* sphere = gc->AddChildComponent<MeshRenderer>();
			sphere->SetMeshProxy(sphereMeshProxy);
			sphere->SetVertexShader(vs);
			sphere->SetPixelShader(psPlain);
			const Color sphereColor = Color(0.0f, static_cast<float>((i + 1)) / numSpheres, static_cast<float>((j + 1)) / numSpheres, 1.0f);
			sphere->SetColor(sphereColor);

			FloatingComponent* fc = sphere->AddChildComponent<FloatingComponent>();
			fc->Phase = i * numSpheres + j;
		}
	}
	grid->mTransform.Scale = Vector3::One * 0.2f;
	grid->mTransform.Position = Vector3(0.0f, 0.0f, -5.0f);
	

	FPSCC = CreateGameComponent<CameraController>();
	FPSCC->SetCameraToControl(PerspCamera);

	OrbitCC = CreateGameComponent<OrbitCameraController>();
	//orbitCC->SetCameraToControl(PerspCamera);
	OrbitCC->GCToOrbit = sphere1;


	PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	pc->SetPixelShader(basicPS);
	pc->SetVertexShader(basicVS);
}

void HW2Game::Update(float DeltaTime)
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
