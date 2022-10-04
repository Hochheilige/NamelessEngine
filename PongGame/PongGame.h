#pragma once

#include "Game.h"

#include "ShaderCompiler.h"
#include "Mesh.h"
#include "GameComponent.h"
#include "MeshRenderer.h"
#include "InputDevice.h"
#include "AABB2DCollider.h"

#include <iostream>

//float Clamp(float x, float min, float max);
//{
//	return x < min ? min : (x > max ? max : x);
//}

enum class PongType
{
	None,
	Border,
	Player,
	Player1Behind,
	Player2Behind

};

class PongActor : public GameComponent
{
public:
	PongType PongType = PongType::None;
};

class PlayerActor : public PongActor
{
public:
	float MovementSpeed = 1.0f;

	const float MaxPos = 0.63f;

	PlayerActor() 
	{
		PongType = PongType::Player;
	}

	// code for up and code for down
	void SetControls(uint64_t InUpCode, uint64_t InDownCode)
	{
		UpCode = InUpCode;
		DownCode = InDownCode;
	}

	virtual void Update(float DeltaTime) override
	{
		InputDevice* inputDevice = Game::GetInstance()->GetInputDevice();
		if (inputDevice->IsKeyDown(UpCode))
		{
			mTransform.Position.y += MovementSpeed * DeltaTime;
		}
		else if (inputDevice->IsKeyDown(DownCode))
		{
			mTransform.Position.y -= MovementSpeed * DeltaTime;
		}
		const float x = mTransform.Position.y;
		const float min = -MaxPos;
		const float max = MaxPos;
		mTransform.Position.y =  x < min ? min : (x > max ? max : x);
	}

	int GetScore()
	{
		return Score;
	}

private:
	uint64_t UpCode = 0;
	uint64_t DownCode = 0;

	int Score = 0;
	void IncrementScore()
	{
		++Score;
	}
	
	friend class PongGame;
};

class BallActor : public PongActor
{
public:
	DirectX::XMFLOAT3 Direction = DirectX::XMFLOAT3(1.0f,0.4f, 0.0f);
	float StartSpeed = 0.6f;
	float Speed = StartSpeed;
	float Acceleration = 0.05f;
	virtual void Update(float DeltaTime) override;
	
	void ResetSpeed()
	{
		Speed = StartSpeed;
	}
private:
	bool WaitForEndOverlap = false;
};


class PongGame : public Game
{
public:
	virtual void PrepareResources() override;

	void PlayerScored(PongType InPongType)
	{
		float dirX = 0.0f;
		switch (InPongType)
		{
		case PongType::Player1Behind:
			player2->IncrementScore();
			dirX = 1.0f;
			break;
		case PongType::Player2Behind:
			player1->IncrementScore();
			dirX = -1.0f;
			break;
		}

		ball->mTransform.Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.f);
		ball->Direction.x = dirX;
		ball->Direction.y = static_cast<float>(rand()) / RAND_MAX - 1.0f;
		ball->ResetSpeed();

		std::cout << "Player 1: " << player1->GetScore() << "; Player 2: " << player2->GetScore() << std::endl;

	}

	virtual void DestroyResources() override
	{
		delete ball;
		delete player2;
		delete player1;
		delete BorderBottom;
		delete BorderTop;
		delete BorderRight;
		delete BorderLeft;
		delete psPlain;
		delete ps;
		delete vs;
		delete squareMesh;


		Game::DestroyResources();
	}

protected:
	friend int main();
	PongGame() = default;

	SquareMesh* squareMesh;
	CircleMesh* circleMesh;
	SimpleVertexShader* vs;
	PixelShader* ps;
	PixelShader* psPlain;

	PongActor* BorderLeft;
	PongActor* BorderRight;
	PongActor* BorderTop;
	PongActor* BorderBottom;

	PlayerActor* player1;
	PlayerActor* player2;

	BallActor* ball;
};

