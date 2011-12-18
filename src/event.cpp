#include <irrlicht.h>
#include "globs.h"
#include "event.h"

using namespace irr;
using namespace io;
using irr::core::rect;

gui::IGUIEnvironment *env;
gui::IGUIWindow *options;
gui::IGUICheckBox *GUI_minecraftmode;

bool minecraftMode;


void handleGUI(const SEvent& event) {
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


bool EventReceiver::OnEvent(const SEvent& event)
{
	switch(event.EventType) {
	    case EET_KEY_INPUT_EVENT: KeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown; return false;
	    case EET_GUI_EVENT: handleGUI(event); break;
	    case EET_MOUSE_INPUT_EVENT: if(event.MouseInput.Event == EMIE_MOUSE_MOVED) {MousePosition.X = event.MouseInput.X; MousePosition.Y = event.MouseInput.Y;} else if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {printf("Click @ %i, %i\n", MousePosition.X, MousePosition.Y);}; break;
	    default: break;
	}
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

void setup_GUI() {
    options = env->addWindow(rect<s32>(0, 0, 500, 75), false, L"Options");
    GUI_minecraftmode = env->addCheckBox(not UserConfig.read<bool>("display.minecraftmode", false), rect<s32>(10, 30, 100, 40), options, CBOX_ID_MINECRAFT_MODE, L"Minecraft Mode");
}
