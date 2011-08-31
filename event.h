#pragma once
#include <irrlicht.h>

using namespace irr;

class EventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event);
	virtual bool IsKeyPressed(EKEY_CODE keyCode) const;
	EventReceiver();
	virtual ~EventReceiver();
private:
	bool KeyDown[KEY_KEY_CODES_COUNT];
};
