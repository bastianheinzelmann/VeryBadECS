#include "manager.h"

void main()
{
	Manager manager = Manager();

	Entity id = manager.createEntity();

	manager.registerComponent<TransformComponent>();
	manager.registerComponent<AIComponent>();
	manager.addComponent<TransformComponent>(id, TransformComponent());
	manager.addComponent<AIComponent>(id, AIComponent());
	TransformComponent c = manager.getComponent<TransformComponent>(id);
	AIComponent c2 = manager.getComponent<AIComponent>(id);
	//manager.getComponent<TransformComponent>(id);
	//manager.getComponent<AIComponent>(id);
	std::cout << c.test << std::endl;
	std::cout << c2.x << std::endl;

	
}