#pragma once

#include "Transform.h"
#include "Game.h"
#include <vector>

class GameComponent
{
public:

	Transform mTransform;

	GameComponent* GetParent() const { return Parent; }

	Transform GetWorldTransform() const;

	virtual void Update(float DeltaTime) {}

	virtual ~GameComponent() = default;

	bool bShouldUpdate = false;

	void SetParent(GameComponent* InParent);

	// todo: pass contructor args
	template<class T>
	T* AddChildComponent()
	{
		return Game::GetInstance()->CreateGameComponent<T>(this);
	}

	template<class T>
	T* GetComponentInParent()
	{
		GameComponent* comp = Parent;
		while (comp != nullptr)
		{
			if (T* res = dynamic_cast<T*>(comp))
			{
				return res;
			}
		}
		return nullptr;
	}

	template<class T>
	T* GetChildComponent()
	{
		for (GameComponent* gc : Children)
		{
			if (T* res = dynamic_cast<T*>(gc))
			{
				return res;
			}
		}
		return nullptr;
	}

private:
	friend class Game;

	GameComponent* Parent = nullptr;

	std::vector<GameComponent*> Children;
};

