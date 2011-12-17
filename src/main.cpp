#include <irrlicht.h>
#include <string>
#include <ctime>
#include <vector>
#include <boost/filesystem.hpp>

#include "globs.h"
#include "map.h"


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
using core::vector3df;
using video::SColor;

/*std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}*/



EventReceiver receiver;
ConfigFile UserConfig;

video::IVideoDriver* driver;
scene::ISceneManager* smgr;
gui::IGUIEnvironment* guienv;



IrrlichtDevice *setupDevice(EventReceiver &receiver, ConfigFile *UserConfig) {
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	if (IrrlichtDevice::isDriverSupported(video::EDT_OPENGL)) {
		params.DriverType = video::EDT_OPENGL;
	}
	else if (IrrlichtDevice::isDriverSupported(video::EDT_DIRECT3D9)) {
		params.DriverType = video::EDT_DIRECT3D9;
	}
	else if (IrrlichtDevice::isDriverSupported(video::EDT_DIRECT3D8)) {
		params.DriverType = video::EDT_DIRECT3D8;
	}
	else {
		printf("No suitable video driver found.\n");
		return NULL;
	}

	params.WindowSize = core::dimension2d<u32>(UserConfig->read<int>("window_width", 800), UserConfig->read<int>("window_height", 600));
	params.Bits = 32;
	params.Fullscreen = false;
	params.Stencilbuffer = false;
	params.Doublebuffer = false;
	params.EventReceiver = &receiver;
	return createDeviceEx(params);
}


struct controls {
	EKEY_CODE cam_up;
	EKEY_CODE cam_down;
	EKEY_CODE cam_left;
	EKEY_CODE cam_right;
};

std::string get_userdata_path() {
#ifdef __unix
	string result ( getenv("HOME") );
	result += "/.IrrRR/"; // TODO: Is this ok?
#else // __unix
#ifdef __WIN32__
	string result ( getenv("APPDATA") );
	result += "\\IrrRR\\";
#endif // __WIN32__
#endif // __unix
	return result;
}

int main() {
	try {
		UserConfig = ConfigFile(get_userdata_path() + "user.cfg");
	}
	catch (ConfigFile::file_not_found) {
		printf("No user config file found, creating...\n");
		boost::filesystem::create_directories(get_userdata_path());
		FILE *f;
		f = fopen((get_userdata_path() + "user.cfg").c_str(), "w");
		if (f == NULL) {
			printf("Could not create user config file. Aborting. Please check that the path to the user config file is available: %s", (get_userdata_path() + "user.cfg").c_str());
			exit(-1);
		}
		fclose(f);
		UserConfig = ConfigFile(get_userdata_path() + "user.cfg");
	}

	IrrlichtDevice *device = setupDevice(receiver, &UserConfig);
	if (!device) {
		printf("Could not create device.\n");
	}

	device->setWindowCaption(L"IrrRR");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	scene::ICameraSceneNode *cam = smgr->addCameraSceneNode(0, vector3df(0, 10, -2), vector3df(0, 0, 0));
	cam->setFarValue(42000.0f);
	cam->setPosition(core::vector3df(-3,8,0));
	cam->setTarget(core::vector3df(0,0,0));
	cam->setFarValue(42000.0f);

	FILE *mapfile;
	mapfile = fopen("data/maps/test.map", "rb");
	if (mapfile == NULL) {
        printf("Could not open test map.");
        exit(-1);
	}
	Map *map = new Map;
	map->load(mapfile);
	fclose(mapfile);

	controls userControls;

	userControls.cam_up = (EKEY_CODE) UserConfig.read<int>("keys.camera_up", KEY_KEY_W);
	userControls.cam_down = (EKEY_CODE) UserConfig.read<int>("keys.camera_down", KEY_KEY_S);
	userControls.cam_left = (EKEY_CODE) UserConfig.read<int>("keys.camera_left", KEY_KEY_A);
	userControls.cam_right = (EKEY_CODE) UserConfig.read<int>("keys.camera_right", KEY_KEY_D);

/* Set up lighting */
	std::vector<scene::ILightSceneNode*> lights;

#define asdf 500
#define asdfg asdf/2
	for (int i = 0; i<8; i++) {
		vector3df pos (
			(float)((i & 1) != 0)*asdf-asdfg,
			(float)((i & 2) != 0)*asdf-asdfg,
			(float)((i & 4) != 0)*asdf-asdfg
		);
		lights.push_back(smgr->addLightSceneNode(NULL, pos));
	}


/* Set up skybox */

	const io::path skyboxFilename ("data/textures/skybox/top.png");
	video::ITexture *sb_tex = driver->getTexture(skyboxFilename);
	smgr->addSkyBoxSceneNode(sb_tex, sb_tex, sb_tex, sb_tex, sb_tex, sb_tex);

/* Load cube */

	scene::IMesh *cubeMesh = smgr->getMesh("data/models/cube.dae");
	scene::IMeshSceneNode *cube = smgr->addMeshSceneNode(cubeMesh);
	cube -> setMaterialFlag(video::EMF_LIGHTING, true);

/* T-Minus ten! */
	ITimer* timer = device->getTimer();
	timer->setTime(0);
	unsigned int now = 0;
	unsigned int lastUpdate = 0;
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

			camMove.set(0, 0, 0);
			if (receiver.IsKeyPressed(userControls.cam_up)) {
				camMove.X = 0.1;
			}
			if (receiver.IsKeyPressed(userControls.cam_down)) {
				camMove.X = -0.1;
			}
			if (receiver.IsKeyPressed(userControls.cam_left)) {
				camMove.Z = 0.1;
			}
			if (receiver.IsKeyPressed(userControls.cam_right)) {
				camMove.Z = -0.1;
			}
			camPos = cam->getPosition();
			camTarget = cam->getTarget();
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
