#pragma once

#include <SFML/Graphics.hpp>
#include "TiledMap.h"
#include "Assets.h"
#include "Physics.h"
#include <iostream>
#include "GameObjectManager.h"
#include "EventManager.h"
#include "PlayerComponent.h"
#include "BrickableBlockComponent.h"

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
	static constexpr int ACTOR_BRICKABLEBLOCK = 0x9e63306c;

private:
	std::function<void(EventData*)> eventLevelReloadFunction = std::bind(&Level::eventLevelReloadHandler, this, std::placeholders::_1);
	static constexpr int delegateLevelReloadId = 0;
	DelegateFunction delegateLevelReload = std::make_pair(delegateLevelReloadId, eventLevelReloadFunction);
private:
	void eventLevelReloadHandler(EventData* eventData);
private:
	virtual void updateModel();
	virtual void composeFrame();
private:
	void createPlayer(sf::Vector2f& playerPos);
	void createBrickableBlocks(std::vector<sf::FloatRect>& boundingBoxes);
public:
	Level(sf::RenderWindow* window, std::string tiledMapName);
	std::string Go();
};