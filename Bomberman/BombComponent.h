#pragma once

#include "Component.h"
#include "Animation.h"
#include "Physics.h"
#include "SFML\Graphics.hpp"
#include "Utils.h"

class BombComponent : public Component
{
	static constexpr float explodeTime = 5.0f;
	float timeDelay = 0.0f;
	bool exploded = false;
	sf::Vector2f pos;
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, Animation> animations;
	sf::Sprite currentFrame;
	sf::FloatRect boundingBox;
	Physics::Body body;
public:
	static constexpr int COMPONENT_BOMB_ID = 0xe14a6a55;
public:
	BombComponent(sf::Vector2f& pos, TextureAtlas& atlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	bool getIsExploded() const;
	sf::FloatRect getBoundingBox() const;
};