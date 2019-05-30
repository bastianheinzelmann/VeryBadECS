#include <SFML/Graphics.hpp>
#include "manager.h"
#include <set>

void main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	struct TransformComponent : Component
	{
		sf::Vector2f m_position;

		TransformComponent(sf::Vector2f position)
		{
			m_position = position;
		}
	};

	struct AIComponent : Component
	{
		
	};

	struct CircleComponent : Component
	{
		sf::CircleShape m_shape;
		sf::Color m_color;
		float m_radius;

		CircleComponent(sf::Color color, float radius)
		{
			m_color = color;
			m_radius = radius;
			m_shape = sf::CircleShape(radius);
		}
	};

	Manager manager = Manager();

	Entity id = manager.createEntity();
	Entity id2 = manager.createEntity();

	manager.registerComponent<TransformComponent>();
	manager.registerComponent<AIComponent>();
	manager.registerComponent<CircleComponent>();
	//manager.addComponent<TransformComponent>(id, TransformComponent("id1"));
	//manager.addComponent<AIComponent>(id, AIComponent(1));
	//manager.addComponent<TransformComponent>(id2, TransformComponent("id2"));
	//TransformComponent c = manager.getComponent<TransformComponent>(id);
	//AIComponent& aiComponent = manager.getComponent<AIComponent>(id);
	//TransformComponent ctransform2 = manager.getComponent<TransformComponent>(id2);
	//manager.getComponent<TransformComponent>(id);
	//manager.getComponent<AIComponent>(id);

	manager.addComponent<CircleComponent>(id, CircleComponent(sf::Color::Red, 50.0f));
	manager.addComponent<TransformComponent>(id, TransformComponent(sf::Vector2f(0.0f, 0.0f)));

	class RenderSystem : public System
	{
	public:
		RenderSystem(Manager* manager, sf::RenderWindow* window) : System(manager)
		{
			m_window = window;
			m_requiredComponents = std::set<std::type_index>{ std::type_index(typeid(TransformComponent)), std::type_index(typeid(CircleComponent)) };
		}

		void update(Entity entity, float deltaTime) override
		{
			TransformComponent& transform = m_manager->getComponent<TransformComponent>(entity);
			CircleComponent& circle = m_manager->getComponent<CircleComponent>(entity);

			circle.m_shape.setFillColor(circle.m_color);
			circle.m_shape.setPosition(transform.m_position);

			m_window->draw(circle.m_shape);
		}

		sf::RenderWindow* m_window;
	};

	manager.registerSystem(new RenderSystem(&manager, &window));
	manager.subscribeToSystems(id);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		manager.updateAllSystems(1.0);
		window.display();

		//window.clear();
		//window.draw(shape);
		//window.display();
	}

	//class AIUpdateSystem : public System
	//{
	//public :
	//	AIUpdateSystem(Manager* manager) : System(manager)
	//	{
	//		m_requiredComponents = std::set<std::type_index>{ std::type_index(typeid(AIComponent)) };
	//	}

	//	void update(Entity entity, float deltaTime) override
	//	{
	//		std::cout << "Hello i am a child system!" << std::endl;
	//		AIComponent& component = this->m_manager->getComponent<AIComponent>(entity);
	//		component.x += 1;
	//	}	
	//};
	//AIUpdateSystem* systemAI = new AIUpdateSystem(&manager);
	//manager.registerSystem(systemAI);
	//manager.subscribeToSystems(id);


}