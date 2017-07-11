#include "BombComponent.h"

BombComponent::BombComponent(sf::Vector2f & pos, TextureAtlas & atlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner) : pos({pos.x - ((int)pos.x % 64), pos.y - ((int)pos.y % 64)}),
physics(physics), atlas(atlas), currentFrame(), boundingBox(this->pos.x, this->pos.y, 64.0f, 64.0f), renderTarget(renderTarget),
body("bomb", boundingBox), Component(COMPONENT_BOMB_ID, eventManager, owner)
{
	utils::addAnimation({ "bombIdel" }, "bombIdel", atlas, animations);
	utils::addAnimation({ "bombExplode1", "bombExplode2" }, "bombExplode", atlas, animations);

	physics.addElementValue(body);
}

void BombComponent::update(float dt)
{
	timeDelay += dt;
	if (timeDelay > explodeTime && !exploded)
	{
		//TODO: Explode
		currentFrame = animations.find("bombExplode")->second.getKeyFrame();
		exploded = true;
		physics.removeElementById(std::string{ "bomb" });
	}
	else if (timeDelay > explodeTime / 3.0f)
		currentFrame = animations.find("bombExplode")->second.getKeyFrame();
	else
		currentFrame = animations.find("bombIdel")->second.getKeyFrame();

	currentFrame.setPosition(pos);
}

void BombComponent::draw()
{
	//TODO: Draw the bomb below the player
	renderTarget.draw(currentFrame);
}

bool BombComponent::getIsExploded() const
{
	return exploded;
}

sf::FloatRect BombComponent::getBoundingBox() const
{
	return boundingBox;
}
