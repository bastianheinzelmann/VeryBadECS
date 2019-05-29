#pragma once


#include <map>
#include <string>

class Component;

typedef Component* (*CreateComponentFunc)();
typedef std::map<std::string, CreateComponentFunc> ComponentRegistry;

inline ComponentRegistry& getComponentRegistry()
{
	static ComponentRegistry reg;
	return reg;
}

template<class T>
Component* createComponent()
{
	return new T;
}

template<class T>
struct RegistryEntry
{
public:
	static RegistryEntry<T>& Instance(const std::string& name)
	{
		static RegistryEntry<T> inst(name);
		return inst;
	}
private:
	RegistryEntry(const std::string& name)
	{
		ComponentRegistry& reg = getComponentRegistry();
		CreateComponentFunc func = createComponent<T>;


	}
};