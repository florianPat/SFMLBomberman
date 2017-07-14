#include "BrickableBlockComponent.h"
#include "EventHitByBomb.h"
#include <assert.h>

void BrickableBlockComponent::eventEntitiyHitByBombHandler(EventData* eventData)
{
	auto boundingIt = boundingBoxes.begin();
	for (auto it = bodies.begin(); it != bodies.end(); ++it, ++boundingIt)
	{
		if ((*it)->getId() == static_cast<EventHitByBomb*>(eventData)->collisionId)
		{
			int size = destroyBoundingBoxesAnimations.size();
			destroyBoundingBoxesAnimations.push_back(std::make_pair(*boundingIt, Animation(TextureAtlas("DestroyBrickableBlock.atlas").getRegions(), sf::seconds(0.5f).asMicroseconds(), Animation::PlayMode::NORMAL)));

			auto copyBoundingIt = boundingIt++;
			auto copyIt = it++;
			boundingIt = boundingBoxes.erase(copyBoundingIt);

			physics.removeElementById((*copyIt)->getId());
			if (copyIt->unique())
			{
				copyIt->reset();
				it = bodies.erase(copyIt);
			}
			else
				assert(!"InvalidCodePath!");

			if (bodies.empty())
				break;

			if (boundingIt != boundingBoxes.begin())
				--boundingIt;
			if (it != bodies.begin())
				--it;
		}
	}
}

BrickableBlockComponent::BrickableBlockComponent(std::string& textureFileName, std::vector<sf::FloatRect>& boundingBoxes, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner)
	: renderTarget(renderTarget), physics(physics), boundingBoxes(boundingBoxes), bodies(), texture(Assets::textureAssetManager.getOrAddRes(textureFileName)), 
	sprite(*texture), destroyBoundingBoxesAnimations(), Component(COMPONENT_BRICKABLEBLOCK_ID, eventManager, owner)
{
	auto it = boundingBoxes.begin();
	for (int i = 0; it != boundingBoxes.end(); ++it, ++i)
	{
		bodies.push_back(std::make_shared<Physics::Body>(std::string("Brickable" + std::to_string(i)), *it));
		physics.addElementPointer(bodies[i]);
	}

	eventManager->addListener(EventHitByBomb::EVENT_HITBYBOMB_ID, delegateCollectedAllDiamonds);
}

void BrickableBlockComponent::update(float dt)
{
}

void BrickableBlockComponent::draw()
{
	for (auto it = boundingBoxes.begin(); it != boundingBoxes.end(); ++it)
	{
		sprite.setPosition(sf::Vector2f{it->left, it->top});
		renderTarget.draw(sprite);
	}
	//TODO: Handle this in upadte if possible (not possible at this moment, because tilemap "overdraws" it)
	for (auto it = destroyBoundingBoxesAnimations.begin(); it != destroyBoundingBoxesAnimations.end(); ++it)
	{
		if (it->second.isAnimationFinished())
		{
			it = destroyBoundingBoxesAnimations.erase(it);

			if (destroyBoundingBoxesAnimations.empty())
				break;

			if (it != destroyBoundingBoxesAnimations.begin())
				--it;
		}
		else
		{
			sf::Sprite currentFrame = it->second.getKeyFrame();
			currentFrame.setPosition(sf::Vector2f{ it->first.left, it->first.top });
			renderTarget.draw(currentFrame);
		}
	}
}
