#pragma once

#include <vector>

class Scene
{
public:

	virtual void Initialize() = 0;

	std::vector<class Renderer*> GetRenderers();

protected:
	template<class T>
	T* CreateGameComponent(class GameComponent* Parent = nullptr)
	{
		T* comp = new T();

		GameComponents.push_back(comp);

		comp->SetParent(Parent);

		if constexpr (std::is_base_of<class Renderer, T>::value)
		{
			if (comp != nullptr)
				Renderers.emplace_back(comp);
			//renderingSystem->RegisterRenderer(comp);
		}
		//else if constexpr (std::is_base_of<class Collider, T>::value)
		//{
		//	Colliders.push_back(comp);
		//}

		return comp;
	}

private:
	std::vector<class GameComponent*> GameComponents;
	std::vector<class Renderer*> Renderers;
};