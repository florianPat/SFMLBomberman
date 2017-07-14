#pragma once

#include "EventData.h"
#include <string>

struct EventHitByBomb : public EventData
{
	static constexpr int EVENT_HITBYBOMB_ID = 0x66e440ed;
public:
	std::string collisionId;
	EventHitByBomb(std::string& collisionId) : collisionId(collisionId), EventData(EVENT_HITBYBOMB_ID) {};
};