#include "Level.h"

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

	window->display();
}

Level::Level(sf::RenderWindow * window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
map(tiledMapName), clock(), gom(), eventManager()
{
	
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
