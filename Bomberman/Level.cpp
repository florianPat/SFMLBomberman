#include "Level.h"
#include "EventLevelReload.h"

void Level::eventLevelReloadHandler(std::unique_ptr<EventData>)
{
	newLevel = levelName;
	endLevel = true;
}

void Level::updateModel()
{
	float dt = clock.restart().asSeconds();

	gom.updateActors(dt);

	physics.update(dt);
}

void Level::composeFrame()
{
	window->clear();

	map.draw(*window);
	gom.drawActors();

	//physics.debugRenderBodies(*window);

	window->display();
}

void Level::createPlayer(sf::Vector2f & playerPos)
{
	Actor* playerP = gom.addActor(ACTOR_PLAYER);
	playerP->addComponent(std::make_shared<PlayerComponent>(playerPos, TextureAtlas("player.atlas"), physics, *window, &eventManager, playerP));
}

Level::Level(sf::RenderWindow * window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
map(tiledMapName), clock(), gom(), eventManager()
{
	auto objectGroups = map.getObjectGroups();
	for (auto it = objectGroups.begin(); it != objectGroups.end(); ++it)
	{
		if (it->name == "blocked")
		{
			for (size_t i = 0; i < it->objects.size(); ++i)
				physics.addElementValue(Physics::Body(std::string("Blocked" + std::to_string(i)), it->objects[i]));
		}
		else if (it->name == "playerStart")
			createPlayer(sf::Vector2f{ it->objects[0].left, it->objects[0].top });
	}

	eventManager.addListener(EventLevelReload::EVENT_LEVEL_RELOAD_ID, delegateLevelReload);
}

std::string Level::Go()
{
	while (!endLevel && window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		updateModel();
		composeFrame();
	}

	return newLevel;
}
