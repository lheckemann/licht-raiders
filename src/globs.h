#pragma once
#include <string>
#include <irrlicht.h>
#include "event.h"
#include "ConfigFile.h"

extern EventReceiver receiver;
extern ConfigFile UserConfig;

extern video::IVideoDriver* driver;
extern scene::ISceneManager* smgr;
extern gui::IGUIEnvironment* guienv;

extern void bork(std::string msg);
