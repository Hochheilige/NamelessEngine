#include "PongGame.h"

void BallActor::Update(float DeltaTime)
{
	{
		DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&Direction);
		vec = DirectX::XMVector3Normalize(vec);
		DirectX::XMStoreFloat3(&Direction, vec);

		Speed += Acceleration * DeltaTime;

		const float speedPerFrame = Speed * DeltaTime;

		mTransform.Position.x += speedPerFrame * Direction.x;
		mTransform.Position.y += speedPerFrame * Direction.y;
		//Transform.Position.z += Speed * Direction.z;

		// check overlap
		// if overlapping border, then inverse y direction
		// if overlapping player inverse x direction
		// if overlapping player back move the ball to center and update the Score
		Collider* col = Game::GetInstance()->GetOverlapping(GetChildComponent<AABB2DCollider>());

		if (col != nullptr && !WaitForEndOverlap)
		{

			PongActor* pa = dynamic_cast<PongActor*>(col->GetParent());
			if (pa != nullptr)
			{
				switch (pa->PongType)
				{
				case PongType::Border:
					Direction.y = -Direction.y;
					break;
				case PongType::Player:
					Direction.x = -Direction.x;
					break;
				case PongType::Player1Behind:
				case PongType::Player2Behind:
					class PongGame* game = dynamic_cast<PongGame*>(Game::GetInstance());
					if (game != nullptr)
					{
						game->PlayerScored(pa->PongType);
					}
					break;
				}
				WaitForEndOverlap = true;
			}
		}
		if (col == nullptr && WaitForEndOverlap)
		{
			WaitForEndOverlap = false;
		}

		if (abs(mTransform.Position.x) > 2.0f || abs(mTransform.Position.y) > 2.0f)
		{
			mTransform.Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			ResetSpeed();
		}

	}
}

void PongGame::PrepareResources()
{
	Game::PrepareResources();
	// create mesh
	squareMesh = new SquareMesh();
	circleMesh = new CircleMesh();

	MeshProxy* squareMeshProxy = squareMesh->CreateMeshProxy();
	MeshProxy* circleMeshProxy = circleMesh->CreateMeshProxy();


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

	// Create borders
	BorderLeft = new PongActor();
	BorderRight = new PongActor();
	BorderTop = new PongActor();
	BorderBottom = new PongActor();

	const float borderWidth = 0.03f;

	MeshRenderer* mr = BorderLeft->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(squareMeshProxy);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderLeft->mTransform.Scale.x = borderWidth;
	BorderLeft->mTransform.Position.x = -1 + BorderLeft->mTransform.Scale.x / 2.0f;
	BorderLeft->AddChildComponent<AABB2DCollider>();
	BorderLeft->PongType = PongType::Player1Behind;

	mr = BorderRight->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(squareMeshProxy);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderRight->mTransform.Scale.x = borderWidth;
	BorderRight->mTransform.Position.x = 1 - BorderLeft->mTransform.Scale.x / 2.0f;
	BorderRight->AddChildComponent<AABB2DCollider>();
	BorderRight->PongType = PongType::Player2Behind;


	mr = BorderTop->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(squareMeshProxy);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderTop->mTransform.Scale.y = borderWidth * 12.0f / 8.0f;
	BorderTop->mTransform.Position.y = 1 - BorderTop->mTransform.Scale.y / 2.0f;
	BorderTop->AddChildComponent<AABB2DCollider>();
	BorderTop->PongType = PongType::Border;


	mr = BorderBottom->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(squareMeshProxy);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderBottom->mTransform.Scale.y = borderWidth * 12.0f / 8.0f;
	BorderBottom->mTransform.Position.y = -1 + BorderBottom->mTransform.Scale.y / 2.0f;
	BorderBottom->AddChildComponent<AABB2DCollider>();
	BorderBottom->PongType = PongType::Border;


	// create players
	player1 = CreateGameComponent<PlayerActor>();
	mr = player1->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(squareMeshProxy);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.1f, 1.0f });
	player1->mTransform.Scale.y = 0.3f;
	player1->mTransform.Scale.x = borderWidth;
	player1->mTransform.Position.x = -0.7f;
	player1->SetControls(87, 83);
	player1->AddChildComponent<AABB2DCollider>();
	player1->bShouldUpdate = true;

	player2 = CreateGameComponent<PlayerActor>();
	mr = player2->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(squareMeshProxy);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.1f, 0.4f, 0.4f, 1.0f });
	player2->mTransform.Scale.y = 0.3f;
	player2->mTransform.Scale.x = borderWidth;
	player2->mTransform.Position.x = +0.7f;
	//player2->Transform.Rotation.SetEulerAngles({ 0.0f, 0.0f, 45.0f });
	player2->SetControls(38, 40);
	player2->AddChildComponent<AABB2DCollider>();
	player2->bShouldUpdate = true;


	ball = CreateGameComponent<BallActor>();
	ball->bShouldUpdate = true;
	mr = ball->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(circleMeshProxy);
	mr->SetPixelShader(ps);
	mr->SetVertexShader(vs);
	//mr->SetColor({ 0.1f, 0.4f, 0.1f, 1.0f });
	ball->mTransform.Scale = Vector3(0.02f, 0.02f, 0.02f);
	ball->AddChildComponent<AABB2DCollider>();
}
