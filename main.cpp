#include <irrlicht.h>
#include <string>
#include <ctime>
#include <vector>
#include "ConfigFile.h"
#include "event.h"
#include "enttype.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#include <windows.h>
#define Pass(ms) Sleep(ms) 
#else
#include <unistd.h>
#define Pass(ms) usleep(ms*1000)
#endif

#define FPS 63.0

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

IrrlichtDevice *setupDevice(EventReceiver &receiver, ConfigFile *UserConfig) {
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	if (IrrlichtDevice::isDriverSupported(EDT_OPENGL)) {	
		params.DriverType = EDT_OPENGL;
	}
	else if (IrrlichtDevice::isDriverSupported(EDT_DIRECT3D9)) {
		params.DriverType = EDT_DIRECT3D9;
	}
	else if (IrrlichtDevice::isDriverSupported(EDT_DIRECT3D8)) {
		params.DriverType = EDT_DIRECT3D8;
	}
	else {
		printf("No suitable video driver found.\n");
		return NULL;
	}
	
	params.WindowSize = dimension2d<u32>(UserConfig->read<int>("window_width", 640), UserConfig->read<int>("window_height", 480));
	params.Bits = 32;
	params.Fullscreen = false;
	params.Stencilbuffer = false;
	params.Doublebuffer = false;
	params.EventReceiver = &receiver;
	return createDeviceEx(params);
}


typedef struct {
	EKEY_CODE cam_up;
	EKEY_CODE cam_down;
	EKEY_CODE cam_left;
	EKEY_CODE cam_right;
} controls;


int main() {
	EventReceiver receiver;
	ConfigFile MainConfig = ConfigFile("data/config/main.cfg");
	ConfigFile UserConfig = ConfigFile("data/config/user.cfg");

	IrrlichtDevice *device = setupDevice(receiver, &UserConfig);
	if (!device) {
		printf("Could not create device.\n");
	}

	device->setWindowCaption(L"IrrRR");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	ICameraSceneNode *cam = smgr->addCameraSceneNode(0, vector3df(0, 10, -2), vector3df(0, 0, 0));
	cam->setFarValue(42000.0f);

	controls userControls;

	userControls.cam_up = (EKEY_CODE) UserConfig.read<int>("keys.camera_up", KEY_KEY_W);
	userControls.cam_down = (EKEY_CODE) UserConfig.read<int>("keys.camera_down", KEY_KEY_S);
	userControls.cam_left = (EKEY_CODE) UserConfig.read<int>("keys.camera_left", KEY_KEY_A);
	userControls.cam_right = (EKEY_CODE) UserConfig.read<int>("keys.camera_right", KEY_KEY_D);

/*	userControls.cam_up = (EKEY_CODE) cam_up;
	userControls.cam_down = (EKEY_CODE) cam_down;
	userControls.cam_left = (EKEY_CODE) cam_left;
	userControls.cam_right = (EKEY_CODE) cam_right;*/

/* Set up lighting */
	std::vector<ILightSceneNode*> lights;

	for (int i = 0; i<8; i++) {
		vector3df pos ( 
			((i & 1) != 0)*128-64,
			((i & 2) != 0)*128-64,
			((i & 4) != 0)*128-64
		);
		lights.push_back(smgr->addLightSceneNode(cam, pos));
	}


/* Set up skybox */

	const std::string _skyboxFilename (MainConfig.read<std::string>("skybox", ""));
	const irr::io::path skyboxFilename (_skyboxFilename.c_str());
	ITexture *sb_tex = driver->getTexture(skyboxFilename);
	smgr->addSkyBoxSceneNode(sb_tex, sb_tex, sb_tex, sb_tex, sb_tex, sb_tex);

/* Load cube */

	IMesh *cubeMesh = smgr->getMesh("data/models/cube.dae");
	IMeshSceneNode *cube = smgr->addMeshSceneNode(cubeMesh);

/* Load entity types */

	std::vector<EntityType> entTypes;
	std::string _entTypeNames (MainConfig.read<std::string>("enttypes", ""));
	std::vector<std::string> entTypeNames;
	split(_entTypeNames, ',', entTypeNames);
	std::vector<std::string>::iterator i;
	for (i = entTypeNames.begin(); i < entTypeNames.end(); i++) { // Create entity types
		entTypes.push_back(EntityType::EntityType(*i, &MainConfig));
	}

/* T-Minus ten! */
	ITimer* timer = device->getTimer(); 
	timer->setTime(0);
	float now = 0;
	float lastUpdate = 0;
	int frame = 0;
	vector3df camMove(0, 0, 0);
	vector3df camPos = cam->getPosition();
	vector3df camTarget = cam->getTarget();
/* We have liftoff! */
	while (device->run()) {
		now = timer->getTime();
		if (now >= lastUpdate + 1000.0/FPS) {
			frame++;
			lastUpdate = now;
			driver->beginScene(true, true, SColor(255, 0, 0, 0));

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

// TODO: Read keys from config
			camMove.set(0, 0, 0);
			if (receiver.IsKeyPressed(userControls.cam_up)) {
				camMove.Z = 0.5;
			}
			if (receiver.IsKeyPressed(userControls.cam_down)) {
				camMove.Z = -0.5;
			}
			if (receiver.IsKeyPressed(userControls.cam_left)) {
				camMove.X = -0.5;
			}
			if (receiver.IsKeyPressed(userControls.cam_right)) {
				camMove.X = 0.5;
			}
			camPos += camMove;
			camTarget += camMove;
			cam->setPosition(camPos);
			cam->setTarget(camTarget);

			if(receiver.IsKeyPressed(KEY_ESCAPE)) break;
		}
		else {
			Pass(10);
		}
	}
	device->drop();
	return 0;
}
