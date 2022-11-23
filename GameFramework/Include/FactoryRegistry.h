#pragma once
#include <unordered_map>

template <typename Key, typename T>
	class FactoryRegistry
{
public:
	typedef T* (*FactoryFunction)();
	typedef std::unordered_map<Key, FactoryFunction*> FactoryMap;

	Key Register(Key key, FactoryFunction factory)
	{
		mapFactory.emplace(key, factory);
		return key;
	}

	// tries to create instance based on the key
	// using creator function (if provided)
	T* CreateInstance(Key key)
	{
		auto iter = mapFactory.find(key);
		if (iter == mapFactory.end())
		{
			return nullptr;
		}

		return (*iter->second)();
	}

protected:
	FactoryMap mapFactory;
};
