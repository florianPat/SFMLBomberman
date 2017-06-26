#pragma once

#include <SFML/Graphics.hpp>
#include "TiledMap.h"
#include "Assets.h"
#include "Physics.h"
#include <iostream>
#include "GameObjectManager.h"
#include "EventManager.h"
#include "PlayerComponent.h"

class Level
{
private:
	sf::RenderWindow* window;
	Physics physics;
	TiledMap map;
	sf::Clock clock;
	EventManager eventManager;
	GameObjectManager gom;

	std::string levelName;

	std::string newLevel = "";
	bool endLevel = false;

	static constexpr int ACTOR_PLAYER = 0xa2ecb56a;
private:
	virtual void updateModel();
	virtual void composeFrame();
private:
	void createPlayer(sf::Vector2f& playerPos);
public:
	Level(sf::RenderWindow* window, std::string tiledMapName);
	std::string Go();
};