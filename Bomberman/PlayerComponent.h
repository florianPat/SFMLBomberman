#pragma once

#include "Component.h"
#include "SFML\Graphics.hpp"
#include "Animation.h"
#include "TextureAtlas.h"
#include "Physics.h"
#include <unordered_map>
#include <vector>
#include "Utils.h"

class PlayerComponent : public Component
{
	static constexpr float speed = 25.0f;
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, Animation> animations;
	sf::Sprite currentFrame;
	sf::Vector2f startingPos;
	sf::FloatRect boundingBox;
	std::shared_ptr<Physics::Body> body;
	sf::View view;
public:
	static constexpr int COMPONENT_PLAYER_ID = 0xe9479fee;
public:
	PlayerComponent(sf::Vector2f& playerPos, TextureAtlas& atlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
};