#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(sf::Vector2f & playerPos, TextureAtlas & atlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner) : renderTarget(renderTarget),
	physics(physics), atlas(atlas), startingPos(startingPos), boundingBox(), view(renderTarget.getDefaultView()),
	body(std::make_shared<Physics::Body>(playerPos, "player", &boundingBox, false, false, std::vector<std::string>{"Blocked0", "Blocked1", "Blocked2", "Blocked3", "Blocked4", "Blocked5", "Blocked6", "Blocked7", "Blocked8", "Blocked9", "Blocked10", "Blocked11", "Blocked12", "Blocked13"})),
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
