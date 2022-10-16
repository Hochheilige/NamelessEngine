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

	CreateSphereObject(3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1, 1, 1);

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			for (int k = 0; k < 10; ++k)
				box.push_back(CreateCubeObject(i, 40.0f + j, k, 45.0f, 45.0f, 0.0f, 1.0f, 1.0f, 1.0f));

	CreateCubeObject(5, -0.5, 0, 0, 0, 0, 10, 0.5, 10);

	const char* bunny_path = "..\\Assets\\stanford-bunny.fbx";
	bunny = CreateObject(0, 1, 0, 0, 0, 0, 0.005, 0.005, 0.005, bunny_path);

	FPSCC = CreateGameComponent<CameraController>();
	FPSCC->SetCameraToControl(PerspCamera);


	PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	pc->SetPixelShader(basicPS);
	pc->SetVertexShader(basicVS);


	// Physics part

	// Base objects for every physics scenea
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicWorld->setGravity(btVector3(0, -10, 0));

	// Create rigid bodies
	
	// static platform
	btCollisionShape* platform = new btBoxShape(btVector3(5, 0.25, 5));
	collisionShapes.push_back(platform);

	btTransform platformTransform;
	platformTransform.setIdentity();
	platformTransform.setOrigin(btVector3(5, -0.5, 0));
	btScalar platformMass(0);

	bool isDynamic = (platformMass != 0.0f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		platform->calculateLocalInertia(platformMass, localInertia);

	btDefaultMotionState* platformMotionState = new btDefaultMotionState(platformTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(platformMass, platformMotionState, platform, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicWorld->addRigidBody(body);

	// Dynamic cube
	btCollisionShape* box = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	btCollisionShape* sphere = new btSphereShape(1);

	/*btCollisionShape* model = new btConvexHullShape(bunny.);*/
	
	collisionShapes.push_back(sphere);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(3, 10, 0));

	btScalar mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	isDynamic = (mass != 0.f);

	localInertia = btVector3(0, 0, 0);
	if (isDynamic)
		sphere->calculateLocalInertia(mass, localInertia);



	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbSphereInfo(mass, myMotionState, sphere, localInertia);
	btRigidBody* sphereBody = new btRigidBody(rbSphereInfo);

	dynamicWorld->addRigidBody(sphereBody);


	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 10; ++k)
			{


				collisionShapes.push_back(box);

				/// Create Dynamic Objects
				btTransform startTransform;
				startTransform.setIdentity();
				startTransform.setRotation(btQuaternion(45, 45, 0));
				startTransform.setOrigin(btVector3(i, 40 + j, k));

				btScalar mass(1.f);

				//rigidbody is dynamic if and only if mass is non zero, otherwise static
				isDynamic = (mass != 0.f);

				localInertia = btVector3(0, 0, 0);
				if (isDynamic)
					box->calculateLocalInertia(mass, localInertia);



				//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
				btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
				btRigidBody::btRigidBodyConstructionInfo rbBoxInfo(mass, myMotionState, box, localInertia);
				btRigidBody* boxBody = new btRigidBody(rbBoxInfo);

				dynamicWorld->addRigidBody(boxBody);
			}
		}
	}

}

void Sandbox::Update(float DeltaTime)
{
	// Physics Simulation
	dynamicWorld->stepSimulation(DeltaTime);

	for (int i = 0; i < 251; ++i)
	{
		btCollisionObject* obj = dynamicWorld->getCollisionObjectArray()[i + 1];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		GameComponents[i]->mTransform.Position = Vector3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
		GameComponents[i]->mTransform.Rotation = Quaternion(trans.getRotation());
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


