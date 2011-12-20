#pragma once
#include <string>
#include <irrlicht.h>
#include "event.h"
#include "ConfigFile.h"

#define MAX_FPS 75.0 // Needs to ne a fair bit bigger than what we actually want. Don't ask me why.

extern EventReceiver receiver;
extern ConfigFile UserConfig;

extern video::IVideoDriver* driver;
extern scene::ISceneManager* smgr;
extern gui::IGUIEnvironment* env;

extern void bork(std::string msg);

extern bool minecraftMode;

extern gui::IGUIWindow* options;
