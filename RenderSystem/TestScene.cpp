#include "TestScene.h"

#include "ShaderCompiler.h"
#include "Shader.h"
#include "Mesh.h"
#include "DisplayWin32.h"
#include "MeshRenderer.h"
#include "CameraController.h"
#include "OrbitCameraController.h"
#include "InputDevice.h"
#include "PlaneComponent.h"

void TestScene::Initialize()
{
	// create meshes
	boxMesh = new BoxMesh();
	boxMeshProxy = boxMesh->CreateRenderingPrimitiveProxy();
	circleMesh = new CircleMesh();
	circleMeshProxy = circleMesh->CreateRenderingPrimitiveProxy();
	sphereMesh = new SphereMesh();
	sphereMeshProxy = sphereMesh->CreateRenderingPrimitiveProxy();


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

	MeshRenderer* box = CreateGameComponent<MeshRenderer>();
	box->SetMeshProxy(sphereMeshProxy);
	box->SetVertexShader(vs);
	box->SetPixelShader(ps);
	box->mTransform.Rotation.SetEulerAngles(0.0f, 0.0f, 30.0f);
	box->mTransform.Position.x = 3.0f;

	MeshRenderer* box2 = CreateGameComponent<MeshRenderer>();
	box2->SetMeshProxy(sphereMeshProxy);
	box2->SetVertexShader(vs);
	box2->SetPixelShader(ps);


	MeshRenderer* box3 = CreateGameComponent<MeshRenderer>();
	box3->SetMeshProxy(sphereMeshProxy);
	box3->SetVertexShader(vs);
	box3->SetPixelShader(ps);


	MeshRenderer* box4 = CreateGameComponent<MeshRenderer>();
	box4->SetMeshProxy(sphereMeshProxy);
	box4->SetVertexShader(vs);
	box4->SetPixelShader(ps);
	box4->mTransform.Rotation.SetEulerAngles(0.0f, 0.0f, -30.0f);
	box4->mTransform.Position.x = -3.0f;

	MeshRenderer* box5 = box4->AddChildComponent<MeshRenderer>();
	box5->SetMeshProxy(sphereMeshProxy);
	box5->SetVertexShader(vs);
	box5->SetPixelShader(ps);
	box5->mTransform.Position.z = 2.0f;
	box5->mTransform.Scale = Vector3::One * 0.5f;

	MeshRenderer* box6 = box5->AddChildComponent<MeshRenderer>();
	box6->SetMeshProxy(sphereMeshProxy);
	box6->SetVertexShader(vs);
	box6->SetPixelShader(ps);
	box6->mTransform.Position.z = 2.0f;
	box6->mTransform.Scale = Vector3::One * 0.5f;

	MeshRenderer* box7 = box6->AddChildComponent<MeshRenderer>();
	box7->SetMeshProxy(sphereMeshProxy);
	box7->SetVertexShader(vs);
	box7->SetPixelShader(ps);
	box7->mTransform.Position.z = 2.0f;
	box7->mTransform.Scale = Vector3::One * 0.5f;


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
