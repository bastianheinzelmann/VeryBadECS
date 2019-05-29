#pragma once

#include <vector>

class IComponentManager
{

};

template<typename T>
class ComponentManager : public IComponentManager
{
public:
	unsigned int insertComponent(T component)
	{
		// no time shitty code
		// TODO a sparse set would be a looooooooot better
		m_components.push_back(component);
		return m_components.size() - 1;
	}

	std::vector<T> m_components;
};