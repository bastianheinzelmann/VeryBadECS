#include <SFML/Graphics.hpp>
#include "manager.h"
#include <set>

void main()
{
	const int GRID_SIZE_X = 32;
	const int GRID_SIZE_Y = 18;

	const int WINDOW_SIZE_X = 1280;
	const int WINDOW_SIZE_Y = 720;

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	struct TransformComponent : Component
	{
		sf::Vector2f m_position;

		TransformComponent(sf::Vector2f position)
		{
			m_position = position;
		}

		TransformComponent(){}
	};

	struct AIComponent : Component
	{
		
	};

	struct CanvasComponent : Component
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

		CircleComponent() {}
	};

	struct VelocityComponent : Component
	{
		sf::Vector2f m_velocity;

		VelocityComponent(sf::Vector2f velocity)
		{
			m_velocity = velocity;
		}
	};

	struct InputComponent : Component
	{

	};

	Manager manager = Manager();

	Entity player = manager.createEntity();

	manager.registerComponent<TransformComponent>();
	manager.registerComponent<AIComponent>();
	manager.registerComponent<CircleComponent>();
	manager.registerComponent<VelocityComponent>();
	manager.registerComponent<InputComponent>();
	manager.registerComponent<CanvasComponent>();


	manager.addComponent<CircleComponent>(player, CircleComponent(sf::Color::Green, 20.0f));
	manager.addComponent<TransformComponent>(player, TransformComponent(sf::Vector2f(10.0f, 10.0f)));
	manager.addComponent<VelocityComponent>(player, VelocityComponent(sf::Vector2f(10.0f, 10.0f)));
	manager.addComponent<InputComponent>(player, InputComponent());

	class MovementSystem : public System
	{
	public:
		MovementSystem(Manager* manager) : System(manager)
		{
			m_requiredComponents = std::set<std::type_index>{ std::type_index(typeid(TransformComponent)), std::type_index(typeid(VelocityComponent)) };
		}

		void update(Entity entity, float deltaTime)
		{
			TransformComponent& transform = m_manager->getComponent<TransformComponent>(entity);
			VelocityComponent& velocity = m_manager->getComponent<VelocityComponent>(entity);
			transform.m_position += velocity.m_velocity * deltaTime;
		}
	};

	class InputSystem : public System
	{
	public:
		InputSystem(Manager* manager) : System(manager)
		{
			m_requiredComponents = std::set<std::type_index>{ std::type_index(typeid(InputComponent)), std::type_index(typeid(VelocityComponent)), std::type_index(typeid(CircleComponent)) };
		}

		void update(Entity entity, float deltaTime)
		{
			VelocityComponent& velocity = m_manager->getComponent<VelocityComponent>(entity);
			CircleComponent& circle = m_manager->getComponent<CircleComponent>(entity);
			velocity.m_velocity = sf::Vector2f(0, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				velocity.m_velocity.x += -m_velocity;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				velocity.m_velocity.x += m_velocity;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				velocity.m_velocity.y += -m_velocity;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				velocity.m_velocity.y += m_velocity;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
			{
				circle.m_color = sf::Color::Yellow;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
			{
				circle.m_color = sf::Color::Cyan;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
			{
				circle.m_color = sf::Color::Magenta;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
			{
				circle.m_color = sf::Color::Black;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
			{
				circle.m_color = sf::Color::White;
			}
		}

		float m_velocity = 0.2f;
	};

	class CanvasSystem : public System
	{
	public:
		CanvasSystem(Manager* manager, Entity player) : System(manager)
		{
			m_requiredComponents = std::set<std::type_index>{ std::type_index(typeid(TransformComponent)), std::type_index(typeid(CircleComponent)), std::type_index(typeid(CanvasComponent)) };
			m_player = player;
		}

		void updateAllEntities(float deltaTime) override
		{
			m_playerTransform = m_manager->getComponent<TransformComponent>(m_player);
			m_playerCircle = m_manager->getComponent<CircleComponent>(m_player);
			System::updateAllEntities(deltaTime);
		}

		void update(Entity entity, float deltaTime)
		{
			TransformComponent transform = m_manager->getComponent<TransformComponent>(entity);
			CircleComponent& circle = m_manager->getComponent<CircleComponent>(entity);
			float radius = circle.m_radius / 2;
			float playerRadius = m_playerCircle.m_radius / 2;
			float dist = sqrt(pow((transform.m_position.x - radius - m_playerTransform.m_position.x - playerRadius), 2) + pow((transform.m_position.y - radius - m_playerTransform.m_position.y - playerRadius), 2));

			if (dist < m_playerCircle.m_radius + 10.0f)
			{
				circle.m_color = m_playerCircle.m_color;
			}
			else
			{

			}
		}


		CircleComponent m_playerCircle;
		Entity m_player;
		TransformComponent m_playerTransform;
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem(Manager* manager, sf::RenderWindow* window) : System(manager)
		{
			m_window = window;
			m_requiredComponents = std::set<std::type_index>{ std::type_index(typeid(TransformComponent)), std::type_index(typeid(CircleComponent)) };
		}

		void updateAllEntities(float deltaTime) override
		{
			m_window->clear();
			System::updateAllEntities(deltaTime);
			m_window->display();
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

	manager.registerSystem(new MovementSystem(&manager));
	manager.registerSystem(new RenderSystem(&manager, &window));
	manager.registerSystem(new InputSystem(&manager));
	manager.registerSystem(new CanvasSystem(&manager, player));
	manager.subscribeToSystems(player);

	for (int i = 0; i < GRID_SIZE_X; ++i)
	{
		for (int j = 0; j < GRID_SIZE_Y; ++j)
		{
			int offsetX = (WINDOW_SIZE_X / GRID_SIZE_X) * i;
			int offsetY = (WINDOW_SIZE_Y / GRID_SIZE_Y) * j;
			Entity entity = manager.createEntity();
			manager.addComponent<TransformComponent>(entity, TransformComponent(sf::Vector2f(offsetX, offsetY)));
			manager.addComponent<CircleComponent>(entity, CircleComponent(sf::Color::Black, 10.0f));
			manager.addComponent<CanvasComponent>(entity, CanvasComponent());
			manager.subscribeToSystems(entity);
		}
	}

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time dt = deltaClock.restart();
		manager.updateAllSystems(dt.asMilliseconds());
	}
}