#pragma once
#include <string>
#include <irrlicht.h>
#include "event.h"
#include "ConfigFile.h"
#include "map.h"
#include "entity.h"

#define MAX_FPS 65.0 // Needs to be a fair bit bigger than what we actually want. Don't ask me why.
#define ID_SELECTABLE 0x100
#define DEFAULT_ID 0


extern EventReceiver receiver;
extern ConfigFile UserConfig;

extern video::IVideoDriver* driver;
extern scene::ISceneManager* smgr;
extern gui::IGUIEnvironment* env;
extern scene::ISceneCollisionManager* collMan;

extern void bork(std::string msg);

extern bool minecraftMode;

extern gui::IGUIWindow* options;

extern scene::ISceneNode* selected_node;

extern Map *map;

struct NodeOwner {
	enum NODE_OWNER_TYPE {
		TYPE_TILE,
		TYPE_ENTITY
	} ownerType;
	Tile* tileOwner;
	Entity* entityOwner;
};
