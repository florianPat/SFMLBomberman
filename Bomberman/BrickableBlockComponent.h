#pragma once

#include "Component.h"
#include "Actor.h"
#include "Physics.h"
#include <vector>
#include <memory>
#include "SFML\Graphics.hpp"
#include "Assets.h"
#include "Animation.h"

class BrickableBlockComponent : public Component
{
	sf::RenderTarget& renderTarget;
	Physics& physics;
	std::vector<sf::FloatRect> boundingBoxes;
	std::vector<std::shared_ptr<Physics::Body>> bodies;
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	std::vector<std::pair<sf::FloatRect, Animation>> destroyBoundingBoxesAnimations;
private:
	void eventEntitiyHitByBombHandler(EventData* eventData);
private:
	std::function<void(EventData*)> eventEntitiyHitByBombFunction = std::bind(&BrickableBlockComponent::eventEntitiyHitByBombHandler, this, std::placeholders::_1);
	static constexpr int delegateEntitiyHitByBombId = 0xd20a76ad;
	DelegateFunction delegateCollectedAllDiamonds = std::make_pair(delegateEntitiyHitByBombId, eventEntitiyHitByBombFunction);
public:
	static constexpr int COMPONENT_BRICKABLEBLOCK_ID = 0x3590dd31;
public:
	BrickableBlockComponent(std::string& textureFileName, std::vector<sf::FloatRect>& boundingBoxes, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
};