#include "manager.h"
#include <set>

void main()
{
	struct TransformComponent : Component
	{
		std::string test;

		TransformComponent(std::string text)
		{
			test = text;
		}
	};

	struct AIComponent : Component
	{
		int x;

		AIComponent(int a)
		{
			x = a;
		}
	};

	Manager manager = Manager();

	Entity id = manager.createEntity();
	Entity id2 = manager.createEntity();

	manager.registerComponent<TransformComponent>();
	manager.registerComponent<AIComponent>();
	manager.addComponent<TransformComponent>(id, TransformComponent("id1"));
	manager.addComponent<AIComponent>(id, AIComponent(1));
	manager.addComponent<TransformComponent>(id2, TransformComponent("id2"));
	TransformComponent c = manager.getComponent<TransformComponent>(id);
	AIComponent& aiComponent = manager.getComponent<AIComponent>(id);
	TransformComponent ctransform2 = manager.getComponent<TransformComponent>(id2);
	//manager.getComponent<TransformComponent>(id);
	//manager.getComponent<AIComponent>(id);

	class AIUpdateSystem : public System
	{
	public :
		AIUpdateSystem(Manager* manager) : System(manager)
		{
			m_requiredComponents = std::set<std::type_index>{ std::type_index(typeid(AIComponent)) };
		}

		void update(Entity entity, float deltaTime) override
		{
			std::cout << "Hello i am a child system!" << std::endl;
			AIComponent& component = this->m_manager->getComponent<AIComponent>(entity);
			component.x += 1;
		}	
	};
	AIUpdateSystem* systemAI = new AIUpdateSystem(&manager);
	manager.registerSystem(systemAI);
	manager.subscribeToSystems(id);

	std::cout << "Old Comp: " << aiComponent.x << std::endl;

	manager.updateAllSystems(1.0f);

	std::cout << "Updated Comp: " << aiComponent.x << std::endl;
}