#pragma once

#include "Component.h"
#include "Animation.h"
#include "Physics.h"
#include "SFML\Graphics.hpp"
#include "Utils.h"
#include "Assets.h"

class BombComponent : public Component
{
	static constexpr float explodeTime = 4.0f;
	float timeDelay = 0.0f;
	bool exploded = false;
	bool showFire = false;
	sf::Vector2f pos;
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, Animation> animations;
	sf::Sprite currentFrame;
	sf::FloatRect boundingBox;
	sf::FloatRect boundingBoxTriggerArea;
	Physics::Body body;
	std::shared_ptr<Physics::Body> triggerExplosionBody;
	std::shared_ptr<sf::Texture> flameTileTexture;
	sf::Sprite flameTileSprite;
	sf::RenderTexture flameTileRenderTexture;
	sf::Sprite flameSprite;
	sf::Vector2f move;
public:
	static constexpr int COMPONENT_BOMB_ID = 0xe14a6a55;
public:
	//flameRange has to be an odd number!
	BombComponent(sf::Vector2f& pos, int flameRange, TextureAtlas& atlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	bool getIsExploded() const;
	sf::FloatRect getBoundingBox() const;
};