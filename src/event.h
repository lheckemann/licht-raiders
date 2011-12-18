#pragma once
#include <irrlicht.h>

using namespace irr;

#define CBOX_ID_MINECRAFT_MODE 100

class EventReceiver : public IEventReceiver
{
public:
    core::position2di MousePosition;
	virtual bool OnEvent(const SEvent& event);
	virtual bool IsKeyPressed(EKEY_CODE keyCode) const;
	EventReceiver();
	virtual ~EventReceiver();
private:
	bool KeyDown[KEY_KEY_CODES_COUNT];
};

extern void setup_GUI();
