#include "Physics.h"
#include <assert.h>

void Physics::handleCollision(std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& it,
	std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& collideElement,
	const sf::FloatRect & bodyRect, const sf::FloatRect & elementRect)
{
	if (bodyRect.intersects(elementRect))
	{
		if (it->second->isTrigger)
		{
			it->second->triggered = true;
			it->second->triggerInformation.triggerElementCollision = collideElement->second->physicsElement.id;
			return; //NOTE: Safty first...
		}
		else if (collideElement->second->isTrigger)
		{
			it->second->triggered = true;
			it->second->triggerInformation.triggerElementCollision = collideElement->second->physicsElement.id;
		}
			
		if (it->second->vel.y >= 0 &&
			it->second->physicsElement.shoes.intersects(elementRect))
		{
			it->second->isGrounded = true;
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::SHOES;
				return;
			}
			it->second->pos.y = elementRect.top - elementRect.height + 0.25f;
			it->second->vel.y = 0.0f;
		}
		else if (it->second->vel.y <= 0 &&
			it->second->physicsElement.head.intersects(elementRect))
		{
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::HEAD;
				return;
			}
			it->second->pos.y = elementRect.top + elementRect.height - 0.25f;
			it->second->vel.y = 0.0f;
		}
		else if (it->second->vel.x < 0 &&
			it->second->physicsElement.leftBody.intersects(elementRect))
		{
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::LEFT;
				return;
			}
			it->second->vel.x = 0;
			it->second->pos.x = elementRect.left + elementRect.width;
		}
		else if (it->second->vel.x > 0 &&
			it->second->physicsElement.rightBody.intersects(elementRect))
		{
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::RIGHT;
				return;
			}
			it->second->vel.x = 0;
			it->second->pos.x = elementRect.left - it->second->physicsElement.colliders.collidersPointer->width;
		}
	}
}

Physics::Physics() : bodies()
{
}

void Physics::update(float dt)
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it)
	{
		if (!it->second->isStatic || it->second->isTrigger)
		{
			it->second->triggered = false;
			it->second->triggerInformation.triggerElementCollision = "";
			it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::NONE;

			if (!it->second->isTrigger)
			{
				it->second->isGrounded = false;

				it->second->updateShoes();
				it->second->updateLeftRightBody();
				it->second->updateHead();
			}

			for (auto collisionIdIt = it->second->physicsElement.collisionIds.begin(); collisionIdIt != it->second->physicsElement.collisionIds.end(); ++collisionIdIt)
			{
				auto collideElement = bodies.find(*collisionIdIt);
				if (collideElement != bodies.end())
				{
					if (it->second->physicsElement.collidersInPointer)
					{
						sf::FloatRect bodyRect = *it->second->physicsElement.colliders.collidersPointer;

						if (collideElement->second->physicsElement.collidersInPointer)
						{
							sf::FloatRect elementRect = *collideElement->second->physicsElement.colliders.collidersPointer;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
						else
						{
							sf::FloatRect elementRect = collideElement->second->physicsElement.colliders.collidersValue;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
					}
					else
					{
						sf::FloatRect bodyRect = it->second->physicsElement.colliders.collidersValue;

						if (collideElement->second->physicsElement.collidersInPointer)
						{
							sf::FloatRect elementRect = *collideElement->second->physicsElement.colliders.collidersPointer;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
						else
						{
							sf::FloatRect elementRect = collideElement->second->physicsElement.colliders.collidersValue;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
					}
				}

				it->second->pos += it->second->vel * dt;
			}
		}
	}
}

void Physics::addElementPointer(std::shared_ptr<Body> body)
{
	bodies.emplace(body->physicsElement.id, body);
}

void Physics::addElementValue(Body body)
{
	std::shared_ptr<Body> bodyPtr = std::make_shared<Body>(Body(body.physicsElement.id, body.physicsElement.colliders.collidersValue, false, true, {}));
	bodies.emplace(body.physicsElement.id, bodyPtr);
}

bool Physics::removeElementById(std::string & id)
{
	return bodies.erase(id);
}

void Physics::Body::updateShoes()
{
	if (!isStatic)
	{
		physicsElement.shoes.left = physicsElement.colliders.collidersPointer->left + 10.0f;
		physicsElement.shoes.top = physicsElement.colliders.collidersPointer->top + physicsElement.colliders.collidersPointer->height - 10.0f;
		physicsElement.shoes.width = physicsElement.colliders.collidersPointer->width - 20.0f;
		physicsElement.shoes.height = 10.0f;
	}
}

void Physics::Body::updateLeftRightBody()
{
	if (!isStatic)
	{
		physicsElement.leftBody.left = physicsElement.colliders.collidersPointer->left;
		physicsElement.leftBody.top = physicsElement.colliders.collidersPointer->top + 0.5f;
		physicsElement.leftBody.width = physicsElement.colliders.collidersPointer->width / 2.0f;
		physicsElement.leftBody.height = physicsElement.colliders.collidersPointer->height - 15.0f;

		physicsElement.rightBody.left = physicsElement.colliders.collidersPointer->left + physicsElement.colliders.collidersPointer->width / 2.0f;
		physicsElement.rightBody.top = physicsElement.colliders.collidersPointer->top + 0.5f;
		physicsElement.rightBody.width = physicsElement.colliders.collidersPointer->width / 2.0f;
		physicsElement.rightBody.height = physicsElement.colliders.collidersPointer->height - 15.0f;
	}
}

void Physics::Body::updateHead()
{
	if (!isStatic)
	{
		physicsElement.head.left = physicsElement.colliders.collidersPointer->left + 10.0f;
		physicsElement.head.top = physicsElement.colliders.collidersPointer->top;
		physicsElement.head.width = physicsElement.colliders.collidersPointer->width - 20.0f;
		physicsElement.head.height = 15.0f;
	}
}

Physics::Body::Body(sf::Vector2f& pos, std::string name, sf::FloatRect* collider, bool isTrigger, bool isStatic, std::vector<std::string> collisionId)
	: isStatic(isStatic), isTrigger(isTrigger), pos(pos), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIds = collisionId;
	this->physicsElement.collidersInPointer = true;
	this->physicsElement.colliders.collidersPointer = collider;
}

Physics::Body::Body(std::string name, sf::FloatRect collider, bool isTrigger, bool isStatic, std::vector<std::string> collisionId)
	: isStatic(isStatic), isTrigger(isTrigger), pos(0.0f, 0.0f), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIds = collisionId;
	this->physicsElement.collidersInPointer = false;
	this->physicsElement.colliders.collidersValue = collider;
}

bool Physics::Body::getIsGrounded()
{
	assert(!isStatic); //For now, see TODO below

	if (!isStatic)
		return isGrounded;
	else
		return false; //TODO:  Throw exeption...
}

bool Physics::Body::getIsTriggerd()
{
	return triggered;
}

sf::Vector2f& Physics::Body::getPos()
{
	assert(!isStatic); //For now, see TODO below

	if (!isStatic)
		return pos;
	else
		return pos; //TODO: Throw exeption...
}

void Physics::Body::setPos(sf::Vector2f newPos)
{
	assert(!isStatic); //For now, see TODO below

	if (!isStatic)
		pos = newPos;
	//TODO: else Throw exeption;
}

Physics::Body::TriggerInformation & Physics::Body::getTriggerInformation()
{
	return triggerInformation;
}

std::string & Physics::Body::getId()
{
	return physicsElement.id;
}
