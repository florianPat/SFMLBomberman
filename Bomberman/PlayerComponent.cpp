#include "PlayerComponent.h"
#include "EventHitByBomb.h"
#include "EventLevelReload.h"

void PlayerComponent::eventEntitiyHitByBombHandler(EventData* eventData)
{
	if (body->getId() == static_cast<EventHitByBomb*>(eventData)->collisionId)
	{
		std::unique_ptr<EventLevelReload> eventLevelReloadData = std::make_unique<EventLevelReload>();
		eventManager->TriggerEvent(std::move(eventLevelReloadData));
	}
}

PlayerComponent::PlayerComponent(sf::Vector2f & playerPos, TextureAtlas & atlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner) : renderTarget(renderTarget),
	physics(physics), atlas(atlas), startingPos(startingPos), boundingBox(), view(renderTarget.getDefaultView()),
	collisionIds{ "Blocked0", "Blocked1", "Blocked2", "Blocked3", "Blocked4", "Blocked5", "Blocked6", "Blocked7", "Blocked8", "Blocked9", "Blocked10", "Blocked11", "Blocked12", "Blocked13", "Brickable0", "Brickable1", "Brickable2", "Brickable3", "bomb" },
	body(std::make_shared<Physics::Body>(playerPos, "player", &boundingBox, &collisionIds)),
	Component(COMPONENT_PLAYER_ID, eventManager, owner)
{
	physics.addElementPointer(body);

	utils::addAnimation({ "playerBackIdel" }, "backIdel", atlas, animations);
	utils::addAnimation({ "playerFrontIdel" }, "frontIdel", atlas, animations);
	utils::addAnimation({ "playerBackWalk0", "playerBackWalk1" }, "backWalk", atlas, animations);
	utils::addAnimation({ "playerFrontWalk0", "playerFrontWalk1" }, "frontWalk", atlas, animations);
	utils::addAnimation({ "playerLeftWalk0" }, "leftPreWalk", atlas, animations);
	utils::addAnimation({ "playerLeftWalk1", "playerLeftWalk2" }, "leftWalk", atlas, animations);
	utils::addAnimation({ "playerRightWalk0" }, "rightPreWalk", atlas, animations);
	utils::addAnimation({ "playerRightWalk1", "playerRightWalk2" }, "rightWalk", atlas, animations);

	eventManager->addListener(EventHitByBomb::EVENT_HITBYBOMB_ID, delegateCollectedAllDiamonds);
}

void PlayerComponent::update(float dt)
{
	body->vel = { 0.0f, 0.0f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		body->vel.y = -speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		body->vel.y = speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		body->vel.x = -speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		body->vel.x = speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && owner->getComponent<BombComponent>(BombComponent::COMPONENT_BOMB_ID) == nullptr)
	{
		owner->addComponent(std::make_shared<BombComponent>(sf::Vector2f{ body->getPos().x + currentFrame.getTextureRect().width / 2.0f, body->getPos().y + currentFrame.getTextureRect().height / 2.0f }, 3, TextureAtlas("bomb.atlas"), physics, renderTarget, eventManager, owner));
		//NOTE: "bomb" also has to be the last one in collisionIds, so that this works!
		collisionIds.pop_back();
		isInsideBomb = true;
	}

	auto bombComponent = owner->getComponent<BombComponent>(BombComponent::COMPONENT_BOMB_ID);
	if (bombComponent)
	{
		if (isInsideBomb)
		{
			if (!boundingBox.intersects(bombComponent->getBoundingBox()))
			{
				collisionIds.push_back("bomb");
				isInsideBomb = false;
			}
		}

		if (bombComponent->getIsExploded())
			owner->removeComponent(bombComponent->COMPONENT_BOMB_ID);
	}

	if (body->vel.x > 0)
		currentFrame = animations.find("rightWalk")->second.getKeyFrame();
	else if (body->vel.x < 0)
		currentFrame = animations.find("leftWalk")->second.getKeyFrame();
	else if (body->vel.y > 0)
		currentFrame = animations.find("frontWalk")->second.getKeyFrame();
	else if (body->vel.y < 0)
		currentFrame = animations.find("backWalk")->second.getKeyFrame();
	else
		currentFrame = animations.find("frontIdel")->second.getKeyFrame();


	currentFrame.setPosition(body->getPos() + body->vel * dt);

	boundingBox.left = currentFrame.getPosition().x;
	boundingBox.top = currentFrame.getPosition().y;
	boundingBox.width = (float)currentFrame.getTextureRect().width;
	boundingBox.height = (float)currentFrame.getTextureRect().height;
}

void PlayerComponent::draw()
{
	currentFrame.setPosition(body->getPos());
	renderTarget.draw(currentFrame);
}
