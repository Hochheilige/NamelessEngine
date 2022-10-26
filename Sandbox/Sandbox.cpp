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
#include "ScriptObject.h"
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
	const auto mLoader_Boot = mono->GetMethod("Scripts.Internal", "Loader", "Boot()");
	csGameInstance = mono->InvokeMethod(mLoader_Boot, nullptr, nullptr, nullptr);
	
	//TODO
	/*const auto mGame_OnSpecifyEngineSettings = mono->GetVirtualMethod("Scripts", "Game", "OnSpecifyEngineSettings(Scripts.EngineSettings.Builder)", csGameInstance);
	mono->InvokeMethod(mGame_OnSpecifyEngineSettings, csGameInstance, nullptr, nullptr);*/

	const auto mGame_Load = mono->GetVirtualMethod("Scripts", "Game", "OnLoad()", csGameInstance);
	mono->InvokeMethod(mGame_Load, csGameInstance, nullptr, nullptr);
}

RigidBodyComponent* CreateCubeComponent(float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z, float scale_x, float scale_y, float scale_z, float mass)
{
	auto physics = PhysicsModuleData::GetInstance();
	RigidBodyComponent* comp = new RigidBodyComponent();

	comp->Shape = new btBoxShape(btVector3(scale_x / 2, scale_y / 2, scale_z / 2));
	physics->GetCollisionShapes().push_back(comp->Shape);

	comp->Transform.setIdentity();
	comp->Transform.setOrigin(btVector3(trans_x, trans_y, trans_z));
	comp->Transform.setRotation(btQuaternion(rot_x, rot_y, rot_z));
	comp->Mass = mass;

	btVector3 localInertia = btVector3(0, 0, 0);
	if (mass != 0.0f)
		comp->Shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(comp->Transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(comp->Mass, myMotionState, comp->Shape, localInertia);
	comp->Body = new btRigidBody(rbInfo);

	physics->GetDynamicsWorls()->addRigidBody(comp->Body);

	return comp;
}

RigidBodyComponent* CreateSphereComponent(float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z, float scale_x, float scale_y, float scale_z, float mass)
{
	auto physics = PhysicsModuleData::GetInstance();

	RigidBodyComponent* comp = new RigidBodyComponent();


	comp->Shape = new btSphereShape(scale_x);
	physics->GetCollisionShapes().push_back(comp->Shape);

	comp->Transform.setIdentity();
	comp->Transform.setOrigin(btVector3(trans_x, trans_y, trans_z));
	comp->Transform.setRotation(btQuaternion(rot_x, rot_y, rot_z));

	comp->Mass = mass;

	btVector3 localInertia = btVector3(0, 0, 0);
	if (mass != 0.0f)
		comp->Shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(comp->Transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(comp->Mass, myMotionState, comp->Shape, localInertia);
	comp->Body = new btRigidBody(rbInfo);

	physics->GetDynamicsWorls()->addRigidBody(comp->Body);

	return comp;
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
	PerspCamera->Transform.Position = PerspCamera->Transform.Rotation.GetForwardVector() * -5.0f;

	OrthoCamera = new Camera();
	const float height = 16.0f;
	OrthoCamera->UpdateProjectionMatrixOrthographic(Display->GetAspectRatio() * height, height, -0.1f, -10.0f);
	OrthoCamera->Transform.Position = Vector3(0.0f, 5.0f, -4.0f);
	OrthoCamera->Transform.Rotation.SetEulerAngles(90.0f, 0.0f, 0.0f);

	Vector4 vec(0.0f, 0.0f, 1.0f, 1.0f);
	vec = Vector4::Transform(vec, OrthoCamera->GetProjectionMatrix());

	CreateCubeObject(5, -0.5, 0, 0, 0, 0, 10, 0.5, 10);

	//CreateCubeObject(2.0f, 2.0f, 0.0f, 45.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			for (int k = 0; k < 10; ++k)
				box.push_back(CreateCubeObject(i, 40.0f + j, k, 45.0f, 45.0f, 0.0f, 1.0f, 1.0f, 1.0f));

	

	const char* bunny_path = "..\\Assets\\stanford-bunny.fbx";
	bunny = CreateObject(0, 1, 0, 0, 0, 0, 0.005, 0.005, 0.005, bunny_path);

	FPSCC = CreateGameComponent<CameraController>();
	FPSCC->SetCameraToControl(PerspCamera);


	PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	pc->SetPixelShader(basicPS);
	pc->SetVertexShader(basicVS);


	// Physics part


	// Create rigid bodies
	
	// static platform
	RigidBodyComponent* platform = CreateCubeComponent(5, -0.5, 0, 0, 0, 0, 10, 0.5, 10, 0);
	rigidBodies.push_back(platform);
	//btCollisionShape* platform = new btBoxShape(btVector3(5, 0.25, 5));
	//collisionShapes.push_back(platform);

	//btTransform platformTransform;
	//platformTransform.setIdentity();
	//platformTransform.setOrigin(btVector3(5, -0.5, 0));
	//btScalar platformMass(0);

	//bool isDynamic = (platformMass != 0.0f);

	//btVector3 localInertia(0, 0, 0);
	//if (isDynamic)
	//	platform->calculateLocalInertia(platformMass, localInertia);

	//btDefaultMotionState* platformMotionState = new btDefaultMotionState(platformTransform);
	//btRigidBody::btRigidBodyConstructionInfo rbInfo(platformMass, platformMotionState, platform, localInertia);
	//btRigidBody* body = new btRigidBody(rbInfo);

	//dynamicWorld->addRigidBody(body);

	// Dynamic cube
	//btCollisionShape* box = new btBoxShape(btVector3(0.5, 0.5, 0.5));

	RigidBodyComponent* sphere = CreateSphereComponent(3, 10, 0, 0, 0, 0, 1, 1, 1, 1);
	rigidBodies.push_back(sphere);


	RigidBodyComponent* box;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 10; ++k)
			{

				box = CreateCubeComponent(i, 40 + j, k, 45, 45, 0, 1, 1, 1, 1);
				rigidBodies.push_back(box);
			}
		}
	}

}

void Sandbox::Update(float DeltaTime)
{
	const auto mGame_Update = mono->GetVirtualMethod("Scripts", "Game", "OnUpdate()", csGameInstance);
	mono->InvokeMethod(mGame_Update, csGameInstance, nullptr, nullptr);

	// Physics Simulation
	auto physics = PhysicsModuleData::GetInstance();
	physics->OnUpdate(DeltaTime);

	int i = 0;
	for (auto& body : rigidBodies)
	{
		btTransform trans = body->Update();
		GameComponents[i]->mTransform.Position = Vector3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
		GameComponents[i]->mTransform.Rotation = Quaternion(trans.getRotation());
		++i;
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
		if (keyboard->IsDown(KEY_I))
		{
			const char* bunny_path = "..\\Assets\\stanford-bunny.fbx";
			CreateObject(0, 1, 0, 0, 0, 0, 0.005, 0.005, 0.005, bunny_path);
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


