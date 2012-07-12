#pragma once
#include <irrlicht.h>

using namespace irr;

#define CBOX_ID_MINECRAFT_MODE 100

enum KeyAction { // Actions that can be done as a result of a key being pressed
	KEY_ACTION_DO_NOTHING = 0,
	KEY_ACTION_QUIT,
	KEY_ACTION_MOVE_CAM_LEFT,
	KEY_ACTION_MOVE_CAM_RIGHT,
	KEY_ACTION_MOVE_CAM_FORWARDS,
	KEY_ACTION_MOVE_CAM_BACKWARDS,
	KEY_ACTION_MOVE_CAM_UP,
	KEY_ACTION_MOVE_CAM_DOWN,
	KEY_ACTION_ROTATE_CAM_CW,
	KEY_ACTION_ROTATE_CAM_ACW
};

class EventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event);
	EventReceiver();
	virtual ~EventReceiver();
private:
	KeyAction bindings[KEY_KEY_CODES_COUNT]; // bindings table
	bool KeyDown[KEY_KEY_CODES_COUNT]; // Keys down; used only internally in event handler so no getters/setters
	void handle_key(const SEvent&);
	void handle_GUI(const SEvent&);
};

extern void setup_GUI();
