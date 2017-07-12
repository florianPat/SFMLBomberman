#pragma once

#include "EventData.h"

struct EventHitByBomb : public EventData
{
	static constexpr int EVENT_HITBYBOMB_ID = 0x66e440ed;
	EventHitByBomb() : EventData(EVENT_HITBYBOMB_ID) {};
};