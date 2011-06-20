#include <irrlicht.h>
#include <string>
#include <ctime>
#include <vector>
#include "ConfigFile.h"
#include "event.h"

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


IrrlichtDevice *setupDevice(EventReceiver &receiver) {
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
	
	params.WindowSize = dimension2d<u32>(640, 480); //Todo: read from config
	params.Bits = 32;
	params.Fullscreen = false;
	params.Stencilbuffer = false;
	params.Doublebuffer = false;
	params.EventReceiver = &receiver;
	return createDeviceEx(params);
}


int main() {
	EventReceiver receiver;
	ConfigFile MainConfig = ConfigFile("data/config/main.cfg");

	IrrlichtDevice *device = setupDevice(receiver);
	if (!device) {
		printf("Could not create device.\n");
	}

	device->setWindowCaption(L"IrrRR");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	ICameraSceneNode *cam = smgr->addCameraSceneNode(0, vector3df(0, 10, -2), vector3df(0, 0, 0));
	cam->setFarValue(42000.0f);


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
			if (receiver.IsKeyPressed(KEY_KEY_W)) {
				camMove.Z = 0.5;
			}
			if (receiver.IsKeyPressed(KEY_KEY_S)) {
				camMove.Z = -0.5;
			}
			if (receiver.IsKeyPressed(KEY_KEY_D)) {
				camMove.X = 0.5;
			}
			if (receiver.IsKeyPressed(KEY_KEY_A)) {
				camMove.X = -0.5;
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
