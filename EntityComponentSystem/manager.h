#pragma once

#include "ComponentManager.h"
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <string>
#include <iostream>
#include <utility>
#include <stdexcept>
#include <list>
#include <set>
#include "System.h"
#include "Entity.h"

struct Component
{
};

class Manager
{
public:
	Manager()
	{
	}

	template<typename T>
	void addComponent(Entity entity, T component)
	{

		if (m_componentManagers.find(std::type_index(typeid(T))) == m_componentManagers.end())
		{
			std::cout << "Component not registered!" << std::endl;
		}
		else
		{
			if (m_entities.find(entity) == m_entities.end())
			{
				std::cout << "Entity does not exist!" << std::endl;
			}
			else
			{

				if (m_entities[entity].find(std::type_index(typeid(T))) == m_entities[entity].end())
				{

					// that feels so bad
					unsigned int id = static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))])->insertComponent(component);
					m_entities[entity][std::type_index(typeid(T))] = id;
				}
				else
				{
					std::cout << "Entity has already component of this type!";
				}
			}
		}
		//ComponentManager<T>* comp = new ComponentManager<T>();
		//Components[std::type_index(typeid(T))] = comp;
		//comp->m_components.push_back(T());
	}

	template<typename T>
	void registerComponent()
	{
		if (m_componentManagers.find(std::type_index(typeid(T))) == m_componentManagers.end())
		{
			ComponentManager<T>* componentManager = new ComponentManager<T>();
			m_componentManagers[std::type_index(typeid(T))] = componentManager;
		}
		else
		{
			std::cout << "Component type already registered..." << std::endl;
		}
	}

	Entity createEntity()
	{
		m_entities[m_lastIndex] = std::unordered_map<std::type_index, unsigned int>();
		return m_lastIndex++;
	}

	template<typename T>
	T& getComponent(Entity entity)
	{
		
		//ComponentManager<T>* componentManager = static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))]);
		//return componentManager->m_components[0];

		if (m_componentManagers.find(std::type_index(typeid(T))) == m_componentManagers.end())
		{
			std::cout << "Component type is not registered!" << std::endl;
			throw std::exception("Component type is not registered!");
		}
		else
		{
			if (m_entities.find(entity) == m_entities.end())
			{
				std::cout << "Entity i not know" << std::endl;
				throw std::exception("Entity no!");
			}
			else
			{
				// TODO need to check if entity has component type :)
				unsigned int id = m_entities[entity][std::type_index(typeid(T))];
				return (static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))])->m_components[id]);
			}
		}
	}

	void registerSystem(System* system)
	{
		m_systems.push_back(system);
	}

	void subscribeToSystems(Entity entity)
	{
		for (auto system : m_systems)
		{
			bool addToSystem = true;

			for (auto reqComp : system->m_requiredComponents)
			{
				if (m_entities[entity].find(reqComp) == m_entities[entity].end())
				{
					addToSystem = false;
				}
			}

			if(addToSystem)
				system->m_entities.insert(entity);
		}
	}

	void updateAllSystems(float deltaTime)
	{
		for (auto system : m_systems)
		{
			system->updateAllEntities(deltaTime);
		}
	}

private:
	unsigned int m_lastIndex = 0;
	std::unordered_map<std::type_index, IComponentManager*> m_componentManagers;

	std::unordered_map<Entity, std::unordered_map<std::type_index, unsigned int>> m_entities;

	std::list<System*> m_systems;
};