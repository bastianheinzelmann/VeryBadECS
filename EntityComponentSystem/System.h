#pragma once

#include <set>
#include <typeindex>
#include <typeinfo>
//#include "manager.h"
#include "Entity.h"

class Manager;

class System
{
public:

	System(Manager* manager)
	{
		m_manager = manager;
	}

	virtual void updateAllEntities(float deltaTime)
	{
		for (auto entity : m_entities)
		{
			update(entity, deltaTime);
		}
	}

	std::set<std::type_index> m_requiredComponents;
	std::set<Entity> m_entities;

protected:

	virtual void update(Entity entity, float deltaTime)
	{
	}
	Manager* m_manager;
};
