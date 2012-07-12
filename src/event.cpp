#include <irrlicht.h>
#include "globs.h"
#include "event.h"

using namespace irr;
using namespace io;
using irr::core::rect;

//gui::IGUIEnvironment *env;
gui::IGUIWindow *options;
gui::IGUICheckBox *GUI_minecraftmode;

bool minecraftMode;

NodeOwner *selected_owner = NULL;
bool EventReceiver::OnEvent(const SEvent& event){
	switch(event.EventType) {
		case EET_KEY_INPUT_EVENT:
			if (guienv->getFocus() != NULL) return false; // Skip if the event was captured by the GUI
			if (event.KeyInput.PressedDown and KeyDown[event.KeyInput.Key]) return false; // Skip if it's a key repeat
			KeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			handle_key(event); // pass it on to the function that checks the bindings and stuff
			return false;

		case EET_GUI_EVENT:
			handle_GUI(event);
			return false;

		case EET_MOUSE_INPUT_EVENT: // TODO Rewrite
			
			return false;

		default: break;
	}
	return false;
}

void EventReceiver::handle_GUI(const SEvent& event) { // More work required :)
	s32 cid = event.GUIEvent.Caller->getID();
	switch(event.GUIEvent.EventType) {
		case gui::EGET_CHECKBOX_CHANGED:
			if (cid == CBOX_ID_MINECRAFT_MODE) {
				minecraftMode = GUI_minecraftmode->isChecked();
			}
			break;
		default: break;
	}
}

void EventReceiver::handle_key(const SEvent& event) {
	EKEY_CODE keycode = event.KeyInput.Key;
	bool pressed = event.KeyInput.PressedDown;
	KeyAction act = bindings[keycode];
	if (pressed) {
		switch(act) { // if button was pressed down
			case KEY_ACTION_DO_NOTHING: break;
			case KEY_ACTION_MOVE_CAM_LEFT: camMove.Z -= camSpeed; break;
			case KEY_ACTION_MOVE_CAM_RIGHT: camMove.Z += camSpeed; break;
			case KEY_ACTION_MOVE_CAM_FORWARDS: camMove.X -= camSpeed; break;
			case KEY_ACTION_MOVE_CAM_BACKWARDS: camMove.X += camSpeed; break;
			case KEY_ACTION_MOVE_CAM_UP: camHeightChange += camSpeed; break;
			case KEY_ACTION_MOVE_CAM_DOWN: camHeightChange -= camSpeed; break;
			default: break;
		}
	}
	else { // if button was released
		switch(act) {
			case KEY_ACTION_DO_NOTHING: break;
			case KEY_ACTION_MOVE_CAM_LEFT: camMove.Z += camSpeed; break; 
			case KEY_ACTION_MOVE_CAM_RIGHT: camMove.Z -= camSpeed; break;
			case KEY_ACTION_MOVE_CAM_FORWARDS: camMove.X += camSpeed; break;
			case KEY_ACTION_MOVE_CAM_BACKWARDS: camMove.X -= camSpeed; break;
			case KEY_ACTION_MOVE_CAM_UP: camHeightChange -= camSpeed; break;
			case KEY_ACTION_MOVE_CAM_DOWN: camHeightChange += camSpeed; break;
			case KEY_ACTION_QUIT: exit(0); // TODO make this display a dialog or whatever
			default: break;
		}
	}
}

EventReceiver::EventReceiver() { // Loads controls (nothing else, for now...)
	camSpeed = UserConfig.read("controls.camSpeed", 2.0f);
	for (int i; i < KEY_KEY_CODES_COUNT; i++) { // fill it...
		bindings[i] = KEY_ACTION_DO_NOTHING;
	}
	EKEY_CODE key;
	key = (EKEY_CODE) UserConfig.read<int>("keys.quit", KEY_ESCAPE);
	bindings[key] = KEY_ACTION_QUIT;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_up", KEY_KEY_W);
	bindings[key] = KEY_ACTION_MOVE_CAM_FORWARDS;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_down", KEY_KEY_S);
	bindings[key] = KEY_ACTION_MOVE_CAM_BACKWARDS;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_left", KEY_KEY_A);
	bindings[key] = KEY_ACTION_MOVE_CAM_LEFT;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_right", KEY_KEY_D);
	bindings[key] = KEY_ACTION_MOVE_CAM_RIGHT;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_raise", KEY_KEY_Q);
	bindings[key] = KEY_ACTION_MOVE_CAM_UP;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_lower", KEY_KEY_E);
	bindings[key] = KEY_ACTION_MOVE_CAM_DOWN;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_rot_cw", KEY_KEY_Z);
	bindings[key] = KEY_ACTION_ROTATE_CAM_CW;
	key = (EKEY_CODE) UserConfig.read<int>("keys.camera_rot_acw", KEY_KEY_X);
	bindings[key] = KEY_ACTION_ROTATE_CAM_ACW;
}

EventReceiver::~EventReceiver() {}

void setup_GUI() {
	options = guienv->addWindow(rect<s32>(0, 0, 500, 75), false, L"Options");
	GUI_minecraftmode = guienv->addCheckBox(UserConfig.read<bool>("display.minecraftmode", false), rect<s32>(10, 30, 100, 40), options, CBOX_ID_MINECRAFT_MODE, L"Minecraft Mode");
}
