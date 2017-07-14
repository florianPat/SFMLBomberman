#include "BombComponent.h"
#include "EventHitByBomb.h"

BombComponent::BombComponent(sf::Vector2f & pos, int flameRange, TextureAtlas & atlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner) : pos({pos.x - ((int)pos.x % 64), pos.y - ((int)pos.y % 64)}),
physics(physics), atlas(atlas), currentFrame(), boundingBox(this->pos.x, this->pos.y, 64.0f, 64.0f),
boundingBoxTriggerArea(this->pos.x - 64.0f * std::floorf(flameRange / 2.0f), this->pos.y, boundingBox.width * (float)flameRange, boundingBox.height), renderTarget(renderTarget),
body("bomb", boundingBox), flameTileTexture(Assets::textureAssetManager.getOrAddRes("assetsRaw/bomb/flameTile.png")), flameTileSprite(*flameTileTexture),
flameTileRenderTexture(), flameSprite(),
move(std::roundf(boundingBoxTriggerArea.width / 64.0f / 2.0f) * 64.0f, -std::floorf(boundingBoxTriggerArea.width / 64.0f / 2.0f) * 64.0f),
triggerExplosionBody(std::make_shared<Physics::Body>(sf::Vector2f{ boundingBoxTriggerArea.left, boundingBoxTriggerArea.top }, "bombTrigger", &boundingBoxTriggerArea, true, true, std::vector<std::string>{"player", "Brickable0", "Brickable1", "Brickable2", "Brickable3"})), //TODO: Enemies, ...
Component(COMPONENT_BOMB_ID, eventManager, owner)
{
	flameTileRenderTexture.create((unsigned int)boundingBoxTriggerArea.width, (unsigned int)boundingBoxTriggerArea.height);
	for (int i = 0; i < boundingBoxTriggerArea.width / 64.0f; ++i)
	{
		flameTileSprite.setPosition(sf::Vector2f{i * 64.0f, 0});
		flameTileRenderTexture.draw(flameTileSprite);
	}
	flameTileRenderTexture.display();
	flameSprite = sf::Sprite(flameTileRenderTexture.getTexture());
	flameSprite.setPosition(sf::Vector2f{boundingBoxTriggerArea.left, boundingBoxTriggerArea.top});

	utils::addAnimation({ "bombIdel" }, "bombIdel", atlas, animations);
	utils::addAnimation({ "bombExplode1", "bombExplode2" }, "bombExplode", atlas, animations);

	physics.addElementValue(body);
	physics.addElementPointer(triggerExplosionBody); //TODO: Fix triggerExplosionBody rect
}

void BombComponent::update(float dt)
{
	timeDelay += dt;

	if (!exploded)
	{
		if (timeDelay > explodeTime && showFire)
		{
			exploded = true;

			physics.removeElementById(triggerExplosionBody->getId());
			triggerExplosionBody.reset();
			triggerExplosionBody = nullptr;

			physics.removeElementById(body.getId());

			currentFrame = animations.find("bombExplode")->second.getKeyFrame();
		}
		else if (showFire)
		{
			if (triggerExplosionBody->getIsTriggerd())
			{
				std::unique_ptr<EventHitByBomb> eventData = std::make_unique<EventHitByBomb>(triggerExplosionBody->getTriggerInformation().triggerElementCollision);
				eventManager->TriggerEvent(std::move(eventData));
			}

			currentFrame = animations.find("bombExplode")->second.getKeyFrame();
		}
		else if (timeDelay > explodeTime)
		{
			timeDelay -= 2.0f;
			showFire = true;
			currentFrame = animations.find("bombExplode")->second.getKeyFrame();
		}
		else if (timeDelay > explodeTime / 3.0f)
			currentFrame = animations.find("bombExplode")->second.getKeyFrame();
		else
			currentFrame = animations.find("bombIdel")->second.getKeyFrame();
	}

	currentFrame.setPosition(pos);
}

void BombComponent::draw()
{
	//TODO: Draw the bomb below the player (maybe add layers, z or kind of that to the "renderer")
	renderTarget.draw(currentFrame);
	if (showFire)
	{
		renderTarget.draw(flameSprite);

		flameSprite.move(move);
		flameSprite.setRotation(90.0f);
		renderTarget.draw(flameSprite);

		flameSprite.move(-move);
		flameSprite.setRotation(0.0f);
	}
}

bool BombComponent::getIsExploded() const
{
	return exploded;
}

sf::FloatRect BombComponent::getBoundingBox() const
{
	return boundingBox;
}
