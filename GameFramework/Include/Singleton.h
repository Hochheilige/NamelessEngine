#pragma once

template<class T>
class Singleton
{
public:
	static auto GetInstance() -> T* { return Instance; }

protected:
	Singleton()
	{
		Instance = static_cast<T*>(this);
	}

	static T* Instance;
};

template<class T>
T* Singleton<T>::Instance = nullptr;