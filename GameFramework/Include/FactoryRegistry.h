#pragma once
#include <unordered_map>

template <typename Key, typename T>
	class FactoryRegistry
{
public:
	typedef T* (*FactoryFunction)();
	typedef std::unordered_map<Key, FactoryFunction> FactoryMap;

	Key RegisterFactory(Key key, FactoryFunction factory)
	{
		mapFactory.insert(FactoryMap::value_type(key, factory));
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
