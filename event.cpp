#include <irrlicht.h>
#include "event.h"

using namespace irr;
using namespace io;


bool EventReceiver::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

bool EventReceiver::IsKeyPressed(EKEY_CODE keyCode) const
{
	return KeyDown[keyCode];
}

EventReceiver::EventReceiver()
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyDown[i] = false;
}
EventReceiver::~EventReceiver() {}
