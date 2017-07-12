#pragma once

#include "Component.h"
#include "SFML\Graphics.hpp"
#include "Animation.h"
#include "TextureAtlas.h"
#include "Physics.h"
#include <unordered_map>
#include <vector>
#include "Utils.h"
#include "Actor.h"
#include "BombComponent.h"

class PlayerComponent : public Component
{
	static constexpr float speed = 25.0f;
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, Animation> animations;
	sf::Sprite currentFrame;
	std::vector<std::string> collisionIds;
	sf::Vector2f startingPos;
	sf::FloatRect boundingBox;
	std::shared_ptr<Physics::Body> body;
	sf::View view;
	bool isInsideBomb = false;
private:
	void eventEntitiyHitByBombHandler(std::unique_ptr<EventData> eventData);
private:
	std::function<void(std::unique_ptr<EventData>)> eventEntitiyHitByBombFunction = std::bind(&PlayerComponent::eventEntitiyHitByBombHandler, this, std::placeholders::_1);
	static constexpr int delegateEntitiyHitByBombId = 0xa1455aa0;
	DelegateFunction delegateCollectedAllDiamonds = std::make_pair(delegateEntitiyHitByBombId, eventEntitiyHitByBombFunction);
public:
	static constexpr int COMPONENT_PLAYER_ID = 0xe9479fee;
public:
	PlayerComponent(sf::Vector2f& playerPos, TextureAtlas& atlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
};