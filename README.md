Authors: Bastian Heinzelmann, Armin Hotter, Tobias Nemecek

Example implementation of a simple game with our ecs:

Controls:   WASD to move the player cicle 
            And numbers from 1 - 6 to change player color

Moving the circle changes color of surrounding circles.

Usage of the ecs:

New Component types need to inherit from Component struct

	struct TransformComponent : Component
	{
		sf::Vector2f m_position;

		TransformComponent(sf::Vector2f position)
		{
			m_position = position;
		}

		TransformComponent(){}
	};

Component types must be registered in the manager

	manager.registerComponent<TransformComponent>();

New Systems must inherit from the System class and need to override the update-function. Optionally updateAllEntities can also be overridden.

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

New Systems need to be registered in the manager.

	manager.registerSystem(new MovementSystem(&manager));

Entities are simple ids and can be created by the manager with createEntity.

Components can be added to entities by calling addComponent.

To actually update entities they need to be subscribed to a system.
This function checks whether the entity has the required components for each system and subscribes only to fitting systems.

	Entity entity = manager.createEntity();
	manager.addComponent<TransformComponent>(entity, TransformComponent(sf::Vector2f(offsetX, offsetY)));
	manager.addComponent<CircleComponent>(entity, CircleComponent(sf::Color::Black, 10.0f));
	manager.addComponent<CanvasComponent>(entity, CanvasComponent());
	manager.subscribeToSystems(entity);